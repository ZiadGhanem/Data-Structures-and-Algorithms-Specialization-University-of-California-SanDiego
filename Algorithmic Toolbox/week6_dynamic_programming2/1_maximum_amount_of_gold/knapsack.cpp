#include <iostream>
#include <vector>

using std::vector;

int optimal_weight(int W, const vector<int> &w)
{
  vector<vector<int>> knapsack_array(W + 1, vector<int>(w.size() + 1, 0));
  for(int i = 1; i <= w.size(); i++)
  {
    for(int weight = 1; weight <= W; weight++)
    {
      knapsack_array[weight][i] = knapsack_array[weight][i - 1];
      if(w[i - 1] <= weight)
      {
        int value = knapsack_array[weight - w[i - 1]][i - 1] + w[i - 1];
        if(knapsack_array[weight][i] < value)
        {
          knapsack_array[weight][i] = value;
        }
      }
    }
  }
  return knapsack_array[W][w.size()];
}

int main() {
  int n, W;
  std::cin >> W >> n;
  vector<int> w(n);
  for (int i = 0; i < n; i++) {
    std::cin >> w[i];
  }
  std::cout << optimal_weight(W, w) << '\n';
}
