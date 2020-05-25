#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Solution via hash map
int get_majority_element(vector<int> &a)
{
  unordered_map<int, int> number_count;
  for(int number: a)
  {
      if(number_count.find(number) == end(number_count))
      {
        number_count.insert(make_pair(number, 1));
      }
      else
      {
        number_count.at(number)++;
      }
  }
  for (auto &number: number_count)
  {
    if(number.second > a.size() / 2)
    {
      return 1;
    }
  }
  return 0;
}

/*
//Solution via divide and conque
int get_majority_element(vector<int> &a, int left, int right)
{
  if(left == right)
  {
    return -1;
  }
  else if(left + 1 == right)
  {
      return a[left];
  }
  else
  {
    int mid = left + ((right - left) / 2);
    int left_element = get_majority_element(a,left, mid);
    int right_element = get_majority_element(a, mid + 1, right);

    int left_count = 0;
    for(int i = left; i < right; i++)
    {
      if(a[i] == left_element)
      {
        left_count ++;
      }
    }
    if(left_count > (right - left) / 2)
      return left_element;

    int right_count = 0;
    for(int i = left; i < right; i++)
    {
      if(a[i] == right_element)
      {
        right_count ++;
      }
    }
    if(right_count > (right - left) / 2)
      return right_element;

    return -1;
  }
}
*/

int main() {
    int n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < a.size(); ++i) {
        std::cin >> a[i];
    }
    //std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
    std::cout << get_majority_element(a) << '\n';
}
