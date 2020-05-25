#include <iostream>
#include <math.h>

long long get_fibonacci_huge_naive(long long n, long long m) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % m;
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

long long get_fibonacci_huge_fast(long long n, long long m)
{
  long long required_number = n % get_pisano_period(m),
  F0 = 0,
  F1  = 1,
  F = required_number;

  for (long long i = 1; i < required_number; i++)
  {
      F = (F0 + F1) % m;
      F0 = F1;
      F1 = F;
  }
  return F % m;
}

int main()
{
    long long n, m;
    std::cin >> n >> m;
    //std::cout << get_fibonacci_huge_naive(n, m) << '\n';
    std::cout << get_fibonacci_huge_fast(n, m) << '\n';
}
