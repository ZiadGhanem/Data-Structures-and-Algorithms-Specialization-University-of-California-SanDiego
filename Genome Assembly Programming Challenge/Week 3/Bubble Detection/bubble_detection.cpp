
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

#define NUM_READS 1618

using namespace std;

void create_debruijn_graph(vector<string>& reads, int kmer_size, unordered_map<string, unordered_set<string>>& graph)
{
  vector<string> kmer_reads;

  // Create K-mers
  for(auto& read: reads)
  {
    for(int i = 0; i < read.size() - kmer_size + 1; i++)
      kmer_reads.push_back(read.substr(i, kmer_size));
  }

  for(int i = 0; i < kmer_reads.size(); i++)
  {
    // Labelling vertices by (k - 1)mers
    string source = kmer_reads[i].substr(0, kmer_size - 1);
    string destination = kmer_reads[i].substr(1);

    //cout << source << " " << destination << endl;

    // Label edges by k-mers
    if(source != destination)
      graph[source].insert(destination);
  }
}

void get_degree(unordered_map<string, unordered_set<string>>& graph, vector<string>& valid_out, unordered_set<string>& valid_in)
{
  unordered_map<string, int> in_degree;

  for(auto& current_node: graph)
  {
    for(auto& destination: current_node.second)
      in_degree[destination]++;

    if(current_node.second.size() > 1)
      valid_out.push_back(current_node.first);
  }

  for(auto& current_node: in_degree)
  {
    if(current_node.second > 1)
      valid_in.insert(current_node.first);
  }
}

void depth_first_search(
  unordered_map<string, unordered_set<string>>& graph,
  unordered_set<string>& valid_in,
  unordered_set<string>& visited,
  vector<string>& temp_path,
  vector<vector<string>>& paths,
  string source,
  int threshold
)
{
  // If the length of path did not reach the threshold
  if(temp_path.size() <= threshold)
  {
    // Set the node as visited
    visited.insert(source);
    // Add node to path
    temp_path.push_back(source);

    // If we reached a node in valid in then add the current path to paths of the souce node
    if(valid_in.find(source) != end(valid_in))
      paths.push_back(temp_path);

    // Perform depth first search on neighbors
    for(string neighbor: graph[source])
    {
      if(visited.find(neighbor) == end(visited))
        depth_first_search(graph, valid_in, visited, temp_path, paths, neighbor, threshold);
    }

    // Remove node from visited nodes
    visited.erase(source);
    // Remove node from the path
    temp_path.pop_back();
  }
}

bool compare_paths(vector<string>& path_a, vector<string>& path_b)
{
  // If there is a common sink
  if(path_a.back() == path_b.back())
  {
    for(int i = 1; i < path_a.size() - 1; i++)
    {
      for(int j = 1; j < path_b.size() - 1; j++)
      {
        if(path_a[i] == path_b[j])
          return false;
      }
    }
    return true;
  }
  return false;
}

int main(void)
{
  // Get k-mers size and threshold
  int kmer_size, threshold;
  cin >> kmer_size >> threshold;

  // Get Reads
  vector<string> reads;
  string current_read;
  for(int i = 0; i < NUM_READS; i++)
  {
    cin >> current_read;
    reads.push_back(current_read);
  }

  // Create DeBruijn Graph
  unordered_map<string, unordered_set<string>> graph;

  // Create DeBruijn Graph
  create_debruijn_graph(reads, kmer_size, graph);

  // Get Edges with Out-degree > 1 and Edges with In-degree > 1
  vector<string> valid_out;
  unordered_set<string> valid_in;

  get_degree(graph, valid_out, valid_in);

  // Create paths from valid out to valid in
  vector<vector<string>> paths;
  unordered_set<string> visited;
  vector<string> temp_path;

  int bubble_count = 0;
  for(auto& source: valid_out)
  {
    paths.clear();

    depth_first_search(graph, valid_in, visited, temp_path, paths, source, threshold);
    for(int i = 0; i < paths.size() - 1; i++)
    {
      for(int j = i + 1; j < paths.size(); j++)
      {
        if(compare_paths(paths[i], paths[j]))
          bubble_count++;
      }
    }
  }

  cout << bubble_count;

  return 0;

}

