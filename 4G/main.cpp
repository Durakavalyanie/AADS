#include <iostream>
#include <optional>

template <typename T>
class Treap {
 public:
  void Insert(const T& elem, int pos);

  std::optional<T> GetMin(int left_query, int right_query);

  void Reverse(int left_query, int right_query);

  ~Treap();

 private:
  struct Node;

  static void Push(Node* node);

  void Clear(Node* node);

  Node* Merge(Node* left, Node* right);

  std::pair<Node*, Node*> Split(Node* root, int pos);

  Node* root_ = nullptr;
};

template <typename T>
struct Treap<T>::Node {
  Node(const T& input) : key(input), prior(rand()) {}

  void Update() {
    size = 1;
    min = key;
    if (left != nullptr) {
      size += left->size;
      min = std::min(min, left->min);
    }
    if (right != nullptr) {
      size += right->size;
      min = std::min(min, right->min);
    }
  }

  T key = 0;
  int prior = 0;
  int size = 1;
  T min = key;
  bool flag = false;
  Node* left = nullptr;
  Node* right = nullptr;
};

template <typename T>
void Treap<T>::Reverse(int left_query, int right_query) {
  auto [left, right] = Split(root_, left_query);
  auto [mid, very_right] = Split(right, right_query - left_query + 1);
  mid->flag = !mid->flag;
  root_ = Merge(left, Merge(mid, very_right));
}

template <typename T>
void Treap<T>::Push(Treap::Node* node) {
  if (node == nullptr || !(node->flag)) {
    return;
  }
  std::swap(node->left, node->right);
  if (node->left != nullptr) {
    node->left->flag = !node->left->flag;
  }
  if (node->right != nullptr) {
    node->right->flag = !node->right->flag;
  }
  node->flag = false;
}

template <typename T>
std::optional<T> Treap<T>::GetMin(int left_query, int right_query) {
  if (root_ == nullptr) {
    return std::nullopt;
  }
  auto [left, right] = Split(root_, left_query);
  auto [mid, very_right] = Split(right, right_query - left_query + 1);
  int res = mid->min;
  root_ = Merge(left, Merge(mid, very_right));
  return res;
}

template <typename T>
void Treap<T>::Insert(const T& elem, int pos) {
  auto [left_tree, right_tree] = Split(root_, pos);
  Node* new_tree = new Node(elem);
  root_ = Merge(left_tree, Merge(new_tree, right_tree));
  root_->Update();
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
  Push(left);
  Push(right);
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
    Node* root, int pos) {
  if (root == nullptr) {
    return {nullptr, nullptr};
  }
  Push(root);
  int left = (root->left == nullptr) ? 0 : root->left->size;
  if (left >= pos) {
    auto splited = Split(root->left, pos);
    root->left = splited.second;
    root->Update();
    return {splited.first, root};
  }
  auto splited = Split(root->right, pos - left - 1);
  root->right = splited.first;
  root->Update();
  return {root, splited.second};
}

template <typename T>
void RequestsHandler() {
  Treap<T> tree;
  int size;
  int amount;
  std::cin >> size >> amount;
  T value;
  for (int i = 0; i < size; ++i) {
    std::cin >> value;
    tree.Insert(value, i);
  }
  int operation;
  int left_query;
  int right_query;
  for (; amount > 0; --amount) {
    std::cin >> operation >> left_query >> right_query;
    if (operation == 1) {
      tree.Reverse(left_query - 1, right_query - 1);
    } else {
      std::cout << *tree.GetMin(left_query - 1, right_query - 1) << '\n';
    }
  }
}

int main() {
  std::cin.tie(nullptr);
  RequestsHandler<int>();
}
