#pragma once
#ifndef GLM_ENABLE_EXPERIMENTAL
    #define GLM_ENABLE_EXPERIMENTAL
#endif
#ifndef GLM_FORCE_SWIZZLE
    #define GLM_FORCE_SWIZZLE
#endif
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace glm {
    inline bool ab(const float & minA, const float & maxA, const float & minB, const float & maxB)   {
        return maxA >= minB && maxB >= minA;
    }

    inline bool aabb(const vec2 & minA, const vec2 & maxA, const vec2 & minB, const vec2 & maxB)   {
        return all(greaterThanEqual(maxA, minB)) &&
               all(greaterThanEqual(maxB, minA));
    }

    inline bool aaabbb(const vec3 & minA, const vec3 & maxA, const vec3 & minB, const vec3 & maxB)   {
        return all(greaterThanEqual(maxA, minB)) &&
               all(greaterThanEqual(maxB, minA));
    }

    inline bool hover(const vec2 & point, const vec2 & min, const vec2 & max) {
        return aabb(point, point, min, max);
    }
}
