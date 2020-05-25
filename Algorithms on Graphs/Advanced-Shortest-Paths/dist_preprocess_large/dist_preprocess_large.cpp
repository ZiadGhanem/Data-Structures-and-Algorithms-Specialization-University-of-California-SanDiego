/*
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <iostream>
#include <memory>
#include <cassert>

class Graph
{
    typedef int Distance;
    typedef int Vertex;

    // Number of nodes
    int N;
    // Source and target
    int s, t;
    // Estimate of the distance from s to t
    int estimate = INFINITY;
    // Lists of edges outgoing from each node
    std::vector<std::vector<std::pair<int, int>>> outgoing_edges;
    // Lists of edges incoming to each node
    std::vector<std::vector<std::pair<int, int>>> incoming_edges;

    static constexpr int INFINITY = std::numeric_limits<int>::max() / 2;
    // Levels of nodes for node ordering
    std::vector<int> level;
    // Ranks of nodes - positions in the node ordering
    std::vector<int> rank;

    // Distance to node v, bidistance[0][v] - from source in the forward search, bidistance[1][v] - from target
    // in the backward search.
    std::vector<std::vector<Distance>> bidistance;

    // Wrapper around STL priority_queue
    class StlHeap
    {
    public:
        using T = std::pair<Distance, Vertex>;
        using Queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

        StlHeap() {
            queue.reset(new Queue());
        }

        bool empty() const {
            return queue->empty();
        }

        void update(Vertex v, Distance d) {
            queue->push(std::make_pair(d,v));
        }

        void clear() {
            queue.reset(new Queue());
        }

        std::pair<Distance, Vertex> pop() {
            pair<Distance, Vertex> top = queue->top();
            queue->pop();
            return top;
        }

    private:
        std::unique_ptr<Queue> queue;
    };

    // Priority queues for forward and backward searches
    StlHeap diqueue[2];
public:
    Graph() {
        read_stdin();
        bidistance.resize(2, std::vector<int>(N, INFINITY));
    }

    int get_n() { return N;}

    std::vector<std::pair<int, int>>& get_adjacent(int v, bool forward = true) {
        if (forward) {
            return outgoing_edges[v];
        } else {
            return incoming_edges[v];
        }
    }

    void preprocess() {
        distance.resize(N, INFINITY);
        // Priority queue will store pairs of (importance, node) with the least important node in the head
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int, int>>> queue;

        // Implement the rest of the algorithm yourself
    }

    // Returns distance from s to t in the graph
    int query(int u, int w) {
        update(u, 0, true);
        update(w, 0, false);
        s = u;
        t = w;
        // Implement the rest of the algorithm yourself

        return -1;
    }

private:
    // Try to relax the node v using distance d either in the forward or in the backward search
    void update(int v, int d, bool forward) {
        // Implement this method yourself
    }

    class VertexSet
    {
    public:
        VertexSet(int n = 0) : visited(n) {}
        void resize(int n) {
            visited.resize(n);
        }
        void add(int v) {
            if (!visited[v]) {
                vertices.push_back(v);
                visited[v] = true;
            }
        }
        const std::vector<int>& get() const {
            return vertices;
        }
        const bool has(int v) {
            return visited[v];
        }
        void clear() {
            for (int v : vertices) {
                visited[v] = false;
            }
            vertices.clear();
        }

    private:
        std::vector<int> visited;
        std::vector<int> vertices;
    };
    VertexSet visited;

    // QEntry = (distance, vertex)
    typedef std::pair<int,int> QEntry;
    std::priority_queue<QEntry, std::vector<QEntry>, std::greater<QEntry>> queue;

    struct Shortcut {
        int from;
        int to;
        int cost;
    };

    // Adds all the shortcuts for the case when node v is contracted, and returns the importance of node v
    // in this case
    int do_shortcut(int v, std::vector<Shortcut>& shortcuts, int& mylevel) {
        // Implement this method yourself

        // Add neighbors and shortcut cover heuristics
        return (shortcuts.size() - outgoing_edges[v].size() - incoming_edges[v].size()) + mylevel;
    }

    void set_n(int n) {
        N = n;
        outgoing_edges.resize(n);
        incoming_edges.resize(n);
    }


    void add_edge_to_list(std::vector<std::pair<int,int>>& list, int w, int c) {
        for (int i = 0; i < list.size(); ++i) {
            std::pair<int, int>& p = list[i];
            if (p.first == w) {
                if (p.second > c) {
                    p.second = c;
                }
                return;
            }
        }
        list.push_back(w, c);
    }

    void add_directed_edge(int u, int v, int c) {
        add_edge_to_list(outgoing_edges[u], v, c);
        add_edge_to_list(incoming_edges[v], u, c);
    }

    void add_edge(int u, int v, int c) {
        add_directed_edge(u, v, c);
    }

    void finalize() {
        // Remove unnecessary edges
    }

    bool read_stdin() {
        int u,v,c,n,m;
        assert(scanf("%d %d", &n, &m) == 2);
        set_n(n);
        for (int i = 0; i < m; ++i) {
            assert(scanf("%d %d %d", &u, &v, &c) == 3);
            add_edge(u-1, v-1, c);
        }
        finalize();
        return true;
    }
};

int main() {
    Graph g;
    g.preprocess();
    std::cout << "Ready" << std::endl;

    int t;
    assert(scanf("%d", &t) == 1);
    for (int i = 0; i < t; ++i) {
        int u, v;
        assert(scanf("%d %d", &u, &v) == 2);
        printf("%d\n", g.query(u-1, v-1, 3));
    }
}
*/

