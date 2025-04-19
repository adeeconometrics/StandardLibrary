#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

/**
 * @brief Bidirectional iterator for Array container
 * 
 * @tparam ArrayType The array container type this iterator is for
 * 
 * @requires ArrayType must have value_type defined
 */
template<typename ArrayType>
class Array_Iterator {
public:
    using value_type = typename ArrayType::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

public:
    constexpr explicit Array_Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

    auto operator++() noexcept -> Array_Iterator& {
        m_ptr++;
        return *this;
    }

    auto operator++(int) noexcept -> Array_Iterator {
        Array_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator--() noexcept -> Array_Iterator& {
        m_ptr--;
        return *this;
    }

    auto operator--(int) noexcept -> Array_Iterator {
        Array_Iterator temp = *this;
        --(*this);
        return temp;
    }

    [[nodiscard]] auto operator[](difference_type index) noexcept -> reference { 
        return m_ptr[index]; 
    }
    
    [[nodiscard]] auto operator*() noexcept -> reference { 
        return *m_ptr; 
    }
    
    [[nodiscard]] auto operator->() noexcept -> pointer { 
        return m_ptr; 
    }

    [[nodiscard]] auto operator==(const Array_Iterator& other) const noexcept -> bool {
        return m_ptr == other.m_ptr;
    }

    [[nodiscard]] auto operator!=(const Array_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }

private:
    pointer m_ptr{nullptr};
};

/**
 * @brief Const bidirectional iterator for Array container
 * 
 * @tparam ArrayType The array container type this const iterator is for
 * 
 * @requires ArrayType must have value_type defined
 */
template<typename ArrayType>
class cArray_Iterator {
public:
    using value_type = typename ArrayType::value_type;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

public:
    constexpr explicit cArray_Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

    auto operator++() noexcept -> cArray_Iterator& {
        m_ptr++;
        return *this;
    }

    auto operator++(int) noexcept -> cArray_Iterator {
        cArray_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator--() noexcept -> cArray_Iterator& {
        m_ptr--;
        return *this;
    }

    auto operator--(int) noexcept -> cArray_Iterator {
        cArray_Iterator temp = *this;
        --(*this);
        return temp;
    }

    [[nodiscard]] auto operator[](difference_type index) const noexcept -> reference { 
        return m_ptr[index]; 
    }
    
    [[nodiscard]] auto operator*() const noexcept -> reference { 
        return *m_ptr; 
    }
    
    [[nodiscard]] auto operator->() const noexcept -> pointer { 
        return m_ptr; 
    }

    [[nodiscard]] auto operator==(const cArray_Iterator& other) const noexcept -> bool {
        return m_ptr == other.m_ptr;
    }

    [[nodiscard]] auto operator!=(const cArray_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }

private:
    pointer m_ptr{nullptr};
};

/**
 * @brief Fixed-size array container with compile-time size
 * 
 * @tparam ValueType The type of elements stored in the array
 * @tparam Size The fixed size of the array, must be > 0
 * 
 * @requires ValueType must be default constructible
 * 
 * Complexity guarantees:
 * - operator[]: O(1)
 * - add(): O(1)
 * - size(): O(1)
 * - empty(): O(1)
 * - begin(), end(): O(1)
 * - cbegin(), cend(): O(1)
 */
template<
    typename ValueType,
    size_t Size,
    typename = std::enable_if_t<(Size > 0)>,
    typename = std::enable_if_t<std::is_default_constructible_v<ValueType>>
>
class Array {
public:
    using value_type = ValueType;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = Array_Iterator<Array<ValueType, Size>>;
    using const_iterator = cArray_Iterator<Array<ValueType, Size>>;
    using size_type = size_t;

private:
    value_type* m_ptr{nullptr};
    size_type m_size{Size};
    size_type m_index{0};

public:
    constexpr explicit Array() {
        m_ptr = new value_type[Size]{};
    }

    explicit Array(const Array& other) {
        m_ptr = new value_type[Size];
        m_size = other.size();
        std::copy(other.cbegin(), other.cend(), m_ptr);
    }

    explicit Array(Array&& other) noexcept {
        swap(other);
    }

    explicit Array(std::initializer_list<value_type> list) {
        if (list.size() > Size) {
            throw std::length_error("Initializer list size exceeds array capacity");
        }
        m_ptr = new value_type[Size]{};
        std::copy(list.begin(), list.end(), m_ptr);
        m_index = list.size();
    }

    auto operator=(const Array& other) -> Array& {
        Array temp(other);
        swap(temp);
        return *this;
    }

    auto operator=(Array&& other) noexcept -> Array& {
        swap(other);
        return *this;
    }

    auto operator=(std::initializer_list<value_type> list) -> Array& {
        if (list.size() > Size) {
            throw std::length_error("Initializer list size exceeds array capacity");
        }
        std::copy(list.begin(), list.end(), m_ptr);
        m_index = list.size();
        return *this;
    }

    ~Array() {
        delete[] m_ptr;
        m_ptr = nullptr;
    }

    [[nodiscard]] auto operator[](size_type idx) const -> const_reference {
        if (idx >= Size) {
            throw std::out_of_range("Array index out of bounds");
        }
        return m_ptr[idx];
    }

    [[nodiscard]] auto operator[](size_type idx) -> reference {
        if (idx >= Size) {
            throw std::out_of_range("Array index out of bounds");
        }
        return m_ptr[idx];
    }

    auto add(const value_type& element) -> void {
        if (m_index >= Size) {
            throw std::length_error("Array is full");
        }
        m_ptr[m_index++] = element;
    }

    [[nodiscard]] constexpr auto size() const noexcept -> size_type { return Size; }
    [[nodiscard]] auto empty() const noexcept -> bool { return Size == 0; }

    [[nodiscard]] auto begin() noexcept -> iterator { return iterator(m_ptr); }
    [[nodiscard]] auto end() noexcept -> iterator { return iterator(m_ptr + Size); }
    [[nodiscard]] auto cbegin() const noexcept -> const_iterator { return const_iterator(m_ptr); }
    [[nodiscard]] auto cend() const noexcept -> const_iterator { return const_iterator(m_ptr + Size); }

private:
    auto swap(Array& other) noexcept -> void {
        using std::swap;
        swap(m_ptr, other.m_ptr);
        swap(m_size, other.m_size);
        swap(m_index, other.m_index);
    }
};

#endif // __ARRAY_HPP__