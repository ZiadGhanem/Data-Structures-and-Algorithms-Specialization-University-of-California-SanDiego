#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> optimal_sequence(int n)
{
  vector<int> sequence;
  // The number of steps required by +1, *2 or *3
  vector<int> steps(3);
  // Dynamic programming vector to keep minimum steps of each number
  vector<int> minimum_steps(n + 1);
  minimum_steps[0] = 0;
  for(int current_number = 1; current_number <= n; current_number++)
  {
    for(int i = 0; i < 3; i++)
    {
      steps[i] = INT32_MAX;
    }

    steps[0] = minimum_steps[current_number - 1] + 1;

    if(current_number % 2 == 0)
    {
      steps[1] = minimum_steps[current_number / 2] + 1;
    }
    if(current_number % 3 == 0)
    {
      steps[2] = minimum_steps[current_number / 3] + 1;
    }

    minimum_steps[current_number] = *min_element(begin(steps), end(steps));
  }

  while (n >= 1)
  {
    sequence.push_back(n);
    if((n % 3 == 0) && (minimum_steps[n] == minimum_steps[n / 3] + 1))
    {
      n /= 3;
    }
    else if((n % 2 == 0) && (minimum_steps[n] == minimum_steps[n / 2] + 1))
    {
      n /= 2;
    }
    else
    {
      n = n - 1;
    }
  }
  reverse(sequence.begin(), sequence.end());
  return sequence;
}


int main() {
  int n;
  std::cin >> n;
  vector<int> sequence = optimal_sequence(n);
  std::cout << sequence.size() - 1 << std::endl;
  for (size_t i = 0; i < sequence.size(); ++i)
  {
    std::cout << sequence[i] << " ";
  }
}
