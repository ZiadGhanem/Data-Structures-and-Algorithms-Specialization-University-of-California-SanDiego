#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using namespace std;

class TreeOrders {
  int n;
  vector <int> key;
  vector <int> left;
  vector <int> right;

public:
  void read() {
    cin >> n;
    key.resize(n);
    left.resize(n);
    right.resize(n);
    for (int i = 0; i < n; i++) {
      cin >> key[i] >> left[i] >> right[i];
    }
  }

  void get_in_order(int index)
  {
    if(index == -1)
      return;
    get_in_order(left[index]);
    cout << key[index] << " ";
    get_in_order(right[index]);
  }


  void in_order()
  {
    //vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that
    get_in_order(0);
    cout << endl;

    //return result;
  }

  void get_pre_order(int index)
  {
    if(index == -1)
      return;
    cout << key[index] << " ";
    get_pre_order(left[index]);
    get_pre_order(right[index]);

  }

  void pre_order() {
    // Finish the implementation
    // You may need to add a new recursive method to do that
    get_pre_order(0);
    cout << endl;
  }

  void get_post_order(int index)
  {
    if(index == -1)
      return;
    get_post_order(left[index]);
    get_post_order(right[index]);
    cout << key[index] << " ";
  }

  void post_order() {
    // Finish the implementation
    // You may need to add a new recursive method to do that
    get_post_order(0);
    cout << endl;
  }
};

void print(vector <int> a) {
  for (size_t i = 0; i < a.size(); i++) {
    if (i > 0) {
      cout << ' ';
    }
    cout << a[i];
  }
  cout << '\n';
}

int main_with_large_stack_space() {
  ios_base::sync_with_stdio(0);
  TreeOrders t;
  t.read();
  t.in_order();
  t.pre_order();
  t.post_order();
  return 0;
}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
  {
      if (rl.rlim_cur < kStackSize)
      {
          rl.rlim_cur = kStackSize;
          result = setrlimit(RLIMIT_STACK, &rl);
          if (result != 0)
          {
              std::cerr << "setrlimit returned result = " << result << std::endl;
          }
      }
  }
#endif

  return main_with_large_stack_space();
}
