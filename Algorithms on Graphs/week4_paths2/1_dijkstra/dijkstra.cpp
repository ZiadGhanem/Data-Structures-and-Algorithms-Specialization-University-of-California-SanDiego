#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int distance(vector<vector<pair<int, int>>> &adj, int s, int t)
{
  vector<int> distances(adj.size(), INT32_MAX / 2);
  distances[s] = 0;
  priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> dijkstra_heap;
  dijkstra_heap.push(make_pair(distances[s], s));
  while(!dijkstra_heap.empty())
  {
    pair<int, int> current_pair = dijkstra_heap.top();
    dijkstra_heap.pop();
    for(auto neighbor: adj[current_pair.second])
    {
      int current_distance = current_pair.first + neighbor.first;
      if(distances[neighbor.second] > current_distance)
      {
        distances[neighbor.second] = current_distance;
        dijkstra_heap.push(make_pair(current_distance, neighbor.second));
      }
    }
  }
  return (distances[t] < INT32_MAX / 2)?distances[t]:-1;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<pair<int, int>>> adj(n);
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(make_pair(w, y - 1));
  }
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, s, t);
}
