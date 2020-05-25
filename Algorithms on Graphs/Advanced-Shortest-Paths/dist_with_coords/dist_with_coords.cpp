/*
#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <cmath>

using namespace std;

// See the explanations of these typedefs and constants in the starter for friend_suggestion
typedef vector<vector<vector<int>>> Adj;
typedef long long Len;
typedef vector<priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>>> Queue;

const Len INFINITY = numeric_limits<Len>::max() / 4;

class AStar {
    // See the descriptions of these fields in the starter for friend_suggestion
    int n_;
    Adj adj_;
    Adj cost_;
    vector<vector<Len>> distance_;
    vector<int> workset_;
    vector<bool> visited_;
    // Coordinates of the nodes./
    std::vector<std::pair<Len,Len>> xy_;

public:
    AStar(int n, Adj adj, Adj cost, std::vector<std::pair<Len,Len>> xy)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n_, INFINITY)), visited_(n), xy_(xy)
    { workset_.reserve(n); }

    // See the description of this method in the starter for friend_suggestion
    void clear() {
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = INFINITY;
            visited_[v] = false;
        }
        workset_.clear();
    }

    // See the description of this method in the starter for friend_suggestion
    void visit(Queue& q, int side, int v, Len dist) {
        // Implement this method yourself
    }

    // Returns the distance from s to t in the graph
    Len query(int s, int t) {
        clear();
        Queue q(2);
        visit(q, 0, s, 0);
        visit(q, 1, t, 0);
        // Implement the rest of the algorithm yourself

        return -1;
    }
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<std::pair<Len,Len>> xy(n);
    for (int i=0;i<n;++i){
        int a, b;
        scanf("%d%d", &a, &b);
        xy[i] = make_pair(a,b);
    }
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i=0; i<m; ++i) {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);
        adj[0][u-1].push_back(v-1);
        cost[0][u-1].push_back(c);
        adj[1][v-1].push_back(u-1);
        cost[1][v-1].push_back(c);
    }

    AStar astar(n, adj, cost, xy);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", astar.query(u-1, v-1));
    }
}
*/


#include <iostream>
#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <math.h>

using namespace std;

const long long infinity = numeric_limits<long long>::max() / 4;

struct Edge
{
  int neighbor;
  long long cost;
};

struct Coordinates
{
  int x;
  int y;
};

struct Node
{
  vector<Edge> edges;
  Coordinates coordinates;
};


struct Graph
{
    Node *nodes;
    int number_nodes;

    Graph(int number_nodes)
    :number_nodes(number_nodes)
    {
      this->nodes = new Node[number_nodes];
    }
};


class BiDirectionalAStar
{
public:
  BiDirectionalAStar(int number_nodes)
  :number_nodes_(number_nodes)
  {
    this->forward_graph_ = new Graph(this->number_nodes_);
    this->reverse_graph_ = new Graph(this->number_nodes_);
  }

  void add_coordinates(int node_index, int a, int b)
  {
    Coordinates current_coordinates = {
      .x = a,
      .y = b
    };

    this->forward_graph_->nodes[node_index].coordinates = current_coordinates;
    this->reverse_graph_->nodes[node_index].coordinates = current_coordinates;
  }

  void add_edge(int source, int destination, int cost)
  {
    Edge forward_edge = {
      .neighbor = destination,
      .cost = cost
    };

    Edge reverse_edge = {
      .neighbor = source,
      .cost = cost
    };

    this->forward_graph_->nodes[source].edges.push_back(forward_edge);
    this->reverse_graph_->nodes[destination].edges.push_back(reverse_edge);
  }

  long long get_euclidean_distance(int source_index, int destination_index)
  {
    return sqrt(
        pow(this->forward_graph_->nodes[source_index].coordinates.x - this->forward_graph_->nodes[destination_index].coordinates.x, 2)
      + pow(this->forward_graph_->nodes[source_index].coordinates.y - this->forward_graph_->nodes[destination_index].coordinates.y, 2));
  }

