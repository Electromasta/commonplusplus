#pragma once
#include "common/core.types.h"
#include "common/memory.types.h"
#include "common/logging.types.h"
#include <typeindex>
#include <typeinfo>

namespace Common  {
    template <typename T>
    class Cache {
        public:
            enum Error { NotFound };
            Cache() = default;
            ~Cache() { flush(); }

            Result<const T*, Error> view(const String & name) const {
                if (!cache.contains(name)) return std::unexpected(Error::NotFound);
                return &cache.at(name);
            }

            Result<T, Error> get(const String & name) {
                if (!cache.contains(name)) return std::unexpected(Error::NotFound);
                return cache.at(name);
            }

            void add(const String & name, T item)  {
                if (cache.contains(name)) { Log::warn("Reassigning: {}", name); cache.erase(name); }
                cache.emplace(name, std::move(item));
            }

            void flush() { cache.clear(); }
        private:
            Cache(const Cache&) = delete;
            Cache& operator=(const Cache&) = delete;

            Hashmap<String, T> cache;
    };
}

namespace Common  {
    template <typename T>
    class TypeCache {
        public:
            enum Error { NotFound };
            TypeCache() = default;
            ~TypeCache() { flush(); }

            template<typename TT>
            Result<TT*, Error>  get() {
                auto it = cache.find(std::type_index(typeid(TT)));
                if (it == cache.end()) return std::unexpected(Error::NotFound);
                return static_cast<TT*>(it->second.get());
            }

            template<typename TT, typename... Args>
            void add(Args&&... args)   {
                std::type_index key = std::type_index(typeid(TT));
                if (cache.contains(key)) { Log::warn("Reassigning: {}", typeid(TT).name()); }
                cache.insert_or_assign(key, std::make_unique<TT>(std::forward<Args>(args)...));
            }

            void flush() { cache.clear(); }
        private:
            TypeCache(const TypeCache&) = delete;
            TypeCache& operator=(const TypeCache&) = delete;

            Hashmap<std::type_index, Pointer<T>> cache;
    };
}
