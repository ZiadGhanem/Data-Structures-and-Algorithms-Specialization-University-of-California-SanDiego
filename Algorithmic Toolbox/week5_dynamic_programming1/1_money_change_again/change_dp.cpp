#include <iostream>
#include <vector>

using namespace std;

int get_change(int money)
{
  vector<int> coins = {1, 3, 4};

  vector<int> min_number_coins(money + 1, INT32_MAX);
  min_number_coins[0] = 0;

  int number_coins;

  for(int m = 1; m <= money; m++)
  {
    for(size_t i = 0; i < coins.size(); i++)
    {
      if(m >= coins[i])
      {
        number_coins = min_number_coins[m - coins[i]] + 1;
        if(number_coins < min_number_coins[m])
        {
          min_number_coins[m] = number_coins;
        }
      }
    }
  }
  return min_number_coins[money];
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
