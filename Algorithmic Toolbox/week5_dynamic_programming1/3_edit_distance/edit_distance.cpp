#include <iostream>
#include <string>
#include <vector>

using namespace std;

int edit_distance(const string &str1, const string &str2)
{
  int distance_array[str1.length() + 1][str2.length() + 1];
  int insertion, deletion, mismatch, match;
  for(int i = 0; i <= str1.length(); i++)
  {
    for(int j = 0; j <= str2.length(); j++)
    {
      if(i == 0)
      {
        distance_array[i][j] = j;
      }
      else if(j == 0)
      {
        distance_array[i][j] = i;
      }
      else
      {
        insertion = distance_array[i][j - 1] + 1;
        deletion = distance_array[i - 1][j] + 1;
        match = distance_array[i - 1][j - 1];
        mismatch = distance_array[i - 1][j - 1] + 1;

        if(str1[i - 1] == str2[j - 1])
        {
          distance_array[i][j] = min(min(insertion, deletion), match);
        }
        else
        {
          distance_array[i][j] = min(min(insertion, deletion), mismatch);
        }
      }
    }
  }

  return distance_array[str1.length()][str2.length()];
}

int main() {
  string str1;
  string str2;
  std::cin >> str1 >> str2;
  std::cout << edit_distance(str1, str2) << std::endl;
  return 0;
}
