#include <iostream>
#include <vector>
using std::vector;

long long get_fibonacci_partial_sum_naive(long long from, long long to) {
    long long sum = 0;

    long long current = 0;
    long long next  = 1;

    for (long long i = 0; i <= to; ++i) {
        if (i >= from) {
            sum += current;
        }

        long long new_current = next;
        next = next + current;
        current = new_current;
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



int get_fibonacci_partial_sum_fast(long long from, long long to)
{
  int second = fibonacci_sum_fast(to);
  if(from == 0)
    return second;

  int first = fibonacci_sum_fast(from - 1);

  int result = second - first;

  if(result < 0)
    result += 10;

  return result;
}

int main() {
    long long from, to;
    std::cin >> from >> to;
    //std::cout << get_fibonacci_partial_sum_naive(from, to) << '\n';
    std::cout << get_fibonacci_partial_sum_fast(from, to) << '\n';
}
