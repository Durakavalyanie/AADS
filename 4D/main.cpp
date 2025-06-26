#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <vector>

class Sparse {
 public:
  Sparse(const std::vector<int>& input);

  int GetSecondStat(int left, int right);

 private:
  struct SegInfo;

  static SegInfo SecondStat(const SegInfo& left, const SegInfo& right);

  std::vector<std::vector<SegInfo>> arr_;
  size_t log2_size_;
  size_t deg2_size_;
  std::vector<int> log_arr_;
};

struct Sparse::SegInfo {
  SegInfo() = default;

  SegInfo(const std::pair<int, int>& inp_first,
          const std::pair<int, int>& inp_second)
      : first(inp_first), second(inp_second) {}

  std::optional<std::pair<int, int>> first = std::nullopt;
  std::optional<std::pair<int, int>> second = std::nullopt;
};

Sparse::SegInfo Sparse::SecondStat(const SegInfo& left, const SegInfo& right) {
  std::vector<std::pair<int, int>> temp = {*left.first, *left.second,
                                           *right.first, *right.second};
  std::sort(temp.begin(), temp.end());
  if (temp[0].second == temp[1].second) {
    return SegInfo(temp[1], temp[2]);
  }
  return SegInfo(temp[0], temp[1]);
}

Sparse::Sparse(const std::vector<int>& input) {
  log2_size_ = std::ceil(std::log2(input.size()));
  deg2_size_ = static_cast<size_t>(std::pow(2, log2_size_));

  arr_.assign(deg2_size_, std::vector<SegInfo>(log2_size_));

  for (size_t i = 0; i < input.size(); ++i) {
    arr_[i][0].first = {input[i], i};
    arr_[i][0].second = std::nullopt;
  }
  for (size_t i = input.size(); i < deg2_size_; ++i) {
    arr_[i][0].first = arr_[i][0].second = std::nullopt;
  }
  for (size_t j = 0; j < log2_size_ - 1; ++j) {
    for (size_t i = 0; i + (1 << j) < deg2_size_; ++i) {
      arr_[i][j + 1] = SecondStat(arr_[i][j], arr_[i + (1 << j)][j]);
    }
  }
  log_arr_.resize(deg2_size_ + 1);
  log_arr_[1] = 0;
  for (size_t i = 2; i <= deg2_size_; ++i) {
    log_arr_[i] = log_arr_[i / 2] + 1;
  }
}

int Sparse::GetSecondStat(int left, int right) {
  int height = log_arr_[right - left + 1];
  return (SecondStat(arr_[left][height],
                     arr_[right - (1 << height) + 1][height])
              .second)
      ->first;
}

int main() {
  int size;
  int amount;
  std::cin >> size >> amount;
  std::vector<int> input(size);
  for (int elem : input) {
    std::cin >> elem;
  }
  Sparse table(input);
  int left;
  int right;
  for (; amount > 0; --amount) {
    std::cin >> left >> right;
    std::cout << table.GetSecondStat(left - 1, right - 1) << '\n';
  }
}
