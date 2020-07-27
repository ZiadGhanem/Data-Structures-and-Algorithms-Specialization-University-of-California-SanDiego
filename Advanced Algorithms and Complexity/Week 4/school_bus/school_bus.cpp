/*
#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
typedef vector<vector<int> > Matrix;

const int INF = 1e9;

Matrix read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

std::pair<int, vector<int> > optimal_path(const Matrix& graph) {
    // This solution tries all the possible sequences of stops.
    // It is too slow to pass the problem.
    // Implement a more efficient algorithm here.
    size_t n = graph.size();
    vector<int> p(n);
    for (size_t i = 0; i < n; ++i)
        p[i] = i;

    vector<int> best_path;
    int best_ans = INF;

    do {
        int cur_sum = 0;
        bool ok = true;
        for (size_t i = 1; i < n && ok; ++i)
            if (graph[p[i - 1]][p[i]] == INF)
                ok = false;
            else
                cur_sum += graph[p[i - 1]][p[i]];
        if (graph[p.back()][p[0]] == INF)
            ok = false;
        else
            cur_sum += graph[p.back()][p[0]];

        if (!ok)
            continue;
        if (cur_sum < best_ans) {
            best_ans = cur_sum;
            best_path = p;
        }
    } while (std::next_permutation(p.begin(), p.end()));

    if (best_ans == INF)
        best_ans = -1;
    for (size_t i = 0; i < best_path.size(); ++i)
        ++best_path[i];
    return std::make_pair(best_ans, best_path);
}

void print_answer(const std::pair<int, vector<int> >& answer) {
    std::cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << "\n";
}

int main() {
    print_answer(optimal_path(read_data()));
    return 0;
}
*/

/*
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef vector<vector<int> > Matrix;

const int INF = 1e9;

Matrix read_data()
{
    int vertex_count, edge_count;
    cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i)
    {
        int from, to, weight;
        cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

void dfs_backtracking(
  const Matrix& graph,
  vector<bool>& visited,
  vector<int>& current_path,
  vector<int>& best_path,
  int source,
  int total_visited,
  int current_distance,
  int* best_distance
)
{
  // If the distance to this node is already higher than the best distance then this is a wrong path
  if(current_distance > *best_distance)
    return;

  // Make this node visited
  visited[source] = true;
  // Add the node to the path
  current_path.push_back(source);
  // Increase number of visited nodes
  total_visited++;

  // If we have already visited all nodes then visit the first node
  if(total_visited == graph.size() && (graph[source][0] != INF))
  {
      current_distance += graph[source][0];
      if(current_distance < *best_distance)
      {
        // Set this distance as best distance
        *best_distance = current_distance;
        // Set the best path
        best_path = current_path;
      }
  }
  else
  {
    // Visit neighbors of this node
    for(int neighbor = 0; neighbor < graph.size(); neighbor++)
    {
      if(source == neighbor)
        continue;
      // If there is a path to the neighbor and the neighbor was not visited before then visit it
      if((graph[source][neighbor] != INF) && !visited[neighbor])
          dfs_backtracking(graph, visited, current_path, best_path, neighbor, total_visited, current_distance + graph[source][neighbor], best_distance);
    }
  }

  visited[source] = false;
  current_path.pop_back();

}


pair<int, vector<int>> optimal_path(const Matrix& graph)
{
  int best_distance = INF;
  vector<int> best_path;
  vector<int> current_path;
  vector<bool> visited(graph.size(), false);
  dfs_backtracking(graph, visited, current_path, best_path, 0, 0, 0, &best_distance);
  if (best_distance == INF)
      best_distance = -1;
  for (size_t i = 0; i < best_path.size(); ++i)
      ++best_path[i];
  return make_pair(best_distance, best_path);

}


void print_answer(const pair<int, vector<int> >& answer)
{
    cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        cout << path[i] << " ";
    cout << "\n";
}

int main()
{
    print_answer(optimal_path(read_data()));
    return 0;
}
*/


#include <iostream>
#include <algorithm>
#include <vector>
#include <bitset>

using namespace std;

typedef vector<vector<int> > Matrix;

const int INF = 1e9;

Matrix read_data()
{
    int vertex_count, edge_count;
    cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i)
    {
        int from, to, weight;
        cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

pair<int, vector<int>> optimal_path(const Matrix& graph)
{
  // TSP Path
  vector<int> best_path(graph.size());

  // Dynamic Programming Table
  vector<vector<int>> dp_table(graph.size(), vector<int>((1 << graph.size()), INF));
  // Table to keep track of second to last node
  vector<vector<int>> parent(graph.size(), vector<int>(1 << graph.size()));

  // The distance from 0 to 0 with only node 0 visited is zero
  dp_table[0][1 << 0] = 0;

  // Loop over all possible subsets sizes
  for(int subset_size = 2; subset_size <= graph.size() ; subset_size++)
  {
    // Current subset
    vector<int> current_subset(graph.size(), 0);

    // Assume that the first nodes are the ones visited
    for(int i = 0; i < subset_size; i++)
      current_subset[i] = 1;

    // Create all possible permutations
    do
    {
      // Only instances in which the first node exists
      if(current_subset[0] == 0)
        continue;

      // Convert the subset to bits (n <= 17 and int which is 32 bits is enough)
      int current_subset_bits = 0;
      for(int i = 0; i < graph.size(); i++)
          current_subset_bits |= (current_subset[i] << i);

      // Loop over node i being the final node
      for(int i = 1; i < graph.size(); i++)
      {
        // If node i is in the subset
        if(current_subset[i])
        {
          // Loop over node j being the second to last node
          for(int j = 0; j < graph.size(); j++)
          {
            int current_distance = dp_table[j][current_subset_bits & ~(1 << i)] + graph[j][i];
            // If node j is in the subset and it is not the same as node i and there is an edge from j to i
            // The distance to get to node i = The distance to node j by passing all the nodes in the current subset except node i + distance from j to i
            if((current_subset[j]) && (j != i) && (graph[j][i] != INF) && (current_distance < dp_table[i][current_subset_bits]))
            {
              dp_table[i][current_subset_bits] = current_distance;
              parent[i][current_subset_bits] = j;
            }
          }
        }
      }
    }
    while(prev_permutation(begin(current_subset), end(current_subset)));
  }

  // Find the last node before returning back to node 0
  int result = INF;
  for(int i = 0; i < graph.size(); i++)
  {
    int current_distance = dp_table[i].back() + graph[i][0];
    if(graph[i][0] != INF && (current_distance < result))
    {
      result = current_distance;
      // Set the last node
      best_path.back() = i + 1;
    }
  }

  if(result != INF)
  {
    // Backtracking to get the best path
    // Create a mask which visits all nodes
    int mask = (1 << graph.size()) - 1;
    for(int i = graph.size() - 2; i >= 0; i--)
    {
      best_path[i] = parent[best_path[i + 1] - 1][mask] + 1;
      // Remove the last node from visited nodes
      mask &= ~(1 << (best_path[i + 1] - 1));
    }
    return {result, best_path};
  }

  else
    return {-1, {}};
}


void print_answer(const pair<int, vector<int> >& answer)
{
    cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        cout << path[i] << " ";
    cout << "\n";
}

int main()
{
    print_answer(optimal_path(read_data()));
    return 0;
}
