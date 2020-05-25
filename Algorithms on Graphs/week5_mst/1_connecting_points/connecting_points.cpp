#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

struct edge{
  int source;
  int destination;
  double distance;
};

class unionFind
{
  public:
    unionFind(int number_vertices_)
    :number_vertices(number_vertices_){
      for(int i = 0; i < number_vertices_; i++)
        this->parent.push_back(i);
      this->rank.resize(number_vertices_, 0);
    }

    int find(int x){
      if(this->parent[x] != x)
        this->parent[x] = find(this->parent[x]);
      return this->parent[x];
    }

    void make_union(int x, int y){
      int s1 = find(x);
      int s2 = find(y);

      if(s1 == s2)
        return;

      if(this->rank[s1] == this->rank[s2])
      {
        this->parent[s1] = s2;
        this->rank[s2]++;
      }
      else if(this->rank[s1] > this->rank[s2])
        this->parent[s2] = s1;
      else
        this->parent[s1] = s2;
    }

  private:
    int number_vertices;
    vector<int> parent;
    vector<int> rank;
};

vector<edge> get_distance_vector(vector<int> &x, vector<int> &y)
{
  vector<edge> edges;

  for(int i = 0; i < x.size() - 1; i++)
  {
    for(int j = i + 1; j < x.size(); j++)
    {
      edge current_edge;
      current_edge.source = i;
      current_edge.destination = j;
      current_edge.distance = sqrt(pow(x[i] - x[j] , 2) + pow(y[i] - y[j] , 2));
      edges.push_back(current_edge);
    }
  }

  return edges;
}

double minimum_distance(vector<int> &x, vector<int> &y)
{
  double result = 0.;
  //write your code here
  vector<edge> edges = get_distance_vector(x, y);
  sort(begin(edges), end(edges), [](edge x, edge y){
    return (y.distance > x.distance);
  });
  unionFind kruskals_sets(x.size());
  for(auto edge: edges)
  {
    if(kruskals_sets.find(edge.source) != kruskals_sets.find(edge.destination))
    {
      kruskals_sets.make_union(edge.source, edge.destination);
      result += edge.distance;
    }
  }

  return result;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> x(n), y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cout << std::setprecision(10) << minimum_distance(x, y) << std::endl;
}
