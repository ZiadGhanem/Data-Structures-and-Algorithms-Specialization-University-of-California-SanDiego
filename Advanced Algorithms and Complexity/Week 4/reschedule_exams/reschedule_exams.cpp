#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <stack>

using namespace std;


class TwoSatisfiability
{
  private:
    int num_vars;
    int num_clauses;
    vector<vector<int>> forward_graph;
    vector<vector<int>> reverse_graph;


    void get_order_dfs(int source, vector<bool>& visited, vector<int>& post_order)
    {
      // Create a DFS stack
      stack<int> dfs_stack;

      // Add source node to stack
      dfs_stack.push(source);
      // Set the source node as visited
      visited[source] = true;

      // While there is a non visited node
      while(!dfs_stack.empty())
      {
        // Get the node at the top of the stack
        int current_node = dfs_stack.top();

        // Assume that all the neighbors of the node are already visited at first
        bool visting_neighbors = false;

        // Visit neighbors of the node
        for(int neighbor: reverse_graph[current_node])
        {
          // If a neighbor node is not visited
          if(!visited[neighbor])
          {
            // Then we're currently still visitng neighbors
            visting_neighbors = true;
            // Set the neighbor as visited
            visited[neighbor] = true;
            // Add the neighbor to the stack
            dfs_stack.push(neighbor);
            break;
          }
        }

        if(!visting_neighbors)
        {
          post_order.push_back(dfs_stack.top());
          dfs_stack.pop();
        }
      }
    }

    int compute_scc(int source, vector<bool>& visited, unordered_set<int>& current_scc)
    {
      // Create a DFS stack
      stack<int> dfs_stack;
      // Add source node to stack
      dfs_stack.push(source);
      // Set the source node as visited
      visited[source] = true;

      // While there is a non visited node
      while(!dfs_stack.empty())
      {
        // Get the node at the top of the stack
        int current_node = dfs_stack.top();
        dfs_stack.pop();

        // Add node to the SCC
        current_scc.insert(current_node);

        // Visit neighbors of the node
        for(int neighbor: forward_graph[current_node])
        {
          // If a neighbor node is not visited
          if(!visited[neighbor])
          {
            // Set the neighbor as visited
            visited[neighbor] = true;

            // Add the neighbor to the stack
            dfs_stack.push(neighbor);
          }
        }
      }

    }

  public:
    TwoSatisfiability(int n, int m):
        num_vars(n),
        num_clauses(m)
    {
      forward_graph.resize(2 * n);
      reverse_graph.resize(2 * n);
    }

    void add_clause(int u, int v)
    {
      int u_bar, v_bar;

      // Map both variables to node IDs
      // x: {0--->num_vars-1} and x_bar:{num_vars--->2*nums_vars - 1}
      if(u > 0)
      {
        u--;
        u_bar = u + num_vars;
      }
      else
      {
        u_bar = -(u + 1);
        u = u_bar + num_vars;
      }

      if(v > 0)
      {
        v--;
        v_bar = v + num_vars;
      }
      else
      {
        v_bar = -(v + 1);
        v = v_bar + num_vars;
      }


      // Form forward implication graph and reverse implication graph
      forward_graph[u_bar].push_back(v);
      reverse_graph[v].push_back(u_bar);
      forward_graph[v_bar].push_back(u);
      reverse_graph[u].push_back(v_bar);
    }

    bool is_satisfiable(vector<int>& result)
    {
      // Get the Post order of nodes
      vector<bool> visited(reverse_graph.size(), false);
      vector<int> post_order;
      for(int current_node = reverse_graph.size() - 1; current_node >= 0; current_node--)
      {
        if(!visited[current_node])
          get_order_dfs(current_node, visited, post_order);
      }



      visited.assign(forward_graph.size(), false);
      vector<bool> assigned(forward_graph.size(), false);

      unordered_set<int> current_scc;
      for (auto current_node = rbegin(post_order); current_node != rend(post_order); ++current_node)
      {
        if(!visited[*current_node])
        {
          // Get the strongly connected components get using DFS in reverse order of post order
          current_scc.clear();

          compute_scc(*current_node, visited, current_scc);

          // Loop over variables in current strongly connected component
          for(auto var: current_scc)
          {
            // If x and x_bar are in the same SCC then the clauses are unsatisfiable
            if(((var < num_vars) && (current_scc.find(var + num_vars) != end(current_scc))) ||
            ((var >= num_vars) && (current_scc.find(var - num_vars) != end(current_scc))))
              return false;

            // Assign literals
            if(!assigned[var])
            {
              assigned[var] = true;

              if(var < num_vars)
              {
                result[var] = 1;
                assigned[var + num_vars] = true;
              }
              else
              {
                result[var - num_vars] = 0;
                assigned[var - num_vars] = true;
              }
            }
          }

        }
      }

      return true;
    }
};

