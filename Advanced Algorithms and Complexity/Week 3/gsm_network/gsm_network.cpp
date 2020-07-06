#include <ios>
#include <iostream>
#include <vector>

using namespace std;

const int b = 3;

struct Edge {
    int from;
    int to;
};

struct ConvertGSMNetworkProblemToSat {
    int numVertices;
    vector<Edge> edges;

    ConvertGSMNetworkProblemToSat(int n, int m) :
        numVertices(n),
        edges(m)
    {  }

    void makeCombiUtil(vector<vector<int> >& ans,
    vector<int>& tmp, int n, int left, int k)
    {
        // Pushing this vector to a vector of vector
        if (k == 0) {
            ans.push_back(tmp);
            return;
        }

        // i iterates from left to n. First time
        // left will be 1
        for (int i = left; i <= n; ++i)
        {
            tmp.push_back(i);
            makeCombiUtil(ans, tmp, n, i + 1, k - 1);

            // Popping out last inserted element
            // from the vector
            tmp.pop_back();
        }
    }

    // Gets all combinations of size k of numbers
    // from start to end.
    vector<vector<int>> makeCombi(int start, int end, int k)
    {
        vector<vector<int> > ans;
        vector<int> tmp;
        makeCombiUtil(ans, tmp, end, start, k);
        return ans;
    }

    void printEquisatisfiableSatFormula() {

        // Output number of clauses and number of boolean variables
        cout << (((b * (b - 1) / 2) + 1) * numVertices) + (b * edges.size()) << " " << b * numVertices << endl;

        // Get combinations of numbers between 1 and b of size 2
        vector<vector<int>> combinations = makeCombi(1, b, 2);

        // Exactly one color for each vertex
        for(int i = 0; i < numVertices; i++)
        {
          int vertex_start = i * b;
          for(int j = 1; j <= b; j++)
            cout << vertex_start + j << " ";
          cout << "0" << endl;
          for(vector<int> combination: combinations)
            cout << -(vertex_start + combination[0]) << " " << - (vertex_start + combination[1]) << " 0" << endl;
        }

        // Ends of one edge can not have the same color
        for(auto edge: edges)
        {
          for(int i = 1; i <= b; i++)
            cout << -((edge.from * b) + i) << " " << -((edge.to * b) + i) << " 0" << endl;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        converter.edges[i].from  = u - 1;
        converter.edges[i].to = v - 1;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
