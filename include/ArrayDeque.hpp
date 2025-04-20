#ifndef __ARRAY_DEQUE_HPP__
#define __ARRAY_DEQUE_HPP__

#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <iterator>
#include <utility>
#include <algorithm>

namespace details {
    template<typename T, typename = void>
    struct is_iterable : std::false_type {};

    template<typename T>
    struct is_iterable<T, std::void_t<
        decltype(std::begin(std::declval<T>())),
        decltype(std::end(std::declval<T>()))
    >> : std::true_type {};
}

/**
 * @brief Bidirectional iterator for ArrayDeque container
 * 
 * @tparam ArrayDequeType The deque container type this iterator is for
 * 
 * @requires ArrayDequeType must have value_type defined
 * 
 * @note This iterator properly handles the circular buffer wraparound
 */
template<typename ArrayDequeType>
class ArrayDeque_Iterator {
public:
    using value_type = typename ArrayDequeType::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    pointer m_ptr{nullptr};         // Current element pointer
    pointer m_begin{nullptr};       // Start of buffer
    pointer m_end{nullptr};         // End of buffer
    size_t m_index{0};             // Current logical index
    size_t m_size{0};              // Size of deque

public:
    constexpr ArrayDeque_Iterator(pointer ptr, pointer begin, pointer end, size_t index, size_t size) noexcept 
        : m_ptr(ptr), m_begin(begin), m_end(end), m_index(index), m_size(size) {}

    auto operator++() noexcept -> ArrayDeque_Iterator& {
        if (m_ptr == m_end - 1) {
            m_ptr = m_begin;
        } else {
            ++m_ptr;
        }
        ++m_index;
        return *this;
    }

    auto operator++(int) noexcept -> ArrayDeque_Iterator {
        ArrayDeque_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator--() noexcept -> ArrayDeque_Iterator& {
        if (m_ptr == m_begin) {
            m_ptr = m_end - 1;
        } else {
            --m_ptr;
        }
        --m_index;
        return *this;
    }

    auto operator--(int) noexcept -> ArrayDeque_Iterator {
        ArrayDeque_Iterator temp = *this;
        --(*this);
        return temp;
    }

    [[nodiscard]] auto operator*() noexcept -> reference { return *m_ptr; }
    [[nodiscard]] auto operator->() noexcept -> pointer { return m_ptr; }

    auto operator==(const ArrayDeque_Iterator& other) const noexcept -> bool {
        return m_index == other.m_index;
    }

    auto operator!=(const ArrayDeque_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }
};

/**
 * @brief Const bidirectional iterator for ArrayDeque container
 */
template<typename ArrayDequeType>
class cArrayDeque_Iterator {
public:
    using value_type = typename ArrayDequeType::value_type;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    pointer m_ptr{nullptr};
    pointer m_begin{nullptr};
    pointer m_end{nullptr};
    size_t m_index{0};
    size_t m_size{0};

public:
    constexpr cArrayDeque_Iterator(pointer ptr, pointer begin, pointer end, size_t index, size_t size) noexcept 
        : m_ptr(ptr), m_begin(begin), m_end(end), m_index(index), m_size(size) {}

    auto operator++() noexcept -> cArrayDeque_Iterator& {
        if (m_ptr == m_end - 1) {
            m_ptr = m_begin;
        } else {
            ++m_ptr;
        }
        ++m_index;
        return *this;
    }

