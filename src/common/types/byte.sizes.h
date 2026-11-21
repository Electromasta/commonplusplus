#pragma once
#include <cstdint>

consteval std::uint64_t operator""_KB(unsigned long long v) { return v * 1024ull; }
consteval std::uint64_t operator""_MB(unsigned long long v) { return v * 1024ull * 1024ull; }
consteval std::uint64_t operator""_GB(unsigned long long v) { return v * 1024ull * 1024ull * 1024ull; }
consteval std::uint64_t operator""_TB(unsigned long long v) { return v * 1024ull * 1024ull * 1024ull * 1024ull; }