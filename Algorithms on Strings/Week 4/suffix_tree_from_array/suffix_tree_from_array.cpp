/*
#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>

using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::vector;

// Data structure to store edges of a suffix tree.
struct Edge {
  // The ending node of this edge.
  int node;
  // Starting position of the substring of the text
  // corresponding to the label of this edge.
  int start;
  // Position right after the end of the substring of the text
  // corresponding to the label of this edge.
  int end;

  Edge(int node_, int start_, int end_) : node(node_), start(start_), end(end_) {}
  Edge(const Edge& e) : node(e.node), start(e.start), end(e.end) {}
};

// Build suffix tree of the string text given its suffix array suffix_array
// and LCP array lcp_array. Return the tree as a mapping from a node ID
// to the vector of all outgoing edges of the corresponding node. The edges in the
// vector must be sorted in the ascending order by the first character of the edge label.
// Root must have node ID = 0, and all other node IDs must be different
// nonnegative integers.
//
// For example, if text = "ACACAA$", an edge with label "$" from root to a node with ID 1
// must be represented by Edge(1, 6, 7). This edge must be present in the vector tree[0]
// (corresponding to the root node), and it should be the first edge in the vector
// (because it has the smallest first character of all edges outgoing from the root).
map<int, vector<Edge> > SuffixTreeFromSuffixArray(
    const vector<int>& suffix_array,
    const vector<int>& lcp_array,
    const string& text) {
  map<int, vector<Edge> > tree;
  // Implement this function yourself
  return tree;
}


int main() {
  char buffer[200001];
  scanf("%s", buffer);
  string text = buffer;
  vector<int> suffix_array(text.length());
  for (int i = 0; i < text.length(); ++i) {
    scanf("%d", &suffix_array[i]);
  }
  vector<int> lcp_array(text.length() - 1);
  for (int i = 0; i + 1 < text.length(); ++i) {
    scanf("%d", &lcp_array[i]);
  }
  // Build the suffix tree and get a mapping from
  // suffix tree node ID to the list of outgoing Edges.
  map<int, vector<Edge> > tree = SuffixTreeFromSuffixArray(suffix_array, lcp_array, text);
  printf("%s\n", buffer);
  // Output the edges of the suffix tree in the required order.
  // Note that we use here the contract that the root of the tree
  // will have node ID = 0 and that each vector of outgoing edges
  // will be sorted by the first character of the corresponding edge label.
  //
  // The following code avoids recursion to avoid stack overflow issues.
  // It uses a stack to convert recursive function to a while loop.
  // The stack stores pairs (node, edge_index).
  // This code is an equivalent of
  //
  //    OutputEdges(tree, 0);
  //
  // for the following _recursive_ function OutputEdges:
  //
  // void OutputEdges(map<int, vector<Edge> > tree, int node_id) {
  //   const vector<Edge>& edges = tree[node_id];
  //   for (int edge_index = 0; edge_index < edges.size(); ++edge_index) {
  //     printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
  //     OutputEdges(tree, edges[edge_index].node);
  //   }
  // }
  //
  vector<pair<int, int> > stack(1, make_pair(0, 0));
  while (!stack.empty()) {
    pair<int, int> p = stack.back();
    stack.pop_back();
    int node = p.first;
    int edge_index = p.second;
    if (!tree.count(node)) {
      continue;
    }
    const vector<Edge>& edges = tree[node];
    if (edge_index + 1 < edges.size()) {
      stack.push_back(make_pair(node, edge_index + 1));
    }
    printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
    stack.push_back(make_pair(edges[edge_index].node, 0));
  }
  return 0;
}
*/


#include <iostream>
#include <map>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>
#include <stack>

using namespace std;


struct SuffixTreeNode{
  SuffixTreeNode* parent;
  map<char, SuffixTreeNode*> children;
  int string_depth;
  int edge_start;
  int edge_end;
};


