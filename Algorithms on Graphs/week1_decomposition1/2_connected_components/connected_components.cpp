#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int number_of_components(vector<vector<int> > &adj) {
  int res = 0;
  //write your code here
  vector<bool> visited(adj.size(), false);
  stack<int> dfs_stack;
  for(int node = 0; node < adj.size(); node++)
  {
    if(!visited[node])
    {
      res++;
      dfs_stack.push(node);
      while(!dfs_stack.empty())
      {
        int current_node = dfs_stack.top();
        dfs_stack.pop();

        if(!visited[current_node])
        {
          visited[current_node] = true;
          for(auto neighbour: adj[current_node])
            dfs_stack.push(neighbour);
        }
      }
    }
  }
  return res;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  std::cout << number_of_components(adj);
}
