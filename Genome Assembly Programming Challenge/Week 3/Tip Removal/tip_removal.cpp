
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

#define NUM_READS 1618
#define READ_LENGTH 100
#define KMER_SIZE  15

using namespace std;

void create_debruijn_graph(
  vector<string>& reads,
  unordered_map<string, unordered_set<string>>& forward_graph,
  unordered_map<string, unordered_set<string>>& reverse_graph)
{
  vector<string> kmer_reads;

  // Create K-mers
  for(auto& read: reads)
  {
    for(int i = 0; i < read.size() - KMER_SIZE + 1; i++)
      kmer_reads.push_back(read.substr(i, KMER_SIZE));
  }

  for(int i = 0; i < kmer_reads.size(); i++)
  {
    // Labelling vertices by (k - 1)mers
    string source = kmer_reads[i].substr(0, KMER_SIZE - 1);
    string destination = kmer_reads[i].substr(1);

    //cout << source << " " << destination << endl;

    // Label edges by k-mers
    forward_graph[source].insert(destination);
    reverse_graph[destination].insert(source);
  }
}

void get_post_order(
  unordered_map<string, unordered_set<string>>& reverse_graph,
  unordered_set<string>& visited,
  string source,
  vector<string>& post_order)
  {
    // Create a DFS stack
    stack<string> dfs_stack;

    // Add source node to stack
    dfs_stack.push(source);
    // Set the source node as visited
    visited.insert(source);

    // While there is a non visited node
    while(!dfs_stack.empty())
    {
      // Get the node at the top of the stack
      string current_node = dfs_stack.top();

      // Assume that all the neighbors of the node are already visited at first
      bool visting_neighbors = false;

      // Visit neighbors of the node
      for(string neighbor: reverse_graph[current_node])
      {
        // If a neighbor node is not visited
        if(visited.find(neighbor) == end(visited))
        {
          // Then we're currently still visitng neighbors
          visting_neighbors = true;
          // Set the neighbor as visited
          visited.insert(neighbor);
          // Add the neighbor to the stack
          dfs_stack.push(neighbor);
          break;
        }
      }

      if(!visting_neighbors)
      {
        post_order.push_back(dfs_stack.top());
        dfs_stack.pop();
      }
  }
}

void compute_scc(
  unordered_map<string, unordered_set<string>>& forward_graph,
  unordered_set<string>& visited,
  string source)
{
  // Create a DFS stack
  stack<string> dfs_stack;
  // Add source node to stack
  dfs_stack.push(source);
  // Set the source node as visited
  visited.insert(source);

  // While there is a non visited node
  while(!dfs_stack.empty())
  {
    // Get the node at the top of the stack
    string current_node = dfs_stack.top();
    dfs_stack.pop();

    // Visit neighbors of the node
    for(string neighbor: forward_graph[current_node])
    {
      // If a neighbor node is not visited
      if(visited.find(neighbor) == end(visited))
      {
        // Set the neighbor as visited
        visited.insert(neighbor);

        // Add the neighbor to the stack
        dfs_stack.push(neighbor);
      }
    }
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

  // Create Forward and Reverse De-Bruijn Graph
  unordered_map<string, unordered_set<string>> forward_graph;
  unordered_map<string, unordered_set<string>> reverse_graph;
  create_debruijn_graph(reads, forward_graph, reverse_graph);

  // Get Post-Order of nodes
  unordered_set<string> visited;
  vector<string> post_order;
  for(auto& current_node :reverse_graph)
  {
    if(visited.find(current_node.first) == end(visited))
      get_post_order(reverse_graph, visited, current_node.first, post_order);
  }

  // Get the number strongly connected components get using DFS in reverse order of post order
  visited.clear();
  int num_sccs = 0;
  for(auto current_node = rbegin(post_order); current_node != rend(post_order); ++current_node)
  {
    if(visited.find(*current_node) == end(visited))
    {
      compute_scc(forward_graph, visited, *current_node);
      num_sccs++;
    }
  }

  cout << num_sccs - 1;

  return 0;

}
