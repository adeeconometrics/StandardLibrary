#ifndef __ARRAY_STACK_HPP__
#define __ARRAY_STACK_HPP__

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace details {
template <typename T, typename = void> struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};
} // namespace details

/**
 * @brief Bidirectional iterator for ArrayStack container
 *
 * @tparam ArrayStackType The stack container type this iterator is for
 *
 * @requires ArrayStackType must have value_type defined
 *
 * @note This iterator provides bidirectional iteration over the stack elements
 */
template <typename ArrayStackType> class ArrayStack_Iterator {
public:
  using value_type = typename ArrayStackType::value_type;
  using pointer = value_type *;
  using reference = value_type &;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

public:
  constexpr explicit ArrayStack_Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

  auto operator++() noexcept -> ArrayStack_Iterator & {
    m_ptr++;
    return *this;
  }

  auto operator++(int) noexcept -> ArrayStack_Iterator {
    ArrayStack_Iterator temp = *this;
    ++(*this);
    return temp;
  }

  auto operator--() noexcept -> ArrayStack_Iterator & {
    m_ptr--;
    return *this;
  }

  auto operator--(int) noexcept -> ArrayStack_Iterator {
    ArrayStack_Iterator temp = *this;
    --(*this);
    return temp;
  }

  [[nodiscard]] auto operator*() noexcept -> reference { return *m_ptr; }
  [[nodiscard]] auto operator->() noexcept -> pointer { return m_ptr; }

  auto operator==(const ArrayStack_Iterator &other) const noexcept -> bool {
    return m_ptr == other.m_ptr;
  }

  auto operator!=(const ArrayStack_Iterator &other) const noexcept -> bool {
    return !(*this == other);
  }

private:
  pointer m_ptr{nullptr};
};

/**
 * @brief Const bidirectional iterator for ArrayStack container
 *
 * @tparam ArrayStackType The stack container type this const iterator is for
 *
 * @requires ArrayStackType must have value_type defined
 */
template <typename ArrayStackType> class cArrayStack_Iterator {
public:
  using value_type = typename ArrayStackType::value_type;
  using pointer = const value_type *;
  using reference = const value_type &;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

public:
  constexpr explicit cArrayStack_Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

  auto operator++() noexcept -> cArrayStack_Iterator & {
    m_ptr++;
    return *this;
  }

  auto operator++(int) noexcept -> cArrayStack_Iterator {
    cArrayStack_Iterator temp = *this;
    ++(*this);
    return temp;
  }

  auto operator--() noexcept -> cArrayStack_Iterator & {
    m_ptr--;
    return *this;
  }

  auto operator--(int) noexcept -> cArrayStack_Iterator {
    cArrayStack_Iterator temp = *this;
    --(*this);
    return temp;
  }

  [[nodiscard]] auto operator*() const noexcept -> reference { return *m_ptr; }
  [[nodiscard]] auto operator->() const noexcept -> pointer { return m_ptr; }

  auto operator==(const cArrayStack_Iterator &other) const noexcept -> bool {
    return m_ptr == other.m_ptr;
  }

  auto operator!=(const cArrayStack_Iterator &other) const noexcept -> bool {
    return !(*this == other);
  }

private:
  pointer m_ptr{nullptr};
};

/**
 * @brief Fixed-size stack container with compile-time size
 *
 * @tparam ValueType The type of elements stored in the stack
 * @tparam Size The fixed size of the stack, must be > 0
 *
 * @requires ValueType must be default constructible and copy constructible
 *
 * Complexity guarantees:
 * - Construction: O(1)
 * - Destruction: O(1)
 * - push(): O(1) amortized
 * - pop(): O(1)
 * - top(): O(1)
 * - size(): O(1)
 * - empty(): O(1)
 * - full(): O(1)
 * - begin(), end(): O(1)
 * - cbegin(), cend(): O(1)
 *
 * @example
 * ArrayStack<int, 5> stack;
 * stack.push(1);
 * stack.push(2);
 * assert(stack.top() == 2);
 * stack.pop();
 * assert(stack.top() == 1);
 */
template <
    typename ValueType, size_t Size, typename = std::enable_if_t<(Size > 0)>,
    typename = std::enable_if_t<std::is_default_constructible_v<ValueType> &&
                                std::is_copy_constructible_v<ValueType>>>
