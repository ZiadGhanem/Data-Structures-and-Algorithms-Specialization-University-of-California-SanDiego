#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Data {
    string pattern, text;
};

Data read_input() {
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

void print_occurrences(const std::vector<int>& output) {
    for (size_t i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

bool are_equal(const string &s1, const string &s2)
{
  if(s1.length() != s2.length())
    return false;

  for(int i = 0; i < s1.length(); i++)
  {
    if(s1[i] != s2[i])
      return false;
  }
  return true;
}

unsigned long long poly_hash(const string& s, unsigned long long prime, unsigned long long multiplier)
{
    unsigned long long hash = 0;
    for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
        hash = (hash * multiplier + s[i]) % prime;
    return hash;
}

vector<unsigned long long> precompute_hashes(const string &text, int pattern_length, unsigned long long prime, unsigned long long multiplier)
{
  vector<unsigned long long> hashes(text.length() - pattern_length + 1);
  hashes[text.length() - pattern_length] = poly_hash(text.substr(text.length() - pattern_length , pattern_length), prime, multiplier);
  unsigned long long y = 1;
  for(int i = 0; i < pattern_length; i++)
    y = (y * multiplier) % prime;
  for(int i = text.length() - pattern_length - 1; i >= 0; i--)
    hashes[i] = (unsigned long long)(((multiplier * hashes[i + 1]) + text[i] - (y * text[i + pattern_length])) % prime);

  return hashes;
}

vector<int> get_occurrences(const Data& input)
{
    const string& pattern = input.pattern, text = input.text;

    unsigned long long prime = 1000000007;
    unsigned long long multiplier = 1;
    vector<int> result;
    vector<unsigned long long> hashes = precompute_hashes(text, pattern.length(), prime, multiplier);
    unsigned long long pattern_hash = poly_hash(pattern, prime, multiplier);
    for(int i = 0; i < text.length() - pattern.length() + 1; i++)
    {
      if(pattern_hash != hashes[i])
        continue;
      if(text.compare(i, pattern.length(), pattern) == 0)
        result.push_back(i);
    }
    return result;
}

/*
std::vector<int> get_occurrences(const Data& input) {
    const string& s = input.pattern, t = input.text;
    std::vector<int> ans;
    for (size_t i = 0; i + s.size() <= t.size(); ++i)
        if (t.substr(i, s.size()) == s)
            ans.push_back(i);
    return ans;
}
*/

int main() {
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences(read_input()));
    return 0;
}
