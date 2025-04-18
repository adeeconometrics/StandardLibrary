/**
 * @file Vector.cpp
 * @author ddamiana
 * @brief Implementation of Vector with move and copy semantics.
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

template <typename Vector> class vector_iterator {
public:
  using value_type = typename Vector::value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  vector_iterator(pointer_type ptr) : m_ptr(ptr) {}

  vector_iterator &operator++() {
    m_ptr++;
    return *this;
  }

  vector_iterator operator++(int) {
    vector_iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  vector_iterator &operator--() {
    m_ptr--;
    return *this;
  }

  vector_iterator operator--(int) {
    vector_iterator iterator = *this;
    --(*this);
    return iterator;
  }

  reference_type operator[](int index) { return m_ptr[index]; }

  reference_type operator*() { return *m_ptr; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const vector_iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const vector_iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

template <typename Vector> class cvector_iterator {
public:
  using value_type = typename Vector::value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  cvector_iterator(pointer_type ptr) : m_ptr(ptr) {}

  cvector_iterator &operator++() {
    m_ptr++;
    return *this;
  }

  cvector_iterator operator++(int) {
    cvector_iterator iterator = *this;
    ++(*this);
    return iterator;
  }

  cvector_iterator &operator--() {
    m_ptr--;
    return *this;
  }

  cvector_iterator operator--(int) {
    cvector_iterator iterator = *this;
    --(*this);
    return iterator;
  }

  const reference_type operator[](int index) const { return m_ptr[index]; }

  const reference_type operator*() const { return *m_ptr; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cvector_iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cvector_iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

template <typename T> class Vector {
public:
  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  typedef T *pointer;
  typedef size_t size_type;
  typedef vector_iterator<Vector<T>> iterator;
  typedef cvector_iterator<Vector<T>> const_iterator;

private:
  size_t m_size{1};
  size_t m_index{0};
  T *m_ptr{nullptr};

public:
  explicit Vector(size_type i_size) {
    m_size = i_size;
    m_ptr = new T[m_size];
  }

  explicit Vector() { m_ptr = new T[m_size]; }

  explicit Vector(std::initializer_list<T> list) {
    m_ptr = new T[list.size()];
    m_size = list.size();
    for (auto i : list)
      push_back(i);
  }

  explicit Vector(Vector<T> &&other) noexcept { other.swap(*this); }

  explicit Vector(const Vector<T> &other) {
    m_ptr = new T[other.size()];
    m_size = other.size();
    std::copy(other.begin(), other.end(), m_ptr);
  }

  Vector<T> &operator=(Vector<T> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  Vector<T> &operator=(const Vector<T> &other) {
    Vector<T>(other).swap(*this);
    return *this;
  }

  ~Vector() {
    clear(); // this will call all of our destructors
    ::operator delete(
        m_ptr, m_size * 2 * sizeof(T)); // this will not call any destructors
  }

  void operator++() { ++m_index; }

  void operator++(int) { m_index++; }

  void operator--() { --m_index; }

  void operator--(int) { m_index--; }

  const_reference operator[](size_t idx) const {
    try {
      if (m_size < idx)
        throw std::domain_error("Vector index out of bound.");

      if (idx >= m_size)
        grow();

      return m_ptr[idx];
    } catch (const std::domain_error &de) {
      std::cerr << de.what();
      exit(1);
    }
  }

  reference operator[](size_t idx) {
    try {
      if (m_size < idx)
        throw std::domain_error("Vector index out of bound.");

      if (idx >= m_size)
        grow();

      return m_ptr[idx];
    } catch (const std::domain_error &de) {
      std::cerr << de.what();
      exit(1);
    }
  }

  void push_back(const T &a) {
    if (m_index >= m_size)
      grow();

    m_ptr[m_index] = a;
    ++m_index;
  }

  void push_back(T &&value) {
    if (m_index >= m_size)
      grow();

    m_ptr[m_index] = std::move(value);
    ++m_index;
  }

  void pop_back() {
    if (m_size > 0) {
      m_ptr[m_size].~T(); // can we do this for primitives? -- this still works
                          // with primities
      m_size -= 1;
    }
  }

  template <typename... Args> T &emplace_back(Args &&...args) {
    if (m_index >= m_size)
      grow();

    new (&m_ptr[size]) T(std::forward<Args>(args)...); // placement new
    return m_ptr[m_size++];
  }

  void clear() {
    for (size_t i = 0; i < m_size; ++i)
      m_ptr[i].~T();

    m_size = 0;
  }

  void display() const noexcept {
    for (size_t i = 0; i < m_index; ++i)
      std::cout << "a[" << i << "]: " << m_ptr[i] << std::endl;
  }

  void peek() const noexcept {
    for (size_t i = 0; i < m_size; ++i)
      std::cout << "a[" << i << "]: " << m_ptr[i] << std::endl;
  }

  // return iterator at a particular offset
  iterator begin() { return iterator(m_ptr); }

  iterator end() { return iterator(m_ptr + m_size); }

  const_iterator cbegin() { return const_iterator(m + ptr); }

  const_iterator cend() { return const_iterator(m + ptr + m_size); }

  size_type size() noexcept { return m_size; }

  void fit() {
    size_t t_size = m_size * 2;
    // instead of calling the constructor, we call new to allocate
    T *temp = (T *)::operator new(t_size * sizeof(T));
    for (size_t i = 0; i < m_index; ++i)
      temp[i] = std::move(m_ptr[i]);

    for (size_t i = 0; i < m_index; ++i)
      m_ptr[i].~T(); // calls the destructor and maintains RAII

    // instead calling the destructor
    ::operator delete(m_ptr, t_size * sizeof(T));
    m_ptr = nullptr;
    m_ptr = temp;
    m_size = m_index;
  }

private:
  void grow() {
    size_t t_size = m_size * 2;
    T *temp = (T *)::operator new(t_size * sizeof(T));

    for (size_t i = 0; i < m_index; ++i)
      temp[i] = std::move(m_ptr[i]);

    for (size_t i = 0; i < m_size; ++i)
      m_ptr[i].~T();

    ::operator delete(m_ptr, t_size * sizeof(T));
    m_ptr = nullptr;
    m_ptr = temp;
    m_size = t_size;
  }

  void swap(Vector<T> &other) noexcept {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_size, other.m_size);
    std::swap(m_index, other.m_index);
  }
};

int main() {
  Vector<size_t> v{1, 2, 3};

  for (size_t i = 0; i < 16; ++i)
    v.push_back(i + 2);

  v.fit();

  for (Vector<size_t>::iterator it = v.begin(); it != v.end(); ++it)
    std::cout << *it << "\n";
}