#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <iostream>
#include <memory>
#include <cassert>

#define INFINITY  (long long)(INT64_MAX/4)

using namespace std;

struct Edge{
  int neighbor_id;
  long long cost;
};


struct Vertex{
  vector<Edge> in_edges;                // Incoming edges to this vertex
  vector<Edge> out_edges;               // Outgoing edges to this vertex
  int vertex_ordering;                  // Position in node ordering queue
  bool contracted;                      // If the vertex was contracted

  // Paremeters for computing importance
  int edge_difference;                  // EdgeDifference = Number of added shortcuts - Number of incoming edges - Number of outgoing incoming edges
  int contracted_neighbors;             // Number of contracted neighbours
  int shortcut_cover;                    // Number of added shortcuts if this vertex is contracted

  int importance;                       // Importance = Edge Difference + Shortcut Cover + Contracted Neighbors

  // To avoid reintialization of distances to infinity every time
  int contract_id;                      // ID of vertex that is going to be contracted
  int source_id;                        // ID of vertex which we apply Dikstra from while contracting

  long long distance;                   // Value of distance while contracting using Unidirectional Dijkstra

  // Used during query by Bidirectional Dijkstra
  int forward_query_id;                 // ID for forward search
  int reverse_query_id;                // ID for reverse search

  long long forward_query_distance;     // Distance for forward search
  long long reverse_query_distance;     // Distance for reverse search

  Vertex()
  {
    this->contracted = false;
    this->contracted_neighbors = 0;
    this->contract_id = -1;
    this->source_id = -1;

    this->distance = INFINITY;

    this->forward_query_id = -1;
    this->reverse_query_id = -1;

    this->forward_query_distance = INFINITY;
    this->reverse_query_distance = INFINITY;
  }
};

class DistPreprocessLarge{

private:
  int number_vertices;                // Number of vertices of graph
  Vertex* graph;                      // Number of vertices

