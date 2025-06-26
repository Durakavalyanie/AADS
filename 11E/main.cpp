#include <iostream>
#include <vector>

class Solver {
 public:
  Solver(std::vector<int>& arr1, std::vector<int>& arr2)
      : arr1_(arr1), arr2_(arr2) {}

  void Merge() {
    int end = (arr1_.size() + arr2_.size()) - 1;
    int buf_right = (end + 1) / 2;
    int buf_left = std::min(static_cast<size_t>((end + 3) / 4), arr1_.size());
    InplaceMerge(0, end, buf_left, buf_right);
  }

 private:
  void MergeInBUf(int left1, int right1, int left2, int right2, int buf_begin) {
    while (left1 <= right1 && left2 <= right2) {
      if (GetElem(left1) < GetElem(left2)) {
        std::swap(GetElem(left1), GetElem(buf_begin));
        ++left1;
        ++buf_begin;
      } else {
        std::swap(GetElem(left2), GetElem(buf_begin));
        ++left2;
        ++buf_begin;
      }
    }

    while (left1 <= right1) {
      std::swap(GetElem(left1), GetElem(buf_begin));
      ++left1;
      ++buf_begin;
    }

    while (left2 <= right2) {
      std::swap(GetElem(left2), GetElem(buf_begin));
      ++left2;
      ++buf_begin;
    }
  }

  void MergeSort(int begin, int end, int buf_begin, int buf_end) {
    if (begin >= end) {
      return;
    }

    int buf_mid = buf_begin + (buf_end - buf_begin) / 2;
    int mid = begin + buf_mid - buf_begin;
    MergeSort(buf_begin, buf_mid, begin, mid);
    MergeSort(buf_mid + 1, buf_end, mid + 1, end);
    MergeInBUf(begin, mid, mid + 1, end, buf_begin);
  }

  void InplaceMerge(int start, int end, int buf_left, int buf_right) {
    if (buf_right == start) {
      while (GetElem(start) > GetElem(start + 1) && start < end) {
        std::swap(GetElem(start), GetElem(start + 1));
        ++start;
      }
      return;
    }

    int left1 = start;
    int left2 = buf_right + 1;

    int merge_buf_start = end - (end - left2 + buf_left);
    MergeInBUf(left1, buf_left - 1, left2, end, merge_buf_start);

    int sort_start_ind = (merge_buf_start + 1) / 2;
    int sort_end_ind = merge_buf_start - 1;
    MergeSort(sort_start_ind, sort_end_ind, start,
              sort_end_ind - sort_start_ind);
    InplaceMerge(
        start, end,
        std::min(static_cast<size_t>(sort_end_ind - sort_start_ind + 1),
                 arr1_.size()),
        merge_buf_start - 1);
  }

  int& GetElem(size_t ind) {
    if (ind < arr1_.size()) {
      return arr1_[ind];
    }
    return arr2_[ind - arr1_.size()];
  }

  std::vector<int>& arr1_;
  std::vector<int>& arr2_;
};

int main() {
  std::vector<int> arr1;
  int size1;
  std::cin >> size1;
  int size2;
  std::cin >> size2;
  int elem;
  for (int i = 0; i < size1; ++i) {
    std::cin >> elem;
    arr1.push_back(elem);
  }

  std::vector<int> arr2;
  for (int i = 0; i < size2; ++i) {
    std::cin >> elem;
    arr2.push_back(elem);
  }

  if (arr1.size() > arr2.size()) {
    std::swap(arr1, arr2);
  }

  Solver solution(arr1, arr2);
  solution.Merge();

  for (auto elem : arr1) {
    std::cout << elem << ' ';
  }

  for (auto elem : arr2) {
    std::cout << elem << ' ';
  }

  return 0;
}