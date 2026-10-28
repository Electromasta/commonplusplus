#pragma once
#include <rfl.hpp>
#include "common/core.types.h"
#include "common/types/bits.types.h"

namespace rfl {
    template <size_t N>
    struct Reflector<Bitset<N>> {
        static_assert(N <= 64, "Bitset reflection only supports up to 64 bits");
        using ReflType = u64;
        static ReflType from(const Bitset<N> & bits) { return bits.to_ullong(); }
        static Bitset<N> to(const ReflType & r) { return Bitset<N>(r); }
    };
}