  void compute_importance(int vertex_id){
    // EdgeDifference = Number of shortcuts - Number of incoming edges - Number of outgoing incoming edges
    this->graph[vertex_id].edge_difference = (this->graph[vertex_id].in_edges.size() * this->graph[vertex_id].out_edges.size()) - this->graph[vertex_id].in_edges.size() - this->graph[vertex_id].out_edges.size();
    // Number of added shortcuts if this vertex is contracted
    this->graph[vertex_id].shortcut_cover = this->graph[vertex_id].in_edges.size() + this->graph[vertex_id].out_edges.size();
    // Importance = Edge Difference + Shortcut Cover + Contracted Neighbors
    this->graph[vertex_id].importance = (14 * this->graph[vertex_id].edge_difference) + (10 * this->graph[vertex_id].contracted_neighbors) + (25 * this->graph[vertex_id].shortcut_cover);
  }

  // Check if the neighbor vertex is not the contracted vertex to find witness path
  bool check_id(int source_id, int destination_id)
  {
    return ((this->graph[source_id].contract_id != this->graph[destination_id].contract_id)
            ||
            (this->graph[source_id].source_id != this->graph[destination_id].source_id));
  }

  // Unidirectional Dijkstra to find witness path
  void dijkstra(int source_id, int contract_id, long long max_distance)
  {
    // Set current distance to zero
    this->graph[source_id].distance = 0;
    // Set source ID of contraction
    this->graph[source_id].source_id = source_id;
    // Set contract vertex ID
    this->graph[source_id].contract_id = contract_id;
    // Dijkstra Priority queue
    priority_queue <pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> dijkstra_heap;
    // Add source vertex to heap
    dijkstra_heap.push(make_pair(0, source_id));

    while(!dijkstra_heap.empty())
    {
      // Extract the vertex with minimum distance
      int current_vertex = dijkstra_heap.top().second;
      dijkstra_heap.pop();

      // If the distance exceeded the maximum distance exit
      if(this->graph[current_vertex].distance > max_distance)
        return;

      // Loop over outgoing edges of current vertex
      for(Edge current_edge: this->graph[current_vertex].out_edges){
        // If the neighbor was not contracted
        if(!this->graph[current_edge.neighbor_id].contracted){
          long long current_distance = this->graph[current_vertex].distance + current_edge.cost;
          // Check if the neighbor vertex is not visited before
          if(check_id(current_vertex, current_edge.neighbor_id) || (this->graph[current_edge.neighbor_id].distance > current_distance)){
            // Set new distance to neighbor vertex
            this->graph[current_edge.neighbor_id].distance = current_distance;
            // Update source ID of neighbor vertex
            this->graph[current_edge.neighbor_id].source_id = source_id;
            // Update contract ID of neighbor vertex
            this->graph[current_edge.neighbor_id].contract_id = contract_id;
            // Add vertex to heap
            dijkstra_heap.push(make_pair(current_distance, current_edge.neighbor_id));
          }
        }
      }
    }
  }

  void contract_vertex(int contract_id){
    // Set the vertex as contracted
    this->graph[contract_id].contracted = true;

    // Update the number of contracted neighbors for this vertex neigbors
    for(Edge current_edge: this->graph[contract_id].in_edges){
      this->graph[current_edge.neighbor_id].contracted_neighbors++;
    }
    for(Edge current_edge: this->graph[contract_id].out_edges){
      this->graph[current_edge.neighbor_id].contracted_neighbors++;
    }

    // Find maximum Incoming Edge where neighbor is not contracted
    long long max_in_edge = 0;
    for(Edge current_edge: this->graph[contract_id].in_edges){
      if(!this->graph[current_edge.neighbor_id].contracted){
        max_in_edge = max(max_in_edge, current_edge.cost);
      }
    }

    // Find maximum Outgoing Edge where neighbor is not contracted
    long long max_out_edge = 0;
    for(Edge current_edge: this->graph[contract_id].out_edges){
      if(!this->graph[current_edge.neighbor_id].contracted){
        max_out_edge = max(max_out_edge, current_edge.cost);
      }
    }

    long long max_distance = max_in_edge + max_out_edge;

    // Loop over incoming edges
    for(Edge in_edge: this->graph[contract_id].in_edges){
      // If this edge is not contracted
      if(!this->graph[in_edge.neighbor_id].contracted){
        // Perform dijsktra from incoming edge source to all outgoing edges vertices
        this->dijkstra(in_edge.neighbor_id, contract_id, max_distance);
        // Loop over outgoing edges
        for(Edge out_edge: this->graph[contract_id].out_edges){
          // If this vertex is not contracted and the outgoing edge neigbour was found by dijkstra and the distance to it is shorter
          if((!this->graph[out_edge.neighbor_id].contracted)
          && ((this->graph[out_edge.neighbor_id].contract_id != contract_id)
          || (this->graph[out_edge.neighbor_id].source_id != this->graph[in_edge.neighbor_id].source_id)
          || (this->graph[out_edge.neighbor_id].distance > (in_edge.cost + out_edge.cost))))
          {
            // Add shortcut
            this->add_edge(in_edge.neighbor_id, out_edge.neighbor_id, in_edge.cost + out_edge.cost);
          }
        }
      }
    }
  }

public:

