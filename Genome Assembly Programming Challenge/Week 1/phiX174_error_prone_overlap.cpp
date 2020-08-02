#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#define NUM_READS 1618
#define READ_LENGTH 100
#define ALLOWED_ERRORS 2
#define MER 12

using namespace std;

struct Overlap{
  int index;
  int value;
};

bool are_equal(string& read_a, string& read_b)
{
  int mismatches = 0;
  for(int i = 0; i < read_a.size(); i++)
  {
    if(read_a[i] != read_b[i])
      mismatches++;
    if(mismatches > ALLOWED_ERRORS)
      return false;
  }
  return true;
}


bool get_overlap(string read_a, string read_b, int& overlap_value)
{
  for(int i = READ_LENGTH - 1; i >= READ_LENGTH - MER; i--)
  {
    if(are_equal(read_a, read_b))
    {
      overlap_value = i;
      return true;
    }

    // Remove first character from first read
    read_a.erase(0, 1);
    // Remove last character from second read
    read_b.pop_back();
  }

  return false;
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
  vector<Overlap> path;

  int current_read = 0;
  int max_overlap_value = -1;
  int max_overlap_index = -1;
  Overlap current_overlap;

  vector<bool> visited(reads.size(), false);

  // Visit all reads using greedy approach
  for(int i = 0; i < reads.size(); i++)
  {
    // Add non overlap part to the genome
    result += reads[current_read].substr(max_overlap_value + 1);

    // Make current read visited
    visited[current_read] = true;

    // Add to path
    current_overlap.index = current_read;
    current_overlap.value = READ_LENGTH - (max_overlap_value + 1);
    path.push_back(current_overlap);

    // Get a non visited read with the maximum overlap with current read
    max_overlap_value = -1;
    max_overlap_index = -1;

    for(int j = 0; j < reads.size(); j++)
    {
      // Get the unvisited read with the maximum overlap
      if(!visited[j])
      {
        // If the overlap between current read and next read is within the error range
        if(get_overlap(reads[current_read], reads[j], current_overlap.value) && (current_overlap.value >= max_overlap_value))
        {
            max_overlap_value = current_overlap.value;
            max_overlap_index = j;
        }
      }
    }

    // Set the maximum overlap read as current read
    current_read = max_overlap_index;
  }

  cout << result << endl;

  unordered_map<char, int> char_frequency;
  int offset = 0;

  // Every read used in the genome
  for(Overlap current_read: path)
  {
    // The letters added to the genome by this read
    for(int i = 0; i < current_read.value; i++)
    {
      char_frequency.clear();
      // Reads overlapping with this read
      for(int j = 0; j < 4; j++)
      {
        // Choose neighbor
        Overlap neighbor = path[(current_read.index + j) % path.size()];

        // If the overlapping read overlaps at the current letter
        // Then increase the letter frequency
        if(i >= neighbor.value)
            char_frequency[reads[neighbor.index][i - neighbor.value]]++;
      }

      // Get the letter with maximum frequency
      char max_letter;
      int max_frequency = 0;
      for(auto letter: char_frequency)
      {
        if(letter.second >= max_frequency)
        {
          max_frequency = letter.second;
          max_letter = letter.first;
        }
      }

      // Change the letter
      result[i + offset] = max_letter;
    }
    // Increase the offset by the letters contributed by current read
    offset += current_read.value;
  }

  return result.substr(0, result.size() - largest_prefix_suffix(result));
}

int main(void)
{
  // Get non-repeated reads
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
