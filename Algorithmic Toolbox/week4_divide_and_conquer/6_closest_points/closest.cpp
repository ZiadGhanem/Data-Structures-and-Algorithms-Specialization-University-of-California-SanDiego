#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

using namespace std;


struct point{
  int x;
  int y;
};

double euclidean_distance(pair<point, point> points_pair)
{
  return sqrt(pow(points_pair.first.x - points_pair.second.x, 2) + pow(points_pair.first.y - points_pair.second.y, 2));
}

pair<point, point> closest_split_pair(vector<point> points_x, vector<point> points_y, double delta)
{
  pair<point, point> minimum_pair,
  current_pair;

  double x_bar = points_x[points_x.size() / 2].x;
  vector<point> s_y;
  for(auto point: points_y)
  {
    if(point.x >= (x_bar - delta) && point.x <= (x_bar + delta))
    {
      s_y.push_back(point);
    }
  }
  double minimum_distance = delta,
  current_distance;
  for(int i = 0; i < s_y.size() - 1; i++)
  {
    for(int j = 0; j < min(7, int(s_y.size()) - i); j++)
    {
      current_pair = make_pair(s_y[i], s_y[i + j]);
      current_distance = euclidean_distance(current_pair);
      if(current_distance < minimum_distance)
      {
        minimum_pair = current_pair;
        minimum_distance = current_distance;
      }
    }
  }
  return minimum_pair;
}

pair<point, point> closest_pair_brute(vector<point> &points_x)
{
  double minimum_distance = DBL_MAX,
  current_distance;

  pair<point, point> current_pair, minimum_pair;

  for(int i = 0; i < points_x.size() - 1; i++)
  {
    for(int j = i + 1; j < points_x.size(); j++)
    {
      current_pair = make_pair(points_x[i], points_x[j]);
      current_distance = euclidean_distance(current_pair);
      if(current_distance < minimum_distance)
      {
        minimum_distance = current_distance;
        minimum_pair = current_pair;
      }
    }
  }
  return minimum_pair;
}

pair<point, point> closest_pair(vector<point> &points_x, vector<point> &points_y)
{
  if(points_x.size() <= 3)
  {
    return closest_pair_brute(points_x);
  }
  else
  {
    vector<pair<point, point>> pairs;
    int mid = points_x.size() / 2;
    point midpoint = points_x[mid];
    vector<point> points_x_left(begin(points_x), begin(points_x) + mid);
    vector<point> points_x_right(begin(points_x) + mid + 1, end(points_x));

    vector<point> points_y_left(points_x_left.size());
    vector<point> points_y_right(points_x_right.size());

    for(point current_point: points_y)
    {
      if(current_point.x <= midpoint.x)
      {
        points_y_left.push_back(current_point);
      }
      else
      {
        points_y_right.push_back(current_point);
      }
    }


    pairs.push_back(closest_pair(points_x_left, points_y_left));
    pairs.push_back(closest_pair(points_x_right, points_y_right));
    double delta = min(euclidean_distance(pairs[0]), euclidean_distance(pairs[1]));
    pairs.push_back(closest_split_pair(points_x, points_y, delta));

    double current_distance, minimum_distance = DBL_MAX;
    pair<point, point> minimum_pair;
    for(auto pair: pairs)
    {
      current_distance = euclidean_distance(pair);
      if(current_distance < minimum_distance)
      {
        minimum_distance = current_distance;
        minimum_pair = pair;
      }
    }
    return minimum_pair;
  }
}


double minimal_distance(vector<point> &points)
{
  vector<point> x_sorted(points.size()),
  y_sorted(points.size());

  partial_sort_copy(begin(points), end(points), begin(x_sorted), end(x_sorted), [](const point a, const point b)
  {
    return (a.x < b.x);
  });

  partial_sort_copy(begin(points), end(points), begin(y_sorted), end(y_sorted), [](const point a, const point b)
  {
    return (a.y < b.y);
  });

  return euclidean_distance(closest_pair(x_sorted, y_sorted));
}

/*
double minimal_distance_naive(vector<int> x, vector<int> y)
{
  double minimum_distance = DBL_MAX;
  for(int i = 0; i < x.size() - 1; i++)
  {
    for(int j = i + 1; j < x.size(); j++)
    {
      minimum_distance = min(minimum_distance, sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)));
    }
  }
  return minimum_distance;
}

double minimal_distance(vector<int> x, vector<int> y) {
  //write your code here
  return 0.;
}
*/

int main()
{
  size_t n;
  std::cin >> n;
  vector<point> points(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> points[i].x >> points[i].y;
  }
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << minimal_distance(points) << "\n";
}

/*
int main()
{
  size_t n;
  std::cin >> n;
  vector<int> x(n);
  vector<int> y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << minimal_distance_naive(x, y) << "\n";
}
*/
