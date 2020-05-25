#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Vertex
{
  bool is_visited;
  vector<int> edges;

  Vertex()
  :is_visited(false)
  {}
};

class Graph
{
  public:
    Graph(int number_vertices)
    :number_vertices(number_vertices)
    {
        vertices = new Vertex[this->number_vertices];
    }

    void add_edge(int source, int destination)
    {
      this->vertices[source].edges.push_back(destination);
    }

    Graph* create_reverse_graph(void)
    {
      Graph* reverse_graph = new Graph(this->number_vertices);

      for(int source_vertex = 0; source_vertex < this->number_vertices; source_vertex++)
      {
        for(auto destination_vertex: this->vertices[source_vertex].edges)
        {
          reverse_graph->add_edge(destination_vertex, source_vertex);
        }
      }

      return reverse_graph;
    }

    vector<int> get_topological_order(void)
    {
      vector<int> topological_order;

      for(int i = 0; i < this->number_vertices; i++)
        this->vertices[i].is_visited = false;

      for(int i = 0; i < this->number_vertices; i++)
      {
        if(!(this->vertices[i].is_visited))
        {
          depth_first_search(i, topological_order);
        }
      }

      for(int i = 0; i < this->number_vertices; i++)
        this->vertices[i].is_visited = false;

      return topological_order;
    }

    void depth_first_search(int start_vertex, vector<int> &topological_order)
    {
      this->vertices[start_vertex].is_visited = true;
      for(auto destination_vertex: this->vertices[start_vertex].edges)
      {
        if(!this->vertices[destination_vertex].is_visited)
        {
          depth_first_search(destination_vertex, topological_order);
        }
      }
      topological_order.insert(begin(topological_order), start_vertex);
    }

    int number_strongly_connected_components(void)
    {
      int result = 0;
      Graph* reverse_graph = this->create_reverse_graph();
      vector<int> topological_order = reverse_graph->get_topological_order();
      for_each(begin(topological_order), end(topological_order), [&](int current_vertex){
        if(!this->vertices[current_vertex].is_visited)
        {
          vector<int> temporary;
          depth_first_search(current_vertex, temporary);
          result++;
        }
      });
      return result;
    }



  private:
    int number_vertices;
    Vertex* vertices;
};

int main()
{
  size_t n, m;
  std::cin >> n >> m;
  //vector<vector<int> > adj(n, vector<int>());
  Graph graph(n);
  for (size_t i = 0; i < m; i++)
  {
    int x, y;
    std::cin >> x >> y;
    //adj[x - 1].push_back(y - 1);
    graph.add_edge(x-1, y-1);
  }
  //std::cout << number_of_strongly_connected_components(adj);
  cout << graph.number_strongly_connected_components();
}
