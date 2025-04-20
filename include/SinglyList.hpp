#ifndef __SINGLY_LIST_HPP__
#define __SINGLY_LIST_HPP__

#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include <utility>

template <typename T>
class Node final {
public:
    T data;
    Node* next{nullptr};
    
    explicit Node(const T& m_data) : data(m_data) {}
    Node() = default;
};

template <typename T>
class SinglyList_Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using node_pointer = Node<T>*;

public:
    constexpr explicit SinglyList_Iterator(node_pointer ptr) noexcept : m_ptr(ptr) {}

    auto operator++() noexcept -> SinglyList_Iterator& {
        if (m_ptr != nullptr) {
            m_ptr = m_ptr->next;
        }
        return *this;
    }

    auto operator++(int) noexcept -> SinglyList_Iterator {
        SinglyList_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator*() const noexcept -> reference { 
        return m_ptr->data;
    }

    auto operator->() const noexcept -> pointer { 
        return std::addressof(m_ptr->data);
    }

    auto operator==(const SinglyList_Iterator& other) const noexcept -> bool {
        return m_ptr == other.m_ptr;
    }

    auto operator!=(const SinglyList_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }

private:
    node_pointer m_ptr;
};

template <typename T>
class cSinglyList_Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
    using node_pointer = const Node<T>*;

public:
    constexpr explicit cSinglyList_Iterator(node_pointer ptr) noexcept : m_ptr(ptr) {}

    auto operator++() noexcept -> cSinglyList_Iterator& {
        if (m_ptr) {
            m_ptr = m_ptr->next;
        }
        return *this;
    }

    auto operator++(int) noexcept -> cSinglyList_Iterator {
        cSinglyList_Iterator temp = *this;
        ++(*this);
        return temp;
    }

    auto operator*() const noexcept -> reference { 
        return m_ptr->data;
    }

    auto operator->() const noexcept -> pointer { 
        return std::addressof(m_ptr->data);
    }

    auto operator==(const cSinglyList_Iterator& other) const noexcept -> bool {
        return m_ptr == other.m_ptr;
    }

    auto operator!=(const cSinglyList_Iterator& other) const noexcept -> bool {
        return !(*this == other);
    }

private:
    node_pointer m_ptr;
};

/**
 * Summary of complexity on List:
 * - void add_front(const T& data) = O(1)
 * - void add_back(const T& data) = O(1)
 * - void remove() = O(1) (best-case), O(n) (worst-case)
 * - void clear() = O(n)
 * - size_t size() = O(1)
 * - iterator begin() = O(1)
 * - iterator end() = O(1)
 * - const_iterator cbegin() = O(1)
 * - const_iterator cend() = O(1)
 */
template <
    typename T,
    typename = std::enable_if_t<
        std::is_default_constructible_v<T> &&
        std::is_copy_constructible_v<T>
    >
>
class SinglyList {
private:
    Node<T>* m_front{nullptr};
    Node<T>* m_back{nullptr};
    size_t m_size{0};

public:
    using iterator = SinglyList_Iterator<T>;
    using const_iterator = cSinglyList_Iterator<T>;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;

public:
    explicit SinglyList() { m_front = m_back = new Node<T>(); }

    explicit SinglyList(std::initializer_list<T> _list) {
        for (auto i : _list) {
            add(i);
        }
    }   

    explicit SinglyList(SinglyList&& other) noexcept 
        : m_front(other.m_front)
        , m_back(other.m_back)
        , m_size(other.m_size) {
        other.m_front = nullptr;
        other.m_back = nullptr;
        other.m_size = 0;
    }

    explicit SinglyList(const SinglyList& other) {
        for (auto it = other.cbegin(); it != other.cend(); it++) {
            add(*it);
        }
    }

    auto operator=(SinglyList&& other) noexcept -> SinglyList& {
        if (this != &other) {
            clear();
            m_front = other.m_front;
            m_back = other.m_back;
            m_size = other.m_size;
            other.m_front = nullptr;
            other.m_back = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    auto operator=(const SinglyList& other) -> SinglyList& {
        if (this != &other) {
            SinglyList temp(other);
            swap(temp);
        }
        return *this;
    }

    ~SinglyList() noexcept {
        clear();
    }

    void add(const T &data) noexcept {
        Node<T> *node = new Node<T>(data);
        if (is_empty()) {
            m_front = node;
            m_back = node;
        } else {
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
            m_back = node;
        }
        ++m_size;
    }

    auto remove(const T& data) -> void {
        if (is_empty()) {
            throw std::out_of_range("Cannot remove from empty list");
        }

        if (m_front->data == data) {
            remove_front();
            return;
        }

        Node<T>* current = m_front;
        while (current->next != nullptr) {
            if (current->next->data == data) {
                Node<T>* to_delete = current->next;
                if (to_delete == m_back) {
                    m_back = current;
                    m_back->next = nullptr;
                } else {
                    current->next = to_delete->next;
                }
                delete to_delete;
                --m_size;
                return;
            }
            current = current->next;
        }
        throw std::out_of_range("Element not found in list");
    }

    auto remove_front() -> void {
        if (is_empty()) {
            throw std::out_of_range("Cannot remove from empty list");
        }
        Node<T>* temp = m_front;
        m_front = m_front->next;
        delete temp;
        --m_size;
        
        if (m_size == 0) {
            m_back = nullptr;
        }
    }

    auto remove_back(Node<T>* prev) noexcept -> auto {
        Node<T>* temp = m_back;
        m_back = prev;
        m_back->next = nullptr;
        delete temp;
        --m_size;
    }

    auto clear() noexcept -> void {
        while (m_front != nullptr) {
            Node<T>* temp = m_front;
            m_front = m_front->next;
            delete temp;
        }
        m_back = nullptr;
        m_size = 0;
    }

    size_t size() const noexcept { return m_size; }

    T top() const noexcept { return m_front->data; }

    T bottom() const noexcept { return m_back->data; }    
    
    auto begin() noexcept -> iterator { return iterator(m_front); }    
    auto end() noexcept -> iterator { return iterator(nullptr); }
    
    auto cbegin() const noexcept -> const_iterator { return const_iterator(m_front); }
    auto cend() const noexcept -> const_iterator { return const_iterator(nullptr); }

    bool is_empty() const noexcept { return m_size == 0; }

private:
    auto swap(SinglyList& other) noexcept -> void {
        std::swap(m_front, other.m_front);
        std::swap(m_back, other.m_back);
        std::swap(m_size, other.m_size);
    }
};

#endif // __SINGLY_LIST_HPP__
