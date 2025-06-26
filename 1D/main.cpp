#include <iostream>
#include <vector>

bool CheckDistance(const std::vector<int32_t>& coordinates, int32_t fes_amount,
                   int32_t dist) {
  int32_t fes_count = 1;
  int32_t vec_size = coordinates.size();
  int32_t start = coordinates[0];

  for (int32_t count = 1; count < vec_size; ++count) {
    if (coordinates[count] - start >= dist) {
      if (fes_count == fes_amount - 1) {
        return true;
      }
      ++fes_count;
      start = coordinates[count];
    }
  }
  return false;
}

int PredicatBinSearch(const std::vector<int32_t>& coordinates,
                      int32_t fes_amount) {
  int32_t high = *(coordinates.end() - 1) - coordinates[0] + 1;
  int32_t low = 0;
  int32_t mid;
  bool predicat;

  while (high > low) {
    mid = (high + low) / 2;
    predicat = CheckDistance(coordinates, fes_amount, mid);
    if (predicat) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }
  return high - 1;
}

int main() {
  int32_t box_amount;
  int32_t fes_amount;
  std::cin >> box_amount >> fes_amount;

  std::vector<int32_t> coordinates(box_amount, 0);
  for (int32_t elem : coordinates) {
    std::cin >> elem;
  }
  std::cout << PredicatBinSearch(coordinates, fes_amount);
}
