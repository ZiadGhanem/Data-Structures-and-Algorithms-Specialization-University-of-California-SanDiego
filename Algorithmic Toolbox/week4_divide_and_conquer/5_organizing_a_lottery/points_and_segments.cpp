#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct point
{
  char type;
  int location;
  int index;
};

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points)
{
  vector<int> cnt(points.size());
  vector<point> all_points;
  for(int current_start: starts)
  {
    point current_point;
    current_point.type='l';
    current_point.location = current_start;
    all_points.push_back(current_point);
  }
  for(int current_end: ends)
  {
    point current_point;
    current_point.type='r';
    current_point.location = current_end;
    all_points.push_back(current_point);
  }
  for(size_t i = 0; i < points.size(); i++)
  {
    point current_point;
    current_point.type= 'p';
    current_point.location = points[i];
    current_point.index = i;
    all_points.push_back(current_point);
  }

  sort(begin(all_points), end(all_points), []
  (const point &a, const point &b)
  {
    if(a.location == b.location)
    {
      if(a.type == 'l')
      {
        return true;
      }
      else if(b.type == 'l')
      {
        return false;
      }
      else if(a.type == 'p')
      {
        return true;
      }
      else if(b.type == 'p')
      {
        return false;
      }
      else if(a.type == 'r')
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else if(a.location < b.location)
    {
      return true;
    }
    else
    {
      return false;
    }
  });


  int segments_count = 0;
  for(point current_point: all_points)
  {
    if(current_point.type == 'l')
      segments_count++;
    else if(current_point.type == 'r')
      segments_count--;
    else if(current_point.type == 'p')
      cnt[current_point.index] = segments_count;
  }

  return cnt;
}




vector<int> naive_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < starts.size(); j++) {
      cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
    }
  }
  return cnt;
}



int main()
{
  int n, m;
  std::cin >> n >> m;
  vector<int> starts(n), ends(n);
  for (size_t i = 0; i < starts.size(); i++) {
    std::cin >> starts[i] >> ends[i];
  }
  vector<int> points(m);
  for (size_t i = 0; i < points.size(); i++) {
    std::cin >> points[i];
  }
  //use fast_count_segments
  //vector<int> cnt = naive_count_segments(starts, ends, points);
  vector<int> cnt = fast_count_segments(starts, ends, points);
  for (size_t i = 0; i < cnt.size(); i++) {
    std::cout << cnt[i] << ' ';
  }
}
