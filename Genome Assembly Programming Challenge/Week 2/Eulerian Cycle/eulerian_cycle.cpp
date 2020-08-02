#include <iostream>
#include <vector>
#include <stack>

using namespace std;


bool find_eulerian_cycle(vector<vector<int>>& graph, int num_edges, vector<int>& path)
{

  // Check if the graph is in balanced
  vector<int> in_degree(graph.size(), 0);
  for(auto& current_node: graph)
  {
    for(int destination: current_node)
      in_degree[destination]++;
  }


  for(int i = 0; i < graph.size(); i++)
  {
    if(in_degree[i] != graph[i].size())
      return false;
  }

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

  // Check if cycle has is sufficient number of edges included or not. If not then original graph might be disconnected and Euler Path can't exist in this case.
  if(path.size() < num_edges)
    return false;

  return true;

}

int main(void)
{
  // Read the number of vertices and edges
  int num_vertices, num_edges;
  cin >> num_vertices >> num_edges;

  // Read and create the graph;
  vector<vector<int>> graph(num_vertices);
  int source, destination;
  for(int i = 0; i < num_edges; i++)
  {
    cin >> source >> destination;
    source--;
    destination--;
    graph[source].push_back(destination);
  }

  vector<int> path;

  if(find_eulerian_cycle(graph, num_edges, path))
  {
    cout << "1" << endl;
    for(int i = 0; i < path.size() - 1; i++)
      cout << path[i] + 1 << " ";
  }
  else
  {
    cout << "0" << endl;
  }

  return 0;
}