  DistPreprocessLarge(int n)
  :number_vertices(n){
    this->graph = new Vertex[n];
  }

  void add_edge(int source_id, int destination_id, long long cost){
    // Add each edge to both incoming and outgoing edges
    this->graph[source_id].out_edges.push_back((Edge){
      .neighbor_id = destination_id,
      .cost = cost
    });

    this->graph[destination_id].in_edges.push_back((Edge){
      .neighbor_id = source_id,
      .cost = cost
    });
  }

  void preprocess(void){
    int extract_number = 0;
    // Create a min heap for importance of each vertex
    priority_queue <pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> importance_priority_queue;
    // Calculate initial importance for each vertex and add it to importance min heap
    for(int i = 0; i < this->number_vertices; i++){
      this->compute_importance(i);
      importance_priority_queue.push(make_pair(this->graph[i].importance, i));
    }

    while(!importance_priority_queue.empty()){
      // Extract vertex of least importance
      int contract_id = importance_priority_queue.top().second;
      importance_priority_queue.pop();

      // Recompute importance
      this->compute_importance(contract_id);

      // If it is not the vertex with minimum importance anymore readd it to the min heap
      if((!importance_priority_queue.empty()) && (this->graph[contract_id].importance > this->graph[importance_priority_queue.top().second].importance)){
        importance_priority_queue.push(make_pair(this->graph[contract_id].importance, contract_id));
      }
      else{
        // Set the vertex ordering
        this->graph[contract_id].vertex_ordering = extract_number;
        // Contract the vertex
        this->contract_vertex(contract_id);
        extract_number++;
      }
    }
/*
    for(int i = 0; i < this->number_vertices; i++)
    {
      for(Edge current_edge: this->graph[i].out_edges)
      {
        cout << i + 1 << " " << current_edge.neighbor_id + 1 << " " << current_edge.cost << endl;;
      }
    }
*/
  }

