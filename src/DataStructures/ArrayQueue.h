/**
 * @file ArrayQueue.cpp
 * @author ddamiana
 * @brief Array-based implementation of Queue.
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

template <typename T, size_t Size> class ArrayQueue_Iterator {
public:
  typedef typename T::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr ArrayQueue_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  ArrayQueue_Iterator &operator++() {
    m_ptr++;
    return *this;
  }

  ArrayQueue_Iterator operator++(int) {
    ArrayQueue_Iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  ArrayQueue_Iterator &operator--() {
    m_ptr--;
    return *this;
  }

  ArrayQueue_Iterator operator--(int) {
    ArrayQueue_Iterator iterator = *this;
    --(*this);
    return iterator;
  }

  reference_type operator*() { return *m_ptr; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const ArrayQueue_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const ArrayQueue_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

template <typename T, size_t Size> class cArrayQueue_Iterator {
public:
  typedef typename T::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  explicit cArrayQueue_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cArrayQueue_Iterator &operator++() {
    m_ptr++;
    return *this;
  }

  cArrayQueue_Iterator operator++(int) {
    cArrayQueue_Iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  cArrayQueue_Iterator &operator--() {
    m_ptr--;
    return *this;
  }

  cArrayQueue_Iterator operator--(int) {
    cArrayQueue_Iterator iterator = *this;
    --(*this);
    return iterator;
  }

  const reference_type operator*() const { return *m_ptr; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cArrayQueue_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cArrayQueue_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

/**
 * Summary of complexity on ArrayQueue:
 * - void enqueue(const T& data) = O(1)
 * - void push(const T& data) = O(1)
 * - void dequeue() = O(1)
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
template <typename T, size_t Size> class ArrayQueue {

public:
  typedef T value_type;
  typedef ArrayQueue_Iterator<ArrayQueue<T, Size>> iterator;
  typedef cArrayQueue_Iterator<ArrayQueue<T, Size>> const_iterator;

private:
  friend ArrayQueue_Iterator<T, Size>;
  friend cArrayQueue_Iterator<T, Size>;

  T *m_ptr{nullptr};
  size_t m_index{0};

public:
  explicit ArrayQueue() {
    m_ptr = new T[Size];
    initialize();
  }

  // explicit ArrayQueue(std::initalizer_list<T, Size> list) {
  // for (size_t i = 0; i < Size; ++i)
  //   m_ptr[i] = 0;
  //   try {
  //     if (list.size() > Size)
  //       throw std::exception;

  //     m_ptr = new T[Size];
  //     for (auto i : list)
  //       enqueue(i);
  //   } catch (const std::exception &e) {
  //     std::cerr << e.what() << '\n';
  //     exit(1);
  //   }
  // }

  explicit ArrayQueue(ArrayQueue<T, Size> &&other) noexcept {
    other.swap(*this);
  }

  explicit ArrayQueue(const ArrayQueue<T, Size> &other) {
    m_ptr = new T[Size];
    std::copy(other.begin(), other.end(), m_ptr);
  }

  ArrayQueue<T, Size> &operator=(const ArrayQueue<T, Size> &other) {
    Array<T, Size> copy(other).swap(other);
    return *this;
  }

  ArrayQueue<T, Size> &operator=(ArrayQueue<T, Size> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ~ArrayQueue() {
    if (!is_empty())
      clear();
  }

  const T &operator[](size_t idx) const {
    try {
      if (Size < idx)
        throw std::domain_error("Queue index out of bound.");

      return m_ptr[idx];
    } catch (const std::domain_error &de) {
      std::cerr << de.what() << '\n';
      exit(1);
    }
  }

  void push(const T &element) {
    try {
      if (is_full())
        throw std::length_error("Queue is already full.");

      if (m_index < Size) {
        m_ptr[m_index] = element;
        ++m_index;
      }
    } catch (const std::lenght_error &le) {
      std::cerr << le.what() << '\n';
      exit(1);
    }
  }

  void enqueue(const T &element) {
    try {
      if (is_full())
        throw std::length_error("Queue is already full.");

      if (m_index < Size) {
        m_ptr[m_index] = element;
        ++m_index;
      }
    } catch (const std::lenght_error &le) {
      std::cerr << le.what() << '\n';
      exit(1);
    }
  }

  void dequeue() {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed to request: Out of range.");
      m_ptr[m_index - 1] = 0;
      --m_index;
    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
    }
  }

  void pop() {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed to request: Out of range.");
      m_ptr[m_index - 1] = 0;
      --m_index;
    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
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

  T top() const noexcept { return m_ptr[0]; }

  T bottom() const noexcept { return m_ptr[Size - 1]; }

  iterator begin() { return iterator(m_ptr); }

  iterator end() { return iterator(m_ptr + Size); }

  const_iterator cbegin() { return const_iterator(m_ptr); }

  const_iterator cend() { return const_iterator(m_ptr + Size); }

  bool is_empty() const noexcept {
    for (size_t i = 0; i < Size; ++i) {
      if (m_ptr[i] != 0)
        return false;
    }
    return true;
  }

  bool is_full() const noexcept { return m_index == Size; }

private:
  void swap(ArrayQueue<T, Size> &other) noexcept {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_size, other.m_size);
    std::swap(index, other.index);
  }
};
