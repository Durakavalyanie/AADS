#include <algorithm>
#include <iostream>
#include <limits>
#include <list>
#include <set>
#include <unordered_map>
#include <vector>

class BinomialHeap {
 public:
  void Insert(int value, int index) {
    Node* new_node = new Node(value, index);
    index_node_[index] = new_node;
    std::vector<Node*> temp_roots(1, new_node);
    LinkRootList(roots_, temp_roots);
  }

  void Merge(BinomialHeap& other) {
    LinkRootList(roots_, other.roots_);
    index_node_.insert(other.index_node_.begin(), other.index_node_.end());
    other.roots_.clear();
    other.index_node_.clear();
  }

  int GetMin() {
    Node* min_node = roots_[FindMinRootPos()];
    return (min_node != nullptr) ? min_node->value
                                 : std::numeric_limits<int>::max();
  }

  int ExtractMin() {
    Node* min_node = ExtractMinTree();
    if (min_node != nullptr) {
      std::vector<Node*> new_heap;
      Node* child = min_node->child;

      while (child != nullptr) {
        Node* next = child->sibling;
        child->parent = nullptr;
        new_heap.push_back(child);
        child = next;
      }

      std::reverse(new_heap.begin(), new_heap.end());
      BinomialHeap temp_heap;
      temp_heap.roots_ = new_heap;
      LinkRootList(roots_, temp_heap.roots_);

      int deleted_ind = min_node->index;
      index_node_.erase(deleted_ind);
      delete min_node;
      return deleted_ind;
    }
    return -1;
  }

  void Remove(int index) {
    Node* node = index_node_[index];
    if (node != nullptr) {
      DeleteNode(node);
      index_node_.erase(index);
    }
  }

  void Update(int index, int value) {
    Node* node = index_node_[index];
    if (node != nullptr) {
      if (value < node->value) {
        DecreaseKey(node, value);
      } else {
        int del_ind = node->index;
        DeleteNode(node);
        Insert(value, del_ind);
      }
    }
  }

  ~BinomialHeap() {
    for (auto& iter : index_node_) {
      delete iter.second;
    }
  }

 private:
  struct Node;

  std::vector<Node*> roots_;
  std::unordered_map<int, Node*> index_node_;

  static Node* MergeTrees(Node* r1, Node* r2) {
    if (r1 == nullptr || r2 == nullptr) {
      return (r1 == nullptr) ? r2 : r1;
    }
    if (r1->value > r2->value ||
        (r1->value == r2->value && r1->index > r2->index)) {
      std::swap(r1, r2);
    }
    r2->parent = r1;
    r2->sibling = r1->child;
    r1->child = r2;
    ++(r1->degree);
    return r1;
  }

  static void LinkRootList(std::vector<Node*>& heap1,
                           std::vector<Node*>& heap2) {
    Node* carry = nullptr;
    std::vector<Node*> result;
    std::vector<Node*>& to_heap = heap1;

    if (heap1.size() < heap2.size()) {
      std::swap(heap1, heap2);
    }

    for (size_t i = 0; i < heap2.size(); ++i) {
      if (heap1[i] != nullptr && heap2[i] != nullptr) {
        result.push_back(carry);
        carry = MergeTrees(heap1[i], heap2[i]);
      } else if (heap1[i] != nullptr) {
        result.push_back(MergeTrees(heap1[i], carry));
        carry = nullptr;
      } else {
        result.push_back(MergeTrees(heap2[i], carry));
        carry = nullptr;
      }
    }

    for (size_t i = heap2.size(); i < heap1.size(); ++i) {
      result.push_back(MergeTrees(heap1[i], carry));
      carry = nullptr;
    }

    if (carry != nullptr) {
      result.push_back(carry);
    }

    to_heap = result;
  }

  Node* ExtractMinTree() {
    int min_pos = FindMinRootPos();
    Node* min_node = nullptr;

    if (min_pos >= 0) {
      min_node = roots_[min_pos];
      roots_[min_pos] = nullptr;
    }

    return min_node;
  }

