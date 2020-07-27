/*
#include <bits/stdc++.h>

using namespace std;


struct Clause {
    int firstVar;
    int secondVar;
};


struct TwoSatisfiability {
    int numVars;
    vector<Clause> clauses;

    TwoSatisfiability(int n, int m) :
        numVars(n),
        clauses(m)
    {  }

    bool isSatisfiable(vector<int>& result) {
        // This solution tries all possible 2^n variable assignments.
        // It is too slow to pass the problem.
        // Implement a more efficient algorithm here.
        for (int mask = 0; mask < (1 << numVars); ++mask) {
            for (int i = 0; i < numVars; ++i) {
                result[i] = (mask >> i) & 1;
            }

            bool formulaIsSatisfied = true;

            for (const Clause& clause: clauses) {
                bool clauseIsSatisfied = false;
                if (result[abs(clause.firstVar) - 1] == (clause.firstVar < 0)) {
                    clauseIsSatisfied = true;
                }
                if (result[abs(clause.secondVar) - 1] == (clause.secondVar < 0)) {
                    clauseIsSatisfied = true;
                }
                if (!clauseIsSatisfied) {
                    formulaIsSatisfied = false;
                    break;
                }
            }

            if (formulaIsSatisfied) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    TwoSatisfiability twoSat(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
    }

    vector<int> result(n);
    if (twoSat.isSatisfiable(result)) {
        cout << "SATISFIABLE" << endl;
        for (int i = 1; i <= n; ++i) {
            if (result[i-1]) {
                cout << -i;
            } else {
                cout << i;
            }
            if (i < n) {
                cout << " ";
            } else {
                cout << endl;
            }
        }
    } else {
        cout << "UNSATISFIABLE" << endl;
    }

    return 0;
}
*/


#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <algorithm>

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


int main() {
    ios::sync_with_stdio(false);

    int n, m, u, v;
    cin >> n >> m;
    TwoSatisfiability two_sat(n, m);
    for(int i = 0; i < m; i++)
    {
      cin >> u >> v;
      two_sat.add_clause(u, v);
    }

    vector<int> result(n);
    if (two_sat.is_satisfiable(result)) {
        cout << "SATISFIABLE" << endl;
        for (int i = 1; i <= n; ++i) {
            if (result[i-1]) {
                cout << i;
            } else {
                cout << -i;
            }
            if (i < n) {
                cout << " ";
            } else {
                cout << endl;
            }
        }
    } else {
        cout << "UNSATISFIABLE" << endl;
    }

    return 0;
}
