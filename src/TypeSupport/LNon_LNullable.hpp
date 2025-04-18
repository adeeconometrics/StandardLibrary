/**
 * @file NonNullable.h
 * @author ddamiana
 * @brief 
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <type_traits>

template<typename T, typename = std::enable_if<std::is_pointer<T>::value>>
class NonNullable final{
  public:
    NonNullable() = delete;
    NonNullable(const T& t) {
        if(t == nullptr) throw new std::invalid_argument("T must not be nullable.");
    }
    ~NonNullable() = default;
};