  long long get_potential(int current_index, int source_index, int destination_index)
  {
    long long forward_euclidean_distance = get_euclidean_distance(current_index, destination_index);
    long long reverse_euclidean_distance = get_euclidean_distance(current_index, source_index);
    long long average_euclidean_distance = (forward_euclidean_distance - reverse_euclidean_distance) / 2;
    return average_euclidean_distance;
  }


  long long query(int source, int destination)
  {
    // Create distance vector
    vector<long long> forward_distance(this->number_nodes_, infinity),
    reverse_distance(this->number_nodes_, infinity);

    long long potential[this->number_nodes_];

    potential[source] = get_potential(source, source, destination);
    potential[destination] = get_potential(destination, source, destination);

    forward_distance[source] = 0;
    reverse_distance[destination] = 0;

    // Create Minimum Heap for bidirectional search
    priority_queue <pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> forward_heap,
    reverse_heap;

    // If the nodes were processed
    vector<bool> forward_visited(this->number_nodes_, false),
    reverse_visited(this->number_nodes_, false);

    forward_visited[source] = true;
    reverse_visited[destination] = true;

    // Append both source and destination nodes to Heaps
    forward_heap.push(make_pair(forward_distance[source], source));
    reverse_heap.push(make_pair(reverse_distance[destination], destination));

    bool found = false;

    // Iterate for a maximum of number of nodes
    while(!forward_heap.empty() && !reverse_heap.empty())
    {
      // Forward A*
      // Extract minimum node
      pair<int, int> forward_pair = forward_heap.top();
      forward_heap.pop();
      // Process minimum node
      for(Edge current_edge: this->forward_graph_->nodes[forward_pair.second].edges)
      {
        if(!forward_visited[current_edge.neighbor])
        {
          potential[current_edge.neighbor] = get_potential(current_edge.neighbor, source, destination);
          long long current_distance = forward_pair.first + (current_edge.cost - potential[forward_pair.second] + potential[current_edge.neighbor]);

          if(forward_distance[current_edge.neighbor] > current_distance)
          {
            forward_distance[current_edge.neighbor] = current_distance;
            forward_heap.push(make_pair(current_distance, current_edge.neighbor));
          }
        }
      }
      // Set minimum node as processed
      forward_visited[forward_pair.second] = true;
      // Check if minimum node was previously processed in reverse dijkstra
      if(reverse_visited[forward_pair.second])
      {
        found = true;
        break;
      }

      // Reverse A*
      // Extract minimum node
      pair<int, int> reverse_pair = reverse_heap.top();
      reverse_heap.pop();
      // Process minimum node
      for(Edge current_edge: this->reverse_graph_->nodes[reverse_pair.second].edges)
      {
        if(!reverse_visited[current_edge.neighbor])
        {
          potential[current_edge.neighbor] = get_potential(current_edge.neighbor, source, destination);
          long long current_distance = reverse_pair.first + (current_edge.cost + potential[reverse_pair.second] - potential[current_edge.neighbor]);

          if(reverse_distance[current_edge.neighbor] > current_distance)
          {
            reverse_distance[current_edge.neighbor] = current_distance;
            reverse_heap.push(make_pair(current_distance, current_edge.neighbor));
          }
        }
      }
      // Set minimum node as processed
      reverse_visited[reverse_pair.second] = true;
      // Check if minimum node was previously processed in reverse dijkstra
      if(forward_visited[reverse_pair.second])
      {
        found = true;
        break;
      }
    }


    if(found)
    {
      long long distance = infinity;

      for(int current_node = 0; current_node < this->number_nodes_; current_node++)
      {
        if(forward_visited[current_node] || reverse_visited[current_node])
        {
          long long current_distance = forward_distance[current_node] + reverse_distance[current_node]
                                        + potential[source]
                                        - potential[destination];

          if(current_distance < distance)
            distance = current_distance;
        }
      }

      return distance;
    }
    else
    {
      return -1;
    }
  }

private:
  int number_nodes_;
  Graph *forward_graph_;
  Graph *reverse_graph_;
};

