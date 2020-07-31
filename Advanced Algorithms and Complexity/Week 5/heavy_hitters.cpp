#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

/*
int main() {
    int n, t;
    scanf("%d %d", &n, &t);

    map<int, int> M;
    int id, value;

    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &id, &value);
        assert(M.find(id) == M.end());
        M.insert(pair<int, int>(id, value));
    }

    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &id, &value);
        map<int,int>::iterator it = M.find(id);
        assert(it != M.end());
        it->second -= value;
    }

    int num_queries = 0;
    scanf("%d", &num_queries);
    for (int q = 0; q < num_queries; ++q) {
        int query = 0;
        scanf("%d", &query);

        map<int,int>::iterator it = M.find(query);
        assert(it != M.end());
        if (it->second >= t)
            cout << "1 ";
        else
            cout << "0 ";
    }

    return 0;
}
*/

class HashFunction{
  private:
    long a, b, p, m;

  public:
    HashFunction(long a, long b, long p, long m):
    a(a), b(b), p(p), m(m)
    {}

    long operator()(long x) const{
      return ((a * x + b) % p) % m;
    }
};

class SignFunction{
private:
    HashFunction hash_function;

public:
  SignFunction(long a, long b, long p)
  :hash_function(a, b, p, 1000)
  {}

  int operator()(long x) const{
    return (hash_function(x) < 500)? -1 : 1;
  }
};

class CountSketch{
  private:
    int num_buckets;
    int t;
    const size_t prime = 1000000019;

    vector<vector<int>> count_sketch;
    vector<HashFunction> hash_function;
    vector<SignFunction> sign_function;

  public:
    CountSketch(int n):
    num_buckets(50 * log(n)),
    t(log(n) + 1)
    {
      count_sketch.resize(t, vector<int>(num_buckets));

      for(int r = 0; r < t; r++)
      {
        // Generate t hash functions
        hash_function.push_back(HashFunction(rand() % 50 + 3,
                                             rand() % 50 + 3,
                                             prime,
                                             num_buckets));
        // Generate t sign functions
        sign_function.push_back(SignFunction(rand() % 100 + 10,
                                             rand() % 100 + 10,
                                             prime));
      }
    }

    void update(int i, long value)
    {
      for(int r = 0; r < t; r++)
        count_sketch[r][hash_function[r](i)] += sign_function[r](i) * value;
    }

    long estimate(int i)
    {
      vector<long> counts;

      for(int r = 0; r < t; r++)
        counts.push_back(count_sketch[r][hash_function[r](i)] * sign_function[r](i));

      sort(begin(counts), end(counts));

      if(t % 2 == 0)
        return (counts[t / 2] + counts[(t / 2) - 1]) / 2;
      else
        return counts[t / 2];
    }

};

int main()
{
  // Read the number of children and the threshold.
  int num_children, threshold;
  scanf("%d %d", &num_children, &threshold);

  // Create the count sketch
  CountSketch count_sketch(num_children);

  // The number of good things each child did during the year.
  int id, value;
  for (int i = 0; i < num_children; ++i)
  {
    scanf("%d %d", &id, &value);
    count_sketch.update(id, value);
  }

  // The number of bad things each child did during the year
  for (int i = 0; i < num_children; ++i)
  {
    scanf("%d %d", &id, &value);
    count_sketch.update(id, -value);
  }

  // Read the number of queries.
  int num_queries = 0;
  scanf("%d", &num_queries);

  // Read the ids of the queries
  int query;
  for (int q = 0; q < num_queries; ++q)
  {
    scanf("%d", &query);
    if(count_sketch.estimate(query) >= threshold)
      cout << "1" << endl;
    else
      cout << "0" << endl;
  }

  return 0;
}
