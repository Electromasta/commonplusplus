#pragma once
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string_view>

namespace common {
    struct tinystring   {
        static constexpr std::uint8_t CAP = 15;
        char chars[CAP];
        std::uint8_t len : 4;
        std::uint8_t count : 3;
        bool flag : 1;

        constexpr tinystring() noexcept : chars{}, len{}, count{}, flag{} {}
        constexpr tinystring(const char* s) noexcept : chars{}, len{}, count{}, flag{} {
            if (!s) return;
            while (len < CAP && s[len])  {
                chars[len] = s[len];
                ++len;
            }
            assert(s[len] == '\0' && "tinystring: input exceeds CAP, truncated");
        }
        constexpr tinystring(std::string_view sv) noexcept : chars{}, len{}, count{}, flag{} {
            assert(sv.size() <= CAP && "tinystring: input exceeds CAP, truncated");
            len = static_cast<std::uint8_t>(sv.size() < CAP ? sv.size() : CAP);
            for (std::uint8_t i = 0; i < len; ++i) chars[i] = sv[i];
        }

        friend constexpr bool operator==(const tinystring & a, const tinystring & b) = default;
        constexpr std::string_view view() const noexcept { return { data(), size() }; }
        constexpr operator   std::string_view() const noexcept { return view(); }

        void setSize(std::uint8_t size) noexcept {
            assert(size < 16 && "tinystring: size must be 0-15");
            flag = (size/8 == 1); 
            count = size % 8;
        }
        
        constexpr std::uint8_t getSize() const noexcept { return (flag * 8) + count; }
        constexpr const char*  data()  const noexcept { return chars; }
        constexpr std::uint8_t size()  const noexcept { return len; }
        constexpr bool         empty() const noexcept { return len == 0; }
        constexpr bool         hasPrefix(std::string_view p) const noexcept { return view().starts_with(p); }
        constexpr bool         hasSuffix(std::string_view s) const noexcept { return view().ends_with(s); }
        constexpr bool         hasAffix(std::string_view s) const noexcept { return view().contains(s); }

        constexpr char*       begin()       noexcept { return chars; }
        constexpr char*       end()         noexcept { return chars + len; }
        constexpr const char* begin() const noexcept { return chars; }
        constexpr const char* end()   const noexcept { return chars + len; }
    };
}

template<> struct std::hash<common::tinystring> {
    size_t operator()(const common::tinystring & s) const noexcept {
        const char* p = reinterpret_cast<const char*>(&s);
         std::uint64_t lo, hi;
        std::memcpy(&lo, p, 8);  
        std::memcpy(&hi, p + 8, 8);
        lo ^= hi * 0x9E3779B97F4A7C15ull;
        lo ^= lo >> 29;  
        lo *= 0xBF58476D1CE4E5B9ull;  
        lo ^= lo >> 32;
        return static_cast<size_t>(lo);
    }
};

using String16 = common::tinystring;
