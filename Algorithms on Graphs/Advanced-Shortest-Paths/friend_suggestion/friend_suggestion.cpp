/*

#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

using namespace std;

// External vector of size 2 - for forward and backward search.
// Internal 2-dimensional vector is vector of adjacency lists for each node.
typedef vector<vector<vector<int>>> Adj;

// Distances can grow out of int type
typedef long long Len;

// Vector of two priority queues - for forward and backward searches.
// Each priority queue stores the closest unprocessed node in its head.
typedef vector<priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>>> Queue;

const Len INFINITY = numeric_limits<Len>::max() / 4;

class Bidijkstra {
    // Number of nodes
    int n_;
    // Graph adj_[0] and cost_[0] correspond to the initial graph,
    // adj_[1] and cost_[1] correspond to the reversed graph.
    // Graphs are stored as vectors of adjacency lists corresponding
    // to nodes.
    // Adjacency list itself is stored in adj_, and the corresponding
    // edge costs are stored in cost_.
    Adj adj_;
    Adj cost_;
    // distance_[0] stores distances for the forward search,
    // and distance_[1] stores distances for the backward search.
    vector<vector<Len>> distance_;
    // Stores all the nodes visited either by forward or backward search.
    vector<int> workset_;
    // Stores a flag for each node which is True iff the node was visited
    // either by forward or backward search.
    vector<bool> visited_;

public:
    Bidijkstra(int n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n, INFINITY)), visited_(n)
    { workset_.reserve(n); }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear() {
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = INFINITY;
            visited_[v] = false;
        }
        workset_.clear();
    }

    // Processes visit of either forward or backward search
    // (determined by value of side), to node v trying to
    // relax the current distance by dist.
    void visit(Queue& q, int side, int v, Len dist) {
        // Implement this method yourself
    }

    // Returns the distance from s to t in the graph.
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

    Bidijkstra bidij(n, adj, cost);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", bidij.query(u-1, v-1));
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

using namespace std;

const long long INFINITY = numeric_limits<long long>::max() / 4;

struct Edge
{
  int neighbor;
  long long cost;
};

struct Graph
{
    vector<vector<Edge>> nodes;

    Graph(int number_nodes)
    {
      this->nodes.resize(number_nodes);
    }
};

class BiDirectionalDijkstra
{
  public:
    BiDirectionalDijkstra(int number_nodes)
    :number_nodes_(number_nodes)
    {
      this->forward_graph_ = new Graph(this->number_nodes_);
      this->reverse_graph_ = new Graph(this->number_nodes_);
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

      this->forward_graph_->nodes[source].push_back(forward_edge);
      this->reverse_graph_->nodes[destination].push_back(reverse_edge);
    }
    long long query(int source, int destination)
    {
      // Create distance vector
      vector<long long> forward_distance(this->number_nodes_, INFINITY),
      reverse_distance(this->number_nodes_, INFINITY);

      // Set distance from source to source in forward graph to zero
      forward_distance[source] = 0;
      // Set distance from destination to destination in reverse graph to zero
      reverse_distance[destination] = 0;

      // Create Minimum Heap for bidirectional search
      priority_queue <pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> forward_heap,
      reverse_heap;

      // Append both source and destination nodes to Heaps
      forward_heap.push(make_pair(forward_distance[source], source));
      reverse_heap.push(make_pair(reverse_distance[destination], destination));

      // If the nodes were processed
      vector<bool> forward_visited(this->number_nodes_, false),
      reverse_visited(this->number_nodes_, false);

      forward_visited[source] = true;
      reverse_visited[destination] = false;

      bool found = false;

      // Iterate for a maximum of number of nodes
      while(!forward_heap.empty() || !reverse_heap.empty())
      {
        // Forward Dijkstra
        if(!forward_heap.empty())
        {
          // Extract minimum node
          pair<int, int> forward_pair = forward_heap.top();
          forward_heap.pop();
          // Process minimum node
          for(Edge current_edge: this->forward_graph_->nodes[forward_pair.second])
          {
            if(!forward_visited[current_edge.neighbor])
            {
              long long current_distance = forward_pair.first + current_edge.cost;
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
        }

        // Reverse Dijkstra
        if(!reverse_heap.empty())
        {
          // Extract minimum node
          pair<int, int> reverse_pair = reverse_heap.top();
          reverse_heap.pop();
          // Process minimum node
          for(Edge current_edge: this->reverse_graph_->nodes[reverse_pair.second])
          {
            if(!reverse_visited[current_edge.neighbor])
            {
              long long current_distance = reverse_pair.first + current_edge.cost;
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
      }


      if(found)
      {
        long long distance = INFINITY;

        for(int current_node = 0; current_node < this->number_nodes_; current_node++)
        {
          if(forward_visited[current_node] || reverse_visited[current_node])
          {
            long long current_distance = forward_distance[current_node] + reverse_distance[current_node];
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
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m;
  cin >> n >> m;

  BiDirectionalDijkstra bidirectional_dijkstra(n);

  for (int i = 0; i < m; ++i)
  {
    int u, v, c;
    cin >> u >> v >> c;

    bidirectional_dijkstra.add_edge(u - 1, v - 1, c);
  }

  int t;
  cin >> t;

  for (int i=0; i<t; ++i)
  {
      int u, v;
      cin >> u >> v;
      printf("%lld\n", bidirectional_dijkstra.query(u-1, v-1));
  }

}
