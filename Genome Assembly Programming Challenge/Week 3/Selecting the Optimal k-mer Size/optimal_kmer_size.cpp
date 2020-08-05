#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

#define NUM_READS 400
#define READ_LENGTH 100

using namespace std;


bool find_eulerian_cycle(unordered_map<string, unordered_set<string>>& graph)
{
  // Check if the graph is not balanced
  unordered_map<string, int> in_degree;
  for(auto& current_node: graph)
  {
    for(auto& destination: current_node.second)
      in_degree[destination]++;
  }


  for(auto& current_node: graph)
  {
    //cout << current_node.first << " " << in_degree[current_node.first] << " " << current_node.second.size() << endl;
    if(current_node.second.size() != in_degree[current_node.first])
      return false;
  }

  return true;
}

void create_debruijn_graph(vector<string>& reads, unordered_map<string, unordered_set<string>>& graph)
{
  for(int i = 0; i < reads.size(); i++)
  {
    // Labelling vertices by (k - 1)mers
    string source = reads[i].substr(0, reads[i].length() - 1);
    string destination = reads[i].substr(1);

    //cout << source << " " << destination << endl;

    // Label edges by k-mers
    graph[source].insert(destination);
  }
}

int main(void)
{
  // Get Reads
  vector<string> reads;
  string current_read;
  for(int i = 0; i < NUM_READS; i++)
  {
    cin >> current_read;
    reads.push_back(current_read);
  }

    // Create DeBruijn Graph
    unordered_map<string, unordered_set<string>> graph;
    vector<string> kmer_reads;

    for(int k = READ_LENGTH; k >= 2; k--)
    {
      graph.clear();
      kmer_reads.clear();

      // Create K-mers
      for(auto& read: reads)
      {
        for(int i = 0; i < READ_LENGTH - k + 1; i++)
          kmer_reads.push_back(read.substr(i, k));
      }

      // Create DeBruijn Graph
      create_debruijn_graph(kmer_reads, graph);

      // Find an Eulerian path in DeBruijn Graph
      if(find_eulerian_cycle(graph))
      {
        cout << k;
        break;
      }
    }

    return 0;

}
