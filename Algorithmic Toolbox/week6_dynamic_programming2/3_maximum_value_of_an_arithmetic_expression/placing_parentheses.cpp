#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class placingParentheses
{
  public:
    placingParentheses(const string &exp);
    long long get_maximum_value(void);

  private:
    int n;
    vector<long long> numbers;
    vector<long long> operations;
    vector<vector<long long>> minimum_array;
    vector<vector<long long>> maximum_array;
    pair<long long, long long> min_and_max(int i, int j);
    long long eval(long long a, long long b, char op);
};


placingParentheses::placingParentheses(const string &exp)
{
  for(auto character: exp)
  {
    if(character == '+' || character == '-' || character == '*')
    {
      this->operations.push_back(character);
    }
    else
    {
      this->numbers.push_back(character - '0');
    }
  }

  this-> n = numbers.size();

  this->minimum_array.resize(n, vector<long long>(n));
  this->maximum_array.resize(n, vector<long long>(n));
}

long long placingParentheses::eval(long long a, long long b, char op)
{
  if (op == '*')
  {
    return a * b;
  }
  else if (op == '+')
  {
    return a + b;
  }
  else if (op == '-')
  {
    return a - b;
  }
  else
  {
    assert(0);
  }
}

pair<long long, long long> placingParentheses::min_and_max(int i, int j)
{
  long long minimum_value = INT64_MAX;
  long long maximum_value = INT64_MIN;
  vector<long long> values(4);

  for(int k = i; k < j; k++)
  {
    values[0] = this->eval(this->maximum_array[i][k], this->maximum_array[k + 1][j], this->operations[k]);
    values[1] = this->eval(this->maximum_array[i][k], this->minimum_array[k + 1][j], this->operations[k]);
    values[2] = this->eval(this->minimum_array[i][k], this->maximum_array[k + 1][j], this->operations[k]);
    values[3] = this->eval(this->minimum_array[i][k], this->minimum_array[k + 1][j], this->operations[k]);

    minimum_value = min(minimum_value, *min_element(begin(values), end(values)));
    maximum_value = max(maximum_value, *max_element(begin(values), end(values)));

  }
  return make_pair(minimum_value, maximum_value);
}

long long placingParentheses::get_maximum_value(void)
{
  for(int i = 0; i < n; i++)
  {
    this->minimum_array[i][i] = this->numbers[i];
    this->maximum_array[i][i] = this->numbers[i];
  }

  for(int s = 1; s < n; s++)
  {
    for(int i = 0; i < this->n - s; i++)
    {
      int j = i + s;
      pair<long, long> min_max_pair = this->min_and_max(i, j);
      this->minimum_array[i][j] = min_max_pair.first;
      this->maximum_array[i][j] = min_max_pair.second;
    }
  }
  return this->maximum_array[0][this->n - 1];
}

int main() {
  string s;
  std::cin >> s;
  placingParentheses placing_parentheses(s);
  std::cout << placing_parentheses.get_maximum_value() << '\n';
}
