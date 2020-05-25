#include <iostream>
#include <vector>

using namespace std;

class Graph
{
public:
    Graph(void)
    {
      size_t n, m;
      std::cin >> n >> m;
      this->adjacency_list.resize(n, vector<int>());
      for (size_t i = 0; i < m; i++)
      {
        int x, y;
        std::cin >> x >> y;
        this->adjacency_list[x - 1].push_back(y - 1);
      }

      this->visited = new bool[n];
      this->in_stack = new bool[n];

      for(int i = 0; i < n; i++)
      {
        this->visited[i] = false;
        this->in_stack[i] = false;
      }
    }

    int acyclic(void)
    {
      for(int i = 0; i < this->adjacency_list.size(); i++)
      {
        if(this->acylic_util(i))
          return 1;
      }

      return 0;
    }

  private:
    bool acylic_util(int current_node)
    {
      if(!this->visited[current_node])
      {
        this->visited[current_node] = true;
        this->in_stack[current_node] = true;

        for(auto neighbor: this->adjacency_list[current_node])
        {
          if(!visited[neighbor] && this->acylic_util(neighbor))
            return true;
          else if(this->in_stack[neighbor])
            return true;
        }
      }

      this->in_stack[current_node] = false;
      return false;
    }

    vector<vector<int>> adjacency_list;
    bool *visited;
    bool *in_stack;
};

int main()
{
  Graph graph;
  std::cout << graph.acyclic();
}
