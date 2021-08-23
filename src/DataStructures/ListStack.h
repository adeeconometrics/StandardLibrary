/**
 * @file ListStack.cpp
 * @author ddamiana
 * @brief List implementation of Stack.
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

template <typename T> class Node final {
  T data;
  Node *next{nullptr};

  explicit Node() {}
  explicit Node(const T &_data) : data(_data) {}
};

template <typename T> class ListStack_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr ListStack_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  ListStack_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  ListStack_Iterator &operator++(int) {
    ListStack_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  reference_type operator*() { return m_ptr->next->data; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const ListStack_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const ListStack_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

template <typename T> class cListStack_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cListStack_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cListStack_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  cListStack_Iterator &operator++(int) {
    cListStack_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  const reference_type operator*() const { return m_ptr->next->data; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cListStack_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cListStack_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

/**
 * Summary of complexity on ListStack:
 * - void push(const T& data) = O(1)
 * - void pop() = O(1)
 * - void display() = O(n)
 * - T top() = O(1)
 * - T bottom() = O(1)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T> class ListStack {
private:
  friend ListStack_Iterator<T>;
  friend cListStack_Iterator<T>;

  Node<T> *m_front{nullptr}, *m_back{nullptr};
  size_t m_size{0};

public:
  typedef ListStack_Iterator<T> iterator;
  typedef cListStack_Iterator<T> const_iterator;

public:
  explicit ListStack(){};

  explicit ListStack(std::initializer_list<T> list) {
    for (auto i : list)
      push(i);
  };

  explicit ListStack(ListStack<T> &&other) noexcept { other.swap(*this); }

  explicit ListStack(const ListStack<T> &other) {
    Node<T> *ptr = other.m_front;
    while (ptr != nullptr) {
      push(ptr->data);
      ptr = ptr->next;
    }
  }

  ListStack &operator=(ListStack &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ListStack &operator=(const ListStack &other) {
    if (&other != this)
      ListStack<T>(other).swap(*this);

    return *this;
  }

  ~ListStack() {
    if (!is_empty())
      clear();
  };

  void push(const T &data) noexcept {
    Node<T> *node = new Node(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      node->next = m_front;
      m_front = node;
    }
    ++m_size;
  }

  void pop() {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed request: Out of range");
      m_front = m_front->next;
      --m_size;
    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
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

  void display() const noexcept {
    Node<T> *ptr = m_front;
    while (ptr->next != nullptr) {
      std::cout << ptr->data << std::endl;
      ptr = ptr->next;
    }
  }

  size_t size() const noexcept { return m_size; }

  T top() const noexcept { return m_front->data; }

  T bottom() const noexcept { return m_back->data; }

  iterator begin() { return iterator(m_front); }

  iterator end() { return iterator(m_back); }

  const_iterator cbegin() { return const_iterator(m_front); }

  const_iterator cend() { return const_iterator(m_back); }

  bool is_empty() const noexcept {
    return m_front == nullptr && m_back == nullptr;
  }

private:
  void swap(ListStack<T> &other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
  }
};