int main()
{
  int n, m;
  cin >> n >> m;

  BiDirectionalAStar bidirectional_a_star(n);

  for (int i=0; i < n; i++)
  {
      int a, b;
      cin >> a >> b;

      bidirectional_a_star.add_coordinates(i, a, b);
  }

  for (int i = 0; i < m; ++i)
  {
    int u, v, c;
    cin >> u >> v >>c;

    bidirectional_a_star.add_edge(u - 1, v - 1, c);
  }

  int t;
  cin >> t;
  for (int i=0; i<t; i++)
  {
      int u, v;
      cin >> u >> v;
      printf("%lld\n", bidirectional_a_star.query(u-1, v-1));
  }
}


/*
class AStar
{
public:
  AStar(int number_nodes)
  :number_nodes_(number_nodes)
  {
    this->graph_ = new Graph(this->number_nodes_);
  }

  void add_coordinates(int node_index, int a, int b)
  {
    Coordinates current_coordinates = {
      .x = a,
      .y = b
    };

    this->graph_->nodes[node_index].coordinates = current_coordinates;
  }

  void add_edge(int source, int destination, int cost)
  {
    Edge forward_edge = {
      .neighbor = destination,
      .cost = cost
    };

    this->graph_->nodes[source].edges.push_back(forward_edge);
  }

  long long get_euclidean_distance(int source_index, int destination_index)
  {
    return sqrt(
        pow(this->graph_->nodes[source_index].coordinates.x - this->graph_->nodes[destination_index].coordinates.x, 2)
      + pow(this->graph_->nodes[source_index].coordinates.y - this->graph_->nodes[destination_index].coordinates.y, 2));
  }

  long long query(int source, int destination)
  {
    // Create distance vector
    vector<long long> distance(this->number_nodes_, infinity);

    long long euclidean_distance[this->number_nodes_];
    euclidean_distance[source] = get_euclidean_distance(source, destination);

    distance[source] = euclidean_distance[source];

    // Create Minimum Heap for bidirectional search
    priority_queue <pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> a_star_heap;

    // Append both source node to heap
    a_star_heap.push(make_pair(distance[source], source));

    while(!a_star_heap.empty())
    {
      pair<int, int> current_pair = a_star_heap.top();
      a_star_heap.pop();
      // Process minimum node
      for(Edge current_edge: this->graph_->nodes[current_pair.second].edges)
      {
        euclidean_distance[current_edge.neighbor] = get_euclidean_distance(current_edge.neighbor, destination);
        long long current_distance = current_pair.first + current_edge.cost - euclidean_distance[current_pair.second] + euclidean_distance[current_edge.neighbor];

        if(distance[current_edge.neighbor] > current_distance)
        {
          distance[current_edge.neighbor] = current_distance;
          a_star_heap.push(make_pair(current_distance, current_edge.neighbor));
        }
      }

      if(current_pair.second == destination)
        break;
    }

    return (distance[destination] == infinity)?-1:distance[destination];
  }

  private:
    int number_nodes_;
    Graph *graph_;
};

int main()
{
  int n, m;
  cin >> n >> m;

  AStar a_star(n);

  for (int i=0; i < n; i++)
  {
      int a, b;
      cin >> a >> b;

      a_star.add_coordinates(i, a, b);
  }

  for (int i = 0; i < m; ++i)
  {
    int u, v, c;
    cin >> u >> v >>c;

    a_star.add_edge(u - 1, v - 1, c);
  }

  int t;
  cin >> t;
  for (int i=0; i<t; i++)
  {
      int u, v;
      cin >> u >> v;
      printf("%lld\n", a_star.query(u-1, v-1));
  }
}
*/
