#include <iostream>
#include <vector>

struct Node {
  int value;
  Node* left;
  Node* right;

  Node(int val) : value(val), left(nullptr), right(nullptr) {}
  Node(Node* lhs, Node* rhs) : value(0), left(lhs), right(rhs) {
    if (lhs != nullptr) {
      value += left->value;
    }
    if (rhs != nullptr) {
      value += right->value;
    }
  }
};

class PersistentSegmentTree {
 public:
  PersistentSegmentTree(const std::vector<int>& arr) {
    size_ = arr.size();
    roots_.push_back(Build(arr, 0, size_ - 1));
  }

  void Update(int version, int index, int new_value) {
    roots_.push_back(Update(roots_[version], 0, size_ - 1, index, new_value));
  }

  int At(int version, int index) {
    return At(roots_[version], 0, size_ - 1, index);
  }

  ~PersistentSegmentTree() {
    for (Node* elem : nodes_) {
      delete elem;
    }
  }

 private:
  int size_;
  std::vector<Node*> roots_;
  std::vector<Node*> nodes_;

  Node* Build(const std::vector<int>& arr, int left, int right) {
    Node* new_node;
    if (left == right) {
      new_node = new Node(arr[left]);
      nodes_.push_back(new_node);
      return new_node;
    }
    int mid = (left + right) / 2;
    new_node = new Node(Build(arr, left, mid), Build(arr, mid + 1, right));
    nodes_.push_back(new_node);
    return new_node;
  }

  Node* Update(Node* node, int left, int right, int index, int new_value) {
    Node* new_node;
    if (left == right) {
      new_node = new Node(new_value);
      nodes_.push_back(new_node);
      return new_node;
    }
    int mid = (left + right) / 2;
    if (index <= mid) {
      new_node = new Node(Update(node->left, left, mid, index, new_value),
                          node->right);
      nodes_.push_back(new_node);
      return new_node;
    }
    new_node = new Node(node->left,
                        Update(node->right, mid + 1, right, index, new_value));
    nodes_.push_back(new_node);
    return new_node;
  }

  int At(Node* node, int left, int right, int index) {
    if (left == right) {
      return node->value;
    }
    int mid = (left + right) / 2;
    if (index <= mid) {
      return At(node->left, left, mid, index);
    }
    return At(node->right, mid + 1, right, index);
  }
};

int main() {
  std::vector<int> arr;
  int size;
  std::cin >> size;
  int elem;
  for (int i = 0; i < size; ++i) {
    std::cin >> elem;
    arr.push_back(elem);
  }

  PersistentSegmentTree pst(arr);

  int requests;
  std::cin >> requests;

  std::string type;
  int arg1;
  int arg2;
  int arg3;
  for (int i = 0; i < requests; ++i) {
    std::cin >> type >> arg1 >> arg2;
    --arg1;
    --arg2;

    if (type == "get") {
      std::cout << pst.At(arg1, arg2) << '\n';
    } else {
      std::cin >> arg3;
      pst.Update(arg1, arg2, arg3);
    }
  }

  return 0;
}