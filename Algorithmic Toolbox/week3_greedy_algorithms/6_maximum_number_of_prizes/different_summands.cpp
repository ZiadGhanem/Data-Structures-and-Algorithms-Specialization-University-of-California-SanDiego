#include <iostream>
#include <vector>
#include <unordered_set>

using std::vector;
using namespace std;

/*
vector<int> optimal_summands(int n)
{
  unordered_set<int> summands;
  int current_value = 1;
  while(n > 0)
  {
    if((n - current_value != current_value) && (summands.find(n - current_value) == end(summands)))
    {
      summands.insert(current_value);
      n -= current_value;
    }
    current_value++;
  }
  vector<int> result(begin(summands), end(summands));
  return result;
}
*/

vector<int> optimal_summands(int n)
{
  vector<int> summands;
  for(int i =1; i <= n; i++)
  {
    n -= i;
    if(n <= i)
    {
      summands.push_back(n + i);
    }
    else
    {
      summands.push_back(i);
    }
  }
  return summands;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> summands = optimal_summands(n);
  std::cout << summands.size() << '\n';
  for (size_t i = 0; i < summands.size(); ++i) {
    std::cout << summands[i] << ' ';
  }
}