  long long query(int source_id, int destination_id, int query_id){
    // Clear distance from source vertex for forward search query
    this->graph[source_id].forward_query_distance = 0;
    // Set source vertex as discovered in current forward query
    this->graph[source_id].forward_query_id = query_id;

    // Clear distance from destination for reverse search query
    this->graph[destination_id].reverse_query_distance = 0;
    // Set destination vertex as discovered in current reverse query
    this->graph[destination_id].reverse_query_id = query_id;

    // Create min heaps for forward and reverse search for Bidirectional Dijkstra
    priority_queue <pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> forward_heap,
    reverse_heap;

    // Append both source and destination nodes to Heaps
    forward_heap.push(make_pair(0, source_id));
    reverse_heap.push(make_pair(0, destination_id));

    long long estimate = INFINITY;

    while(!forward_heap.empty() || !reverse_heap.empty())
    {
      // Forward Search
      if(!forward_heap.empty())
      {
        // Extract vertex with minimum distance
        int current_vertex = forward_heap.top().second;
        forward_heap.pop();

        // Set vertex as processed in current forward search query
        this->graph[current_vertex].forward_query_id = query_id;

        // Loop over outgoing edges of the current vertex
        for(Edge current_edge: this->graph[current_vertex].out_edges)
        {
          // If the neighbor has a higher vertex order
          if(this->graph[current_vertex].vertex_ordering < this->graph[current_edge.neighbor_id].vertex_ordering)
          {
            // If the neighbor was not already discovered in the current forward search query
            long long current_distance = this->graph[current_vertex].forward_query_distance + current_edge.cost;
            if((this->graph[current_edge.neighbor_id].forward_query_id != query_id)
            ||
            (this->graph[current_edge.neighbor_id].forward_query_distance > current_distance))
            {
              this->graph[current_edge.neighbor_id].forward_query_id = query_id;
              // Set the new distance to the neighbor
              this->graph[current_edge.neighbor_id].forward_query_distance = current_distance;
              // Add neighbor to heap
              forward_heap.push(make_pair(current_distance, current_edge.neighbor_id));
            }
          }
        }

        // Check if the current vertex was discovered in reverse search of this query
        if(this->graph[current_vertex].reverse_query_id == query_id)
        {
          if(this->graph[current_vertex].forward_query_distance + this->graph[current_vertex].reverse_query_distance < estimate)
          {
            estimate = this->graph[current_vertex].forward_query_distance + this->graph[current_vertex].reverse_query_distance;
          }
        }
      }

      // Reverse Search
      if(!reverse_heap.empty())
      {
        // Extract vertex with minimum distance
        int current_vertex = reverse_heap.top().second;
        reverse_heap.pop();

        // Set vertex as processed in current reverse search query
        this->graph[current_vertex].reverse_query_id = query_id;

        // Loop over incoming edges of the current vertex (Reverse Graph)
        for(Edge current_edge: this->graph[current_vertex].in_edges)
        {
          // If the neighbor has a higher vertex order
          if(this->graph[current_vertex].vertex_ordering < this->graph[current_edge.neighbor_id].vertex_ordering)
          {
            long long current_distance = this->graph[current_vertex].reverse_query_distance + current_edge.cost;
            // If the neighbor was not already discovered in the current reverse search query
            if((this->graph[current_edge.neighbor_id].reverse_query_id != query_id)
            ||
            (this->graph[current_edge.neighbor_id].reverse_query_distance > current_distance))
            {
              this->graph[current_edge.neighbor_id].reverse_query_id = query_id;
              // Set the new distance to the neigbor
              this->graph[current_edge.neighbor_id].reverse_query_distance = current_distance;
              // Add neigbor to heap
              reverse_heap.push(make_pair(current_distance, current_edge.neighbor_id));
            }
          }
        }

        // Check if the current vertex was discovered in forward search of this query
        if(this->graph[current_vertex].forward_query_id == query_id)
        {
          if(this->graph[current_vertex].forward_query_distance + this->graph[current_vertex].reverse_query_distance < estimate)
          {
            estimate = this->graph[current_vertex].forward_query_distance + this->graph[current_vertex].reverse_query_distance;
          }
        }
      }
    }

    return (estimate == INFINITY)? -1:estimate;
  }
};

int main(void){
  int u,v,c,n,m,t;
  assert(scanf("%d %d", &n, &m) == 2);
  DistPreprocessLarge dist_preprocess_large(n);

  for (int i = 0; i < m; ++i) {
      assert(scanf("%d %d %d", &u, &v, &c) == 3);
      dist_preprocess_large.add_edge(u-1, v-1, c);
  }

  dist_preprocess_large.preprocess();
  cout << "Ready" << endl;

  assert(scanf("%d", &t) == 1);
  for (int i = 0; i < t; ++i) {
      int u, v;
      assert(scanf("%d %d", &u, &v) == 2);
      printf("%d\n", dist_preprocess_large.query(u - 1, v - 1, i));
  }
}