  void DeleteNode(Node* node) {
    DecreaseKey(node, std::numeric_limits<int>::min());
    ExtractMin();
  }

  void DecreaseKey(Node* node, int new_value) {
    node->value = new_value;
    while (node->parent != nullptr && (node->value < node->parent->value ||
                                       (node->value == node->parent->value &&
                                        node->index < node->parent->index))) {
      index_node_[node->index] = node->parent;
      index_node_[node->parent->index] = node;
      std::swap(node->value, node->parent->value);
      std::swap(node->index, node->parent->index);
      node = node->parent;
    }
  }

  int FindMinRootPos() {
    int pos = -1;
    Node* min_node = nullptr;
    for (size_t i = 0; i < roots_.size(); ++i) {
      if (min_node == nullptr) {
        min_node = roots_[i];
        pos = i;
      }
      if (roots_[i] == nullptr) {
        continue;
      }
      if (roots_[i]->value < min_node->value ||
          (roots_[i]->value == min_node->value &&
           roots_[i]->index < min_node->index)) {
        min_node = roots_[i];
        pos = i;
      }
    }

    return pos;
  }

  struct Node {
    int value;
    int index;
    Node* parent;
    Node* sibling;
    Node* child;
    int degree;

    Node(int val, int ind)
        : value(val),
          index(ind),
          parent(nullptr),
          sibling(nullptr),
          child(nullptr),
          degree(0) {}
  };
};

class RangeOfHeaps {
 public:
  RangeOfHeaps(int size) : heaps_(size), heap_indexes_(size) {}

  void Add(int heap, int value) {
    static int cur_ind = 0;
    heaps_[heap].Insert(value, cur_ind);
    index_heap_.push_back(heap);
    heap_indexes_[heap].push_back(cur_ind);
    ++cur_ind;
  }

  void Shift(int from_heap, int to_heap) {
    heaps_[to_heap].Merge(heaps_[from_heap]);
    auto iter = heap_indexes_[from_heap].begin();
    while (iter != heap_indexes_[from_heap].end()) {
      if (index_heap_[*iter] == -1) {
        heap_indexes_[from_heap].erase(iter++);
        continue;
      }
      index_heap_[*iter] = to_heap;
      ++iter;
    }
    heap_indexes_[to_heap].splice(heap_indexes_[to_heap].end(),
                                  heap_indexes_[from_heap]);
  }

  void Delete(int index) {
    heaps_[index_heap_[index]].Remove(index);
    index_heap_[index] = -1;
  }

  void Update(int index, int new_value) {
    heaps_[index_heap_[index]].Update(index, new_value);
  }

  int GetMin(int heap) { return heaps_[heap].GetMin(); }

  void ExtractMin(int heap) { index_heap_[heaps_[heap].ExtractMin()] = -1; }

 private:
  std::vector<BinomialHeap> heaps_;
  std::vector<int> index_heap_;
  std::vector<std::list<int>> heap_indexes_;
};

int main() {
  int requests;
  int size;

  std::cin >> size >> requests;
  RangeOfHeaps heaps(size);

  for (; requests > 0; --requests) {
    int op;
    int arg1;
    int arg2;
    std::cin >> op >> arg1;
    --arg1;
    if (op == 0) {
      std::cin >> arg2;
      heaps.Add(arg1, arg2);
    } else if (op == 1) {
      std::cin >> arg2;
      --arg2;
      heaps.Shift(arg1, arg2);
    } else if (op == 2) {
      heaps.Delete(arg1);
    } else if (op == 3) {
      std::cin >> arg2;
      heaps.Update(arg1, arg2);
    } else if (op == 4) {
      std::cout << heaps.GetMin(arg1) << '\n';
    } else {
      heaps.ExtractMin(arg1);
    }
  }

  return 0;
}