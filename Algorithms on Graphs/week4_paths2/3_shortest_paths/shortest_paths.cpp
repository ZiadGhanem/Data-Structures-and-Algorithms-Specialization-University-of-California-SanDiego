#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

void shortest_paths(vector<vector<int>> &adj, vector<vector<int> > &cost, int s, vector<long long> &distance, vector<int> &reachable, vector<int> &shortest)
{
  //write your code here
  distance[s] = 0;
  // Queue to detect nodes along negative cycles
  queue<int> bfs_queue;
  // Run Bellman-Ford algorithm for |V| - 1 iterations + extra iteration to detect negative cycles
  for(int i = 0; i < adj.size(); i++)
  {
    // Loop over all edges
    for(int j = 0; j < adj.size(); j++)
    {
      for(int k = 0; k < adj[j].size(); k++)
      {
        long long current_distance = distance[j] + cost[j][k];
        if(distance[adj[j][k]] > current_distance)
        {
          distance[adj[j][k]] = current_distance;
          // At last iteration if the distance was updated then there's a negative cycle
          if(i == adj.size() - 1)
          {
            bfs_queue.push(adj[j][k]);
          }
        }
      }
    }
  }

  // Run BFS to detect nodes along negative cycles path
  while(!bfs_queue.empty())
  {
    int current_node = bfs_queue.front();
    bfs_queue.pop();
    for(int neighbor_index = 0; neighbor_index < adj[current_node].size(); neighbor_index++)
    {
      if(shortest[adj[current_node][neighbor_index]])
      {
        shortest[adj[current_node][neighbor_index]] = 0;
        bfs_queue.push(adj[current_node][neighbor_index]);
      }
    }
  }

  // Check if a vertex wasn't updated at all then it is not reachable
  for(int i = 0; i < adj.size(); i++)
  {
    if(distance[i] < INT32_MAX)
      reachable[i] = 1;
  }
}

int main() {
  int n, m, s;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cin >> s;
  s--;
  vector<long long> distance(n, INT32_MAX);
  vector<int> reachable(n, 0);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, reachable, shortest);
  for (int i = 0; i < n; i++) {
    if (!reachable[i]) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i] << "\n";
    }
  }
}
