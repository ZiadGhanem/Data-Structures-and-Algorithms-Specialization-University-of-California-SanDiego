#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

bool is_greater_or_equal(string a, string b)
{
  return (stoi(a + b) >= stoi(b + a));
}

string largest_number(vector<string> a)
{
  string result,
  max_number;
  int max_index;

  while(!a.empty())
  {
    max_number = "0000";
    for(int number_index = 0; number_index < a.size(); number_index++)
    {
      if(is_greater_or_equal(a[number_index], max_number))
      {
        max_number = a[number_index];
        max_index = number_index;
      }
    }
    result += max_number;
    a.erase(begin(a) + max_index);
  }

  return result;


  //write your code here
  /*
  std::stringstream ret;
  for (size_t i = 0; i < a.size(); i++)
  {
    ret << a[i];
  }
  string result;
  ret >> result;
  return result;
  */
}

int main() {
  int n;
  std::cin >> n;
  vector<string> a(n);
  for (size_t i = 0; i < a.size(); i++)
  {
    std::cin >> a[i];
  }
  std::cout << largest_number(a);
}
