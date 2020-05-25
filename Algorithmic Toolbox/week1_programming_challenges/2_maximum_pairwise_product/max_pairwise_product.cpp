#include <iostream>
#include <vector>
#include <algorithm>

/*
int MaxPairwiseProduct(const std::vector<int>& numbers) {
    int max_product = 0;
    int n = numbers.size();

    for (int first = 0; first < n; ++first) {
        for (int second = first + 1; second < n; ++second) {
            max_product = std::max(max_product,
                numbers[first] * numbers[second]);
        }
    }

    return max_product;
}
*/

long long MaxPairwiseProduct(const std::vector<int>& numbers)
{
  long long larger_number = INT64_MIN + 1;
  long long smaller_number = INT64_MIN;

  for(auto number: numbers)
  {
    if(number >= larger_number)
    {
      smaller_number = larger_number;
      larger_number = number;
    }
    else if(number > smaller_number)
    {
      smaller_number = number;
    }
  }

  return (long long)smaller_number * larger_number;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << MaxPairwiseProduct(numbers) << "\n";
    return 0;
}