/*
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

#define NUM_READS 1618

using namespace std;

void create_debruijn_graph(vector<string>& reads, int kmer_size, unordered_map<string, unordered_set<string>>& graph)
{
  vector<string> kmer_reads;

  // Create K-mers
  for(auto& read: reads)
  {
    for(int i = 0; i < read.size() - kmer_size + 1; i++)
      kmer_reads.push_back(read.substr(i, kmer_size));
  }

  for(int i = 0; i < kmer_reads.size(); i++)
  {
    // Labelling vertices by (k - 1)mers
    string source = kmer_reads[i].substr(0, kmer_size - 1);
    string destination = kmer_reads[i].substr(1);

    //cout << source << " " << destination << endl;

    // Label edges by k-mers
    if(source != destination)
      graph[source].insert(destination);
  }
}

void get_degree(unordered_map<string, unordered_set<string>>& graph, vector<pair<string, string>>& out_in_pairs)
{
  unordered_map<string, int> in_degree;
  vector<string> valid_out;
  vector<string> valid_in;

  for(auto& current_node: graph)
  {
    for(auto& destination: current_node.second)
      in_degree[destination]++;

    if(current_node.second.size() > 1)
    {
      //cout << "More than one out edge: " << current_node.first << " , " << current_node.second.size() << endl;
      valid_out.push_back(current_node.first);
    }
  }

  for(auto& current_node: in_degree)
  {
    if(current_node.second > 1)
    {
      //cout << "More than one in edge: " << current_node.first << " , " << current_node.second << endl;
      valid_in.push_back(current_node.first);
    }
  }

  for(auto& node_a: valid_out)
  {
    for(auto& node_b: valid_in)
      out_in_pairs.push_back({node_a, node_b});
  }
}

void depth_first_search(
  unordered_map<string, unordered_set<string>>& graph,
  unordered_set<string>& temp_path,
  vector<unordered_set<string>>& paths,
  string source,
  string destination,
  int threshold
)
{
  // If the length of path did not reach the threshold
  if(temp_path.size() <= threshold)
  {
    // Add node to path
    temp_path.insert(source);

    // If we reached a node in valid in then add the current path to paths of the souce node
    if(source == destination)
      paths.push_back(temp_path);

    // Perform depth first search on neighbors
    for(string neighbor: graph[source])
    {
      if(temp_path.find(neighbor) == end(temp_path))
        depth_first_search(graph, temp_path, paths, neighbor, destination, threshold);
    }
    // Remove node from the path
    temp_path.erase(source);
  }
}

bool compare_paths(unordered_set<string>& path_a, unordered_set<string>& path_b, string& source, string& destination)
{
  // Compare all nodes between start and end
  for(auto& node_a: path_a)
  {
    if((node_a != source) && (node_a != destination) && (path_b.find(node_a) != end(path_b)))
      return false;
  }
  return true;
}

int main(void)
{
  // Get k-mers size and threshold
  int kmer_size, threshold;
  cin >> kmer_size >> threshold;

  // Get Reads
  vector<string> reads;
  string current_read;
  for(int i = 0; i < NUM_READS; i++)
  {
    cin >> current_read;
    reads.push_back(current_read);
  }

  // Create DeBruijn Graph
  unordered_map<string, unordered_set<string>> graph;

  // Create DeBruijn Graph
  create_debruijn_graph(reads, kmer_size, graph);

  // Get Edges with Out-degree > 1 and Edges with In-degree > 1
  vector<pair<string, string>> out_in_pairs;

  get_degree(graph, out_in_pairs);

  vector<unordered_set<string>> paths;
  unordered_set<string> temp_path;

  int bubble_count = 0;
  for(auto& source_destination: out_in_pairs)
  {
    paths.clear();
    depth_first_search(graph, temp_path, paths, source_destination.first, source_destination.second, threshold);
    for(int i = 0; i < paths.size() - 1; i++)
    {
      for(int j = i + 1; j < paths.size(); j++)
      {
        if(compare_paths(paths[i], paths[j], source_destination.first, source_destination.second))
          bubble_count++;
      }
    }
  }

  cout << bubble_count;


  return 0;

}
*/
