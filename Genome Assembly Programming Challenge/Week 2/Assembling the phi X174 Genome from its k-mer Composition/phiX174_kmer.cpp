#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <algorithm>

#define NUM_READS 5396
#define READ_LENGTH 10

using namespace std;


void find_eulerian_cycle(unordered_map<string, vector<string>>& graph, string& path)
{
  // Try to find Euler cycle in this modified graph using HIERHOLZER’S ALGORITHM. ( Time Complexity : O( V+E ) )
  stack<string> euler_stack;

  // Choose any vertex v and push it onto a stack. Initially all edges are unmarked.
  euler_stack.push(begin(graph)->first);

  // While the stack is non-empty, look at the top vertex, u, on the stack.
  while(!euler_stack.empty())
  {
    string current_node = euler_stack.top();

    // If u has no unmarked incident edge, then pop u off the stack and add it to euler path.
    if(graph[current_node].empty())
    {
      euler_stack.pop();
      path += current_node[0];
    }
    // If u has an unmarked incident edge, say, to a vertex w, then push w onto the stack and mark the edge uw.
    else
    {
      euler_stack.push(graph[current_node][0]);
      graph[current_node].erase(begin(graph[current_node]));
    }
  }
}

void create_debruijn_graph(vector<string>& reads, unordered_map<string, vector<string>>& graph)
{
  for(int i = 0; i < reads.size(); i++)
  {
    // Labelling vertices by (k - 1)mers
    string source = reads[i].substr(0, READ_LENGTH - 1);
    string destination = reads[i].substr(1);

    // Label edges by k-mers
    graph[source].push_back(destination);
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
  unordered_map<string, vector<string>> graph;
  create_debruijn_graph(reads, graph);

  // Find an Eulerian path in DeBruijn Graph
  string path;
  find_eulerian_cycle(graph, path);
  path.pop_back();
  reverse(begin(path), end(path));
  cout << path;

  return 0;
}
