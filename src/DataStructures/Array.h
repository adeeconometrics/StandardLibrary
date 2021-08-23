/**
 * @file Array.cpp
 * @author ddamiana
 * @brief Array implementation with move semantics.
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

template <typename Array> class Array_Iterator {

public:
  typedef typename Array::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  Array_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  Array_Iterator &operator++() {
    m_ptr++;
    return *this;
  }

  Array_Iterator &operator++(int) {
    Array_Iterator temp = *this;
    ++(*this);
    return temp;
  }

  Array_Iterator &operator--() {
    m_ptr--;
    return *this;
  }

  Array_Iterator &operator--(int) {
    Array_Iterator temp = *this;
    --(*this);
    return temp;
  }

  reference_type operator[](int index) { return m_ptr[index]; }

  reference_type operator*() { return *m_ptr; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const Array_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const Array_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

template <typename Array> class cArray_Iterator {

public:
  typedef typename Array::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  cArray_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cArray_Iterator &operator++() {
    m_ptr++;
    return *this;
  }

  cArray_Iterator &operator++(int) {
    Array_Iterator temp = *this;
    ++(*this);
    return temp;
  }

  cArray_Iterator &operator--() {
    m_ptr--;
    return *this;
  }

  cArray_Iterator &operator--(int) {
    cArray_Iterator temp = *this;
    --(*this);
    return temp;
  }

  const reference_type operator[](int index) const { return m_ptr[index]; }

  const reference_type operator*() const { return *m_ptr; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cArray_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cArray_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

/**
 * Summary of complexity on Array:
 * - void add(const T& element) = O(1)
 * - void initialize() = O(n)
 * - void display() = O(n)
 * - size_t size() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T, size_t Size> class Array {
public:
  typedef m_ptr value_type;
  typedef Array_Iterator<Array<T, Size>> iterator;
  typedef cArray_Iterator<Array<T, Size>> const_iterator;

private:
  T *m_ptr{nullptr};
  size_t m_size{Size};
  size_t index{0};

public:
  explicit Array() {
    try {
      if (Size < 0)
        throw std::bad_alloc();

      index = 0;
      m_ptr = new T[size];

      initialize();

    } catch (const std::bad_alloc &e) {
      std::cerr << "Allocation failed: " << e.what << ". Size must be positive."
                << std::endl;
      exit(1);
    }
  }

  explicit Array(const Array<T> &other) {
    m_ptr = new T[other.size()];
    m_size = other.size();
    std::copy(other.begin(), other.end(), m_ptr);
  }

  explicit Array(Array &&other) noexcept { other.swap(*this); }

  explicit Array(std::initializer_list<T> list) {
    try {
      if (Size < 0)
        throw std::bad_alloc;

      if (list.size() > Size)
        throw std::bad_alloc();

      initialize();

      for (std::list_initalizer<T>::iterator it = list.begin();
           it != list.end(); ++it)
        add(*it);

    } catch (const std::bad_alloc &be) {
      std::cerr << "Allocation failed: " << be.what()
                << ". Input size goes beyond array size." << '\n';
      exit(1);
    }
  }

  Array &operator=(const Array &other) {
    Array<T> copy(other).swap(other);
    return *this;
  }

  Array &operator=(Array &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ~Array() {
    delete[] m_ptr;
    m_ptr = nullptr;
  }

  Array &operator=(std::initializer_list<T> list) {
    index = 0;
    m_size = list.size();

    for (std::initializer_list<T>::iterator it = list.begin(); it != list.end();
         ++it)
      add(*it);
  }

  void operator++() { m_ptr[++index]; }

  void operator++(int) { m_ptr[index++]; }

  void operator--() { m_ptr[--index]; }

  void operator--(int) { m_ptr[index--]; }

  const T &operator[](size_t idx) const {
    try {
      if (idx < 0 || idx < size)
        throw std::domain_error("Array index out of bound");

      return m_ptr[idx];
    } catch (const std::domain_error &de) {
      std::cerr << de.what() << '\n';
      exit(1);
    }
  }

  T &operator[](size_t idx) {
    try {
      if (idx < 0 || idx < size)
        throw std::domain_error("Array index out of bound");

      return m_ptr[idx];
    } catch (const std::domain_error &de) {
      std::cerr << de.what() << '\n';
      exit(1);
    }
  }

  void add(const T &element) {
    m_ptr[index] = element;
    index += 1;
  }

  void initialize() {
    // initalize elements to 0
    for (size_t i = 0; i < Size; ++i)
      m_ptr[i] = 0;
  }

  // void remove(const T &element);

  void display() const {
    for (size_t i = 0; i < size; ++i)
      std::cout << "a[" << i << "] " << m_ptr[i] << "\n";
  }

  size_t size() noexcept { return Size; }

  iterator begin() { return Array_Iterator(m_ptr); }

  iterator end() { return Array_Iterator(m_ptr + Size); }

  const_iterator cbegin() { return const_iterator(m + ptr); }

  const_iterator cend() { return const_iterator(m + ptr + m_size); }

  bool is_empty() const noexcept { return m_ptr == nullptr && size == 0; }

private:
  void swap(Array<T> &other) noexcept {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_size, other.m_size);
    std::swap(index, other.index);
  }
};