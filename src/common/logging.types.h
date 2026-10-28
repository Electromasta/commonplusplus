#pragma once
#include <spdlog/spdlog.h>
#include "common/types/tinystring.h"

namespace Log {
    using spdlog::set_level;
    using spdlog::set_pattern;
    using spdlog::set_default_logger;

    using spdlog::info;
    using spdlog::warn;
    using spdlog::error;
    using spdlog::debug;
    using spdlog::trace;
    using spdlog::critical;
}

template <> struct fmt::formatter<common::tinystring> : fmt::formatter<std::string_view> {
    auto format(const common::tinystring & s, format_context & ctx) const {
        return fmt::formatter<std::string_view>::format(s.view(), ctx);
    }
};