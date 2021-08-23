/**
 * @file circular_squeue.h
 * @author ddamiana
 * @brief 
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <initializer_list>
#include <iostream>
#include <stdexcept>

template <typename T> class squeue_iterator {
public:
  typedef typename T::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr squeue_iterator(pointer_type ptr) : m_ptr(ptr) {}

  squeue_iterator &operator++() {
    m_ptr++;
    return *this;
  }

  squeue_iterator operator++(int) {
    squeue_iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  squeue_iterator &operator--() {
    m_ptr--;
    return *this;
  }

  squeue_iterator operator--(int) {
    squeue_iterator iterator = *this;
    --(*this);
    return iterator;
  }

  reference_type operator*() { return *m_ptr; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const squeue_iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const squeue_iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

template <typename T> class csqueue_iterator {
public:
  typedef typename T::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  explicit csqueue_iterator(pointer_type ptr) : m_ptr(ptr) {}

  csqueue_iterator &operator++() {
    m_ptr++;
    return *this;
  }

  csqueue_iterator operator++(int) {
    csqueue_iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  csqueue_iterator &operator--() {
    m_ptr--;
    return *this;
  }

  csqueue_iterator operator--(int) {
    csqueue_iterator iterator = *this;
    --(*this);
    return iterator;
  }

  const reference_type operator*() const { return *m_ptr; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const csqueue_iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const csqueue_iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

/**
 * Summary of complexity on CSQueue:
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
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T, size_t Size> class CSQueue {

public:
  typedef T value_type;
  typedef squeue_iterator<CSQueue<T, Size>> iterator;
  typedef csqueue_iterator<CSQueue<T, Size>> const_iterator;

private:
  friend squeue_iterator<T>;
  friend csqueue_iterator<T>;

  T *m_ptr{nullptr};
  size_t m_index{0};

public:
  explicit CSQueue() {
    m_ptr = new T[Size];
    // initalize elements to 0
    for (size_t i = 0; i < Size; ++i)
      m_ptr[i] = 0;
  }

  // explicit CSQueue(std::initalizer_list<T> list) {
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

  ~CSQueue() {
    if (!is_empty())
      clear();
  }

  const T &operator[](size_t idx) const {
    try {
      if (idx < 0)
        throw "Error.";

      return m_ptr[idx];
    } catch (const char *str) {
      std::cerr << str << std::endl;
      exit(1);
    }
  }

  void push(const T &element) {
    if (m_index < Size) {
      m_ptr[m_index] = element;
      ++m_index;
    } else {
      m_index = 0;
      m_ptr[m_index] = element;
    }
  }

  void enqueue(const T &element) {
    if (m_index < Size) {
      m_ptr[m_index] = element;
      ++m_index;
    } else {
      m_index = 0;
      m_ptr[m_index] = element;
    }
  }

  void dequeue() {
    if (m_index > 0) {
      m_ptr[m_index - 1] = 0;
      --m_index;
    } else {
      m_index = Size;
    }
  }

  void pop() {
    if (m_index > 0) {
      m_ptr[m_index - 1] = 0;
      --m_index;
    } else {
      m_index = Size;
    }
  }

  void display() const {
    T *ptr = m_ptr[0];
    for (size_t i = 0; i < Size; ++i)
      std::cout << ptr[i] << '\n';
  }

  void clear() {
    delete[] m_ptr;
    m_ptr = nullptr;
  }

  T top() const { return m_ptr[0]; }

  T bottom() const { return m_ptr[Size - 1]; }

  iterator begin() { return iterator(m_ptr); }

  iterator end() { return iterator(m_ptr + Size); }

  const_iterator cbegin() { return const_iterator(m_ptr); }

  const_iterator cend() { return const_iterator(m_ptr + Size); }

  bool is_empty() const { return m_ptr == nullptr; }

  bool is_full() const { return m_index == Size; }
};

int main() {
  CSQueue<int, 3> s;
  s.push(1);
  s.enqueue(2);
  s.push(3);
  s.pop();
  for (CSQueue<int, 3>::iterator it = s.begin(); it != s.end(); ++it)
    std::cout << *it << " ";

  std::cout << "\n";
  std::cout << "top : " << s.top() << '\n';
  std::cout << "bottom : " << s.bottom() << '\n';
}