/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using std::vector;
using std::cin;
using std::cout;

class MaxMatching {
 public:
  void Solve() {
    vector<vector<bool>> adj_matrix = ReadData();
    vector<int> matching = FindMatching(adj_matrix);
    WriteResponse(matching);
  }

 private:
  vector<vector<bool>> ReadData() {
    int num_left, num_right;
    cin >> num_left >> num_right;
    vector<vector<bool>> adj_matrix(num_left, vector<bool>(num_right));
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j) {
        int bit;
        cin >> bit;
        adj_matrix[i][j] = (bit == 1);
      }
    return adj_matrix;
  }

  void WriteResponse(const vector<int>& matching) {
    for (int i = 0; i < matching.size(); ++i) {
      if (i > 0)
        cout << " ";
      if (matching[i] == -1)
        cout << "-1";
      else
        cout << (matching[i] + 1);
    }
    cout << "\n";
  }

  vector<int> FindMatching(const vector<vector<bool>>& adj_matrix) {
    // Replace this code with an algorithm that finds the maximum
    // matching correctly in all cases.
    int num_left = adj_matrix.size();
    int num_right = adj_matrix[0].size();
    vector<int> matching(num_left, -1);
    vector<bool> busy_right(num_right, false);
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j)
        if (matching[i] == -1 && !busy_right[j] && adj_matrix[i][j]) {
          matching[i] = j;
          busy_right[j] = true;
        }
    return matching;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  MaxMatching max_matching;
  max_matching.Solve();
  return 0;
}
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>

using namespace std;


class MatchingGraph{

  public:
    int left_count;
    int right_count;
    vector<unordered_map<int, bool>> graph;

    // Create Graph with Number of vertices = Left Count + Right Count + Source + Destination
    MatchingGraph(int left_count_, int right_count_)
    :graph(left_count_ + right_count_ + 2),
    left_count(left_count_),
    right_count(right_count_)
    {
      // Add edges from source vertex to left vertices
      for(int i = 2; i < left_count + 2; i++)
      {
        // Add forward edge
        this->graph[0][i] = true;
        // Add reverse edge
        this->graph[i][0] = false;
      }

      // Add edges from right vertices to destination vertex
      for(int i = left_count + 2; i < this->graph.size(); i++)
      {
        // Add forward edge
        this->graph[i][1] = true;
        // Add reverse edge
        this->graph[1][i] = false;
      }
    }

    int size(void)
    {
      return this->graph.size();
    }

    int get_left_count()
    {
      return this->left_count;
    }

    // Edges are added with vertices offset = 2 due to source and destination vertices
    void add_edge(int from, int to)
    {
      // Add forward edge
      this->graph[from + 2][left_count + to + 2] = true;

      // Add reverse edge
      this->graph[left_count + to + 2][from + 2] = false;
    }

    int get_match(int vertex)
    {
      for(auto it = begin(this->graph[vertex + 2]); it != end(this->graph[vertex + 2]); it++)
      {
        if((!it->second) && (it->first >= left_count + 2))
        {
          return (it->first - this->left_count - 2);
        }
      }

      return -1;
    }

    // Breadth First Search to find if there is a path from source to destination
    bool check_path(vector<int>& parent)
    {
      // False if the vertex has not been visited before, true otherwise
      vector<bool> visited(this->graph.size());

      // Breadth First Search Queue
      queue<int> bfs_queue;

      // Add source vertex to BFS queue
      bfs_queue.push(0);
      // Set source vertex as visited
      visited[0] = true;

      // While there is vertices to process
      while(!bfs_queue.empty())
      {
        // Get the vertex in front of queue
        int current_vertex = bfs_queue.front();
        bfs_queue.pop();

        // Loop over neighboring vertices
        for(auto it = begin(this->graph[current_vertex]); it != end(this->graph[current_vertex]); it++)
        {
          // If vertex is not visited before as there is an available flow
          if(!visited[it->first] && it->second)
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
      return (visited[1] == true);
    }


    void update_path(vector<int>& parent)
    {
      // Update residual graph
      for(int current_vertex = 1; current_vertex != 0; current_vertex = parent[current_vertex])
      {
        this->graph[parent[current_vertex]][current_vertex] = false;
        this->graph[current_vertex][parent[current_vertex]] = true;
      }
    }
};


MatchingGraph find_matching(MatchingGraph& matching_graph)
{
    // Parent of each vertex along path
    vector<int> parent(matching_graph.size());

    // While there is a source to destination path
    while(matching_graph.check_path(parent))
    {
      // Update the path
      matching_graph.update_path(parent);
    }

    return matching_graph;
}


MatchingGraph read_data(void)
{
    // Get number of left vertices and right vertices
    int left_count, right_count;
    cin >> left_count >> right_count;

    // Create Matching Graph
    MatchingGraph matching_graph(left_count, right_count);

    // Read edges
    for (int i = 0; i < left_count; i++)
    {
      for (int j = 0; j < right_count; j++)
      {
        int bit;
        cin >> bit;
        if(bit == 1)
          matching_graph.add_edge(i, j);
      }
    }

    return matching_graph;
}

void write_response(MatchingGraph& matching_graph)
{
  for(int i = 0; i < matching_graph.get_left_count(); i++)
  {
    int match = matching_graph.get_match(i);
    if (i > 0)
      cout << " ";
    if (match == -1)
      cout << "-1";
    else
      cout << (match + 1);
  }
  cout << "\n";
}

int main(void)
{
  std::ios_base::sync_with_stdio(false);
  // Create flow graph
  MatchingGraph matching_graph = read_data();
  // Find number of matches
  matching_graph = find_matching(matching_graph);
  // Display matchings
  write_response(matching_graph);
}
