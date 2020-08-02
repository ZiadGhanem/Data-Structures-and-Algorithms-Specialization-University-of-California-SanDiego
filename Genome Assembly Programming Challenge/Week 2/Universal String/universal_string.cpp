#include <iostream>
#include <vector>
#include <stack>

using namespace std;


void find_eulerian_cycle(vector<vector<int>>& graph, vector<int>& path)
{
  // Try to find Euler cycle in this modified graph using HIERHOLZER’S ALGORITHM. ( Time Complexity : O( V+E ) )
  stack<int> euler_stack;

  // Choose any vertex v and push it onto a stack. Initially all edges are unmarked.
  euler_stack.push(0);

  // While the stack is non-empty, look at the top vertex, u, on the stack.
  while(!euler_stack.empty())
  {
    int current_node = euler_stack.top();

    // If u has no unmarked incident edge, then pop u off the stack and add it to euler path.
    if(graph[current_node].empty())
    {
      euler_stack.pop();
      path.insert(begin(path), current_node);
    }
    // If u has an unmarked incident edge, say, to a vertex w, then push w onto the stack and mark the edge uw.
    else
    {
      euler_stack.push(graph[current_node][0]);
      graph[current_node].erase(begin(graph[current_node]));
    }
  }
}

int main(void)
{
  // Read k
  int kmer;
  cin >> kmer;

  // Number of nodes in graph
  int max_value = 1 << kmer;

  // Create the graph
  vector<vector<int>> graph(max_value);
  for(int number = 0; number < max_value; number++)
  {
    // Create an edge from number to number shifted to the left by one
    graph[number].push_back((number * 2) % max_value);
    // Create an edge from number to number shifted to the left by one + 1
    graph[number].push_back(((number * 2) % max_value) + 1);
  }

  vector<int> path;
  find_eulerian_cycle(graph, path);

  for(auto node: path)
    cout << node % 2;

  return 0;
}
