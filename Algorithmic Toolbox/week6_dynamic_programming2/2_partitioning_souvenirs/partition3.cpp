#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

bool optimal_weight(int W, const vector<int> &w)
{
  int count = 0;
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

      if(knapsack_array[weight][i] == W)
      {
        count++;
      }
    }
  }
  return (count >= 3);
}

int partition3(vector<int> &souvenirs)
{
  int total_sum = accumulate(begin(souvenirs), end(souvenirs), 0);
  if(total_sum % 3 == 0)
  {
    return (optimal_weight((total_sum / 3), souvenirs));
  }
  return false;
}

/*
vector<int> partition(int partition_size, vector<int> &souvenirs)
{
  vector<int> result;
  int n = souvenirs.size();
  bool dp[n][partition_size + 1];

  for(int i = 0; i < n; i++)
    dp[i][0] = true;

  for(int s = 1; s <= partition_size; s++)
  {
    dp[0][s] = (souvenirs[0] == s)? true:false;
  }

  for(int i = 1; i < n; i++)
  {
    for(int s = 1; s <= partition_size; s++)
    {
      if(dp[i - 1][s])
      {
        dp[i][s] = dp[i - 1][s];
      }
      else if(s >= souvenirs[i])
      {
        dp[i][s] = dp[i - 1][s - souvenirs[i]];
      }
    }
  }

  if(dp[n - 1][partition_size]);
  {
    int s = partition_size;
    for(int i = n - 1; i > 0; i--)
    {
        if(s >= souvenirs[i])
        {
          if(dp[i][s] && dp[i - 1][s - souvenirs[i]])
          {
            result.push_back(souvenirs[i]);
            s -= souvenirs[i];
          }
        }
    }
    if(s == souvenirs[0])
      result.push_back(souvenirs[0]);
  }

  return result;
}


int partition3(vector<int> &souvenirs)
{
  vector<int> current_result;
  vector<int> remaining_souvenirs;
  int total_sum = accumulate(begin(souvenirs), end(souvenirs), 0);
  if(total_sum % 3 == 0)
  {
    for(int i = 0; i < 3; i++)
    {
      current_result = partition(total_sum / 3, souvenirs);
      if(current_result.empty())
      {
        return false;
      }
      remaining_souvenirs.clear();
      for(auto souvenir: souvenirs)
      {
          bool found = false;
          for(int i = 0 ; i < current_result.size() && !current_result.empty(); i++)
          {
            if(current_result[i] == souvenir)
            {
              found = true;
              current_result.erase(begin(current_result) + i);
              break;
            }
          }
          if(!found)
          {
            remaining_souvenirs.push_back(souvenir);
          }
      }
      souvenirs = remaining_souvenirs;
    }
    if(souvenirs.empty())
    {
      return true;
    }

  }
  return false;
}
*/


int main() {
  int n;
  std::cin >> n;
  vector<int> A(n);
  for (size_t i = 0; i < A.size(); ++i)
  {
    std::cin >> A[i];
  }
  std::cout << partition3(A) << '\n';
}
