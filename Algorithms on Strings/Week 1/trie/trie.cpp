#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/*
typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  trie t;
  // write your code here

  // Add a root
  edges new_edge;
  t.push_back(new_edge);

  int edge_count = 0;

  for(string &pattern: patterns)
  {
    for(int trie_index = 0; trie_index < pattern.length(); trie_index++)
    {
      // If the current level exceeds trie size add a new level
      if(trie_index >= t.size())
      {
        edges new_edge;
        t.push_back(new_edge);
      }

      if(t[trie_index].find(pattern[trie_index]) == end(t[trie_index]))
      {
        edge_count++;
        t[trie_index].insert(make_pair(pattern[trie_index], edge_count));
      }
    }
  }
  return t;
}


int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}
*/

struct Vertex
{
  int vertex_id;
  unordered_map<char, Vertex*> children;
};

Vertex* build_trie(vector<string> & patterns)
{
  Vertex *root = new Vertex;
  root->vertex_id = 0;

  Vertex* current_vertex = root;

  int current_id = 0;

  current_vertex = root;

  for(string &pattern: patterns)
  {
    current_vertex = root;

    for(char character: pattern)
    {
      if(current_vertex->children.find(character) == end(current_vertex->children))
      {
        Vertex* new_vertex = new Vertex;
        current_vertex->children.insert(make_pair(character, new_vertex));
        current_vertex = new_vertex;
        current_id++;
        current_vertex->vertex_id = current_id;
      }
      else
      {
        current_vertex = current_vertex->children[character];
      }
    }
  }
  return root;
}

void print_trie(Vertex* trie)
{
  for(auto it = begin(trie->children); it != end(trie->children); it++)
  {
    cout << trie->vertex_id << "->" << it->second->vertex_id << ":" << it->first << "\n";
    print_trie(it->second);
  }
}


int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++)
  {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  Vertex* trie = build_trie(patterns);
  print_trie(trie);

  return 0;
}
