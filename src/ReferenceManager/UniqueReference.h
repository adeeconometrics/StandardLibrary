/**
 * @file unique_reference.h
 * @author ddamiana
 * @brief 
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

template <typename T> class unique_reference {
  T *m_ptr{nullptr};

public:
  explicit unique_reference() = default;
  explicit unique_reference(const unique_reference<T> &Type) = delete;

  explicit unique_reference(T *Type) : m_ptr(Type) {}
  explicit unique_reference(unique_reference<T> &&other) noexcept {
    other.swap(*this);
  }

  unique_reference &operator=(const unique_reference<T> &other) = delete;
  unique_reference &operator=(unique_reference<T> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ~unique_reference() {
    if (m_ptr != nullptr) {
      m_ptr->~T();
      m_ptr = nullptr;
    }
  }

  T &operator*(void) { return *(this->m_ptr); }
  T *operator->(void) { return this->m_ptr; }
  T &operator&(unique_reference<T> &other) { return other.m_ptr; }

private:
  void swap(unique_reference<T> &other) noexcept {
    std::swap(m_ptr, other.m_ptr);
  }
};