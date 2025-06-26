#include <iostream>
#include <vector>

class FrozenSet {
 public:
  void Initialize(const std::vector<int>& input) {
    input_arr_ = &input;
    size_ = input.size() * reciprocal_load_factor_;
    arr_ = std::vector<int>(size_, kFake);
    for (int elem : input) {
      Add(elem);
    }
  }

  bool Contains(int num) {
    return (arr_[Hash1(num)] == num || arr_[Hash2(num)] == num);
  }

 private:
  void Add(int elem) {
    if (Contains(elem)) {
      return;
    }

    int hash1 = Hash1(elem);
    if (arr_[hash1] == kFake) {
      arr_[hash1] = elem;
      return;
    }

    int hash2 = Hash2(elem);
    if (arr_[hash2] == kFake) {
      arr_[hash2] = elem;
      return;
    }

    int to_remove = arr_[hash2];
    int hash_func_num = (Hash1(to_remove) == hash2) ? 2 : 1;

    arr_[hash2] = elem;
    int start = elem;
    while (true) {
      if (to_remove == start) {
        Rehash();
        return;
      }

      auto removed = PutWithFunc(to_remove, hash_func_num);
      if (removed.first == kFake) {
        return;
      }

      to_remove = removed.first;

      hash_func_num = removed.second;
    }
  }

  std::pair<int, int> PutWithFunc(int elem, int hash_func_num) {
    int hash = (hash_func_num == 1) ? Hash1(elem) : Hash2(elem);
    auto removed = arr_[hash];
    arr_[hash] = elem;
    return {removed, ((Hash1(removed) == hash) ? 2 : 1)};
  }

  void Rehash() {
    reciprocal_load_factor_ += 1;
    Initialize(*input_arr_);
  }

  int Hash1(int num) const {
    return std::abs((num * num + num * kHash1Param) % size_);
  }

  int Hash2(int num) const {
    return std::abs((num * num + num * kHash2Param) % size_);
  }

  const int kFake = 1e9 + 1;
  int reciprocal_load_factor_ = 4;
  std::vector<int> arr_;
  const std::vector<int>* input_arr_;
  int size_ = 0;
  const int kHash1Param = 9895201;
  const int kHash2Param = 1921553;
};

int main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int size_1;
  int size_2;

  std::cin >> size_1;

  std::vector<int> arr;
  int buf;

  for (int i = 0; i < size_1; ++i) {
    std::cin >> buf;
    arr.push_back(buf);
  }

  FrozenSet set;
  set.Initialize(arr);

  std::cin >> size_2;
  for (int i = 0; i < size_2; ++i) {
    std::cin >> buf;
    if (set.Contains(buf)) {
      std::cout << "Yes\n";
    } else {
      std::cout << "No\n";
    }
  }
}