#include <iostream>
#include <vector>

using namespace std;

/*
long long get_number_of_inversions(vector<int> &a, vector<int> &b, size_t left, size_t right)
{
  long long number_of_inversions = 0;
  if (right <= left + 1) return number_of_inversions;
  size_t ave = left + (right - left) / 2;
  number_of_inversions += get_number_of_inversions(a, b, left, ave);
  number_of_inversions += get_number_of_inversions(a, b, ave, right);
  //write your code here

  return number_of_inversions;
}
*/


pair<int, vector<int>> merge_sort(vector<int> &numbers, int left, int right)
{
  pair<int, vector<int>> result;
  result.first = 0;
  if(left == right)
  {
    result.second.push_back(numbers[left]);
  }
  else
  {
    int mid = left + ((right - left) / 2);
    pair<int, vector<int>> left_sorted = merge_sort(numbers, left, mid);
    pair<int, vector<int>> right_sorted = merge_sort(numbers, mid + 1, right);
    result.first = left_sorted.first + right_sorted.first;
    int left_index = 0,
        right_index = 0;
    while(left_index < left_sorted.second.size() && right_index < right_sorted.second.size())
    {
      if(left_sorted.second[left_index] > right_sorted.second[right_index])
      {
        result.first += (left_sorted.second.size() - left_index);
        result.second.push_back(right_sorted.second[right_index]);
        right_index++;
      }
      else
      {
        result.second.push_back(left_sorted.second[left_index]);
        left_index++;
      }
    }
    while(left_index < left_sorted.second.size())
    {
      result.second.push_back(left_sorted.second[left_index]);
      left_index++;
    }
    while(right_index < right_sorted.second.size())
    {
      result.second.push_back(right_sorted.second[right_index]);
      right_index++;
    }
  }

  return result;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  vector<int> b(a.size());
  //std::cout << get_number_of_inversions(a, b, 0, a.size()) << '\n';
  pair<int, vector<int>> result = merge_sort(a, 0, a.size()-1);
  std::cout << result.first;
  std::cout << std::endl;
}
