#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using std::map;


vector<int> sort_characters(const string& text)
{
  // Sorted indices of the characters of the string
  vector<int> order(text.length());
  // The number of occurrences of each character in the string
  map<char, int> count;

  // Count the number of occurrences of each character in the string
  for(char letter: text)
    count[letter]++;

  // Accumulate the count of characters in the string
  auto it = begin(count);
  int previous_val = it->second;
  it++;
  for(; it != end(count); it++)
  {
    it->second += previous_val;
    previous_val = it->second;
  }

  // Set the order of each character in the string
  for(int i = text.length() - 1; i >= 0; i--)
  {
    char letter = text[i];
    count[letter]--;
    order[count[letter]] = i;
  }

  return order;
}

vector<int> compute_char_classes(const string& text, vector<int>& order)
{
  // Each character is given a certain class
  vector<int> classes(text.length());

  // Give first character class 0
  classes[order[0]] = 0;
  for(int i = 1; i < text.length(); i++)
  {
    // If the next character is not the same as previous give it a new class
    if(text[order[i]] != text[order[i - 1]])
      classes[order[i]] = classes[order[i - 1]] + 1;
    else
      classes[order[i]] = classes[order[i - 1]];
  }

  return classes;
}

vector<int> sort_doubled(const string& text, int l, vector<int>& order, vector<int>& classes)
{
  // The number of occurrences of each class of double cyclic shifts
  vector<int> count(text.length(), 0);

  // Sorted double cylic shifts of length l
  vector<int> new_order(text.length());

  // Count the number occurences of each class of double cyclic shifts
  for(int i = 0; i < text.length(); i++)
    count[classes[i]]++;

  // Accumulate the count of double cylic shifts
  for(int i = 1; i < text.length(); i++)
    count[i] += count[i - 1];

  // Set the new order of double cylic shifts
  for(int i = text.length() - 1; i >=0 ; i--)
  {
    int start = (order[i] - l + text.length()) % text.length();
    int current_class = classes[start];
    count[current_class]--;
    new_order[count[current_class]] = start;
  }

  return new_order;
}


vector<int> update_classes(vector<int>& new_order, vector<int>& classes, int l)
{
  int n = new_order.size();
  vector<int> new_classes(n);
  new_classes[new_order[0]] = 0;

  // Compare pairs of classes of double cyclic shifts
  for(int i = 1; i < n; i++)
  {
    int cur = new_order[i];
    int prev = new_order[i - 1];
    int mid = (cur + l) %  n;
    int mid_prev = (prev + l) % n;

    // If the two pairs were not equal then give it a new class
    if((classes[cur] != classes[prev]) || (classes[mid] != classes[mid_prev]))
      new_classes[cur] = new_classes[prev] + 1;
    else
      new_classes[cur] = new_classes[prev];
  }

  return new_classes;
}

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text)
{
  // Sort characters using count sort
  vector<int> order = sort_characters(text);

  // Give each character a class
  vector<int> classes = compute_char_classes(text, order);

  // Loop over double cylic shifts
  for(int l = 1; l < text.length(); l*=2)
  {
    // Get the new order of double cylic shifts
    order = sort_doubled(text, l, order, classes);

    // Get new class of double cyclic shifts
    classes = update_classes(order, classes, l);
  }

  return order;
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
