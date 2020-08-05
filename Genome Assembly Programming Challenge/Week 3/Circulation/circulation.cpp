#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

using namespace std;


/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
class FlowGraph {
public:
    struct Edge {
        int from, to, lower_bound, capacity, flow;
    };

private:
    /* List of all - forward and backward - edges */
    vector<Edge> edges;

    /* These adjacency lists store only indices of edges in the edges list */
    vector<vector<int> > graph;

public:
    explicit FlowGraph(int n): graph(n) {}

    void add_edge(int from, int to, int lower_bound, int capacity) {
        /* Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges */
        Edge forward_edge = {from, to, lower_bound, capacity, capacity};
        Edge backward_edge = {to, from, lower_bound, 0, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);
    }

    int size()
    {
        return graph.size();
    }

    vector<int>& get_ids(int from)
    {
        return graph[from];
    }

    Edge& get_edge(int id)
    {
        return edges[id];
    }

    void add_flow(int id, int flow) {
        /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow;
    }
};

bool breadth_first_search(FlowGraph& flow_graph, vector<int>& parent, int source, int destination)
{
  // False if the vertex has not been visited before, true otherwise
  vector<bool> visited(flow_graph.size(), false);

  // Breadth First Search Queue
  queue<int> bfs_queue;

  // Add source vertex to BFS queue
  bfs_queue.push(source);
  // Set source vertex as visited
  visited[source] = true;
  // Set the parent of source as non
  parent[source] = -1;

  // While there is vertices to process
  while(!bfs_queue.empty())
  {
    // Get the vertex in front of queue
    int current_vertex = bfs_queue.front();
    bfs_queue.pop();

    // Loop over neighboring vertices
    vector<int>& neighbors = flow_graph.get_ids(current_vertex);
    for(int& id: neighbors)
    {
      FlowGraph::Edge& current_edge = flow_graph.get_edge(id);
      // If vertex is not visited before and there is an available flow
      if(!visited[current_edge.to] && (current_edge.flow > 0))
      {
        // Set the vertex as visited
        visited[current_edge.to] = true;
        // Set the vertex parent as source vertex
        parent[current_edge.to] = id;
        // Add vertex to BFS queue
        bfs_queue.push(current_edge.to);
      }
    }
  }

  // If the destination vertex was visited return true
  return (visited[destination] == true);
}


int get_max_flow(FlowGraph& flow_graph, int source, int destination)
{
    int flow = 0;

    // Parent of each vertex in breadth first Search
    vector<int> parent(flow_graph.size());

    // While there is a source to destination path
    while(breadth_first_search(flow_graph, parent, source, destination))
    {
      // Get minimum flow along path
      int minimum_flow = INT32_MAX;
      int edge_id = parent[destination];
      while(edge_id != -1)
      {
        FlowGraph::Edge& current_edge = flow_graph.get_edge(edge_id);
        minimum_flow = min(minimum_flow, current_edge.flow);
        edge_id = parent[current_edge.from];
      }

      // Update residual graph
      edge_id = parent[destination];
      while(edge_id != -1)
      {
        FlowGraph::Edge& current_edge = flow_graph.get_edge(edge_id);
        flow_graph.add_flow(edge_id ^ 1, minimum_flow);
        edge_id = parent[current_edge.from];
      }

      flow += minimum_flow;
    }

    return flow;
}

int main()
{
    // Read the number of nodes and number of edges
    int num_nodes, num_edges;
    cin >> num_nodes >> num_edges;

    // Create the flow graph
    FlowGraph flow_graph(num_nodes + 2);
    vector<int> out(num_nodes, 0);
    vector<int> in(num_nodes, 0);

    int source, destination, lower_bound, capacity, total_lower_bounds = 0;
    for(int i = 0; i < num_edges; i++)
    {
      // Read the source, destination, lower bound & capacity
      cin >> source >> destination >> lower_bound >> capacity;
      source--;
      destination--;

      total_lower_bounds += lower_bound;

      // Add edge to flow graph
      flow_graph.add_edge(source, destination, lower_bound, capacity - lower_bound);

      out[source] += lower_bound;
      in[destination] += lower_bound;
    }

    for(int i = 0; i < num_nodes; i++)
    {
      flow_graph.add_edge(num_nodes, i, 0, in[i]);
      flow_graph.add_edge(i, num_nodes + 1, 0, out[i]);
    }

    int max_flow = get_max_flow(flow_graph, num_nodes, num_nodes + 1);
    //cout << total_lower_bounds << " " << max_flow << endl;
    if(total_lower_bounds == max_flow)
    {
      cout << "YES" << endl;
      for(int i = 1; i <= 2 * num_edges; i+= 2)
      {
        FlowGraph::Edge& current_edge = flow_graph.get_edge(i);
        cout << current_edge.flow + current_edge.lower_bound << endl;
      }
    }
    else
    {
      cout << "NO" << endl;
    }

    return 0;
}
