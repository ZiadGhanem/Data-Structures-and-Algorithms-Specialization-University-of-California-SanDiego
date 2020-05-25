#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;

struct Segment
{
  int start, end;
};
/*
vector<int> optimal_points(vector<Segment> &segments)
{
  vector<int> points;
  vector<Segment> overlap_segments;
  while(!segments.empty())
  {
    overlap_segments.clear();
    overlap_segments.push_back(segments[0]);
    segments.erase(begin(segments));
    int i = 0;
    while(i < segments.size())
    {
      if(!(((overlap_segments[0].start > segments[i].start) && (overlap_segments[0].start > segments[i].end)) || ((overlap_segments[0].end < segments[i].start) && (overlap_segments[0].end < segments[i].end))))
      {
        overlap_segments.push_back(segments[i]);
        segments.erase(begin(segments) + i);
      }
      else
      {
        i++;
      }
    }
    int max_point = -1;
    for(auto segment: overlap_segments)
    {
      if(segment.start > max_point)
      {
        max_point = segment.start;
      }
    }
    points.push_back(max_point);
  }

  //write your code here
  for (size_t i = 0; i < segments.size(); ++i)
  {
    points.push_back(segments[i].start);
    points.push_back(segments[i].end);
  }

  sort(begin(points), end(points));
  return points;
}
*/

vector<int> optimal_points(vector<Segment> &segments)
{
  vector<int> points;
  sort(begin(segments), end(segments), [](const Segment &a, const Segment & b)
  {
    return (a.end < b.end);
  });

  int point = segments[0].end;
  points.push_back(segments[0].end);
  for(int i =1; i < segments.size(); i++)
  {
    if(point < segments[i].start || point > segments[i].end)
    {
      points.push_back(segments[i].end);
      point = segments[i].end;
    }
  }
  return points;
}

int main() {
  int n;
  std::cin >> n;
  vector<Segment> segments(n);
  for (size_t i = 0; i < segments.size(); ++i) {
    std::cin >> segments[i].start >> segments[i].end;
  }
  vector<int> points = optimal_points(segments);
  std::cout << points.size() << "\n";
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << " ";
  }
}
