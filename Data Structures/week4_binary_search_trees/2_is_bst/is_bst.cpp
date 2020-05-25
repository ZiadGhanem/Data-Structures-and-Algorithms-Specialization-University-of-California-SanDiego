#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

struct Node {
  int key;
  int left;
  int right;

  Node() : key(0), left(-1), right(-1) {}
  Node(int key_, int left_, int right_) : key(key_), left(left_), right(right_) {}
};


bool IsBinarySearchTree(const vector<Node>& tree)
{
  // Implement correct algorithm here
  if(tree.size() == 0)
    return true;

  vector<int> tree_vector;
  stack<int> inorder_stack;
  int current_node = 0;
  while(current_node != -1 || !inorder_stack.empty())
  {
    while(current_node != -1)
    {
      inorder_stack.push(current_node);
      current_node = tree[current_node].left;
    }

    current_node = inorder_stack.top();
    inorder_stack.pop();

    tree_vector.push_back(tree[current_node].key);

    current_node = tree[current_node].right;
  }


  return is_sorted(begin(tree_vector), end(tree_vector));

}

int main() {
  int nodes;
  cin >> nodes;
  vector<Node> tree;
  for (int i = 0; i < nodes; ++i) {
    int key, left, right;
    cin >> key >> left >> right;
    tree.push_back(Node(key, left, right));
  }
  if (IsBinarySearchTree(tree)) {
    cout << "CORRECT" << endl;
  } else {
    cout << "INCORRECT" << endl;
  }
  return 0;
}
