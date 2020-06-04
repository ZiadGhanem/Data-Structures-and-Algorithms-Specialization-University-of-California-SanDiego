
#include <iostream>
#include <map>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>
#include <queue>

using namespace std;


struct SuffixTreeNode{
  SuffixTreeNode* parent;
  map<char, SuffixTreeNode*> children;
  int string_depth;
  int edge_start;
  int edge_end;
};


vector<int> sort_characters(const string& text)
{
  // Sorted indices of the characters of the string
  vector<int> order(text.length());
  // The number of occurrences of each character in the string
  map<char, int> count;

  // Count the number of occurrences of each character in the string
  for(char letter: text)
    count[letter]++;

  // Accumulate the count of characters in the string
  auto it = begin(count);
  int previous_val = it->second;
  it++;
  for(; it != end(count); it++)
  {
    it->second += previous_val;
    previous_val = it->second;
  }

  // Set the order of each character in the string
  for(int i = text.length() - 1; i >= 0; i--)
  {
    char letter = text[i];
    count[letter]--;
    order[count[letter]] = i;
  }

  return order;
}

vector<int> compute_char_classes(const string& text, vector<int>& order)
{
  // Each character is given a certain class
  vector<int> classes(text.length());

  // Give first character class 0
  classes[order[0]] = 0;
  for(int i = 1; i < text.length(); i++)
  {
    // If the next character is not the same as previous give it a new class
    if(text[order[i]] != text[order[i - 1]])
      classes[order[i]] = classes[order[i - 1]] + 1;
    else
      classes[order[i]] = classes[order[i - 1]];
  }

  return classes;
}

vector<int> sort_doubled(const string& text, int l, vector<int>& order, vector<int>& classes)
{
  // The number of occurrences of each class of double cyclic shifts
  vector<int> count(text.length(), 0);

  // Sorted double cylic shifts of length l
  vector<int> new_order(text.length());

  // Count the number occurences of each class of double cyclic shifts
  for(int i = 0; i < text.length(); i++)
    count[classes[i]]++;

  // Accumulate the count of double cylic shifts
  for(int i = 1; i < text.length(); i++)
    count[i] += count[i - 1];

  // Set the new order of double cylic shifts
  for(int i = text.length() - 1; i >=0 ; i--)
  {
    int start = (order[i] - l + text.length()) % text.length();
    int current_class = classes[start];
    count[current_class]--;
    new_order[count[current_class]] = start;
  }

  return new_order;
}


vector<int> update_classes(vector<int>& new_order, vector<int>& classes, int l)
{
  int n = new_order.size();
  vector<int> new_classes(n);
  new_classes[new_order[0]] = 0;

  // Compare pairs of classes of double cyclic shifts
  for(int i = 1; i < n; i++)
  {
    int cur = new_order[i];
    int prev = new_order[i - 1];
    int mid = (cur + l) %  n;
    int mid_prev = (prev + l) % n;

    // If the two pairs were not equal then give it a new class
    if((classes[cur] != classes[prev]) || (classes[mid] != classes[mid_prev]))
      new_classes[cur] = new_classes[prev] + 1;
    else
      new_classes[cur] = new_classes[prev];
  }

  return new_classes;
}


vector<int> build_suffix_array(const string& text)
{
  // Sort characters using count sort
  vector<int> order = sort_characters(text);

  // Give each character a class
  vector<int> classes = compute_char_classes(text, order);

  // Loop over double cylic shifts
  for(int l = 1; l < text.length(); l*=2)
  {
    // Get the new order of double cylic shifts
    order = sort_doubled(text, l, order, classes);

    // Get new class of double cyclic shifts
    classes = update_classes(order, classes, l);
  }

  return order;
}


int lcp_of_suffixes(const string& text, int i, int j, int equal)
{
  // Make sure the start longest common prefix is not below 0
  int lcp = max(0, equal);
  
  while((i + lcp < text.length()) && (j + lcp < text.length()))
  {
    // If the characters match increase the longest common prefix
    if(text[i + lcp] == text[j + lcp])
      lcp++;
    else
      break;
  }
  return lcp;
}

// Get the order of the suffix at each index
vector<int> invert_suffix_array(vector<int> &suffix_array)
{
  // Inverse of order
  vector<int> pos(suffix_array.size());
  for(int i = 0; i < pos.size(); i++)
    pos[suffix_array[i]] = i;
  return pos;
}


vector<int> build_lcp_array(const string& text, vector<int>& suffix_array)
{
  // Longest common prefix array
  vector<int> lcp_array(text.size() - 1);

  // The first longest common prefix is 0 as there was no previous suffix
  int lcp = 0;

  // We have an array with the start indices of each suffix in sorted order
  // We need the inverse; The order of the suffix at Each index
  vector<int> pos_in_order = invert_suffix_array(suffix_array);

  // Start with the smallest suffix
  int suffix = suffix_array[0];

  for(int i = 0; i < text.length(); i++)
  {
    // Get the order of current suffix
    int order_index = pos_in_order[suffix];

    // If it was the last suffix; No next suffix
    if(order_index == text.length() - 1)
    {
      // Change the current suffix to the next cylic shift
      lcp = 0;
      suffix = (suffix + 1) % text.length();
      continue;
    }

    // Get the suffix after the current suffix
    int next_suffix = suffix_array[order_index + 1];

    // Get their longest common prefix
    // The longest possible common prefix >= previous longest common prefix - 1
    lcp = lcp_of_suffixes(text, suffix, next_suffix, lcp - 1);
    lcp_array[order_index] = lcp;

    // Change the current suffix to the next cylic shift
    suffix = (suffix + 1) % text.length();
  }

  return lcp_array;
}


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
  queue<SuffixTreeNode*> bfs_queue;

  for(auto it = begin(root->children); it != end(root->children); it++)
    bfs_queue.push(it->second);

  while(!bfs_queue.empty())
  {
    SuffixTreeNode* current_node = bfs_queue.front();
    bfs_queue.pop();

    for(auto it = begin(current_node->children); it != end(current_node->children); it++)
      bfs_queue.push(it->second);

    cout << text.substr(current_node->edge_start, current_node->edge_end - current_node->edge_start + 1) << endl;
  }
}

SuffixTreeNode* suffix_tree(const string& text)
{
  // Create suffix array
  vector<int> suffix_array = build_suffix_array(text);
  // Create Longest common prefix array
  vector<int> lcp_array = build_lcp_array(text, suffix_array);

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
  string text;
  cin >> text;

  SuffixTreeNode* tree = suffix_tree(text);
  print_suffix_tree(tree, text);
}
