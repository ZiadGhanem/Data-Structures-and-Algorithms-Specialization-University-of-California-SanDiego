#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string BWT(const string& text) {
  string result = "";
  // write your code here
  int text_length = text.length();
  vector<string> bwt_matrix(text_length);

  // Create all cyclic rotations
  for(int i = 0; i < text_length; i++)
  {
    for(int j = 0; j < text_length; j++)
    {
      bwt_matrix[i] += text[(i + j) % text_length];
    }
  }

  // Sort the cyclic rotations lexicographically
  sort(begin(bwt_matrix), end(bwt_matrix));

  // Get the last columns of the Burrows-Wheeler Matrix
  for(int i = 0; i < text_length; i++)
  {
    result += bwt_matrix[i][text_length - 1];
  }

  return result;
}

int main() {
  string text;
  cin >> text;
  cout << BWT(text) << endl;
  return 0;
}
