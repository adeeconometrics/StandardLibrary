#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename ArrayType>
class Array_Iterator {
public:
    using value_type = typename ArrayType::value_type;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

public:
    explicit Array_Iterator(pointer_type ptr) : m_ptr(ptr) {}

    Array_Iterator& operator++() {
        m_ptr++;
        return *this;
    }

    Array_Iterator operator++(int) {
        Array_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    Array_Iterator& operator--() {
        m_ptr--;
        return *this;
    }

    Array_Iterator operator--(int) {
        Array_Iterator temp = *this;
        --(*this);
        return temp;
    }

    reference_type operator[](int index) { return m_ptr[index]; }
    reference_type operator*() { return *m_ptr; }
    pointer_type operator->() { return m_ptr; }

    bool operator==(const Array_Iterator& other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Array_Iterator& other) const {
        return !(*this == other);
    }

private:
    pointer_type m_ptr;
};

/**
 * @brief Const iterator class for Array container
 * 
 * @tparam ArrayType The array container type this const iterator is for
 */
template <typename ArrayType>
class cArray_Iterator {
public:
    using value_type = typename ArrayType::value_type;
    using pointer_type = const value_type*;
    using reference_type = const value_type&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

public:
    explicit cArray_Iterator(pointer_type ptr) : m_ptr(ptr) {}

    cArray_Iterator& operator++() {
        m_ptr++;
        return *this;
    }

    cArray_Iterator operator++(int) {
        cArray_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    cArray_Iterator& operator--() {
        m_ptr--;
        return *this;
    }

    cArray_Iterator operator--(int) {
        cArray_Iterator temp = *this;
        --(*this);
        return temp;
    }

    reference_type operator[](int index) const { return m_ptr[index]; }
    reference_type operator*() const { return *m_ptr; }
    pointer_type operator->() const { return m_ptr; }

    bool operator==(const cArray_Iterator& other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const cArray_Iterator& other) const {
        return !(*this == other);
    }

private:
    pointer_type m_ptr;
};

/**
 * @brief Fixed-size array container with compile-time size
 * 
 * @tparam ValueType The type of elements stored in the array
 * @tparam Size The fixed size of the array, must be > 0
 * 
 * @requires ValueType must be default constructible
 */
template <
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
    explicit Array() {
        if (Size == 0) {
            throw std::length_error("Array size must be positive");
        }

        m_ptr = new value_type[Size]();
    }

    explicit Array(const Array& other) {
        m_ptr = new value_type[other.size()];
        m_size = other.size();
        std::copy(other.begin(), other.end(), m_ptr);
    }

    explicit Array(Array&& other) noexcept {
        swap(other);
    }

    explicit Array(std::initializer_list<value_type> list) {
        if (list.size() > Size) {
            throw std::length_error("Initializer list size exceeds array capacity");
        }

        m_ptr = new value_type[Size]();
        std::copy(list.begin(), list.end(), m_ptr);
        m_index = list.size();
    }

    ~Array() {
        delete[] m_ptr;
        m_ptr = nullptr;
    }

    Array& operator=(const Array& other) {
        Array copy(other);
        swap(copy);
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        swap(other);
        return *this;
    }

    Array& operator=(std::initializer_list<value_type> list) {
        if (list.size() > Size) {
            throw std::length_error("Initializer list size exceeds array capacity");
        }

        std::copy(list.begin(), list.end(), m_ptr);
        m_index = list.size();
        return *this;
    }

    const_reference operator[](size_type idx) const {
        if (idx >= Size) {
            throw std::out_of_range("Array index out of bounds");
        }
        return m_ptr[idx];
    }

    reference operator[](size_type idx) {
        if (idx >= Size) {
            throw std::out_of_range("Array index out of bounds");
        }
        return m_ptr[idx];
    }

    void add(const value_type& element) {
        if (m_index >= Size) {
            throw std::length_error("Array is full");
        }
        m_ptr[m_index++] = element;
    }

    [[nodiscard]] constexpr size_type size() const noexcept { return Size; }
    [[nodiscard]] bool empty() const noexcept { return Size == 0; }

    iterator begin() noexcept { return iterator(m_ptr); }
    iterator end() noexcept { return iterator(m_ptr + Size); }
    const_iterator cbegin() const noexcept { return const_iterator(m_ptr); }
    const_iterator cend() const noexcept { return const_iterator(m_ptr + Size); }

private:
    void swap(Array& other) noexcept {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_size, other.m_size);
        std::swap(m_index, other.m_index);
    }
};

#endif // ARRAY_HPP