#pragma once
#include <rfl.hpp>
#include "common/core.types.h"

namespace rfl {
    template <>
    struct Reflector<String16> {
        using ReflType = String;
        static ReflType from(const String16 & s) { return String(s.view()); }
        static String16 to(const ReflType & r) { return String16(StringVw(r).substr(0, String16::CAP)); }
    };
}
