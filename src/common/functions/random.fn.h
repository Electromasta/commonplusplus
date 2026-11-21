#pragma once
#include "common/core.types.h"
#include "common/math.types.h"
#include "realm/gen/noise/hash.fn.h"
#include <bit>
#include <cassert>
#include <span>

namespace Fn    {
    // hash bits -> uniform [0, 1). The top bits are used, each output equally likely
    constexpr f32 rand01(u64 h)  { return (f32)(h >> 40) * 0x1.0p-24f; }
    constexpr f64 rand01d(u64 h) { return (f64)(h >> 11) * 0x1.0p-53; }

    // true with probability p
    constexpr bool chance(u64 h, f32 p) { return rand01(h) < p; }

    // hash bits -> integer in [lo, hi] inclusive, by multiply-shift (Lemire) rather than %, which
    // favours low values whenever the span doesn't divide 2^32. Span must fit in 32 bits
    constexpr i32 randRange(u64 h, i32 lo, i32 hi) {
        assert(lo <= hi);
        const u64 span = (u64)((i64)hi - lo + 1);
        return lo + (i32)(((h >> 32) * span) >> 32);
    }

    // xoshiro256** (Blackman & Vigna 2018, public domain)
    struct Rng {
        Array<u64, 4> s;

        constexpr explicit Rng(u64 seed) : s{} {
            for (u64 i = 0; i < 4; i++) s[i] = bitMix(seed + i * 0x9e3779b97f4a7c15ull);
        }

        constexpr u64 next() {
            const u64 result = std::rotl(s[1] * 5, 7) * 9;
            const u64 t = s[1] << 17;
            s[2] ^= s[0];
            s[3] ^= s[1];
            s[1] ^= s[2];
            s[0] ^= s[3];
            s[2] ^= t;
            s[3] = std::rotl(s[3], 45);
            return result;
        }

        constexpr f32  next01()                 { return rand01(next()); }
        constexpr i32  range(i32 lo, i32 hi)    { return randRange(next(), lo, hi); }
        constexpr bool chance(f32 p)            { return Fn::chance(next(), p); }
    };

    // Fisher-Yates (Durstenfeld's in-place form): every ordering equally likely
    template<typename Container>
    constexpr void shuffle(Container & items, Rng & rng) {
        for (i32 i = (i32)items.size() - 1; i > 0; i--) {
            using std::swap;
            swap(items[i], items[rng.range(0, i)]);
        }
    }

    // Index picked in proportion to its weight: {60, 30, 10} gives 0 about 60% of the time.
    constexpr size_t weightedPick(std::span<const f32> weights, f32 roll) {
        f32 total = 0;
        for (f32 w : weights) if (w > 0) total += w;   // only weights the scan below can pick
        f32 target = roll * total;
        size_t last = weights.size();
        for (size_t i = 0; i < weights.size(); i++) {
            if (weights[i] <= 0) continue;
            last = i;
            if (target < weights[i]) return i;
            target -= weights[i];
        }
        return last;
    }
}
