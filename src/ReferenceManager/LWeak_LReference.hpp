/**
 * @file weak_reference.h
 * @author ddamiana
 * @brief 
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "SharedReference.h"

template <typename T> class weak_reference {
  T *m_ptr{nullptr};
  SharedReference
<T> handle;

public:
  explicit weak_reference(SharedReference
<T> &i_ptr)
      : m_ptr(i_ptr.get()), handle(i_ptr) {
    i_ptr.suppress_increment();
  }

  weak_reference() = delete;
  weak_reference(const weak_reference<T> &) = delete;
  weak_reference(weak_reference<T> &&) = delete;
  ~weak_reference() {
    m_ptr = nullptr;
    handle.suppress_decrement();
  }

  weak_reference &operator=(weak_reference<T> &&) = delete;
  weak_reference &operator=(const weak_reference<T> &Type) = delete;

  T &operator*(void) { return *(this->m_ptr); }
  T *operator->(void) { return this->m_ptr; }
  T &operator&(weak_reference<T> &other) { return other.m_ptr; } // test this

  T *get(void) { return (this->m_ptr); }
  int count(void) { return handle.count(); }

  void release(void) noexcept { m_ptr = nullptr; }
  bool is_expired(void) noexcept { return m_ptr == nullptr; }
};