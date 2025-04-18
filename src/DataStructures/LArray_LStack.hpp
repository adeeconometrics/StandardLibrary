/**
 * @file ArrayStack.cpp
 * @author ddamiana
 * @brief Array-based Implementation of Stack.
 * @version 1.1
 * @date 2021-07-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <initializer_list>
#include <iostream>
#include <stdexcept>

template <typename T> class ArrayStack_Iterator {
public:
  typedef typename T::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr ArrayStack_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  ArrayStack_Iterator &operator++() {
    m_ptr++;
    return *this;
  }

  ArrayStack_Iterator operator++(int) {
    ArrayStack_Iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  reference_type operator*() { return *m_ptr; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const ArrayStack_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const ArrayStack_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

template <typename T> class cArrayStack_Iterator {
public:
  typedef typename T::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cArrayStack_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cArrayStack_Iterator &operator++() {
    m_ptr++;
    return *this;
  }

  cArrayStack_Iterator operator++(int) {
    ArrayStack_Iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  const reference_type operator*() const { return *m_ptr; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cArrayStack_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cArrayStack_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

/**
 * Summary of complexity on ArrayStack:
 * - void push(const T& data) = O(1)
 * - void pop() = O(1)
 * - void initialize() = O(n)
 * - void display() = O(n)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - T top() = O(1)
 * - T bottom() = O(1)
 * - bool is_full() = O(1)
 * - bool is_empty() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T, size_t Size> class ArrayStack {
public:
  typedef T value_type;
  typedef ArrayStack_Iterator<ArrayStack<T, Size>> iterator;
  typedef cArrayStack_Iterator<ArrayStack<T, Size>> const_iterator;

private:
  friend ArrayStack_Iterator<T>;
  friend cArrayStack_Iterator<T>;

  T *m_ptr{nullptr};
  size_t m_index{0};

public:
  explicit ArrayStack() {
    try {
      if (Size < 0)
        throw std::bad_alloc;
      initialize();
    } catch (const std::bad_alloc &e) {
      std::cerr << "Allocation failed: " << e.what
                << ". Size must be positive." std::endl;
      exit(1);
    }
  }

  explicit ArrayStack(std::initializer_list<T, Size> list) {
    try {
      if (Size < 0)
        throw std::bad_alloc();
      if (list.size() > Size)
        throw std::bad_alloc();

      m_ptr = new T[Size];

      initialize();

      for (auto i : list)
        push(i);

    } catch (const std::exception &e) {
      std::cerr << e.what << '\n';
      exit(1);
    }
  }

  explicit ArrayStack(ArrayStack<T, Size> &&other) noexcept {
    other.swap(*this);
  }

  explicit ArrayStack(const ArrayStack<T, Size> &other) {
    m_ptr = new T[Size];
    std::copy(other.begin(), other.end(), m_ptr);
  }

  ArrayStack<T, Size> &operator=(const ArrayStack<T, Size> &other) {
    Array<T, Size> copy(other).swap(other);
    return *this;
  }

  ArrayStack<T, Size> &operator=(ArrayStack<T, Size> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ~ArrayStack() {
    if (!is_empty())
      clear();
  }

  void push(const T &element) {
    try {
      if (is_full())
        throw std::length_error();

      m_ptr[m_front] = data;
      ++m_front;
    } catch (const std::exception &e) {
      std::cerr << e.what << '\n';
      exit(1);
    }
  }

  void pop() {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed request: Out of range.");

      m_ptr[m_index] = 0;
      m_index -= 1;

    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      exit(1);
    }
  }

  void initialize() noexcept {
    for (size_t i = 0; i < Size; ++i)
      m_ptr[i] = 0;
  }

  void display() const noexcept {
    T *ptr = m_ptr[0];
    for (size_t i = 0; i < Size; ++i)
      std::cout << ptr[i] << '\n';
  }

  void clear() {
    delete[] m_ptr;
    m_ptr = nullptr;
  }

  T top() const { return m_ptr[m_index]; }

  T bottom() const { return m_ptr[0]; }

  iterator begin() { return iterator(m_ptr); }

  iterator end() { return iterator(m_ptr + Size); }

  const_iterator cbegin() const { return const_iterator(m_ptr); }

  const_iterator cend() const { return const_iterator(m_ptr + Size); }

  bool is_full() const noexcept { return Size == m_index; }

  bool is_empty() const noexcept { return m_ptr == nullptr; }

private:
  void swap(ArrayStack<T, Size> &other) noexcept {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_index, other.m_index);
  }
};