struct Edge {
    int from;
    int to;
};

class Convert3ColoringToSat{
private:
    int num_vertices;
    vector<Edge> edges;
    string colors;

public:
    Convert3ColoringToSat(int n, string colors):
        num_vertices(n),
        colors(colors)
    {}

    void add_edge(int u, int v)
    {
      edges.push_back({u, v});
    }

    string assign_new_colors(void)
    {

      // Calculate the number of variables
      int num_vars = 3 * num_vertices;
      // Calculate the number of clauses
      int num_clauses = (3 * num_vertices) + (3 * edges.size());
      // Create a 2-SAT instance with number of variables and number of clauses
      TwoSatisfiability two_sat(num_vars, num_clauses);

      // Exactly one color for each vertex except the initial color
      for(int i = 0; i < num_vertices; i++)
      {
        int vertex_start = i * 3;
        // If the initial color was Red then we can only use Green or Blue
        if(colors[i] == 'R')
        {
          // Not the initial color
          two_sat.add_clause(-(vertex_start + 1), -(vertex_start + 1));
          // Excatly one of the two other colors
          two_sat.add_clause(vertex_start + 2, vertex_start + 3);
          two_sat.add_clause(-(vertex_start + 2), -(vertex_start + 3));
        }
        // If the initial color was Green then we can only use Red or Blue
        else if(colors[i] == 'G')
        {
          // Not the initial color
          two_sat.add_clause(-(vertex_start + 2), -(vertex_start + 2));
          // Excatly one of the two other colors
          two_sat.add_clause(vertex_start + 1, vertex_start + 3);
          two_sat.add_clause(-(vertex_start + 1), -(vertex_start + 3));
        }
        // If the initial color was Blue then we can only use Red or Green
        else if(colors[i] == 'B')
        {
          // Not the initial color
          two_sat.add_clause(-(vertex_start + 3), -(vertex_start + 3));
          // Excatly one of the two other colors
          two_sat.add_clause(vertex_start + 1, vertex_start + 2);
          two_sat.add_clause(-(vertex_start + 1), -(vertex_start + 2));
        }
      }

      // Ends of one edge can not have the same color
      for(auto edge: edges)
      {
        for(int i = 1; i <= 3; i++)
          two_sat.add_clause(-((edge.from * 3) + i), -((edge.to * 3) + i));
      }


      // 2-SAT result vector
      vector<int> two_sat_result(num_vars);
      // Coloring result
      string coloring_result;
      if(two_sat.is_satisfiable(two_sat_result))
      {
        for(int i = 0; i < num_vertices; i++)
        {
          if(two_sat_result[(i * 3)+ 0])
            coloring_result += 'R';
          else if(two_sat_result[(i * 3) + 1])
            coloring_result += 'G';
          else if(two_sat_result[(i * 3) + 2])
            coloring_result += 'B';
        }
      }
      return coloring_result;
    }
};

int main()
{
    // Read number of vertices, number of edges and initial coloring
    int n, m;
    cin >> n >> m;
    string colors;
    cin >> colors;
    Convert3ColoringToSat coloring_to_sat(n, colors);

    // Add the edges of the graph
    for(int i = 0; i < m; i++)
    {
      int u, v;
      cin >> u >> v;
      coloring_to_sat.add_edge(u - 1, v - 1);
    }

    // Solve the coloring problem
    string new_colors = coloring_to_sat.assign_new_colors();

    if (new_colors.empty())
        cout << "Impossible";
    else
        cout << new_colors << endl;
}
