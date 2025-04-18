/**
 * @file CircularList.cpp
 * @author ddamiana
 * @brief CircularList
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

/**
 * todo:
 * - make CirularList an initializer_list constructor
 * - make CircularList asignable to initializer_list
 */

template <typename T> struct Node final {
  T data;
  Node<T> *next{nullptr};
  Node<T> *prev{nullptr};
  Node(const T &i_data) : data(i_data) {}
  Node() = default;
};

template <typename T> class List_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr List_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  List_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  List_Iterator &operator++(int) {
    List_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  List_Iterator &operator--() {
    m_ptr = m_ptr->prev;
    return *this;
  }

  List_Iterator &operator--(int) {
    List_Iterator temp = *this;
    m_ptr = m_ptr->prev;
    return temp;
  }

  reference_type operator*() { return m_ptr->next->data; }

  pointer_type operator->() { return m_ptr; }

  bool operator==(const List_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const List_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

template <typename T> class cList_Iterator {
public:
  typedef Node<T> value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cList_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cList_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return *this;
  }

  cList_Iterator &operator++(int) {
    cList_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  cList_Iterator &operator--() {
    m_ptr = m_ptr->prev;
    return *this;
  }

  cList_Iterator &operator--(int) {
    cList_Iterator temp = *this;
    m_ptr = m_ptr->prev;
    return temp;
  }

  const reference_type operator*() const { return m_ptr->next->data; }

  const pointer_type operator->() const { return m_ptr; }

  bool operator==(const cList_Iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cList_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type m_ptr{nullptr};
};

template <typename T> class CircularList {
private:
  friend List_Iterator<T>;
  friend cList_Iterator<T>;

  typedef Node<T> Node;

private:
  Node *m_front{nullptr};
  Node *m_back{nullptr};
  size_t m_size{0};

public:
  typedef cList_Iterator<T> const_iterator;
  typedef List_Iterator<T> iterator;

public:
  explicit CircularList() { m_front = m_back = new Node<T>(); }

  explicit CircularList(CircularList<T> &&other) noexcept { other.swap(*this); }

  explicit CircularList(const CircularList<T> &other) {
    Node<T> *ptr = other.m_front;
    while (ptr != nullptr) {
      add(ptr->data);
      ptr = ptr->next;
    }
  }

  CircularList &operator=(CircularList<T> &&other) {
    other.swap(*this);
    return *this;
  }

  CircularList &operator=(const CircularList<T> &other) noexcept {
    if (&other != this)
      CircularList<T>(other).swap(*this);

    return *this;
  }

  ~CircularList() {
    if (!is_empty())
      clear();
  }

  void add(const T &data) noexcept {
    Node *node = new Node(data);
    if (is_empty()) {
      m_front = node;
      m_back = node;
    } else {
      node->next = m_front;
      m_front->prev = node;
      m_front = node;
    }
    m_back->next = m_front;
    m_front->prev = m_back;
    m_size += 1;
  }

  void remove(const T &data) {
    try {
      Node *ptr = m_front;
      Node *prev = ptr;

      while (ptr->next != m_back) {
        if (ptr->data == data) {
          prev->next = ptr->next;
          delete ptr;
          ptr = nullptr;

          m_size -= 1;
          return;
        }
        prev = ptr;
        ptr = ptr->next;
      }

    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      exit(1);
    } catch (const char *msg) {
      std::cerr << msg << '\n';
      exit(1);
    }
  }

  void display() const noexcept {
    Node *ptr = m_front;
    while (ptr != m_back) {
      std::cout << ptr->data << " ";
      ptr = ptr->next;
    }
    std::cout << m_back->data;
  }

  void display_reverse() const noexcept {
    Node *ptr = m_back;
    while (ptr != m_front) {
      std::cout << ptr->data << " ";
      ptr = ptr->prev;
    }
    std::cout << m_front->data;
  }

  void clear() {
    Node *ptr = m_front;
    Node *temp{nullptr};
    while (ptr->next != nullptr) {
      temp = ptr;
      ptr = ptr->next;
      delete temp;
      temp = nullptr;
    }
    m_front = nullptr;
  }

  size_t size() const noexcept { return m_size; }

  const T top() const noexcept { return m_front->data; }

  const T bottom() const noexcept { return m_back->data; }

  iterator begin() { return iterator(front); }

  iterator end() { return iterator(back); }

  const_iterator cbegin() { return const_iterator(front); }

  const_iterator cend() { return const_iterator(back); }

  bool is_empty() const noexcept { return m_front == nullptr; }

private:
  void swap(CircularList<T> &other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
  }
};
