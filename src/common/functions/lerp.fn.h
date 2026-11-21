#pragma once
#include "common/core.types.h"
#include "common/math.types.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/spline.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Fn    {
    using glm::mix;          // a + (b - a) * t
    using glm::lerp;         // same as mix (gtx/compatibility)
    using glm::slerp;        // spherical: quaternions, or unit vec3 directions
    using glm::step;         // 0 below the edge, 1 at or above it
    using glm::smoothstep;   // cubic ease between two edges, clamped
    using glm::saturate;     // clamp to [0, 1]
    using glm::catmullRom;   // spline through v2..v3, with v1 and v4 shaping the tangents
    using glm::hermite;      // spline from explicit endpoints and tangents
    using glm::cubic;        // general cubic polynomial
    using glm::pow;

    //---- integer rounding toward -infinity.
    constexpr i32 floorDiv(i32 a, i32 b) {
        const i32 q = a / b;
        return (a % b != 0 && ((a < 0) != (b < 0))) ? q - 1 : q;
    }

    constexpr i32 floorMod(i32 a, i32 b) {
        const i32 r = a % b;
        return (r != 0 && ((r < 0) != (b < 0))) ? r + b : r;
    }

    constexpr glm::ivec2 floorDiv(glm::ivec2 a, i32 b) { return { floorDiv(a.x, b), floorDiv(a.y, b) }; }
    constexpr glm::ivec3 floorDiv(glm::ivec3 a, i32 b) { return { floorDiv(a.x, b), floorDiv(a.y, b), floorDiv(a.z, b) }; }
    constexpr glm::ivec2 floorMod(glm::ivec2 a, i32 b) { return { floorMod(a.x, b), floorMod(a.y, b) }; }
    constexpr glm::ivec3 floorMod(glm::ivec3 a, i32 b) { return { floorMod(a.x, b), floorMod(a.y, b), floorMod(a.z, b) }; }

    constexpr f32 inverseLerp(f32 a, f32 b, f32 v) {
        return (v - a) / (b - a);
    }

    constexpr f32 remap(f32 x, f32 inLo, f32 inHi, f32 outLo, f32 outHi) {
        return outLo + (x - inLo) * (outHi - outLo) / (inHi - inLo);
    }

    constexpr f32 normalize(f32 x) {
        return glm::clamp(x * 0.5f + 0.5f, 0.0f, 0.999999f);
    }

    constexpr int quantize(f32 x, int quant) {
        return quant * normalize(x);
    }

    // inverse of quantize: the raw value at the centre of step q, so quantize(dequantize(q, n), n) == q
    constexpr f32 dequantize(int q, int quant) {
        return (q + 0.5f) / quant * 2.0f - 1.0f;
    }

    constexpr f32 smootherstep(f32 edge0, f32 edge1, f32 x) {
        const f32 t = glm::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    //---- grid interpolation. Corners are named by offset: c10 is +1 in x, c01 is +1 in y.
    template<typename T, typename U>
    constexpr T bilerp(T c00, T c10, T c01, T c11, U tx, U ty) {
        const T bottom = c00 + (c10 - c00) * tx;
        const T top    = c01 + (c11 - c01) * tx;
        return bottom + (top - bottom) * ty;
    }

    template<typename T, typename U>
    constexpr T trilerp(T c000, T c100, T c010, T c110, T c001, T c101, T c011, T c111, U tx, U ty, U tz) {
        // not near/far: <windows.h> defines both as macros
        const T front = bilerp(c000, c100, c010, c110, tx, ty);
        const T back  = bilerp(c001, c101, c011, c111, tx, ty);
        return front + (back - front) * tz;
    }

    // scalar Catmull-Rom (glm's only takes vectors)
    constexpr f32 catmullRom(f32 p0, f32 p1, f32 p2, f32 p3, f32 t) {
        return 0.5f * ((2.0f * p1) +
                       (p2 - p0) * t +
                       (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
                       (3.0f * (p1 - p2) + p3 - p0) * t * t * t);
    }
}
