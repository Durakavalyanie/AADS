#include <iostream>
#include <vector>

namespace ConstSize {
const int32_t kFiveSize = 5;
}

int32_t Partition(std::vector<int32_t>& arr, int32_t start, int32_t end,
                  int32_t pivot);
void BubbleSort(std::vector<int32_t>& arr, int32_t start, int32_t end) {
  for (int32_t count_1 = start; count_1 < end; ++count_1) {
    for (int32_t count_2 = start; count_2 < end - count_1 + start; ++count_2) {
      if (arr[count_2 + 1] < arr[count_2]) {
        std::swap(arr[count_2 + 1], arr[count_2]);
      }
    }
  }
}

int32_t MedianOfMedians(std::vector<int32_t>& arr, int32_t start, int32_t end) {
  if (start >= end) {
    return arr[start];
  }
  std::vector<int32_t> result((end - start) / ConstSize::kFiveSize + 1);
  int32_t count = 0;
  for (; start < end + 1; start += ConstSize::kFiveSize) {
    BubbleSort(arr, start, std::min(end, start + ConstSize::kFiveSize - 1));
    result[count] =
        arr[(start + std::min(end, start + ConstSize::kFiveSize - 1)) / 2];
    ++count;
  }
  return MedianOfMedians(result, 0, static_cast<int32_t>(result.size()) - 1);
}

int32_t GetOrdinalStat(std::vector<int32_t>& arr, int32_t ordinal_stat,
                       int32_t left, int32_t right) {
  if (right - left < ConstSize::kFiveSize) {
    BubbleSort(arr, left, right);
    return arr[left + ordinal_stat];
  }
  int32_t median = MedianOfMedians(arr, left, right);
  int32_t med_pos = Partition(arr, left, right, median);
  if (med_pos - left > ordinal_stat) {
    return GetOrdinalStat(arr, ordinal_stat, left, med_pos - 1);
  }
  if (med_pos - left < ordinal_stat) {
    return GetOrdinalStat(arr, ordinal_stat - med_pos + left - 1, med_pos + 1,
                          right);
  }
  return median;
}

int32_t Partition(std::vector<int32_t>& arr, int32_t start, int32_t end,
                  int32_t pivot) {
  if (start >= end) {
    return start;
  }
  int32_t pivot_pos = 0;
  for (int32_t count = start; count <= end; ++count) {
    if (arr[count] == pivot) {
      pivot_pos = count;
      break;
    }
  }
  while (start < end) {
    while (arr[start] < pivot) {
      ++start;
    }
    while (arr[end] > pivot) {
      --end;
    }
    if (start != end) {
      std::swap(arr[start], arr[end]);
      if (start == pivot_pos) {
        pivot_pos = end;
        ++start;
      } else if (end == pivot_pos) {
        pivot_pos = start;
        --end;
      } else {
        ++start;
        --end;
      }
    }
  }
  return pivot_pos;
}

void QuickSort(std::vector<int32_t>& arr, int32_t start, int32_t end) {
  if (start >= end) {
    return;
  }
  int32_t pivot = GetOrdinalStat(arr, (start + end) / 2 - start, start, end);
  int32_t pivot_pos = Partition(arr, start, end, pivot);
  QuickSort(arr, start, pivot_pos - 1);
  QuickSort(arr, pivot_pos + 1, end);
}

int main() {
  int32_t amount = 0;
  int32_t value = 0;
  std::vector<int32_t> arr;
  std::cin >> amount;
  for (; amount > 0; --amount) {
    std::cin >> value;
    arr.push_back(value);
  }
  QuickSort(arr, 0, static_cast<int32_t>(arr.size()) - 1);
  for (int32_t elem : arr) {
    std::cout << elem << ' ';
  }
}
