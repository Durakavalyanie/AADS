#include <iostream>
#include <vector>

namespace Const {
const uint32_t kByte = 256;
}

void CountSort(const std::vector<std::pair<int32_t, int32_t>>& byte_arr,
               std::vector<int32_t>& order_arr) {
  std::vector<std::vector<int32_t>> count_arr(Const::kByte);
  for (const auto& elem : byte_arr) {
    count_arr[elem.first].push_back(elem.second);
  }
  int32_t count = 0;
  for (std::vector<int32_t> elem : count_arr) {
    for (int32_t elem_elem : elem) {
      order_arr[count] = elem_elem;
      ++count;
    }
  }
}

void LsdSort(std::vector<uint64_t>& arr) {
  const int32_t kByteInNumber = 8;
  int32_t size = static_cast<int32_t>(arr.size());
  uint64_t divider = 1;
  std::vector<uint64_t> arr_temp(arr.size());
  std::vector<std::pair<int32_t, int32_t>> byte_arr(arr.size());
  std::vector<int32_t> order_arr(arr.size());
  for (int32_t byte = 0; byte < kByteInNumber; ++byte) {
    for (int32_t count = 0; count < size; ++count) {
      byte_arr[count] = {arr[count] / divider % Const::kByte, count};
    }
    CountSort(byte_arr, order_arr);
    for (int32_t count = 0; count < size; ++count) {
      arr_temp[count] = arr[order_arr[count]];
    }
    arr = arr_temp;
    divider *= Const::kByte;
  }
}

int main() {
  int32_t amount = 0;
  uint64_t value = 0;
  std::vector<uint64_t> arr;
  std::cin >> amount;
  for (; amount > 0; --amount) {
    std::cin >> value;
    arr.push_back(value);
  }
  LsdSort(arr);
  for (uint64_t elem : arr) {
    std::cout << elem << '\n';
  }
}
