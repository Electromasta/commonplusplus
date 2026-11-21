#pragma once
#include "common/core.types.h"
#include "common/math.types.h"
#include <glm/gtc/bitfield.hpp>

namespace Fn    {
    // splitmix64 (Steele, Lea & Flood 2014, public domain)
    constexpr u64 bitMix(u64 x) {
        x += 0x9e3779b97f4a7c15ull;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
        return x ^ (x >> 31);
    }

    // lowbias32 (Chris Wellons, hash-prospector, public domain)
    constexpr u32 bitMix32(u32 x) {
        x ^= x >> 16;
        x *= 0x7feb352du;
        x ^= x >> 15;
        x *= 0x846ca68bu;
        x ^= x >> 16;
        return x;
    }

    // FNV-1a, 64-bit (Fowler, Noll & Vo 1991; public domain)
    constexpr u64 stringHash(StringVw s) {
        u64 h = 0xcbf29ce484222325ull;
        for (char c : s) {
            h ^= (u8)c;
            h *= 0x100000001b3ull;
        }
        return h;
    }

    constexpr u64 subSeed(u64 seed, StringVw name) {
        return bitMix(bitMix(seed) ^ stringHash(name));
    }

    // seed is mixed before the salt goes in, so (seed A, salt B) and (seed B, salt A) differ
    constexpr u64 posHash(u64 seed, glm::ivec2 p, u64 salt = 0) {
        u64 h = bitMix(bitMix(seed) ^ salt);
        h = bitMix(h ^ (u32)p.x);
        return bitMix(h ^ (u32)p.y);
    }

    constexpr u64 posHash(u64 seed, glm::ivec3 p, u64 salt = 0) {
        u64 h = bitMix(bitMix(seed) ^ salt);
        h = bitMix(h ^ (u32)p.x);
        h = bitMix(h ^ (u32)p.y);
        return bitMix(h ^ (u32)p.z);
    }

    // Morton / Z-order key (glm::bitfieldInterleave)
    inline u64 mortonKey(glm::ivec2 p) { return glm::bitfieldInterleave((u32)p.x, (u32)p.y); }
    inline u64 mortonKey(glm::ivec3 p) { return glm::bitfieldInterleave((u32)p.x, (u32)p.y, (u32)p.z); }
}
