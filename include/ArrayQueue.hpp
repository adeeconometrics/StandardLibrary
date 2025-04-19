#ifndef __ARRAY_QUEUE_H__
#define __ARRAY_QUEUE_H__

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <algorithm>

/**
 * @brief Forward iterator for ArrayQueue container
 * 
 * @tparam ValueType The type of elements in the queue
 * @tparam Size The fixed size of the queue
 */
template<typename ValueType, size_t Size>
class ArrayQueue_Iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;

public:
    constexpr explicit ArrayQueue_Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

    auto operator++() noexcept -> ArrayQueue_Iterator& {
        m_ptr++;
        return *this;
    }

    auto operator++(int) noexcept -> ArrayQueue_Iterator {
        ArrayQueue_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator--() noexcept -> ArrayQueue_Iterator& {
        m_ptr--;
        return *this;
    }

    auto operator--(int) noexcept -> ArrayQueue_Iterator {
        ArrayQueue_Iterator temp = *this;
        --(*this);
        return temp;
    }

    auto operator*() noexcept -> reference { return *m_ptr; }
    auto operator->() noexcept -> pointer { return m_ptr; }

    auto operator==(const ArrayQueue_Iterator& other) const noexcept -> bool {
        return m_ptr == other.m_ptr;
    }

    auto operator!=(const ArrayQueue_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }

private:
    pointer m_ptr{nullptr};
};

/**
 * @brief Const iterator for ArrayQueue container
 * 
 * @tparam ValueType The type of elements in the queue
 * @tparam Size The fixed size of the queue
 */
template<typename ValueType, size_t Size>
class cArrayQueue_Iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = std::ptrdiff_t;

public:
    constexpr explicit cArrayQueue_Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

    auto operator++() noexcept -> cArrayQueue_Iterator& {
        m_ptr++;
        return *this;
    }

    auto operator++(int) noexcept -> cArrayQueue_Iterator {
        cArrayQueue_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator--() noexcept -> cArrayQueue_Iterator& {
        m_ptr--;
        return *this;
    }

    auto operator--(int) noexcept -> cArrayQueue_Iterator {
        cArrayQueue_Iterator temp = *this;
        --(*this);
        return temp;
    }

    auto operator*() const noexcept -> reference { return *m_ptr; }
    auto operator->() const noexcept -> pointer { return m_ptr; }

    auto operator==(const cArrayQueue_Iterator& other) const noexcept -> bool {
        return m_ptr == other.m_ptr;
    }

    auto operator!=(const cArrayQueue_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }

private:
    pointer m_ptr{nullptr};
};

/**
 * @brief Fixed-size queue container with compile-time size
 * 
 * @tparam ValueType The type of elements stored in the queue
 * @tparam Size The fixed size of the queue, must be > 0
 * 
 * @requires ValueType must be default constructible
 * 
 * Complexity guarantees:
 * - enqueue(const T&): O(1)
 * - dequeue(): O(1)
 * - top(): O(1)
 * - bottom(): O(1)
 * - is_empty(): O(1)
 * - is_full(): O(1)
 * - begin(), end(): O(1)
 * - cbegin(), cend(): O(1)
 */
template<
    typename ValueType,
    size_t Size,
    typename = std::enable_if_t<(Size > 0)>,
    typename = std::enable_if_t<std::is_default_constructible_v<ValueType>>
>
class ArrayQueue {
public:
    using value_type = ValueType;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = ArrayQueue_Iterator<value_type, Size>;
    using const_iterator = cArrayQueue_Iterator<value_type, Size>;
    using size_type = size_t;

private:
    value_type* m_ptr{nullptr};
    size_type m_size{Size};
    size_type m_front{0};  // Index of the front element
    size_type m_count{0};  // Number of elements currently in queue

public:
    constexpr explicit ArrayQueue() {
        m_ptr = new value_type[Size]();
    }

    explicit ArrayQueue(const ArrayQueue& other) {
        m_ptr = new value_type[Size];
        m_front = other.m_front;
        m_count = other.m_count;
        for (size_type i = 0; i < m_count; ++i) {
            m_ptr[(m_front + i) % Size] = other.m_ptr[(other.m_front + i) % Size];
        }
    }

    explicit ArrayQueue(ArrayQueue&& other) noexcept {
        swap(other);
    }

    auto operator=(const ArrayQueue& other) -> ArrayQueue& {
        ArrayQueue temp(other);
        swap(temp);
        return *this;
    }

    auto operator=(ArrayQueue&& other) noexcept -> ArrayQueue& {
        swap(other);
        return *this;
    }

    ~ArrayQueue() {
        delete[] m_ptr;
    }

    auto operator[](size_type idx) const -> const_reference {
        if (idx >= m_count) {
            throw std::out_of_range("Queue index out of bounds");
        }
        return m_ptr[(m_front + idx) % Size];
    }

    auto operator[](size_type idx) -> reference {
        if (idx >= m_count) {
            throw std::out_of_range("Queue index out of bounds");
        }
        return m_ptr[(m_front + idx) % Size];
    }

    auto enqueue(const value_type& element) -> void {
        if (is_full()) {
            throw std::length_error("Queue is full");
        }
        size_type rear = (m_front + m_count) % Size;
        m_ptr[rear] = element;
        ++m_count;
    }

    auto dequeue() -> void {
        if (is_empty()) {
            throw std::out_of_range("Queue is empty");
        }
        m_ptr[m_front] = value_type();
        m_front = (m_front + 1) % Size;
        --m_count;
    }

    [[nodiscard]] auto top() const -> const_reference {
        if (is_empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return m_ptr[m_front];
    }

    [[nodiscard]] auto bottom() const -> const_reference {
        if (is_empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return m_ptr[(m_front + m_count - 1) % Size];
    }

    [[nodiscard]] auto begin() noexcept -> iterator { 
        return iterator(m_ptr + m_front); 
    }
    
    [[nodiscard]] auto end() noexcept -> iterator { 
        return iterator(m_ptr + ((m_front + m_count) % Size)); 
    }
    
    [[nodiscard]] auto cbegin() const noexcept -> const_iterator { 
        return const_iterator(m_ptr + m_front); 
    }
    
    [[nodiscard]] auto cend() const noexcept -> const_iterator { 
        return const_iterator(m_ptr + ((m_front + m_count) % Size)); 
    }

    [[nodiscard]] auto is_empty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto is_full() const noexcept -> bool { return m_count == Size; }
    [[nodiscard]] auto size() const noexcept -> size_type { return m_count; }
    [[nodiscard]] constexpr auto capacity() const noexcept -> size_type { return Size; }

private:
    auto swap(ArrayQueue& other) noexcept -> void {
        using std::swap;
        swap(m_ptr, other.m_ptr);
        swap(m_size, other.m_size);
        swap(m_front, other.m_front);
        swap(m_count, other.m_count);
    }
};

#endif // __ARRAY_QUEUE_H__