#pragma once
#include <cassert>
#include <cstdint>
#include <concepts>
#include <utility>
#include <vector>
#include <unordered_map>
#include "common/types/tinystring.h"

namespace common {
    template <typename T>
    struct no_delete { void operator()(T &) const noexcept {} };

    template <typename T, typename D = no_delete<T>>
    class registry {
        public:
            using Handle = common::tinystring;

            struct Resource { T item; mutable std::uint64_t lastused = 0; std::uint64_t size = 0; bool pinned = false; };
            explicit registry(T none, D deleter = {}) : NONE(std::move(none)), deleter(std::move(deleter)) {};
            registry() = default;
            ~registry() { stop(); }

            Handle insert(Handle handle, T item, std::uint64_t size, bool pinned = false)   {
                if (handle.empty()) { deleter(item); return {}; }
                if (resources.contains(handle)) {
                    Resource & resource = resources.at(handle);
                    totalbytes -= resource.size;
                    deleter(resource.item);
                    resource.item = std::move(item);
                    resource.lastused = clock;
                    resource.size = size;
                }   else {
                    resources.try_emplace(handle, std::move(item), clock, size, pinned);
                }
                totalbytes += size;
                return handle;
            }

            const T & get(Handle handle) const    {
                auto i = resources.find(handle);
                if (i == resources.end()) return NONE;
                i->second.lastused = clock;
                return i->second.item;
            }

            template <typename Fn> requires std::invocable<Fn, Handle, const Resource &>
            void forEach(Fn && fn) const {
                for (const auto & [id, r] : resources) fn(id , r);
            }

            std::vector<Handle> getHandles() const {
                std::vector<Handle> handles; 
                handles.reserve(resources.size());
                for (const auto & [id, r] : resources) handles.push_back(id);
                return handles;
            }

            void release(Handle handle, bool ignorepin = false) {
                if (handle.empty()) return;
                auto key = resources.find(handle);
                if (key == resources.end()) return;
                if (!ignorepin && key->second.pinned) return;
                totalbytes -= key->second.size;
                deleter(key->second.item);
                resources.erase(key);
            }

            void clear() {
                for (auto & [id, r] : resources)
                    deleter(r.item);
                resources.clear();
                totalbytes = 0;
            }

            void stop() {
                clear();
                if constexpr (NULLABLE) if (NONE != T{}) { deleter(NONE); NONE = T{}; }
            }

            void setNone(T none)  {
                if constexpr (NULLABLE) if (NONE != T{}) deleter(NONE);
                NONE = std::move(none);
            }

            const T &        getNone() const { return NONE; }
            void             setClock(std::uint64_t clock) { this->clock = clock; }
            void             pin(Handle handle, bool pinned = true) { if (resources.contains(handle)) resources[handle].pinned = pinned; }
            bool             contains(Handle handle) const { return resources.contains(handle); }
            std::uint32_t    size()  const { return resources.size(); }
            std::uint64_t    bytes() const { return totalbytes; }
            bool             empty() const { return resources.empty(); }
            void             reserve(std::uint32_t n) { resources.reserve(n); }
            void setDeleter(D deleter) { this->deleter = std::move(deleter); }
            static constexpr bool NULLABLE = requires { { T{} != T{} } -> std::convertible_to<bool>; };
        private:
            std::unordered_map<Handle, Resource> resources;
            std::uint64_t clock = 0;
            std::uint64_t totalbytes = 0;
            T NONE = {};
            D deleter;

            registry(const registry&) = delete;
            registry& operator=(const registry&) = delete;
    };
}

//template<typename T>
//using Registry = common::registry<T>;