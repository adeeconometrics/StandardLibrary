#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <type_traits>

template <typename T> class Node final {
public:
  Node *left{nullptr};
  Node *right{nullptr};
  Node *parent{nullptr};
  T data;

  Node(const T &data) : data(data) {}
  Node() = default;
  ~Node() = default;
};

/**
 * @brief responsible for motion along the structure of the tree: up (--x, x--),
 * down-left (++x), down-right (x++) semantics.
 *
 * requirement: constant runtime.
 *
 * technical reference for implementing BST Navigators:
 * http://www.cs.fsu.edu/~lacher/courses/COP4530/lectures/binary_search_trees3/script.html
 *
 * changes:
 *  0 := nullptr
 *  this_node := currNode_
 *  left := rchild_
 *  right :=  lchild_
 *  parent := parent_
 */
template <typename T, typename P> class BinaryTreeNavigator {
private:
  typename Tree<T>::Node *this_node{nullptr};

  friend class Tree<T>;

  T &Retrieve() const {
    if (!is_valid()) {
      std::cerr << "** BinaryTreeNavigator<T,P>::Retrieve() error: invalid "
                   "dereference\n";
      exit(1);
    }
    return this_node->value_;
  }

public:
  typedef T value_type;
  typedef BinaryTreeNavigator<T, P> Navigator;

  Navigator() = default;

  Navigator(typename Tree<T>::Node *ptr) { this_node = ptr; }

  Navigator(const Navigator &n) : this_node(n.this_node) {}

  virtual ~Navigator() {}

  Navigator &operator=(const Navigator &n) {
    this_node = n.this_node;
    return *this;
  }

  // checkers
  bool is_valid() const noexcept { return this_node != false; }

  bool has_parent() const noexcept {
    return this_node != nullptr && this_node->parent != nullptr;
  }

  bool has_left_child() const noexcept {
    return this_node != nullptr && this_node->left != nullptr;
  }

  bool has_right_child() const noexcept {
    return this_node != nullptr && this_node->right != nullptr;
  }

  bool is_left_child() const noexcept {
    return this_node != nullptr && this_node->parent != nullptr &&
           this_node == this_node->parent->left;
  }

  bool is_right_child() const noexcept {
    return this_node != nullptr && this_node->parent != nullptr &&
           this_node == this_node->parent->right;
  }

  bool is_red() const noexcept {
    if (this_node == nullptr)
      return false; // null is not read
    return this_node->is_red();
  }

  bool is_dead() const noexcept {
    if (this_node == nullptr)
      return false; // null is not dead
    return this_node->is_dead();
  }

  // flags - TreeType = Red Black Tree, Height Balanced Tree, AVL Tree
  char get_flags() const noexcept {
    if (this_node)
      return this_node->flags_; // find out what this means
    return false;
  }

  Navigator get_parent() const noexcept {
    Navigator n{nullptr}; // check if this is correct: it says default is null
    if (this_node)
      n.this_node = this_node->parent;
    return n;
  }

  Navigator get_left_child() const noexcept {
    Navigator n{nullptr}; // default is null
    if (this_node)
      n.this_node = this_node->left;
    return n;
  }

  Navigator get_right_child() const noexcept {
    Navigator n{nullptr}; // default is null
    if (this_node)
      n.this_node = this_node->right;
    return n;
  }

  void set_flags(uint8_t flags);

  bool operator==(const Navigator &n2) const {
    return (this_node == n2.this_node);
  }

  bool operator!=(const Navigator &n2) const {
    return (this_node != n2.this_node);
  }

  T &operator*() { return Retrieve(); }

  const T &operator*() const { return Retrieve(); }

  // navigation operators: these do NOT conform to standard iterator semantics

  Navigator &operator++() {
    if (this_node != 0) {
      this_node = this_node->left;
    }
    return *this;
  }

  Navigator &operator++(int) {
    if (this_node != 0) {
      this_node = this_node->right;
    }
    return *this;
  }

  Navigator &operator--() {
    if (this_node != 0) {
      this_node = this_node->parent;
    }
    return *this;
  }

  Navigator &operator--(int) {
    if (this_node != 0) {
      this_node = this_node->parent;
    }
    return *this;
  }

  // structural output
  void Dump(std::ostream &os) const;
};

