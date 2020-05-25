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


int get_fibonacci_partial_sum_fast(long long from, long long to)
{
  if (to <= 2)
      return to;

  long long previous = 1;
  long long current  = 2;
  long long tmp_previous;

  for (long long i = 0; i < to - 2; i++)
  {
      if(i >= from)
      {
        tmp_previous = previous;
        previous = current;
        current = (tmp_previous + current + 1) % 10;
      }
      else
      {
        tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
      }

  }
  return current;
}

int main() {
    long long from, to;
    std::cin >> from >> to;
    //std::cout << get_fibonacci_partial_sum_naive(from, to) << '\n';
    /std::cout << get_fibonacci_partial_sum_fast(from, to) << '\n';
}
