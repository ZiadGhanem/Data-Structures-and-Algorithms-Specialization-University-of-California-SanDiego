#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
  vector<int> result;
  // Implement this function yourself

  int text_length = text.length();
  vector<pair<string, int>> bwt_matrix(text_length);
  for(int i = 0; i < text_length; i++)
    bwt_matrix[i].second = i;


  // Create all cyclic rotations
  for(int i = 0; i < text_length; i++)
  {
    for(int j = 0; j < text_length; j++)
    {
      bwt_matrix[i].first += text[(i + j) % text_length];
    }
  }

  // Sort the cyclic rotations lexicographically
  sort(begin(bwt_matrix), end(bwt_matrix), [](pair<string, int> a, pair<string, int> b){
    return (a.first < b.first);
  });

  for(int i = 0; i < text_length; i++)
    result.push_back(bwt_matrix[i].second);

  return result;
}

int main() {
  string text;
  cin >> text;
  vector<int> suffix_array = BuildSuffixArray(text);
  for (int i = 0; i < suffix_array.size(); ++i) {
    cout << suffix_array[i] << ' ';
  }
  cout << endl;
  return 0;
}
