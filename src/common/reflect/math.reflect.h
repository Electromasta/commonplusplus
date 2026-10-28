#pragma once
#include "common/math.types.h"
#include <rfl.hpp>

// AI Generated
namespace rfl {

    template <>
    struct Reflector<glm::vec2> {
        struct ReflType { float x, y; };
        static ReflType from(const glm::vec2& v) { return { v.x, v.y }; }
        static glm::vec2 to(const ReflType& r) { return { r.x, r.y }; }
    };

    template <>
    struct Reflector<glm::vec3> {
        struct ReflType { float x, y, z; };
        static ReflType from(const glm::vec3& v) { return { v.x, v.y, v.z }; }
        static glm::vec3 to(const ReflType& r) { return { r.x, r.y, r.z }; }
    };

    template <>
    struct Reflector<glm::vec4> {
        struct ReflType { float x, y, z, w; };
        static ReflType from(const glm::vec4& v) { return { v.x, v.y, v.z, v.w }; }
        static glm::vec4 to(const ReflType& r) { return { r.x, r.y, r.z, r.w }; }
    };

    template <>
    struct Reflector<glm::ivec2> {
        struct ReflType { int x, y; };
        static ReflType from(const glm::ivec2& v) { return { v.x, v.y }; }
        static glm::ivec2 to(const ReflType& r) { return { r.x, r.y }; }
    };

    template <>
    struct Reflector<glm::ivec3> {
        struct ReflType { int x, y, z; };
        static ReflType from(const glm::ivec3& v) { return { v.x, v.y, v.z }; }
        static glm::ivec3 to(const ReflType& r) { return { r.x, r.y, r.z }; }
    };

    template <>
    struct Reflector<glm::ivec4> {
        struct ReflType { int x, y, z, w; };
        static ReflType from(const glm::ivec4& v) { return { v.x, v.y, v.z, v.w }; }
        static glm::ivec4 to(const ReflType& r) { return { r.x, r.y, r.z, r.w }; }
    };

}
