#ifndef __DOUBLY_LIST_HPP__
#define __DOUBLY_LIST_HPP__

#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T> struct Node final {
  T data;
  Node *next{nullptr}, *prev{nullptr};
  explicit Node(const T &m_data) : data(m_data) {}
  Node() = default;
};

template <typename T> class IteratorProxy final {
private:
  using node_type =
      std::conditional_t<std::is_const_v<T>, const Node<std::remove_const_t<T>>,
                         Node<T>>;
  node_type *m_current;
  node_type *m_last_valid;

public:
  constexpr explicit IteratorProxy(node_type *current = nullptr,
                                   node_type *last = nullptr) noexcept
      : m_current(current), m_last_valid(last) {}

  [[nodiscard]] auto current() const noexcept -> Node<T> * { return m_current; }

  [[nodiscard]] auto last_valid() const noexcept -> Node<T> * {
    return m_last_valid;
  }

  auto is_end() const noexcept -> bool { return m_current == nullptr; }

  auto move_forward() -> void {
    if (m_current != nullptr) {
      m_last_valid = m_current;
      m_current = m_current->next;
    }
  }

  auto move_backward() -> void {
    if (m_current == nullptr && m_last_valid != nullptr) {
      m_current = m_last_valid;
    } else if (m_current != nullptr && m_current->prev != nullptr) {
      m_current = m_current->prev;
    }
  }
  [[nodiscard]] auto data() const
      -> std::conditional_t<std::is_const_v<T>, const std::remove_const_t<T> &,
                            T &> {
    if (m_current == nullptr) {
      throw std::out_of_range("Cannot dereference end iterator");
    }
    return m_current->data;
  }
  auto operator==(const IteratorProxy &other) const noexcept -> bool {
    return m_current == other.m_current;
  }

  auto operator==(std::nullptr_t) const noexcept -> bool {
    return m_current == nullptr;
  }
};

// Non-member operator== to handle nullptr on the left side
template <typename T>
auto operator==(std::nullptr_t, const IteratorProxy<T> &proxy) noexcept
    -> bool {
  return proxy == nullptr;
}

template <typename T> class DoublyList_Iterator {
public:
  // Standard iterator type traits
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using reference = T &;
  using iterator_type = DoublyList_Iterator;

private:
  IteratorProxy<T> m_proxy;

public:
  constexpr explicit DoublyList_Iterator(Node<T> *current = nullptr,
                                         Node<T> *last = nullptr) noexcept
      : m_proxy(current, last) {}
  auto operator++() -> DoublyList_Iterator & {
    if (m_proxy == nullptr) {
      throw std::out_of_range("Cannot increment end iterator");
    }
    m_proxy.move_forward();
    return *this;
  }

  auto operator++(int) -> DoublyList_Iterator {
    DoublyList_Iterator temp = *this;
    ++(*this);
    return temp;
  }
  auto operator--() -> DoublyList_Iterator & {
    m_proxy.move_backward();
    return *this;
  }

  auto operator--(int) -> DoublyList_Iterator {
    auto temp = *this;
    --(*this);
    return temp;
  }
  auto operator*() const -> reference { return m_proxy.data(); }

  auto operator->() const -> pointer { return &(m_proxy.data()); }

  auto operator==(const DoublyList_Iterator &other) const noexcept -> bool {
    return m_proxy == other.m_proxy;
  }

  auto operator!=(const DoublyList_Iterator &other) const noexcept -> bool {
    return !(*this == other);
  }
};