    auto operator++(int) noexcept -> cArrayDeque_Iterator {
        cArrayDeque_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator--() noexcept -> cArrayDeque_Iterator& {
        if (m_ptr == m_begin) {
            m_ptr = m_end - 1;
        } else {
            --m_ptr;
        }
        --m_index;
        return *this;
    }

    auto operator--(int) noexcept -> cArrayDeque_Iterator {
        cArrayDeque_Iterator temp = *this;
        --(*this);
        return temp;
    }

    [[nodiscard]] auto operator*() const noexcept -> reference { return *m_ptr; }
    [[nodiscard]] auto operator->() const noexcept -> pointer { return m_ptr; }

    auto operator==(const cArrayDeque_Iterator& other) const noexcept -> bool {
        return m_index == other.m_index;
    }

    auto operator!=(const cArrayDeque_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }
};

/**
 * @brief Fixed-size double-ended queue with compile-time size
 * 
 * @tparam ValueType The type of elements stored in the deque
 * @tparam Size The fixed size of the deque, must be > 0
 * 
 * @requires ValueType must be default constructible and copy constructible
 * 
 * Complexity guarantees:
 * - Construction: O(1)
 * - Destruction: O(1)
 * - push_front(), push_back(): O(1)
 * - pop_front(), pop_back(): O(1)
 * - front(), back(): O(1)
 * - size(): O(1)
 * - empty(), full(): O(1)
 * 
 * @example
 * ArrayDeque<int, 5> deque;
 * deque.push_back(1);
 * deque.push_front(0);
 * assert(deque.front() == 0);
 * assert(deque.back() == 1);
 */
template<
    typename ValueType,
    size_t Size,
    typename = std::enable_if_t<(Size > 0)>,
    typename = std::enable_if_t<
        std::is_default_constructible_v<ValueType> &&
        std::is_copy_constructible_v<ValueType>
    >
>
class ArrayDeque {
public:
    using value_type = ValueType;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ArrayDeque_Iterator<ArrayDeque>;
    using const_iterator = cArrayDeque_Iterator<ArrayDeque>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    static_assert(Size > 0, "Deque size must be greater than 0");
    static_assert(std::is_default_constructible_v<ValueType>,
                 "ValueType must be default constructible");
    static_assert(std::is_copy_constructible_v<ValueType>,
                 "ValueType must be copy constructible");

private:
    pointer m_data{nullptr};
    size_type m_front{0};
    size_type m_back{0};
    size_type m_count{0};

    // Helper functions for index management
    [[nodiscard]] auto next_index(size_type index) const noexcept -> size_type {
        return (index + 1) % Size;
    }

    [[nodiscard]] auto prev_index(size_type index) const noexcept -> size_type {
        return (index + Size - 1) % Size;
    }

public:
    constexpr ArrayDeque() {
        m_data = new value_type[Size]{};
    }

    explicit ArrayDeque(std::initializer_list<value_type> init_list) {
        if (init_list.size() > Size) {
            throw std::length_error("Initializer list size exceeds deque capacity");
        }
        m_data = new value_type[Size]{};
        for (const auto& value : init_list) {
            push_back(value);
        }
    }

    ArrayDeque(const ArrayDeque& other) {
        m_data = new value_type[Size];
        m_front = other.m_front;
        m_back = other.m_back;
        m_count = other.m_count;
        std::copy(other.m_data, other.m_data + Size, m_data);
    }

    ArrayDeque(ArrayDeque&& other) noexcept {
        swap(other);
    }

    auto operator=(const ArrayDeque& other) -> ArrayDeque& {
        ArrayDeque temp(other);
        swap(temp);
        return *this;
    }

    auto operator=(ArrayDeque&& other) noexcept -> ArrayDeque& {
        swap(other);
        return *this;
    }

    ~ArrayDeque() {
        delete[] m_data;
    }

    [[nodiscard]] auto front() -> reference {
        if (empty()) {
            throw std::out_of_range("Cannot access front of empty deque");
        }
        return m_data[m_front];
    }

    [[nodiscard]] auto front() const -> const_reference {
        if (empty()) {
            throw std::out_of_range("Cannot access front of empty deque");
        }
        return m_data[m_front];
    }

    [[nodiscard]] auto back() -> reference {
        if (empty()) {
            throw std::out_of_range("Cannot access back of empty deque");
        }
        return m_data[prev_index(m_back)];
    }

