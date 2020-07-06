#include <bits/stdc++.h>
#include <unordered_set>
using namespace std;

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

struct ConvertHampathToSat {
    int numVertices;
    unordered_set<vector<int>, VectorHash> edges;
    //vector<Edge> edges;

    ConvertHampathToSat(int n, int m) :
        numVertices(n),
        edges(m)
    {  }

    void make_combinations_util(
      int start,
      int end,
      int size,
      vector<int>& temp,
      vector<vector<int>>& combinations)
    {
      if(size == 0)
      {
        combinations.push_back(temp);
        return;
      }


      for(int i = start; i <= end; i++)
      {
        temp.push_back(i);
        make_combinations_util(i + 1, end, size - 1, temp, combinations);
        temp.pop_back();
      }
    }

    vector<vector<int>> make_combinations(int start, int end, int size)
    {
      vector<vector<int>> combinations;
      vector<int> temp;
      make_combinations_util(start, end, size, temp, combinations);
      return combinations;
    }

    void printEquisatisfiableSatFormula() {
      vector<string> clauses;

      // Create different combinations between
      vector<vector<int>> combinations = make_combinations(1, numVertices, 2);

      // Each vertex belongs to a path.
      // Each vertex appears just once in a path.
      for(int i = 0; i < numVertices; i++)
      {
        string clause;
        for(int j = 1; j <= numVertices; j++)
          clause += to_string((i * numVertices) + j) + " ";
        clause += "0";
        clauses.push_back(clause);

        for(vector<int> combination: combinations)
          clauses.push_back(to_string(-((i * numVertices) + combination[0])) + " " + to_string(-((i * numVertices) + combination[1])) + " 0");
      }

      // Each position in a path is occupied by some vertex.
      // No two vertices occupy the same position of a path.
      for(int i = 1; i <= numVertices; i++)
      {
        string clause;
        for(int j = 0; j < numVertices; j++)
          clause += to_string((j * numVertices) + i) + " ";
        clause += "0";
        clauses.push_back(clause);

        for(vector<int> combination: combinations)
          clauses.push_back(to_string(-(((combination[0] - 1) * numVertices) + i)) + " " + to_string(-(((combination[1] - 1) * numVertices) + i)) + " 0");
      }



      // Two successive vertices on a path must be connected by an edge
      for(int i = 0; i < numVertices - 1; i++)
      {
        for(int j = i + 1; j < numVertices; j++)
        {
          // If there is no edge between the two vertices then they can not come after each other in the path
          if((edges.find({i , j}) == end(edges)) && (edges.find({j , i}) == end(edges)))
          {
            for(int k = 1; k < numVertices; k++)
            {
              clauses.push_back(to_string(-((i * numVertices) + k)) + " " + to_string(-((j * numVertices) + k + 1)) + " 0");
              clauses.push_back(to_string(-((i * numVertices) + k + 1)) + " " + to_string(-((j * numVertices) + k)) + " 0");
            }
          }
        }
      }


      cout << clauses.size() << " " << numVertices * numVertices << endl;
      for(string clause: clauses)
        cout << clause << endl;

    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertHampathToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      converter.edges.insert({u - 1, v - 1});
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
