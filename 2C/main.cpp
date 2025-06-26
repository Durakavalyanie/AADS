#include <iostream>
#include <vector>

class Heap {
 public:
  void Insert(const std::pair<int64_t, int32_t>& value);

  std::pair<int64_t, int32_t> GetMin();

  void ExtractMin();

  void DecreaseKey(int32_t ind, int64_t delta);

 private:
  void SiftUp(int32_t new_ind);

  void SiftDown(int32_t parent);

  std::vector<std::pair<int64_t, int32_t>> arr_;
};

void Heap::Insert(const std::pair<int64_t, int32_t>& value) {
  arr_.push_back(value);
  SiftUp(static_cast<int32_t>(arr_.size()) - 1);
}

std::pair<int64_t, int32_t> Heap::GetMin() { return arr_[0]; }

void Heap::ExtractMin() {
  arr_[0] = arr_[arr_.size() - 1];
  arr_.pop_back();
  SiftDown(0);
}

void Heap::DecreaseKey(int32_t ind, int64_t delta) {
  int32_t count = 0;
  while (arr_[count].second != ind) {
    ++count;
  }
  arr_[count].first -= delta;
  SiftUp(count);
}

void Heap::SiftUp(int32_t new_ind) {
  int32_t parent_ind = (new_ind - 1) / 2;
  while (parent_ind >= 0 && new_ind > 0) {
    if (arr_[new_ind] < arr_[parent_ind]) {
      std::swap(arr_[new_ind], arr_[parent_ind]);
    }
    new_ind = parent_ind;
    parent_ind = (new_ind - 1) / 2;
  }
}

void Heap::SiftDown(int32_t parent) {
  int32_t size = static_cast<int32_t>(arr_.size());
  int32_t left_child = 2 * parent + 1;
  int32_t right_child = 2 * parent + 2;

  if (left_child < size) {
    if (arr_[parent] > arr_[left_child]) {
      std::swap(arr_[parent], arr_[left_child]);
      SiftDown(left_child);
    }
  }

  if (right_child < size) {
    if (arr_[parent] > arr_[right_child]) {
      std::swap(arr_[parent], arr_[right_child]);
      SiftDown(right_child);
    }
  }
}

void HandleRequests(Heap& tree, int32_t amount) {
  int32_t value1;
  int64_t value2;
  std::string buffer;
  for (int32_t count = 1; count <= amount; ++count) {
    std::cin >> buffer;
    if (buffer == "insert") {
      std::cin >> value1;
      tree.Insert({value1, count});
    } else if (buffer == "getMin") {
      std::cout << tree.GetMin().first << '\n';
    } else if (buffer == "extractMin") {
      tree.ExtractMin();
    } else {
      std::cin >> value1 >> value2;
      tree.DecreaseKey(value1, value2);
    }
  }
}

int main() {
  std::cin.tie(0);
  int32_t amount;
  std::cin >> amount;
  Heap tree;
  HandleRequests(tree, amount);
}