SuffixTreeNode* create_new_leaf(SuffixTreeNode* node, const string& text, int current_suffix)
{
  SuffixTreeNode* leaf = new SuffixTreeNode;
  leaf->parent = node;
  leaf->string_depth = text.length() - current_suffix;
  leaf->edge_start = current_suffix + node->string_depth;
  leaf->edge_end = text.length() - 1;
  node->children[text[leaf->edge_start]] = leaf;

  return leaf;
}


SuffixTreeNode* break_edge(SuffixTreeNode* node, const string& text, int start, int offset)
{
  char start_char = text[start];
  char mid_char = text[start + offset];
  SuffixTreeNode* mid_node = new SuffixTreeNode;

  //cout << "Start index: " << start << " Middle index: " << start + offset << endl;

  mid_node->parent = node;
  mid_node->string_depth = node->string_depth + offset;
  mid_node->edge_start = start;
  mid_node->edge_end = start + offset - 1;


  mid_node->children[mid_char] = node->children[start_char];
  node->children[start_char]->parent = mid_node;
  node->children[start_char]->edge_start += offset;
  node->children[start_char] = mid_node;
  return mid_node;
}


void print_suffix_tree(SuffixTreeNode* root, const string& text)
{
  stack<SuffixTreeNode*> bfs_stack;

  bfs_stack.push(root);

  while(!bfs_stack.empty())
  {
    SuffixTreeNode* current_node = bfs_stack.top();
    bfs_stack.pop();

    for(auto it = rbegin(current_node->children); it != rend(current_node->children); it++)
      bfs_stack.push(it->second);

    if(current_node->edge_start > -1)
      printf("%d %d\n", current_node->edge_start, current_node->edge_end + 1);
    //cout << text.substr(current_node->edge_start, current_node->edge_end - current_node->edge_start + 1) << endl;
  }
}

SuffixTreeNode* suffix_tree_from_suffix_array(const vector<int>& suffix_array, const vector<int>& lcp_array, const string& text)
{
  // Create a root node
  SuffixTreeNode* root = new SuffixTreeNode;

  // Initialize root node
  root->parent = NULL;
  root->string_depth = 0;
  root->edge_start = -1;
  root->edge_end = -1;

  // Previous longest common prefix value
  int lcp_prev = 0;

  // Set current node as Root
  SuffixTreeNode* current_node = root;

  // Loop over all sorted suffixes of the text
  for(int i = 0; i < text.length(); i++)
  {
    // Get current suffix start
    int current_suffix = suffix_array[i];

    // Go to previous node until the length of string is bigger than the previous lcp
    while(current_node->string_depth > lcp_prev)
      current_node = current_node->parent;


    // Create a new node if the string length is the same as previous lcp
    if(current_node->string_depth == lcp_prev)
      current_node = create_new_leaf(current_node, text, current_suffix);
    else
    {
      int edge_start = suffix_array[i - 1] + current_node->string_depth;
      int offset = lcp_prev - current_node->string_depth;
      SuffixTreeNode* mid_node = break_edge(current_node, text, edge_start, offset);
      current_node = create_new_leaf(mid_node, text, current_suffix);
    }

    if(i < text.length() - 1)
      lcp_prev = lcp_array[i];

  }

  return root;
}



int main()
{
  char buffer[200001];
  scanf("%s", buffer);
  string text = buffer;

  vector<int> suffix_array(text.length());
  for (int i = 0; i < text.length(); ++i)
  {
    scanf("%d", &suffix_array[i]);
  }

  vector<int> lcp_array(text.length() - 1);
  for (int i = 0; i + 1 < text.length(); ++i)
  {
    scanf("%d", &lcp_array[i]);
  }

  SuffixTreeNode* tree = suffix_tree_from_suffix_array(suffix_array, lcp_array, text);
  printf("%s\n", buffer);
  print_suffix_tree(tree, text);
}
