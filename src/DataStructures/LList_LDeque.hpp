/**
 * @file ListDeque.cpp
 * @author ddamiana
 * @brief List implementation of Deque. Note that constructors are default to
 * stack.
 * @version 1.1
 * @date 2021-07-28
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <initializer_list>
#include <iostream>
#include <stdexcept>
template <typename T> struct Node final {
  T data;
  Node *next{nullptr}, *prev{nullptr};

  explicit Node(){};
  explicit Node(const T &_data) : data(_data) {}
};

template <typename ListDeque> class ListDeque_Iterator {

public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr ListDeque_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  ListDeque_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  ListDeque_Iterator &operator++(int) {
    ListDeque_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  ListDeque_Iterator &operator--() {
    m_ptr = m_ptr->prev;
    return *this;
  }

  ListDeque_Iterator &operator--(int) {
    ListDeque_Iterator temp = *this;
    m_ptr = m_ptr->prev;
    return temp;
  }

  reference_type operator*() { return m_ptr->next->data; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const ListDeque_Iterator &rhs) { return m_ptr == rhs.m_ptr; }

  bool operator!=(const ListDeque_Iterator &rhs) { return !(*this == rhs); }

private:
  pointer_type m_ptr;
};

template <typename ListDeque> class cListDeque_Iterator {

public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cListDeque_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cListDeque_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  cListDeque_Iterator &operator++(int) {
    cListDeque_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  cListDeque_Iterator &operator--() {
    m_ptr = m_ptr->prev;
    return *this;
  }

  cListDeque_Iterator &operator--(int) {
    cListDeque_Iterator temp = *this;
    m_ptr = m_ptr->prev;
    return temp;
  }

  const reference_type operator*() const { return m_ptr->next->data; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cListDeque_Iterator &rhs) { return m_ptr == rhs.m_ptr; }

  bool operator!=(const cListDeque_Iterator &rhs) { return !(*this == rhs); }

private:
  pointer_type m_ptr;
};

/**
 * Summary of complexity on ListDeque:
 * - void push_front(const T& data) = O(1)
 * - void push_back(const T& data) = O(1)
 * - void pop_front() = O(1)
 * - void pop_back() = O(1)
 * - void display() = O(n)
 * - void display_reverse() = O(n)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T> class ListDeque {
private:
  friend ListDeque_Iterator<T>;
  friend cListDeque_Iterator<T>;

  Node<T> *m_front{nullptr}, *m_back{nullptr};
  size_t m_size{0};

public:
  typedef ListDeque_Iterator<T> iterator;
  typedef cListDeque_Iterator<T> const_iterator;

public:
  explicit ListDeque() { m_front = m_back = new Node<T>(); }

  explicit ListDeque(std::initializer_list<T> _list) {
    for (auto i : list)
      push_front(i);
  }

  explicit ListDeque(ListDeque<T> &&other) noexcept { other.swap(*this); }

  explicit ListDeque(const ListDeque<T> &other) {
    Node<T> *ptr = other.m_front;
    while (ptr != nullptr) {
      push_front(ptr->data);
      ptr = ptr->next;
    }
  }

  ListDeque<T> &operator=(ListDeque<T> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ListDeque<T> &operator=(const ListDeque<T> &other) {
    if (&other != this)
      ListDeque<T>(other).swap(*this);

    return *this;
  }

  ~ListDeque() {
    if (!is_empty())
      clear();
  }

  void push_front(const T &data) noexcept {
    Node<T> *node = new Node(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      node->next = m_front;
      node->prev = m_front->prev;
      m_front = node;
    }
    ++m_size;
  }

  void push_back(const T &data) noexcept {
    Node<T> *node = new Node(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      m_back->next = node;
      node->prev = m_back;
      m_back = node;
    }
    ++m_size;
  }

  void pop_front() {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed request: Out of range.");

      Node<T> *ptr = m_front;
      m_front = m_front->next;

      delete ptr;
      ptr = nullptr;
      --m_size;
    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
    }
  }

  void pop_back() {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed request: Out of range.");

      Node<T> *ptr = m_back;
      m_back = m_back->prev;
      delete ptr;
      ptr = nullptr;

      --m_size;
    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
    }
  }

  void display() const noexcept {
    Node<T> *ptr = m_front;
    while (ptr != nullptr) {
      std::cout << ptr->data << std::endl;
      ptr = ptr->next;
    }
  }

  void display_reverse() const noexcept {
    Node<T> *ptr = m_back;
    while (ptr->prev != nullptr) {
      ptr = ptr->prev;
      std::cout << ptr->data << std::endl;
    }
  }

  void clear() {
    Node<T> *ptr = m_front;
    Node<T> *temp = ptr;
    while (ptr->next != nullptr) {
      temp = ptr;
      delete temp;
      temp = nullptr;
      ptr = ptr->next;
    }

    m_size = 0;
  }

  size_t size() const noexcept { return m_size; }

  iterator begin() { return iterator(m_front); }

  iterator end() { return iterator(m_back); }

  const_iterator cbegin() { return const_iterator(begin); }

  const_iterator cend() { return const_iterator(end); }

  bool is_empty() const noexcept {
    return m_front = nullptr && m_back == nullptr;
  }

private:
  void swap(ListDeque<T> &other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
  }
};