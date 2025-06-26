#include <iostream>
#include <optional>

template <typename T>
class Treap {
 public:
  void Insert(const T& input);

  bool Find(const T& search);

  std::optional<T> OrdinalStat(int num);

  std::optional<T> Next(const T& key);

  std::optional<T> Previous(const T& key);

  void Erase(const T& key);

  ~Treap();

 private:
  struct Node;

  std::optional<T> FindBorder(const T& key, bool is_next);

  void Clear(Node* node);

  Node* Merge(Node* left, Node* right);

  std::pair<Node*, Node*> Split(Node* root, T split_elem);

  bool Find(Node* node, const T& key);

  T NodeOrdinalStat(Node* node, int num);

  Node* root_ = nullptr;
};

template <typename T>
struct Treap<T>::Node {
  Node(const T& input) : key(input), prior(rand()) {}

  void Update() {
    size = 1;
    if (left != nullptr) {
      size += left->size;
    }
    if (right != nullptr) {
      size += right->size;
    }
  }

  T key = 0;
  int prior = 0;
  int size = 1;
  Node* left = nullptr;
  Node* right = nullptr;
};

template <typename T>
void Treap<T>::Insert(const T& input) {
  if (!Find(input)) {
    auto [left_tree, right_tree] = Split(root_, input);
    Node* new_tree = new Node(input);
    root_ = Merge(left_tree, Merge(new_tree, right_tree));
    root_->Update();
  }
}

template <typename T>
bool Treap<T>::Find(const T& search) {
  return Find(root_, search);
}

template <typename T>
std::optional<T> Treap<T>::OrdinalStat(int num) {
  if (root_ == nullptr || num > root_->size) {
    return std::nullopt;
  }
  return NodeOrdinalStat(root_, num);
}

template <typename T>
std::optional<T> Treap<T>::FindBorder(const T& key, bool is_next) {
  auto [left_tree, right_tree] = Split(root_, key);
  Node* node = (is_next) ? right_tree : left_tree;
  if (node == nullptr) {
    return std::nullopt;
  }
  while (((is_next) ? node->left : node->right) != nullptr) {
    node = (is_next) ? node->left : node->right;
  }
  root_ = Merge(left_tree, right_tree);
  return node->key;
}

template <typename T>
std::optional<T> Treap<T>::Next(const T& key) {
  return FindBorder(key, true);
}

template <typename T>
std::optional<T> Treap<T>::Previous(const T& key) {
  return FindBorder(key, false);
}

template <typename T>
void Treap<T>::Erase(const T& key) {
  auto [left_tree, right_tree] = Split(root_, key + 1);
  auto [very_left_tree, mid_tree] = Split(left_tree, key);
  delete mid_tree;
  root_ = Merge(very_left_tree, right_tree);
}

template <typename T>
Treap<T>::~Treap() {
  Clear(root_);
}

template <typename T>
void Treap<T>::Clear(Node* node) {
  if (node == nullptr) {
    return;
  }
  if (node->left != nullptr) {
    Clear(node->left);
  }
  if (node->right != nullptr) {
    Clear(node->right);
  }
  delete node;
}

template <typename T>
typename Treap<T>::Node* Treap<T>::Merge(Node* left, Node* right) {
  if (left == nullptr && right == nullptr) {
    return nullptr;
  }

  if (left == nullptr) {
    return right;
  }
  if (right == nullptr) {
    return left;
  }
  Node* root;
  if (left->prior > right->prior) {
    root = left;
    root->right = Merge(left->right, right);
  } else {
    root = right;
    root->left = Merge(left, right->left);
  }
  root->Update();
  return root;
}

template <typename T>
std::pair<typename Treap<T>::Node*, typename Treap<T>::Node*> Treap<T>::Split(
    Node* root, T split_elem) {
  if (root == nullptr) {
    return {nullptr, nullptr};
  }
  if (split_elem > root->key) {
    Node* tree_left = root;
    std::pair<Node*, Node*> splited = Split(root->right, split_elem);
    tree_left->right = splited.first;
    tree_left->Update();
    if (splited.second != nullptr) {
      splited.second->Update();
    }
    return {tree_left, splited.second};
  }
  Node* tree_right = root;
  std::pair<Node*, Node*> splited = Split(root->left, split_elem);
  tree_right->left = splited.second;
  tree_right->Update();
  if (splited.first != nullptr) {
    splited.first->Update();
  }
  return {splited.first, tree_right};
}

template <typename T>
bool Treap<T>::Find(Node* node, const T& key) {
  if (node == nullptr) {
    return false;
  }
  if (node->key == key) {
    return true;
  }
  if (node->key > key) {
    return Find(node->left, key);
  }
  return Find(node->right, key);
}

template <typename T>
T Treap<T>::NodeOrdinalStat(Node* node, int num) {
  int left_size;
  if (node->left == nullptr) {
    left_size = 0;
  } else {
    left_size = node->left->size;
  }
  if (num == left_size + 1) {
    return node->key;
  }
  if ((node->left != nullptr) && (num <= node->left->size)) {
    return NodeOrdinalStat(node->left, num);
  }
  return NodeOrdinalStat(node->right, num - left_size - 1);
}

template <typename T>
void RequestsHandler() {
  Treap<T> tree;
  std::string request = "start";
  T value;
  int pos;
  std::optional<T> inf_check;
  while (std::cin >> request) {
    if (request == "kth") {
      std::cin >> pos;
      inf_check = tree.OrdinalStat(pos + 1);
      if (inf_check == std::nullopt) {
        std::cout << "none" << '\n';
      } else {
        std::cout << *inf_check << '\n';
      }
    } else {
      std::cin >> value;
      if (request == "insert") {
        tree.Insert(value);
      } else if (request == "delete") {
        tree.Erase(value);
      } else if (request == "exists") {
        std::cout << std::boolalpha << tree.Find(value) << '\n';
      } else if (request == "next") {
        inf_check = tree.Next(value);
        if (inf_check == std::nullopt) {
          std::cout << "none" << '\n';
        } else {
          std::cout << *inf_check << '\n';
        }
      } else if (request == "prev") {
        inf_check = tree.Previous(value);
        if (inf_check == std::nullopt) {
          std::cout << "none" << '\n';
        } else {
          std::cout << *inf_check << '\n';
        }
      }
    }
  }
}

int main() { RequestsHandler<int>(); }
