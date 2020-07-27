#include <iostream>

int fibonacci_sum_naive(long long n) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;
    long long sum      = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
        sum += current;
    }

    return sum % 10;
}


long long get_pisano_period(long long m)
{
  if(m == 1)
    return 1;
  else if(m == 2)
    return 3;
  else
  {
    long long previous = 0,
    current = 1,
    temp;
    for(long long i = 0; i < m * m; i++)
    {
      temp = previous;
      previous = current;
      current = (temp + current) % m;

      if(previous == 0 && current == 1)
        return i + 1;
    }
  }
}


int fibonacci_sum_fast(long long n)
{
  if (n <= 2)
      return n;

  n %= get_pisano_period(10);

  long long previous = 0;
  long long current  = 1;

  for (long long i = 0; i <= n; i++)
  {
      long long tmp_previous = previous;
      previous = current;
      current = (tmp_previous + current) % 10;
  }
  return (current == 0)? 9 : --current;
}

int main() {
    long long n = 0;
    std::cin >> n;
    //std::cout << fibonacci_sum_naive(n);
    std::cout << fibonacci_sum_fast(n);
}
