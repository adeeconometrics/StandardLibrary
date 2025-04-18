/**
 * @file shared_reference.h
 * @author ddamiana
 * @brief 
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "WeakReference.h"

template <typename T> class shared_reference {
  template <typename> friend class WeakReference;
  static size_t m_counter;
  T *m_ptr;

public:
  shared_reference() = default;
  explicit shared_reference(T *i_ptr) : m_ptr(i_ptr) {}
  // explicit shared_reference(shared_reference<T>* rhs) = default;
  explicit shared_reference(shared_reference<T> &rhs) noexcept {
    rhs.copy(*this);
  }
  explicit shared_reference(shared_reference<T> &&rhs) noexcept {
    rhs.swap(*this);
  }

  ~shared_reference() {
    m_counter -= 1;
    if (m_counter == 0) {
      m_ptr->~T();
      m_ptr = nullptr;
    }
  }

  shared_reference<T> &operator=(shared_reference<T> &rhs) noexcept { 
    rhs.copy(*this); 
    return *this;
  }
  
  shared_reference<T> &operator=(shared_reference<T> &&rhs) noexcept {
     rhs.swap(*this);
     return *this;
  }

  T &operator*(void) { return *(this->m_ptr); }
  T *operator->(void) { return this->m_ptr; }
  T &operator&(shared_reference<T> &other) { return other.m_ptr; }
  T *get(void) { return this->m_ptr; }
  size_t count() const { return m_counter; }

private:
  void copy(shared_reference<T> &rhs) noexcept {
    m_counter += 1;
    rhs.m_ptr = m_ptr;
  }

  void swap(shared_reference<T> &rhs) noexcept {
    std::swap(m_ptr, rhs.m_ptr);
    rhs.m_counter = 1;
  }

  void suppress_increment(void) noexcept { m_counter -= 1; }

  void suppress_decrement(void) noexcept { m_counter += 1; }
};

template <typename T> size_t shared_reference<T>::m_counter = 1;