    [[nodiscard]] auto back() const -> const_reference {
        if (empty()) {
            throw std::out_of_range("Cannot access back of empty deque");
        }
        return m_data[prev_index(m_back)];
    }

    auto push_front(const value_type& value) -> void {
        if (full()) {
            throw std::length_error("Cannot push to full deque");
        }
        m_front = prev_index(m_front);
        m_data[m_front] = value;
        ++m_count;
    }

    auto push_front(value_type&& value) -> void {
        if (full()) {
            throw std::length_error("Cannot push to full deque");
        }
        m_front = prev_index(m_front);
        m_data[m_front] = std::move(value);
        ++m_count;
    }

    auto push_back(const value_type& value) -> void {
        if (full()) {
            throw std::length_error("Cannot push to full deque");
        }
        m_data[m_back] = value;
        m_back = next_index(m_back);
        ++m_count;
    }

    auto push_back(value_type&& value) -> void {
        if (full()) {
            throw std::length_error("Cannot push to full deque");
        }
        m_data[m_back] = std::move(value);
        m_back = next_index(m_back);
        ++m_count;
    }

    template<typename... Args>
    auto emplace_front(Args&&... args) -> void {
        if (full()) {
            throw std::length_error("Cannot emplace to full deque");
        }
        m_front = prev_index(m_front);
        m_data[m_front] = value_type(std::forward<Args>(args)...);
        ++m_count;
    }

    template<typename... Args>
    auto emplace_back(Args&&... args) -> void {
        if (full()) {
            throw std::length_error("Cannot emplace to full deque");
        }
        m_data[m_back] = value_type(std::forward<Args>(args)...);
        m_back = next_index(m_back);
        ++m_count;
    }

    auto pop_front() -> void {
        if (empty()) {
            throw std::out_of_range("Cannot pop from empty deque");
        }
        m_front = next_index(m_front);
        --m_count;
    }

    auto pop_back() -> void {
        if (empty()) {
            throw std::out_of_range("Cannot pop from empty deque");
        }
        m_back = prev_index(m_back);
        --m_count;
    }

    auto clear() noexcept -> void {
        m_front = 0;
        m_back = 0;
        m_count = 0;
    }

    [[nodiscard]] auto empty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto is_empty() const noexcept -> bool { return empty(); }
    [[nodiscard]] auto full() const noexcept -> bool { return m_count == Size; }
    [[nodiscard]] auto is_full() const noexcept -> bool { return full(); }
    [[nodiscard]] auto size() const noexcept -> size_type { return m_count; }
    [[nodiscard]] constexpr auto capacity() const noexcept -> size_type { return Size; }
    [[nodiscard]] constexpr auto max_size() const noexcept -> size_type { return Size; }

    [[nodiscard]] auto begin() noexcept -> iterator {
        return iterator(m_data + m_front, m_data, m_data + Size, 0, m_count);
    }

    [[nodiscard]] auto end() noexcept -> iterator {
        return iterator(m_data + m_back, m_data, m_data + Size, m_count, m_count);
    }

    [[nodiscard]] auto begin() const noexcept -> const_iterator {
        return const_iterator(m_data + m_front, m_data, m_data + Size, 0, m_count);
    }

    [[nodiscard]] auto end() const noexcept -> const_iterator {
        return const_iterator(m_data + m_back, m_data, m_data + Size, m_count, m_count);
    }

    [[nodiscard]] auto cbegin() const noexcept -> const_iterator { return begin(); }
    [[nodiscard]] auto cend() const noexcept -> const_iterator { return end(); }

private:
    auto swap(ArrayDeque& other) noexcept -> void {
        using std::swap;
        swap(m_data, other.m_data);
        swap(m_front, other.m_front);
        swap(m_back, other.m_back);
        swap(m_count, other.m_count);
    }
};

#endif // __ARRAY_DEQUE_HPP__
