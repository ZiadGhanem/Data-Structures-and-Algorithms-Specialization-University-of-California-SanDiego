/*
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

typedef Node unordered_map<string, Node*>;


// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.
vector<string> ComputeSuffixTreeEdges(string& text)
{
  Node* root = new Node,
  current_node;

  text += '$';
  for(int i = 0; i < text.length() - 1; i++)
  {
    current_node = root;
    string current_suffix = text.substr(i);
    for(auto it = begin(current_node); it != end(current_node); it++)
    {
      for(int i = 0; i < it->first.length(); i++)
      {
        if(it->first[i] != text[i])
        {

        }
      }
    }
    if(current_node->find(current_suffix) == end(current_node))
    {
      current_node[current_suffix] = (Vertex *)NULL;
    }
  }
}

int main()
{
  string text;
  cin >> text;
  vector<string> edges = ComputeSuffixTreeEdges(text);
  for (int i = 0; i < edges.size(); ++i) {
    cout << edges[i] << endl;
  }
  return 0;
}
*/


/*
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int get_index(char letter){
  switch(letter){
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
    case '$': return 4;
  }
}

char get_letter(int index){
  switch(index){
    case 0: return 'A';
    case 1: return 'C';
    case 2: return 'G';
    case 3: return 'T';
    case 4: return '$';
  }
}

struct Node{
  int number_children;
  Node* child[5];

  Node(){
    number_children = 0;
    for(int i = 0; i < 5; i++){
      child[i] = NULL;
    }
  }
};

class SuffixTree{
  public:

    SuffixTree(string& text){
      Node* current_node;
      for(int i = 0; i < text.length(); i++){
        current_node = this->root;
        string current_suffix = text.substr(i);
        for(char letter: current_suffix){
            int position = get_index(letter);
            if(current_node->child[position] == NULL){
              current_node->child[position] = new Node;
              current_node->number_children ++;
            }
            current_node = current_node->child[position];
        }
      }
    }

    vector<string> get_edges(void)
    {
      vector<string> result;
      this->get_edges_util("", result, this->root);
      return result;
    }

  private:

    Node* root = new Node;

    void get_edges_util(string current_string, vector<string> &edges, Node* current_node)
    {
      for(int i = 0; i < 5; i++)
      {
        if(current_node->child[i] != NULL)
        {
          char current_letter = get_letter(i);

          if(current_node->child[i]->number_children > 1)
          {
            edges.push_back(current_string + current_letter);
            this->get_edges_util("", edges, current_node->child[i]);
          }
          else
          {
            this->get_edges_util(current_string + current_letter, edges, current_node->child[i]);
          }
        }
      }
    }
};


int main() {
  string text;
  cin >> text;
  SuffixTree suffix_tree(text);
  vector<string> edges = suffix_tree.get_edges();
  for (int i = 0; i < edges.size(); ++i) {
    cout << edges[i] << endl;
  }
  return 0;
}
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct Node{
  unordered_map<string, Node*> child;
  Node* parent;
  //Node* slink;
  int depth;
  int start;

  Node(void){
    parent = NULL;
  }
};

/*
void compute_slink(string &text, Node* current_node)
{
  Node* new_node = current_node->parent->slink;
  while(new_node->depth < current_node->depth - 1)
    new_node = new_node->child[text.substr(current_node->start + new_node->depth + 1)];

  if(new_node->depth > current_node->depth - 1)
  {
    Node* new_leaf;
    new_leaf->start = current_node->start + 1;
    new_leaf->depth = current_node->depth - 1;
    new_leaf->slink = NULL;

    Node* parent = new_node->parent;
    new_leaf->child[text.substr(new_node->start + new_leaf->depth)] = new_node;
    new_node->parent = new_leaf;
    parent->child[text.substr(new_leaf->start + parent->depth)] = new_leaf;
    new_leaf->parent = parent;
    new_node = new_leaf;
  }
  current_node->slink = new_node;
}
*/

Node* create_suffix_tree(string &text)
{
  Node* root = new Node;
  root->depth = 0;
  //root->slink = root;

  Node* current_node = root;
  int current_depth = 0;

  for(int i = 0; i < text.length(); i++)
  {
    while((current_depth == current_node->depth) && (current_node->child.find(text.substr(i + current_depth)) != end(current_node->child)))
    {
      current_node = current_node->child[text.substr(i + current_depth)];
      current_depth ++;

      while((current_depth < current_node->depth) && (text.substr(current_node->start + current_depth) == text.substr(i + current_depth)))
        current_depth++;
    }

    if(current_depth < current_node->depth)
    {
      Node* new_node = new Node;
      new_node->start = i;
      new_node->depth = current_depth;
      //new_node->slink = NULL;
      Node* parent = new Node;
      parent = current_node->parent;
      new_node->child[text.substr(current_node->start + current_depth)] = current_node;
      current_node->parent = new_node;
      parent->child[text.substr(i + parent->depth)] = new_node;
      new_node->parent = parent;
      current_node = new_node;
    }

    Node* new_leaf = new Node;
    new_leaf->start = i;
    new_leaf->depth = text.length() - i + 1;
    current_node->child[text.substr(i + current_depth)] = new_leaf;
    new_leaf->parent = current_node;
    current_node = root;
    current_depth = 0;
    /*
    if(current_node->slink == NULL)
      compute_slink(text, current_node);

    current_node = current_node->slink;
    current_depth --;
    */
  }

  return root;
}


void print_tree(Node* current_node)
{
  for(auto it = begin(current_node->child); it != end(current_node->child); it++)
  {
    cout << it->first << endl;
    print_tree(it->second);
  }
}

int main()
{
  string text;
  cin >> text;
  Node* root = create_suffix_tree(text);
  print_tree(root);
  return 0;
}
