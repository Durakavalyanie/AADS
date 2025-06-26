#include <iostream>

enum { KLASTNODE = -1 };

class AvlTree {
 public:
  void Insert(int key);

  int Next(int key);

  ~AvlTree();

 private:
  struct Node;

  static size_t Height(Node* node);

  static int BalanceFactor(Node* node);

  static void FixHeight(Node* node);

  static Node* RotateRight(Node* node);

  static Node* RotateLeft(Node* node);

  static Node* Balance(Node* node);

  int Next(Node* node, int key, int last = KLASTNODE);

  Node* Insert(Node* root, int key);

  void Clear(Node* node);

  bool Find(Node* node, int key);

  Node* root_ = nullptr;
};

void AvlTree::Insert(int key) {
  if (!Find(root_, key)) {
    root_ = Insert(root_, key);
  }
}

int AvlTree::Next(int key) { return Next(root_, key); }

AvlTree::~AvlTree() { Clear(root_); }

struct AvlTree::Node {
  int key;
  size_t height = 1;
  Node* left = nullptr;
  Node* right = nullptr;
  Node(int input_key);
};

AvlTree::Node::Node(int input_key) : key(input_key) {}

int AvlTree::Next(Node* node, int key, int last) {
  if (node == nullptr) {
    return last;
  }
  if (node->key >= key) {
    return Next(node->left, key, node->key);
  }
  return Next(node->right, key, last);
}

AvlTree::Node* AvlTree::Insert(Node* root, int key) {
  if (root == nullptr) {
    return new Node(key);
  }
  if (key < root->key) {
    root->left = Insert(root->left, key);
  } else {
    root->right = Insert(root->right, key);
  }
  return Balance(root);
}

size_t AvlTree::Height(Node* node) {
  return (node == nullptr) ? 0 : node->height;
}

int AvlTree::BalanceFactor(AvlTree::Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return Height(node->right) - Height(node->left);
}

void AvlTree::FixHeight(Node* node) {
  node->height = std::max(Height(node->left), Height(node->right)) + 1;
}

AvlTree::Node* AvlTree::RotateRight(Node* node) {
  if (node->left == nullptr) {
    return node;
  }
  Node* new_root = node->left;
  node->left = new_root->right;
  new_root->right = node;
  FixHeight(node);
  FixHeight(new_root);
  return new_root;
}

AvlTree::Node* AvlTree::RotateLeft(Node* node) {
  if (node->right == nullptr) {
    return node;
  }
  Node* new_root = node->right;
  node->right = new_root->left;
  new_root->left = node;
  FixHeight(node);
  FixHeight(new_root);
  return new_root;
}

AvlTree::Node* AvlTree::Balance(Node* node) {
  FixHeight(node);
  if (BalanceFactor(node) == 2) {
    if (BalanceFactor(node->right) < 0) {
      node->right = RotateRight(node->right);
    }
    return RotateLeft(node);
  }
  if (BalanceFactor(node) == -2) {
    if (BalanceFactor(node->left) > 0) {
      node->left = RotateLeft(node->left);
    }
    return RotateRight(node);
  }
  return node;
}

void AvlTree::Clear(Node* node) {
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

bool AvlTree::Find(Node* node, int key) {
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

void RequestssHandler() {
  const int kMod = 1000000000;
  AvlTree tree;
  char operation;
  char last_oper = '+';
  int value;
  int last_next;
  int amount;
  std::cin >> amount;
  for (; amount > 0; --amount) {
    std::cin >> operation >> value;
    if (operation == '+') {
      if (last_oper == '?') {
        tree.Insert((value + last_next) % kMod);
      } else {
        tree.Insert(value);
      }
    } else {
      last_next = tree.Next(value);
      std::cout << last_next << '\n';
    }
    last_oper = operation;
  }
}

int main() { RequestssHandler(); }
