/**
 * @file SinglyList.cpp
 * @author ddamiana
 * @brief Singly Linked List
 * @version 1.1
 * @date 2021-07-28
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <initializer_list>
#include <iostream>

/**
 * featues to implement
 * - move semantics
 * - emplace feature
 * - proper implementation of destructors
 * - insert method
 */

template <typename T> class Node final {
public:
  T data;
  Node *next{nullptr};
  Node(const T &m_data) : data(m_data) {}
  Node() = default;
};

template <typename T> class SinglyList_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr SinglyList_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  SinglyList_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  SinglyList_Iterator &operator++(int) {
    SinglyList_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  reference_type operator*() { return m_ptr->next->data; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const SinglyList_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const SinglyList_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

template <typename T> class cSinglyList_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cSinglyList_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cSinglyList_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  cSinglyList_Iterator &operator++(int) {
    cSinglyList_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  const reference_type operator*() const { return m_ptr->next->data; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cSinglyList_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cSinglyList_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

/**
 * Summary of complexity on List:
 * - void add_front(const T& data) = O(1)
 * - void add_back(const T& data) = O(1)
 * - void remove() = O(1) (best-case), O(n) (worst-case)
 * - void display() = O(n)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T> class SinglyList {
private:
  friend SinglyList_Iterator<T>;
  friend cSinglyList_Iterator<T>;

  Node<T> *m_front{nullptr}, *m_back{nullptr};
  size_t m_size{0};

public:
  typedef cSinglyList_Iterator<T> const_iterator;
  typedef SinglyList_Iterator<T> iterator;

public:
  explicit SinglyList() { m_front = m_back = new Node<T>(); }

  explicit SinglyList(std::initializer_list<T> _list) {
    for (auto i : _list) {
      add(i);
      m_size += 1;
    }
  }

  // move constructor
  explicit SinglyList(SinglyList<T> &&other) noexcept { other.swap(*this); }
  // copy constructor
  explicit SinglyList(const SinglyList<T> &other) {
    Node<T> *ptr = other.m_front;
    while (ptr != nullptr) {
      add(ptr->data);
      ptr = ptr->next;
    }
  }
  // move assignment
  SinglyList &operator=(SinglyList<T> &&other) noexcept {
    other.swap(*this);
    return *this;
  }
  // copy assignment
  SinglyList &operator=(const SinglyList<T> &other) {
    if (&other != this)
      SinglyList<T>(other).swap(*this);

    return *this;
  }

  ~SinglyList() {
    if (!is_empty())
      clear();
  }

  void add(const T &data) noexcept {
    Node<T> *node = new Node<T>(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      m_back->next = node;
      m_back = node;
    }
    ++m_size;
  }

  void add_front(const T &data) noexcept {
    Node<T> *node = new Node<T>(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      node->next = m_front;
      m_front = node;
    }
    ++m_size;
  }

  void add_back(const T &data) noexcept {
    Node<T> *node = new Node<T>(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      m_back->next = node;
      m_back = node;
    }
    ++m_size;
  }

  void remove(const T &data) {
    try {
      if (is_empty())
        throw std::out_of_range("Error: list is already empty.");

      if (m_front->data == data)
        remove_front();

      Node<T> *ptr = m_front;
      Node<T> *prev = ptr;
      while (ptr->next != nullptr) {
        if (ptr->data == data) {
          if (ptr->data == m_back->data) {
            remove_back(prev);
            return;
          }

          prev->next = ptr->next;

          delete ptr;
          ptr = nullptr;
          m_size -= 1;
          return;
        }
        prev = ptr;
        ptr = ptr->next;
      }
      throw std::out_of_range("Error: element not found");

    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
    }
  }

  void display() const noexcept {
    Node<T> *ptr = m_front;
    while (ptr != nullptr) {
      std::cout << ptr->data << " \n";
      ptr = ptr->next;
    }
  }

  void clear() {
    if (is_empty())
      return;
    Node<T> *ptr = m_front;
    Node<T> *temp = ptr;

    while (ptr->next != nullptr) {
      temp = ptr;
      delete temp;
      temp = nullptr;

      ptr = ptr->next;
    }
    delete ptr;
    ptr = nullptr;
    m_size = 0;
  }

  size_t size() const noexcept { return m_size; }

  T top() const noexcept { return m_front->data; }

  T bottom() const noexcept { return m_back->data; }

  iterator begin() { return iterator(m_front); }

  iterator end() { return iterator(m_back); }

  const_iterator cbegin() { return const_iterator(m_front); }

  const_iterator cend() { return const_iterator(m_back); }

  bool is_empty() const noexcept { return m_front == nullptr && m_size == 0; }

private:
  void remove_back(Node<T> *prev) {
    Node<T> *temp = m_back;
    m_back = prev;
    delete temp;
    temp = nullptr;
  }

  void remove_front() {
    Node<T> *ptr = m_front;
    m_front = m_front->next;

    delete ptr;
    ptr = nullptr;
    m_size -= 1;

    return;
  }

  void swap(SinglyList<T> &other) noexcept {
    std::swap(m_size, other.size);
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
  }
};
