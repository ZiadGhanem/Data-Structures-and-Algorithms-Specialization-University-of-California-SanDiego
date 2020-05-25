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

  // write your code here
  int text_length = bwt.length();
  string text(text_length, '$');


  //The first column of the Burrows-Wheeler matrix is the Burrows-Wheeler transform sorted
  //Each letter should have a corresponding location so we don't have to recalculate LastToFirst map
  vector<pair<char, int>> first_column(text_length);

  for(int i = 0; i < text_length; i++)
  {
    first_column[i].first = bwt[i];
    first_column[i].second = i;
  }

  sort(begin(first_column), end(first_column), [](pair<char, int> a, pair<char, int> b){
    return (a.first < b.first);
  });

/*
  for(auto current: first_column)
    cout << current.first;
  cout << endl;
  for(auto current: first_column)
    cout << current.second << " ";
  cout << endl;
*/

  //Traverse between last and first columns
  int current_index = first_column[0].second;
  for(int i = 0; i < text_length; i++)
  {
    current_index = first_column[current_index].second;
    text[i] = first_column[current_index].first;
  }

  return text;



/*
  int text_length = bwt.length();

  // Original text result
  string text;

  // First columm of BWT Matrix
  string first_column;

  // Mapping from last column index to first column index
  vector<int> last_to_first;

  // Number of occurrences of letter for counting sorted
  map<char, int> count;
  map<char, bool> first_occurrence;
  map<char, int> first_position;

  // Counting sort in O(|BWT|) time

  // Get first occurrence of each letter
  for(int i = 0; i < text_length; i++)
  {
    if(!first_occurrence[bwt[i]])
    {
      first_occurrence[bwt[i]] = true;
      first_position[bwt[i]] = i;
    }
    count[bwt[i]]++;
  }

  // Create first column
  for(auto it = begin(count); it != end(count); it++)
  {
    while(it->second > 0)
    {
      first_column += it->first;
      it->second--;
    }
  }

  for(char letter: first_column)
  {
    last_to_first.push_back(first_position[letter] + count[letter]);
    count[letter]++;
  }


  for(int i: last_to_first)
    cout << i << " ";
  cout << endl;

  int current_index = 0;
  for(int i = 0; i < text_length; i++)
  {
    text += first_column[current_index];
    current_index = last_to_first[current_index];
  }
  reverse(begin(text), end(text));

  return text;
*/
}

int main() {
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

/*
  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
  */
}
