#include "..\AbstractContainers\Comparable.h"
#include <initializer_list>
#include <iostream>
#include <type_traits>

template <typename T, typename = typename std::enable_if<
                          std::is_base_of<Comparable<T>, T>::value ||
                          std::is_integral<T>::value>>
class OrderedList {
private:
  struct Node {
    Node *next{nullptr};
    Node *prev{nullptr};
    T data;
    Node(T arg_data) : data(arg_data) {}
    Node() = default;
  };

  Node *front{nullptr};
  Node *back{nullptr};
  size_t m_size{0};

public:
  OrderedList() = default;
  OrderedList(const std::initializer_list<T> &_list) {
    for (auto i : _list)
      add(i);
  }

  ~OrderedList() {
    if (!is_empty())
      clear();
  }

  void add(const T &data) {
    Node *node = new Node(data);
    if (is_empty()) {
      front = node;
      back = node;
    } else if (data >= front->data) {
      add_front(node);
    } else if (data <= back->data) {
      add_back(node);
    } else {
      Node *ptr = front;
      while (ptr->next != nullptr) {
        if (ptr->next->data < data)
          break;
        ptr = ptr->next;
      }

      if (ptr->next != nullptr) {
        ptr->next->prev = node;
        node->next = ptr->next;
        node->prev = ptr;
        ptr->next = node;
      }
    }
    m_size += 1;
  }

  void remove(const T &data) {
    try {
      if (is_empty())
        throw std::exception();

      else if (front->data == data)
        remove_front();
      else if (back->data == data)
        remove_back();
      else {
        Node *ptr = front;
        Node *prev = ptr;
        while (ptr->next != nullptr) {
          if (ptr->data == data) {
            prev->next = ptr->next;
            ptr->prev = prev;

            delete ptr;
            ptr = nullptr;
            m_size -= 1;
            return;
          }
          prev = ptr;
          ptr = ptr->next;
        }
        throw "Error: element not found in the list.";
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      exit(1);
    } catch (const char *str) {
      std::cerr << str << '\n';
      exit(1);
    }
  }

  void display() const {
    Node *ptr = front;
    while (ptr->next != nullptr) {
      std::cout << ptr->data << " ";
      ptr = ptr->next;
    }
    std::cout << ptr->data << '\n';
  }

  void clear() {
    Node *ptr = front;
    Node *prev = ptr;
    while (ptr->next != nullptr) {
      prev = ptr;
      delete prev;
      prev = nullptr;
      ptr = ptr->next;
    }
    m_size = 0;
  }

  bool is_empty() const { return front == nullptr && back == nullptr; }
  size_t size() const { return m_size; }

private:
  void add_back(Node *node) {
    node->prev = back;
    back->next = node;
    back = node;
  }

  void add_front(Node *node) {
    front->prev = node;
    node->next = front;
    front = node;
  }

  void remove_front() {
    Node *temp = front;
    front = front->next;

    delete temp;
    temp = nullptr;
    m_size -= 1;
  }

  void remove_back() {
    Node *temp = back;
    back = back->prev;

    delete temp;
    temp = nullptr;
    m_size -= 1;
  }
};