class ArrayStack {
public:
  using value_type = ValueType;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = ArrayStack_Iterator<ArrayStack>;
  using const_iterator = cArrayStack_Iterator<ArrayStack>;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  static_assert(Size > 0, "Stack size must be greater than 0");
  static_assert(std::is_default_constructible_v<ValueType>,
                "ValueType must be default constructible");
  static_assert(std::is_copy_constructible_v<ValueType>,
                "ValueType must be copy constructible");

private:
  pointer m_data{nullptr};
  size_type m_size{Size};
  size_type m_top{0};

public:
  // Constructors
  constexpr explicit ArrayStack() { m_data = new value_type[Size]{}; }

  explicit ArrayStack(std::initializer_list<value_type> init_list) {
    if (init_list.size() > Size) {
      throw std::length_error("Initializer list size exceeds stack capacity");
    }
    m_data = new value_type[Size]{};
    construct(init_list.begin(), init_list.end());
  }

  ArrayStack(const ArrayStack &other) {
    m_data = new value_type[Size];
    m_top = other.m_top;
    std::copy(other.m_data, other.m_data + m_top, m_data);
  }

  ArrayStack(ArrayStack &&other) noexcept { swap(other); }

  auto operator=(const ArrayStack &other) -> ArrayStack & {
    ArrayStack temp(other);
    swap(temp);
    return *this;
  }

  auto operator=(ArrayStack &&other) noexcept -> ArrayStack & {
    swap(other);
    return *this;
  }

  ~ArrayStack() { delete[] m_data; }

  // Element access
  [[maybe_unused]] auto top() const -> const_reference {
    if (is_empty()) {
      throw std::out_of_range("Cannot access top of empty stack");
    }
    return m_data[m_top - 1];
  }

  [[maybe_unused]] auto top() -> reference {
    if (is_empty()) {
      throw std::out_of_range("Cannot access top of empty stack");
    }
    return m_data[m_top - 1];
  }

  // Modifiers
  auto push(const value_type &value) -> void {
    if (is_full()) {
      throw std::length_error("Cannot push to full stack");
    }
    m_data[m_top++] = value;
  }

  auto push(value_type &&value) -> void {
    if (is_full()) {
      throw std::length_error("Cannot push to full stack");
    }
    m_data[m_top++] = std::move(value);
  }

  template <typename... Args> auto emplace(Args &&...args) -> void {
    if (is_full()) {
      throw std::length_error("Cannot emplace to full stack");
    }
    m_data[m_top++] = value_type(std::forward<Args>(args)...);
  }

  auto pop() -> void {
    if (is_empty()) {
      throw std::out_of_range("Cannot pop from empty stack");
    }
    --m_top;
  }

  auto clear() noexcept -> void { m_top = 0; }

  // Capacity
  [[nodiscard]] auto empty() const noexcept -> bool { return m_top == 0; }
  [[nodiscard]] auto is_empty() const noexcept -> bool { return empty(); }
  [[nodiscard]] auto full() const noexcept -> bool { return m_top == Size; }
  [[nodiscard]] auto is_full() const noexcept -> bool { return full(); }
  [[nodiscard]] auto size() const noexcept -> size_type { return m_top; }
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_type {
    return Size;
  }
  [[nodiscard]] constexpr auto max_size() const noexcept -> size_type {
    return Size;
  }

  // Iterators
  [[nodiscard]] auto begin() noexcept -> iterator { return iterator(m_data); }
  [[nodiscard]] auto end() noexcept -> iterator {
    return iterator(m_data + m_top);
  }
  [[nodiscard]] auto begin() const noexcept -> const_iterator {
    return const_iterator(m_data);
  }
  [[nodiscard]] auto end() const noexcept -> const_iterator {
    return const_iterator(m_data + m_top);
  }
  [[nodiscard]] auto cbegin() const noexcept -> const_iterator {
    return const_iterator(m_data);
  }
  [[nodiscard]] auto cend() const noexcept -> const_iterator {
    return const_iterator(m_data + m_top);
  }

private:
  auto swap(ArrayStack &other) noexcept -> void {
    using std::swap;
    swap(m_data, other.m_data);
    swap(m_top, other.m_top);
  }

  template <
      typename InputIt,
      typename = std::enable_if_t<std::is_convertible_v<
          typename std::iterator_traits<InputIt>::value_type, value_type>>>
  auto construct(InputIt first, InputIt last) -> void {
    while (first != last && !is_full()) {
      push(*first);
      ++first;
    }
  }
};

#endif // __ARRAY_STACK_HPP__
