#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

bool bipartite(vector<vector<int>> &adj)
{
  vector<char> color(adj.size(), -1);
  queue<int> bfs_queue;

  color[0] = 1;
  bfs_queue.push(0);

  while(!bfs_queue.empty())
  {
    int current_node = bfs_queue.front();
    bfs_queue.pop();
    for(auto neighbor: adj[current_node])
    {
      if(color[neighbor] == -1)
      {
        bfs_queue.push(neighbor);
        color[neighbor] = 1 - color[current_node];
      }
      else if(color[neighbor] == color[current_node])
      {
        return false;
      }
    }
  }
  return true;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  std::cout << bipartite(adj);
}
