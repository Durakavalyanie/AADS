#include <iostream>
#include <vector>

std::vector<int32_t> MinPrimeDivs(int32_t number) {
  std::vector<int32_t> mind(number + 1);
  std::vector<int32_t> primes;

  for (int count = 2; count <= number; ++count) {
    mind[count] = count;
  }

  for (int32_t count = 2; count <= number; ++count) {
    if (mind[count] == count) {
      primes.push_back(count);
    }
    for (int32_t elem : primes) {
      if (elem * count > number || elem > mind[count]) {
        break;
      }
      mind[count * elem] = elem;
    }
  }
  return mind;
}

int32_t Solution(int32_t number) {
  std::vector<int32_t> mind(number + 1);
  mind = MinPrimeDivs(number);
  int32_t res_sum = 0;
  for (int32_t count = 4; count <= number; ++count) {
    if (mind[count] != count) {
      res_sum += mind[count];
    }
  }
  return res_sum;
}

int main() {
  int32_t number = 0;
  std::cin >> number;
  std::cout << Solution(number);
}
