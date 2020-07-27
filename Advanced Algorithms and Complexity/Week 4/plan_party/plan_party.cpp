#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <sys/resource.h>

using namespace std;

struct Vertex {
    int weight;
    vector <int> children;
};
typedef vector<Vertex> Graph;

Graph ReadTree() {
    int vertices_count;
    cin >> vertices_count;

    Graph tree(vertices_count);

    for (int i = 0; i < vertices_count; ++i)
        cin >> tree[i].weight;

    for (int i = 1; i < vertices_count; ++i) {
        int from, to;
        cin >> from >> to;
        tree[from - 1].children.push_back(to - 1);
        tree[to - 1].children.push_back(from - 1);
    }

    return tree;
}

int dfs(const Graph &tree, vector<int>& dp_table, int vertex)
{
    // If the node was not processed before
    if(dp_table[vertex] == INT32_MAX)
    {
      // If the node is a leaf then its fun factor is its weight
      if(tree[vertex].children.size() == 0)
        dp_table[vertex] = tree[vertex].weight;
      // If the node has children
      else
      {
        // Either take the node and its grandchildren
        int m1 = tree[vertex].weight;
        for(int child: tree[vertex].children)
        {
          for(int grandchild: tree[child].children)
              m1 += dfs(tree, dp_table, grandchild);
        }

        // Or take the node's children only
        int m0 = 0;
        for(int child: tree[vertex].children)
            m0 += dfs(tree, dp_table, child);

        dp_table[vertex] = max(m1, m0);
      }
    }

    return dp_table[vertex];
}

int MaxWeightIndependentTreeSubset(const Graph &graph)
{
    // If the tree is empty then the result is zero
    if (graph.size() == 0)
        return 0;

    Graph tree(graph.size());

    // Perform BFS to get a topological order of tree
    queue<int> bfs_queue;
    vector<bool> visited(graph.size(), false);

    bfs_queue.push(0);
    visited[0] = true;

    while(!bfs_queue.empty())
    {
      int current_node = bfs_queue.front();
      bfs_queue.pop();

      tree[current_node].weight = graph[current_node].weight;

      for(int neighbor: graph[current_node].children)
      {
        if(!visited[neighbor])
        {
          visited[neighbor] = true;
          tree[current_node].children.push_back(neighbor);
          bfs_queue.push(neighbor);
        }
      }
    }

    vector<int> dp_table(tree.size(), INT32_MAX);
    return dfs(tree, dp_table, 0);
}

int main() {
    // This code is here to increase the stack size to avoid stack overflow
    // in depth-first search.
    const rlim_t kStackSize = 64L * 1024L * 1024L;  // min stack size = 64 Mb
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    // Here begins the solution
    Graph tree = ReadTree();
    int weight = MaxWeightIndependentTreeSubset(tree);
    cout << weight << endl;
    return 0;
}
