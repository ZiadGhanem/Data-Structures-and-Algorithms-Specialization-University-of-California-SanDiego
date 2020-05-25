#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>


using namespace std;

double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
  double value = 0.0;
  vector<pair<int, int>> ratio;
  for(int i = 0; i < values.size(); i++)
  {
    ratio.push_back(make_pair(values[i], weights[i]));
  }

  sort(begin(ratio), end(ratio), [](const pair<int, int>& a, const std::pair<int, int>& b)
  {
    return (static_cast<double>(a.first)/a.second) > (static_cast<double>(b.first)/b.second);
  });

  for(int i = 0; i < ratio.size(); i++)
  {
    if(capacity < ratio[i].second)
    {
      value += static_cast<double>(capacity) * ratio[i].first / ratio[i].second;
      break;
    }
    else
    {
      value += ratio[i].first;
      capacity -= ratio[i].second;
    }
  }

  return value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;
  vector<int> values(n);
  vector<int> weights(n);
  for (int i = 0; i < n; i++) {
    std::cin >> values[i] >> weights[i];
  }

  double optimal_value = get_optimal_value(capacity, weights, values);

  std::cout.precision(10);
  std::cout << optimal_value << std::endl;
  return 0;
}