/**
 * Requirements:
 * - has_parent()
 * - has_child()
 * - child_of()
 * - parentof()
 * - height()
 * - level
 * - iterator {navigator}
 */
// std::enable_if<is_base_of<Comparable>::value>
template <typename T, typename Node = Node<T>> class Tree {
private:
  Node *m_root{nullptr};
  size_t m_size{0};

public:
  explicit Tree() = default;
  explicit Tree(std::initializer_list<T> list) {
    for (auto i : list) {
      insert_element(i);
    }
  }

  ~Tree() = default;

  void insert_node(Node *node) {
    if (is_empty())
      m_root = node;
    else {
      Node *ptr = m_root;
      while (ptr != nullptr) {
        if (node->data < ptr->data)
          ptr = ptr->left;
        else if (node->data > ptr->data)
          ptr = ptr->right;
        else
          return;
      }

      node.parent(ptr);
      if (node->data < ptr->data)
        ptr->left = node;
      if (node->data > ptr->data)
        ptr->right = node;
    }
    ++m_size;
  }

  void insert_element(const T &data) {
    Node *node = new Node(data);
    insert_node(node);
  }

  void remove_element(const T &data) {
    if (is_empty())
      return;
    delete_element(m_root, data);
    --m_size;
  }

  void traverse_inorder(Node *node) {
    if (node != nullptr) {
      traverse_inorder(node->left);
      std::cout << node->data << " ";
      traverse_inorder(node->right);
    }
  }

  void traverse_preorder(Node *node) {
    if (node != nullptr) {
      std::cout << node->data << " ";
      traverse_preorder(node->left);
      traverse_postorder(node->right);
    }
  }

  void traverse_postorder(Node *node) {
    if (node != nullptr) {
      traverse_postorder(node->left);
      traverse_postorder(node->right);
      std::cout << node->data << " ";
    }
  }

  Node *find_max(const Node *ptr) const noexcept {
    while (ptr->right != nullptr)
      ptr = ptr->left;
    return ptr;
  }

  Node *find_min(const Node *ptr) const noexcept {
    while (ptr->left != nullptr)
      ptr = ptr->left;
    return ptr;
  }

  T &min(const Node *ptr) const noexcept {
    Node *min_val = find_min(ptr);
    return min_val->data;
  }

  T &min(const Node *ptr) const noexcept {
    Node *max_val = find_max(ptr);
    return max_val->data;
  }

  const Node *root() const noexcept { return m_root; }

  const T &root_val() const noexcept { return m_root->data; }

  bool is_element(const int &data) const;

  bool is_empty() const noexcept { return m_root == nullptr; }

  size_t size() const noexcept { return m_size; }

private:
  Node *delete_element(Node *ptr, const T &data) { // minus m_size
    if (ptr == nullptr)
      return ptr;
    else if (data < ptr->data)
      ptr->left = delete_element(ptr->left, data);
    else if (data > ptr->data)
      ptr->right = delete_element(ptr->right, data);
    else {
      // case 1: leaf node
      if (ptr->left == nullptr && ptr->right == nullptr) {
        delete ptr;
        ptr = nullptr;
      }
      // case 2: one child
      else if (ptr->left == nullptr) {
        Node *temp = ptr;
        ptr = ptr->right;
        delete temp;
      } else if (ptr->right == nullptr) {
        Node *temp = ptr;
        ptr = ptr->left;
        delete temp;
      }
      // case 3: 2 child
      else {
        Node *temp = find_min(ptr->right);
        ptr->data = temp->data;
        ptr->right = delete_element(ptr->right, temp->data);
      }

      return ptr;
    }
  }
  // enable find max, find min values
  // void print_tree
  // const Node* child_of
  // const Node* parentof
  // size_t height
  // size_t level
  // exceptions
  // iterators
};