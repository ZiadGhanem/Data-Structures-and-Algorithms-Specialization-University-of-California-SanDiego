#include <iostream>
#include <vector>

using std::vector;

struct edge
{
  int source;
  int destination;
  int cost;
};

int negative_cycle(int number_vertices, vector<edge> &edges)
{
  //write your code here
  bool changed;
  int current_distance;
  vector<int> distances(number_vertices, INT32_MAX  / 2);
  distances[0] = 0;
  for(int i = 0; i < number_vertices; i++)
  {
    changed = false;
    for(auto edge: edges)
    {
      current_distance = distances[edge.source] + edge.cost;
      if(distances[edge.destination] > current_distance)
      {
        distances[edge.destination] = current_distance;
        changed = true;
      }
    }
  }

  return (changed)?1:0;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<edge> edges;
  for (int i = 0; i < m; i++)
  {
    edge current_edge;
    std::cin >> current_edge.source >> current_edge.destination >> current_edge.cost;
    current_edge.source--;
    current_edge.destination--;
    edges.push_back(current_edge);
  }
  std::cout << negative_cycle(n, edges);
}
