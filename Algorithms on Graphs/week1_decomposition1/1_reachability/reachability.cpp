#include <iostream>
#include <vector>
#include <stack>

using namespace std;


int reach(vector<vector<int> > &adj, int x, int y)
{
  vector<bool> visited(adj.size(), false);
  stack<int> dfs_stack;
  dfs_stack.push(x);
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

  return visited[y]? 1:0;
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
  int x, y;
  std::cin >> x >> y;
  std::cout << reach(adj, x - 1, y - 1);
}
