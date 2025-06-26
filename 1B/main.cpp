#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

double AnswerOnRequest(const std::vector<double>& pref_multi, int32_t left,
                       int32_t right) {
  int32_t size = static_cast<int32_t>(pref_multi.size());
  double power = (size - 1) / (right - left + 1.0);
  return pow(pref_multi[right + 1] / pref_multi[left], power);
}

void Preprocess(std::vector<double>& pref_multi) {
  int32_t amount;
  std::cin >> amount;
  double volume;
  pref_multi.push_back(1);
  for (int32_t place = 1; place <= amount; ++place) {
    std::cin >> volume;
    pref_multi.push_back(pref_multi[place - 1] * pow(volume, 1.0 / amount));
  }
}

int main() {
  const int32_t kNumsAfterPoint = 6;
  std::vector<double> pref_multi;
  Preprocess(pref_multi);

  int32_t requests_amount;
  std::cin >> requests_amount;
  int32_t left;
  int32_t right;
  for (int32_t count = 0; count < requests_amount; ++count) {
    std::cin >> left >> right;
    std::cout << std::fixed << std::setprecision(kNumsAfterPoint)
              << AnswerOnRequest(pref_multi, left, right) << '\n';
  }
}
