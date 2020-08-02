#include <iostream>
#include <vector>
#include <unordered_set>

#define NUM_READS 1618
#define READ_LENGTH 100

using namespace std;


int get_overlap(string read_a, string read_b)
{
  for(int i = READ_LENGTH - 1; i >= 0; i--)
  {
    if(read_a == read_b)
      return i;

    // Remove first character from first read
    read_a.erase(0, 1);
    // Remove last character from second read
    read_b.pop_back();
  }

  return -1;
}

int largest_prefix_suffix(string &str)
{
  int n = str.length();

  if(n < 2)
    return 0;

  int len = 0;
  int i = n/2;

  while(i < n)
  {
    if(str[i] == str[len])
    {
      ++len;
      ++i;
    }
    else
    {
      if(len == 0)// no prefix
        ++i;
      else // search for shorter prefixes
        --len;
    }
  }

  return len;

}

string error_free_genome_assembly(vector<string>& reads)
{
  string result;

  int current_read = 0;
  int max_overlap_value = -1;
  int max_overlap_index = -1;

  vector<bool> visited(reads.size(), false);

  // Visit all reads using greedy approach
  for(int i = 0; i < reads.size(); i++)
  {
    // Add non overlap part to the genome
    result += reads[current_read].substr(max_overlap_value + 1);
    // Make current read visited
    visited[current_read] = true;

    // Get a non visited read with the maximum overlap with current read
    max_overlap_value = -1;
    max_overlap_index = -1;
    for(int j = 0; j < reads.size(); j++)
    {
      if(!visited[j])
      {
        int current_overlap_value = get_overlap(reads[current_read], reads[j]);
        if(current_overlap_value >= max_overlap_value)
        {
          max_overlap_value = current_overlap_value;
          max_overlap_index = j;
        }
      }
    }

    // Set the maximum overlap read as current read
    current_read = max_overlap_index;
  }

  return result.substr(0, result.size() - largest_prefix_suffix(result));
}

int main(void)
{
  vector<string> reads;
  unordered_set<string> reads_hashset;

  for(int i = 0; i < NUM_READS; i++)
  {
    string temp;
    cin >> temp;
    if(reads_hashset.find(temp) == end(reads_hashset))
    {
      reads.push_back(temp);
      reads_hashset.insert(temp);
    }
  }

  cout << error_free_genome_assembly(reads);

  return 0;
}
