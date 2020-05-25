#include <iostream>
#include <vector>
#include <deque>

using std::cin;
using std::cout;
using std::vector;
using std::max;

using namespace std;

void max_sliding_window_naive(vector<int> const & A, int w) {
    for (size_t i = 0; i < A.size() - w + 1; ++i) {
        int window_max = A.at(i);
        for (size_t j = i + 1; j < i + w; ++j)
            window_max = max(window_max, A.at(j));

        cout << window_max << " ";
    }

    return;
}

void max_sliding_window_fast(vector<int> const & A, int w)
{
  deque<int> Q(w);
  int i;
  for(int i = 0; i < w; i++)
  {
    while((!Q.empty()) && A[i] >= A[Q.back()])
      Q.pop_back();

    Q.push_back(i);
  }

  for(; i < A.size(); i++)
  {
    cout << A[Q.front()] << " ";
    while((!Q.empty()) && Q.front() <= i - w)
      Q.pop_front();

    while((!Q.empty()) && A[i] >= A[Q.back()])
      Q.pop_back();

    Q.push_back(i);
  }

  cout << A[Q.front()];

  return;
}


int main() {
    int n = 0;
    cin >> n;

    vector<int> A(n);
    for (size_t i = 0; i < n; ++i)
        cin >> A.at(i);

    int w = 0;
    cin >> w;

    max_sliding_window_fast(A, w);

    return 0;
}
