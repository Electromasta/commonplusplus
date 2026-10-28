#pragma once
#include "common/core.types.h"
#include <bitset>

template<size_t N>
using Bitset = std::bitset<N>;

namespace Bitmask   {
    constexpr u8 ONE = 0x1u;
    constexpr u8 TWO = 0x3u;
    constexpr u8 THREE = 0x7u;
    constexpr u8 FOUR = 0xFu;
    constexpr u8 FIVE = 0x1Fu;
    constexpr u8 SIX = 0x3Fu;
    constexpr u8 SEVEN = 0x7Fu;
    constexpr u8 EIGHT = 0xFFu;
    constexpr u16 SIXTEEN = 0xFFFFu;
};