/**
 * @file ListQueue.cpp
 * @author ddamiana
 * @brief List implementation of Queue
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

template <typename T> struct Node final {
  T data;
  Node *next{nullptr};
  explicit Node() {}
  explicit Node(const T &_data) : data(_data) {}
};

template <typename T> class ListQueue_Iterator {
private:
  pointer_type m_ptr;

public:
  typedef Queue::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr ListQueue_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  ListQueue_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return m_ptr;
  }

  ListQueue_Iterator operator++(int) {
    ListQueue_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  reference_type operator*() { return *m_ptr; }

  pointer_type operator->() { return mm_ptr; }

  bool operator==(const ListQueue_Iterator &other) {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const ListQueue_Iterator &other) {
    return !(m_ptr == other.m_ptr);
  }
};

template <typename T> class cListQueue_Iterator {
private:
  pointer_type m_ptr;

public:
  typedef Queue::value_type value_type;
  typedef value_type *pointer_type;
  typedef value_type &reference_type;

public:
  constexpr cListQueue_Iterator(pointer_type ptr) : m_ptr(ptr) {}

  cListQueue_Iterator &operator++() {
    m_ptr = m_ptr->next;
    return m_ptr;
  }

  cListQueue_Iterator operator++(int) {
    cListQueue_Iterator temp = *this;
    m_ptr = m_ptr->next;
    return temp;
  }

  const reference_type operator*() const { return *m_ptr; }

  const pointer_type operator->() const { return mm_ptr; }

  bool operator==(const cListQueue_Iterator &other) {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const cListQueue_Iterator &other) {
    return !(m_ptr == other.m_ptr);
  }
};

/**
 * Summary of complexity on Queue:
 * - void enqueue(const T& data) = O(1)
 * - void add(const T& data) = O(1)
 * - void push(const T& data) = O(1)
 * - void dequeue() = O(1)
 * - void remove() = O(1)
 * - void pop() = O(1)
 * - void display() = O(n)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <typename T> class ListQueue {
private:
  friend ListQueue_Iterator<T>;
  friend cListQueue_Iterator<T>;

  Node<T> *m_front{nullptr}, *m_back{nullptr}, *m_ptr{nullptr};
  size_t m_size{0};

public:
  typedef ListQueue_Iterator<T> iterator;
  typedef cListQueue_Iterator<T> const_iterator;

public:
  explicit ListQueue() { m_front = m_back = new Node<T>(); }

  explicit ListQueue(ListQueue<T> &&other) noexcept { other.swap(*this); }

  explicit ListQueue(const ListQueue<T> &other) {
    Node<T> *ptr = other.m_front;
    while (ptr != nullptr) {
      add(ptr->data);
      ptr = ptr->next;
    }
  }

  explicit ListQueue(std::initializer_list<T> list) {
    for (std::initializer_list<T>::iterator it = list.begin(); it != list.end();
         ++it)
      enqueue(*it);
  }

  ListQueue<T> &operator=(ListQueue<T> &&other) noexcept {
    other.swap(*this);
    return *this;
  }

  ListQueue<T> &operator=(const ListQueue<T> &other) {
    if (&other != this)
      SinglyList<T>(other).swap(*this);

    return *this;
  }

  ~ListQueue() {
    if (!is_empty())
      clear();
  }

  void equeue(const T &element) noexcept {
    Node *node = new Node(element);
    if (is_empty()) {
      m_ptr = node;
      m_front = node;
      m_back = node;
    } else {
      node->next = m_back;
      m_back = node;
    }
    m_size += 1;
  }

  void push(const T &element) noexcept {
    Node *node = new Node(element);
    if (is_empty()) {
      m_ptr = node;
      m_front = node;
      m_back = node;
    } else {
      node->next = m_back;
      m_back = node;
    }
    m_size += 1;
  }

  void add(const T &element) noexcept {
    Node *node = new Node(element);
    if (is_empty()) {
      m_ptr = node;
      m_front = node;
      m_back = node;
    } else {
      node->next = m_back;
      m_back = node;
    }
    m_size += 1;
  }

  void pop() {
    try {
      if (is_empty())
        throw std::out_of_range();

      Node *temp = m_front;
      m_front = m_front->next;
      delete temp;
      temp = nullptr;

      m_size -= 1;

    } catch (const std::out_of_range &ore) {
      std::cout << ore.what() << '\n';
      exit(1);
    }
  }

  void dequeue() {
    try {
      if (is_empty())
        throw std::out_of_range;

      Node *temp = m_front;
      m_front = m_front->next;
      delete temp;
      temp = nullptr;

      m_size -= 1;

    } catch (const std::out_of_range &ore) {
      std::cout << ore.what() << '\n';
      exit(1);
    }
  }

  void display() const noexcept {
    Node *ptr = m_front;
    std::cout << "[ ";
    while (ptr != nullptr) {
      std::cout << ptr->data << ", ";
      ptr = ptr->next;
    }
    std::cout << "] " << std::endl;
  }

  void clear() {
    Node *ptr = m_front;
    Node *temp = ptr;

    while (ptr->next != nullptr) {
      temp = ptr;
      delete temp;
      temp = nullptr;
      ptr = ptr->next;
    }

    m_back = nullptr;
    m_ptr = nullptr;
    m_size = 0;
  }

  size_t size() const noexcept { return m_size; }

  T top() const noexcept { return m_front->data; }

  T bottom() const noexcept { return m_back->data; }

  iterator begin() { return ListQueue_Iterator(m_front); }

  iterator end() { return ListQueue_Iterator(m_back); }

  const_iterator cbegin() { return const_iterator(m_front); }

  const_iterator cend() { return const_iterator(m_back); }

  bool is_empty() const noexcept { return m_front == nullptr && m_size == 0; }

private:
  void swap(ListQueue<T> &other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_front, other.m_front);
    std::swap(m_back, other.m_back);
  }
};