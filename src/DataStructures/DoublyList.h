/**
 * @file DoublyList.cpp
 * @author ddamiana
 * @brief Doubly Linked List
 * @version 1.1
 * @date 2021-07-28
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <initializer_list>
#include <iostream>

template <typename T> struct Node final {
  T data;
  Node *next{nullptr}, *prev{nullptr};
  Node(const T &m_data) : data(m_data) {}
  Node() = default;
};

template <typename T> class DoublyList_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr DoublyList_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  DoublyList_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  DoublyList_Iterator &operator++(int) {
    DoublyList_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  DoublyList_Iterator &operator--() {
    m_ptr = m_ptr->prev;
    return *this;
  }

  DoublyList_Iterator &operator--(int) {
    DoublyList_Iterator temp = *this;
    m_ptr = m_ptr->prev;
    return temp;
  }

  reference_type operator*() { return m_ptr->next->data; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const DoublyList_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const DoublyList_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

template <typename T> class cDoublyList_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cDoublyList_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cDoublyList_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  cDoublyList_Iterator &operator++(int) {
    cDoublyList_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  cDoublyList_Iterator &operator--() {
    m_ptr = m_ptr->prev;
    return *this;
  }

  cDoublyList_Iterator &operator--(int) {
    cDoublyList_Iterator temp = *this;
    m_ptr = m_ptr->prev;
    return temp;
  }

  const reference_type operator*() const { return m_ptr->next->data; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cDoublyList_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cDoublyList_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr;
};

/**
 * Summary of complexity on DoublyList:
 * - void add(const T& data) = O(1)
 * - void add_front(const T& data) = O(1)
 * - void add_back(const T& data) = O(1)
 * - void remove(const T& data)
 * - void display() = O(n)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - T top() = O(1)
 * - T bottom() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 * - bool is_empty() = O(1)
 */
template <typename T> class DoublyList {
private:
  friend DoublyList_Iterator<T>;
  friend cDoublyList_Iterator<T>;

  Node<T> *m_front{nullptr}, *m_back{nullptr};
  size_t m_size{0};

public:
  typedef cDoublyList_Iterator<T> const_iterator;
  typedef DoublyList_Iterator<T> iterator;

public:
  explicit DoublyList() { m_front = m_back = new Node<T>(); }

  explicit DoublyList(std::initializer_list<T> _DoublyList) {
    for (auto i : _DoublyList) {
      add(i);
      m_size += 1;
    }
  }

  explicit DoublyList(DoublyList<T> &&other) noexcept { other.swap(*this); }

  explicit DoublyList(const DoublyList<T> &other) {
    Node<T> *ptr = other.m_front;
    while (ptr != nullptr) {
      add(ptr->data);
      ptr = ptr->next;
    }
  }

  DoublyList<T> &operator=(DoublyList<T> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  DoublyList<T> &operator=(const DoublyList<T> &other) {
    if (&other != this)
      DoublyList<T>(other).swap(*this);

    return *this;
  }

  ~DoublyList() {
    if (!is_empty())
      clear();
  }

  void add(const T &data) noexcept {
    Node<T> *node = new Node<T>(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      node->prev = m_back;
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
      m_front->prev = node;
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
      node->prev = m_back;
      m_back = node;
    }
    ++m_size;
  }

  void remove(const T &data) {
    try {
      if (is_empty())
        throw std::out_of_range("Error: DoublyList is already empty.");

      else if (m_front->data == data) {
        remove_front();
        return;
      } else if (m_back->data == data) {
        remove_back();
        return;
      } else {
        Node<T> *ptr = m_front;
        Node<T> *prev = ptr;
        while (ptr->next != nullptr) {
          if (ptr->data == data) {
            prev->next = ptr->next;
            ptr->prev = prev;

            delete ptr;
            ptr = nullptr;
            m_size -= 1;
            return;
          }
          prev = ptr;
          ptr = ptr->next;
        }
        throw std::out_of_range("Error: element not found");
      }

    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
    }
  }

  void display() const noexcept {
    Node<T> *ptr = m_front;
    while (ptr != nullptr) {
      ptr = ptr->next;
      std::cout << ptr->data << " \n";
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

  const T top() const noexcept { return m_front->data; }

  const T bottom() const noexcept { return m_back->data; }

  iterator begin() { return iterator(m_front); }

  iterator end() { return iterator(m_back); }

  const_iterator cbegin() { return const_iterator(m_front); }

  const_iterator cend() { return const_iterator(m_back); }

  bool is_empty() const noexcept { return m_front == nullptr && m_size == 0; }

private:
  void remove_front() {
    Node<T> *ptr = m_front;
    m_front = m_front->next;

    delete ptr;
    ptr = nullptr;
    m_size -= 1;
  }

  void remove_back() {
    Node<T> *ptr = m_back;
    m_back = m_back->prev;

    delete ptr;
    ptr = nullptr;
    m_size -= 1;
  }

  void swap(DoublyList<T> &other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
  }
};