#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position
//       of this character in the sorted array of
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt,
                   map<char, int>& starts,
                   map<char, vector<int> >& occ_count_before) {
  // Implement this function yourself

  // First columm of BWT Matrix
  string first_column;

  // Counting sort in O(|BWT|) time

  // Count numer of each letter
  for(int i = 0; i < bwt.length(); i++)
  {
    // If it is the first time for the letter
    if(occ_count_before.find(bwt[i]) == end(occ_count_before))
    {
      // Set all previous positions as zeros
      while(occ_count_before[bwt[i]].size() <= i)
        occ_count_before[bwt[i]].push_back(0);
      // Set current position as 1
      occ_count_before[bwt[i]].push_back(1);
    }
    // If the letter was inserted before
    else
    {
      int start_count = occ_count_before[bwt[i]].back();
      // Set all previous positions as the last time it was inserted
      while(occ_count_before[bwt[i]].size() <= i)
        occ_count_before[bwt[i]].push_back(start_count);
      // Set current position as last time + 1
      occ_count_before[bwt[i]].push_back(start_count + 1);
    }
  }

  // Fill remaining with last count
  for(auto it = begin(occ_count_before); it != end(occ_count_before); it++)
  {
    int start_count = it->second.back();
    while(it->second.size() <= bwt.length())
      it->second.push_back(start_count);
  }

  // Get first position of each letter
  int current_position = 0;
  for(auto it = begin(occ_count_before); it != end(occ_count_before); it++)
  {
    starts[it->first] = current_position;
    current_position += it->second.back();
  }

/*
  for(auto it = begin(occ_count_before); it != end(occ_count_before); it++)
  {
    cout << it->first << ": ";
    for(int i = 0; i < it->second.size(); i++)
      cout << it->second[i] << " ";
    cout << endl;
    cout << "First Occurrence: " << starts[it->first] << endl;
  }
  */
}

bool contains_symbol(const string& bwt, char symbol, int top, int bottom)
{
  for(int i = top; i <= bottom; i++)
  {
    if(bwt[i] == symbol)
      return true;
  }

  return false;
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string& pattern,
                     const string& bwt,
                     map<char, int>& starts,
                     map<char, vector<int> >& occ_count_before) {
  // Implement this function yourself
  int top = 0;
  int bottom = bwt.size() - 1;
  int pattern_index = pattern.size() - 1;
  while(top <= bottom)
  {
    if(pattern_index >= 0)
    {
      char symbol = pattern[pattern_index];
      pattern_index--;

      if(contains_symbol(bwt, symbol, top, bottom))
      {
        top = starts[symbol] + occ_count_before[symbol][top];
        bottom = starts[symbol] + occ_count_before[symbol][bottom + 1] - 1;
      }
      else
        return 0;
    }
    else
      return (bottom - top + 1);
  }
  return 0;
}


int main() {
  string bwt;
  cin >> bwt;
  int pattern_count;
  cin >> pattern_count;
  // Start of each character in the sorted list of characters of bwt,
  // see the description in the comment about function PreprocessBWT
  map<char, int> starts;
  // Occurrence counts for each character and each position in bwt,
  // see the description in the comment about function PreprocessBWT
  map<char, vector<int> > occ_count_before;
  // Preprocess the BWT once to get starts and occ_count_before.
  // For each pattern, we will then use these precomputed values and
  // spend only O(|pattern|) to find all occurrences of the pattern
  // in the text instead of O(|pattern| + |text|).
  PreprocessBWT(bwt, starts, occ_count_before);
  for (int pi = 0; pi < pattern_count; ++pi) {
    string pattern;
    cin >> pattern;
    int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
    printf("%d ", occ_count);
  }
  printf("\n");
  return 0;
}
