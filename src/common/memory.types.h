#pragma once
#include <memory>

template<typename T, typename D = std::default_delete<T>>
using Unique = std::unique_ptr<T, D>;

template<typename T>
using Pointer = std::shared_ptr<T>;

template<typename T>
using Shared = std::shared_ptr<T>;