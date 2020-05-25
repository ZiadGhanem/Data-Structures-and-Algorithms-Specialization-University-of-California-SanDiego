#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;



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


string InverseBWT(const string& bwt) {
  int text_length = bwt.length();

  // Original text result
  string text(text_length, '$');

  // First columm of BWT Matrix
  string first_column;

  // Mapping from last column index to first column index
  vector<int> last_to_first;

  // Number of occurrences of letter for counting sorted
  map<char, int> count;
  // First position at which the letter occurs in the first column
  map<char, int> first_position;

  // Counting sort in O(|BWT|) time

  // Count numer of each letter
  for(char letter: bwt)
    count[letter]++;

  // Create first column and get first position of each letter
  int current_position = 0;
  for(auto it = begin(count); it != end(count); it++)
  {
    first_position[it->first] = current_position;
    while(it->second > 0)
    {
      first_column += it->first;
      it->second--;
      current_position++;
    }
  }

  // Create last to first map
  for(char letter: bwt)
  {
    last_to_first.push_back(first_position[letter] + count[letter]);
    count[letter]++;
  }

  // Last to first traversal
  int current_index = 0;
  for(int i = 0; i < text_length; i++)
  {
    text[text_length - i - 1] = first_column[current_index];
    current_index = last_to_first[current_index];
  }

  return text;
}

int main() {
  /*
    srand(time(0));
    char alphabet[4] = {'A', 'C', 'G', 'T'};

    while(true)
    {
      string random_text;
      int n = rand() % 20;
      for (int i = 0; i < n; i++)
          random_text += alphabet[rand() % 4];
      random_text += '$';

      string bwt = BWT(random_text);
      string inv_bwt = InverseBWT(bwt);

      cout << "Original text: " << random_text << endl;
      cout << "BWT: " << bwt << endl;
      cout << "Inverse BWT: " << inv_bwt << endl;
      if(inv_bwt != random_text)
      {
        cout << "Mismatch!" << endl;
        return 0;
      }
      cout << endl << endl;
    }
*/

  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
}
