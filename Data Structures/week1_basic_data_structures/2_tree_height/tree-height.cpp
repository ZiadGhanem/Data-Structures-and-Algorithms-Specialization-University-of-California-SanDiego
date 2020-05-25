#include <algorithm>
#include <iostream>
#include <vector>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#include <stack>
#include <queue>
#endif

using namespace std;

class Node;

class Node {
public:
    int key;
    Node *parent;
    std::vector<Node *> children;

    Node() {
      this->parent = NULL;
    }

    void setParent(Node *theParent) {
      parent = theParent;
      parent->children.push_back(this);
    }
};

/*
int depth_first_search(Node* current_node, vector<bool> visited, int max_height)
{
  visited[current_node->key] = true;
  int current_height = max_height;
  for(int child_index = 0; child_index < current_node->children.size(); child_index++)
  {
    if(!visited[current_node->children[child_index]->key])
    {
      max_height = max(max_height, depth_first_search(current_node->children[child_index], visited, current_height + 1));
    }
  }

  return max_height;
}
*/

int main_with_large_stack_space() {
  std::ios_base::sync_with_stdio(0);
  int n;
  std::cin >> n;

  std::vector<Node> nodes;
  nodes.resize(n);
  for (int child_index = 0; child_index < n; child_index++) {
    int parent_index;
    std::cin >> parent_index;
    if (parent_index >= 0)
      nodes[child_index].setParent(&nodes[parent_index]);
    nodes[child_index].key = child_index;
  }

  // Find root node
  Node *root_node;
  for(int leaf_index = 0; leaf_index < n; leaf_index++)
  {
    if(nodes[leaf_index].parent == NULL)
    {
      root_node = &nodes[leaf_index];
    }
  }
  vector<bool> visited(n, false);
  vector<int> height(n);
  int max_height = INT32_MIN;
  height[root_node -> key] = 1;
  queue<Node*> BFS_queue;
  visited[root_node->key] = true;
  BFS_queue.push(root_node);
  Node* current_node;
  while(!BFS_queue.empty())
  {
    current_node = BFS_queue.front();
    BFS_queue.pop();
    for(int child_index = 0; child_index < current_node->children.size(); child_index++)
    {
      if(!visited[current_node->children[child_index]->key])
      {
        visited[current_node->children[child_index]->key] = true;
        BFS_queue.push(current_node->children[child_index]);
        height[current_node->children[child_index]->key] = height[current_node->key] + 1;
        max_height = max(height[current_node->children[child_index]->key], max_height);
      }
    }
  }
  //std::cout << depth_first_search(&root_node, visited, 1) << std::endl;
  std::cout << max_height << std::endl;
  return 0;
}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
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
              std::cerr << "setrlimit returned result = " << result << std::endl;
          }
      }
  }

#endif
  return main_with_large_stack_space();
}
