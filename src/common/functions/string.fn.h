#pragma once
#include "common/types/tinystring.h"
#include <string_view>
#include <string>
#include <cctype>

namespace Fn::string    {
    inline bool isSpace(std::string_view s) {
        if (s.empty()) return false;
        for (const char & c : s)
            if (!std::isspace(static_cast<unsigned char>(c))) return false;
        return true;
    }

    inline bool isAlpha(std::string_view s) {
        if (s.empty()) return false;
        for (const char & c : s)
            if (!std::isalpha(static_cast<unsigned char>(c))) return false;
        return true;
    }

    inline bool isNum(std::string_view s) {
        if (s.empty()) return false;
        for (const char & c : s)
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        return true;
    }

    inline bool isHex(std::string_view s) {
        if (s.empty()) return false;
        for (const char & c : s)
            if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
        return true;
    }

    inline std::string toUpper(std::string s) {
        for (char & c : s)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return s;
    }

    inline common::tinystring toUpper(common::tinystring s) {
        for (char & c : s)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return s;
    }

    inline std::string toLower(std::string s) {
        for (char & c : s)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    inline common::tinystring toLower(common::tinystring s) {
        for (char & c : s)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    inline std::string toTrim(std::string s) {
        if (s.empty() || isSpace(s)) return "";
        size_t start = s.find_first_not_of(" \t\n\r\f\v");
        size_t end = s.find_last_not_of(" \t\n\r\f\v");
        return s.substr(start, end - start + 1);
    }
}