/**
 * @file ArrayList.cpp
 * @author ddamiana
 * @brief  Randomizeable list
 * @version 0.3 -- with array iterator
 * @date 2021-07-27
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
/**
 * todo
 * - make ArrayList copy_constructible
 * - make ArrayList move_constructible
 * - make ArrayList copy_assignable
 * - make ArrayList move_assignable
 * - make iterator work!
 * - proper overloads
 */

template <typename T> struct Node final {
  Node *next{nullptr};
  T value;

  Node(T i_value) : value(i_value) {}
  Node() = default;
};

template <typename Node> class ArrayList_Iterator {

public:
  typedef Node value_type;
  typedef value_type *pointer_type;
  typedef value_type *&reference_type;

public:
  ArrayList_Iterator(Node *entry_point, size_t size, bool is_back = false) {
    if (is_back) {
      if (m_captured)
        m_array = m_array + size;
    }
    m_array = new value_type *[size];
    pointer_type ptr = entry_point;
    for (size_t i = 0; i < size; ++i) {
      m_array[i] = ptr;
      ptr = ptr->next;
    }
    m_captured = true;
  }

  ~ArrayList_Iterator() {
    delete[] m_array;
    m_array = nullptr;
  }

  reference_type operator++() { return *m_array++; }

  reference_type operator++(int) { return *++m_array; }

  //  ArrayList_Iterator &operator--() {
  //    m_array--;
  //    return *this;
  //  }
  //
  //  ArrayList_Iterator &operator--(int) {
  //    value_type temp = m_array;
  //    --m_array;
  //    return temp;
  //  }

  reference_type operator[](int index) { return *m_array[index]->value; }

  pointer_type *operator*() { return *&m_array; }

  pointer_type operator->() { return m_array; }

  bool operator==(const ArrayList_Iterator &other) const {
    return m_array == other.m_array;
  }

  bool operator!=(const ArrayList_Iterator &other) const {
    return !(*this == other);
  }

private:
  pointer_type *m_array{nullptr};
  bool m_captured{false};
};

template <typename T, size_t Block_Size = 1> class ArrayList {
public:
  typedef ArrayList_Iterator<Node<T>> iterator;
  typedef Node<T> Node;

private:
  friend ArrayList_Iterator<Node>;
  Node *m_front{nullptr}, *m_back{nullptr};
  Node **m_array{nullptr};

  size_t m_size{0};
  size_t m_index{0};
  bool m_captured{false};

public:
  ArrayList() = default;
  ~ArrayList() { clear(); }

  void add(int value) {
    Node *node = new Node(value);

    if (m_front == nullptr) {
      m_front = node;
      m_back = node;
    } else {
      m_back->next = node;
      m_back = node;
    }
    m_size += 1;
  }

  void remove(int value) {
    try {
      if (is_empty())
        throw std::out_of_range("Cannot proceed request: Out of range.");

      Node *ptr{nullptr};
      if (m_front->value == value) {
        ptr = m_front;
        m_front = m_front->next;
        delete ptr;
        ptr = nullptr;
        m_size -= 1;
      } else {
        Node *prev{nullptr};
        ptr = m_front;

        while (ptr->next != nullptr) {
          prev = ptr;
          ptr = ptr->next;
          if (ptr->value == value) {
            prev->next = ptr->next;
            delete ptr;
            ptr = nullptr;
            m_size -= 1;
            break;
          }
        }
      }
      throw std::invalid_argument("Value is not found in the list.");
    } catch (const std::out_of_range &ore) {
      std::cerr << ore.what() << '\n';
      exit(1);
    } catch (const std::invalid_argument &ie) {
      std::cerr << ie.what() << '\n';
    }
  }

  void clear() {
    if (m_array != nullptr) {
      release();
    }

    Node *ptr{m_front};
    Node *temp{nullptr};

    while (ptr->next != nullptr) {
      temp = ptr;
      delete temp;
      temp = nullptr;
      ptr = ptr->next;
    }
    m_size = 0;
  }

  void capture() {
    m_captured = true;
    m_array = new Node *[m_size];

    Node *ptr = m_front;
    for (size_t i = 0; i < m_size; ++i, ptr = ptr->next) {
      m_array[i] = ptr;
    }
  }

  void release() {
    delete[] m_array;
    m_array = nullptr;
  }

  int &operator[](size_t index) {
    if (m_captured == false)
      capture();

    return *&m_array[index]->value;
  }

  void operator++() {
    if (m_captured == false)
      capture();

    m_array++;
    return;
  }

  void operator++(int) {
    if (m_captured == false)
      capture();

    ++m_array;
    return;
  }

  void operator--() {
    if (m_captured == false)
      capture();

    m_array--;
    return;
  }

  void operator--(int) {
    if (m_captured == false)
      capture();

    --m_array;
    return;
  }

  void display() const {
    for (Node *ptr = m_front; ptr->next != nullptr; ptr = ptr->next) {
      std::cout << ptr->value << " ";
    }
  }

  size_t size() const { return m_size; }

  iterator begin() { return iterator(m_front, m_size); }

  iterator end() { return iterator(m_back, m_size, true); }

  bool is_empty() const { return m_front == nullptr; }
};
