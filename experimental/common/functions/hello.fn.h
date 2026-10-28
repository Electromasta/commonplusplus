#pragma once
#include <string>

namespace common  {
    inline std::string cxxversion() {
        std::string version;
        
        #if __cplusplus == 202302L
            version = "C++23";
        #elif __cplusplus == 202302L
            version = "C++23";
        #elif __cplusplus == 202002L
            version = "C++20";
        #elif __cplusplus == 201703L
            version = "C++17";
        #elif __cplusplus == 201402L
            version = "C++14";
        #elif __cplusplus == 201103L
            version = "C++11";
        #elif __cplusplus == 199711L
            version = "C++98";
        #else
            version = "Unknown (" + std::to_string(__cplusplus) + ")";
        #endif
        
        // Add compiler info
        #if defined(__clang__)
            version += " (Clang " + std::to_string(__clang_major__) + "." 
                    + std::to_string(__clang_minor__) + ")";
        #elif defined(__GNUC__)
            version += " (GCC " + std::to_string(__GNUC__) + "." 
                    + std::to_string(__GNUC_MINOR__) + ")";
        #elif defined(_MSC_VER)
            version += " (MSVC " + std::to_string(_MSC_VER) + ")";
        #endif
        
        return version;
    }
}

namespace Fn {
    using common::cxxversion;
}