template <typename T> class cDoublyList_Iterator {
public:
  // Standard iterator type traits
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T *;
  using reference = const T &;
  using iterator_type = cDoublyList_Iterator;

  // Enable conversion from non-const iterator
  friend class DoublyList_Iterator<T>;

private:
  IteratorProxy<const T> m_proxy;

public:
  constexpr explicit cDoublyList_Iterator(
      const Node<T> *current = nullptr, const Node<T> *last = nullptr) noexcept
      : m_proxy(current, last) {}
  auto operator++() -> cDoublyList_Iterator & {
    m_proxy.move_forward();
    return *this;
  }

  auto operator++(int) -> cDoublyList_Iterator {
    cDoublyList_Iterator temp = *this;
    ++(*this);
    return temp;
  }

  auto operator--() -> cDoublyList_Iterator & {
    m_proxy.move_backward();
    return *this;
  }

  auto operator--(int) -> cDoublyList_Iterator {
    auto temp = *this;
    --(*this);
    return temp;
  }
  auto operator*() const -> reference { return m_proxy.data(); }

  auto operator->() const -> pointer { return &(m_proxy.data()); }

  auto operator==(const cDoublyList_Iterator &other) const noexcept -> bool {
    return m_proxy == other.m_proxy;
  }

  auto operator!=(const cDoublyList_Iterator &other) const noexcept -> bool {
    return !(*this == other);
  }
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
  static_assert(std::is_default_constructible_v<T>,
                "Type T must be default constructible");
  static_assert(std::is_copy_constructible_v<T>,
                "Type T must be copy constructible");

private:
  friend class DoublyList_Iterator<T>;
  friend class cDoublyList_Iterator<T>;

  Node<T> *m_front{nullptr};
  Node<T> *m_back{nullptr};
  size_t m_size{0};

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = DoublyList_Iterator<T>;
  using const_iterator = cDoublyList_Iterator<T>;

public:
  constexpr DoublyList() noexcept = default;

  constexpr explicit DoublyList(std::initializer_list<T> init) {
    for (const auto &value : init) {
      add_back(value);
    }
  }

  DoublyList(const DoublyList &other) {
    for (const auto &value : other) {
      add_back(value);
    }
  }

  constexpr DoublyList(DoublyList &&other) noexcept
      : m_front(other.m_front), m_back(other.m_back), m_size(other.m_size) {
    other.m_front = other.m_back = nullptr;
    other.m_size = 0;
  }

  auto operator=(const DoublyList &other) -> DoublyList & {
    if (this != &other) {
      DoublyList temp(other);
      swap(temp);
    }
    return *this;
  }

  auto operator=(DoublyList &&other) noexcept -> DoublyList & {
    if (this != &other) {
      clear();
      m_front = other.m_front;
      m_back = other.m_back;
      m_size = other.m_size;
      other.m_front = other.m_back = nullptr;
      other.m_size = 0;
    }
    return *this;
  }

  ~DoublyList() {
    if (!is_empty())
      clear();
  }
  auto add(const T &data) -> void { add_back(data); }

  auto add_front(const T &data) -> void {
    auto *node = new Node<T>(data);
    if (is_empty()) {
      m_front = m_back = node;
    } else {
      node->next = m_front;
      m_front->prev = node;
      m_front = node;
    }
    ++m_size;
  }

  auto add_back(const T &data) -> void {
    auto *node = new Node<T>(data);
    if (is_empty()) {
      m_front = m_back = node;
    } else {
      node->prev = m_back;
      m_back->next = node;
      m_back = node;
    }
    ++m_size;
  }

  auto remove(const T &data) -> void {
    if (is_empty()) {
      throw std::out_of_range("Cannot remove from empty list");
    }

    if (m_front->data == data) {
      auto *temp = m_front;
      m_front = m_front->next;
      if (m_front) {
        m_front->prev = nullptr;
      } else {
        m_back = nullptr;
      }
      delete temp;
      --m_size;
      return;
    }

    for (auto *curr = m_front; curr != nullptr; curr = curr->next) {
      if (curr->data == data) {
        if (curr == m_back) {
          m_back = curr->prev;
          m_back->next = nullptr;
        } else {
          curr->prev->next = curr->next;
          curr->next->prev = curr->prev;
        }
        delete curr;
        --m_size;
        return;
      }
    }
    throw std::out_of_range("Element not found in list");
  }

  auto clear() noexcept -> void {
    while (m_front != nullptr) {
      auto *temp = m_front;
      m_front = m_front->next;
      delete temp;
    }
    m_back = nullptr;
    m_size = 0;
  }
  [[nodiscard]] auto size() const noexcept -> size_type { return m_size; }

  [[nodiscard]] auto top() const -> const_reference {
    if (is_empty()) {
      throw std::out_of_range("Cannot access top of empty list");
    }
    return m_front->data;
  }

  [[nodiscard]] auto bottom() const -> const_reference {
    if (is_empty()) {
      throw std::out_of_range("Cannot access bottom of empty list");
    }
    return m_back->data;
  }
  auto begin() noexcept -> iterator { return iterator(m_front, nullptr); }

  auto end() noexcept -> iterator { return iterator(nullptr, m_back); }

  auto begin() const noexcept -> const_iterator {
    return const_iterator(m_front, nullptr);
  }

  auto end() const noexcept -> const_iterator {
    return const_iterator(nullptr, m_back);
  }

  auto cbegin() const noexcept -> const_iterator {
    return const_iterator(m_front, nullptr);
  }

  auto cend() const noexcept -> const_iterator {
    return const_iterator(nullptr, m_back);
  }

  [[nodiscard]] auto is_empty() const noexcept -> bool { return m_size == 0; }

private:
  auto remove_front() -> void {
    if (m_front == nullptr)
      return;

    Node<T> *temp = m_front;
    m_front = m_front->next;

    if (m_front != nullptr) {
      m_front->prev = nullptr;
    } else {
      m_back = nullptr;
    }

    delete temp;
    --m_size;
  }

  auto remove_back() -> void {
    if (m_back == nullptr)
      return;

    Node<T> *temp = m_back;
    m_back = m_back->prev;

    if (m_back != nullptr) {
      m_back->next = nullptr;
    } else {
      m_front = nullptr;
    }

    delete temp;
    --m_size;
  }
  auto swap(DoublyList &other) noexcept -> void {
    using std::swap;
    swap(m_size, other.m_size);
    swap(m_front, other.m_front);
    swap(m_back, other.m_back);
  }
};

#endif // __DOUBLY_LIST_HPP__