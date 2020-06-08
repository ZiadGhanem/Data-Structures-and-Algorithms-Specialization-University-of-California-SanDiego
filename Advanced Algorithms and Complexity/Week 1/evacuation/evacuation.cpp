#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

using namespace std;

class FlowGraph{

  public:
    vector<unordered_map<int, int>> graph;

    FlowGraph(int vertex_count): graph(vertex_count){}

    int size(void)
    {
      return this->graph.size();
    }

    void add_edge(int from, int to, int capacity)
    {
      // Add forward edge with available flow equal to capacity
      this->graph[from][to] += capacity;

      // Add reverse edge with available flow equal zero
      this->graph[to][from] += 0;
    }
};

bool breadth_first_search(FlowGraph& flow_graph, vector<int>& parent, int source, int destination)
{
  // False if the vertex has not been visited before, true otherwise
  vector<bool> visited(flow_graph.size());

  // Breadth First Search Queue
  queue<int> bfs_queue;

  // Add source vertex to BFS queue
  bfs_queue.push(source);
  // Set source vertex as visited
  visited[source] = true;

  // While there is vertices to process
  while(!bfs_queue.empty())
  {
    // Get the vertex in front of queue
    int current_vertex = bfs_queue.front();
    bfs_queue.pop();

    // Loop over neighboring vertices
    for(auto it = begin(flow_graph.graph[current_vertex]); it != end(flow_graph.graph[current_vertex]); it++)
    {
      // If vertex is not visited before as there is an available flow
      if(!visited[it->first] && (it->second > 0))
      {
        // Set the vertex as visited
        visited[it->first] = true;
        // Set the vertex parent as source vertex
        parent[it->first] = current_vertex;
        // Add vertex to BFS queue
        bfs_queue.push(it->first);
      }
    }
  }

  // If the destination vertex was visited return true
  return (visited[destination] == true);
}


int max_flow(FlowGraph& flow_graph, int source, int destination)
{
    int flow = 0;

    // Parent of each vertex in breadth first Search
    vector<int> parent(flow_graph.size());

    // While there is a source to destination path
    while(breadth_first_search(flow_graph, parent, source, destination))
    {
      // Get minimum flow along path
      int minimum_flow = INT32_MAX;
      for(int current_vertex = destination; current_vertex != source; current_vertex = parent[current_vertex])
        minimum_flow = min(minimum_flow, flow_graph.graph[parent[current_vertex]][current_vertex]);

      // Update residual graph
      for(int current_vertex = destination; current_vertex != source; current_vertex = parent[current_vertex])
      {
        flow_graph.graph[parent[current_vertex]][current_vertex] -= minimum_flow;
        flow_graph.graph[current_vertex][parent[current_vertex]] += minimum_flow;
      }

      flow += minimum_flow;
    }

    return flow;
}


FlowGraph read_data()
{
    // Get number of vertices and number of edges
    int vertex_count, edge_count;
    cin >> vertex_count >> edge_count;

    // Create FlowGraph
    FlowGraph flow_graph(vertex_count);

    // Read edges
    for (int i = 0; i < edge_count; ++i)
    {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        flow_graph.add_edge(u - 1, v - 1, capacity);
    }

    return flow_graph;
}


int main()
{
    std::ios_base::sync_with_stdio(false);

    FlowGraph graph = read_data();
    cout << max_flow(graph, 0, graph.size() - 1) << "\n";

    return 0;
}
