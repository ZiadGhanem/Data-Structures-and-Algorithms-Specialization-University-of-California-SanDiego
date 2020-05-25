#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Node {
  long long key;
  long long left;
  long long right;

  Node() : key(0), left(-1), right(-1) {}
  Node(long long key_, long long left_, long long right_) : key(key_), left(left_), right(right_) {}
};

class BinarySearchTree
{
public:
    BinarySearchTree(int nodes_) :nodes(nodes_) {}

    void ReadTree(void)
    {
      for (int i = 0; i < this->nodes; ++i)
      {
        int key, left, right;
        cin >> key >> left >> right;
        this->tree.push_back(Node(key, left, right));
      }
    }

    bool IsBinarySearchTree(void)
    {
      if(this->nodes == 0)
        return true;
      return this->IsBinarySearchTreeUtil(0, INT64_MIN, INT64_MAX);
    }

private:
  int nodes;
  vector<Node> tree;

  bool IsBinarySearchTreeUtil(long long current_node, long long minimum, long long maximum)
  {
    if(current_node == -1)
      return true;

    if(this->tree[current_node].key < minimum || this->tree[current_node].key >= maximum)
      return false;

    return (IsBinarySearchTreeUtil(tree[current_node].left, minimum, (this->tree[current_node].key))
            &&
            IsBinarySearchTreeUtil(tree[current_node].right, (this->tree[current_node].key), maximum));
  }
};


int main() {
  int nodes;
  cin >> nodes;
  BinarySearchTree bst(nodes);
  bst.ReadTree();

  if (bst.IsBinarySearchTree()) {
    cout << "CORRECT" << endl;
  } else {
    cout << "INCORRECT" << endl;
  }
  return 0;
}
