#include <iostream>
#include <vector>
#include <sstream>

#define NUM_SQUARES 25
#define SQUARES_PER_LINE  5

using namespace std;

bool solve_puzzle(
  vector<vector<string>>& squares,
  vector<bool>& visited,
  vector<int>& path,
  int current_square,
  int visited_squares
)
{
  // Set the current square as visited
  visited[current_square] = true;
  // Add the current square to the path
  path.push_back(current_square);

  if(visited_squares >= NUM_SQUARES)
    return true;

  // Loop through all unvisited squares
  for(int i = 0; i < NUM_SQUARES; i++)
  {
    if(!visited[i])
    {
      // We are going downwards
      if(visited_squares % SQUARES_PER_LINE == 0)
      {
        // If the bottom of the current sqaure equals the top of the next square
        if((squares[current_square][2] == squares[i][0]) &&
          solve_puzzle(squares, visited, path, i, visited_squares + 1))
          return true;
      }
      // We are going left
      else if((visited_squares / SQUARES_PER_LINE) % 2)
      {
        // If the left of the current sqaure equals the right of the next square
        // and the top of the square equals the bottom of above sqaure
        if((squares[current_square][1] == squares[i][3]) && (squares[i][0] == squares[path[((visited_squares / SQUARES_PER_LINE) * SQUARES_PER_LINE) - (visited_squares % SQUARES_PER_LINE) - 1]][2]) &&
          solve_puzzle(squares, visited, path, i, visited_squares + 1))
          return true;
      }
      // We are going right
      else
      {
        // For first line
        if(visited_squares <= SQUARES_PER_LINE)
        {
          // If the right of the current sqaure equals the left of the next square
          if((squares[current_square][3] == squares[i][1]) &&
            solve_puzzle(squares, visited, path, i, visited_squares + 1))
            return true;
        }
        else
        {
          // If the right of the current sqaure equals the left of the next square
          // and the top of the square equals the bottom of above sqaure
          if((squares[current_square][3] == squares[i][1]) && (squares[i][0] == squares[path[((visited_squares / SQUARES_PER_LINE) * SQUARES_PER_LINE) - (visited_squares % SQUARES_PER_LINE) - 1]][2]) &&
            solve_puzzle(squares, visited, path, i, visited_squares + 1))
            return true;
        }
      }
    }
  }

  // Set the current square as non visited
  visited[current_square] = false;
  // Remove the current square to the path
  path.pop_back();

  return false;
}

int find_top_left_square(vector<vector<string>>& squares)
{
  for(int i = 0; i < squares.size(); i++)
  {
    if(squares[i][0] == "black" && squares[i][1] == "black")
      return i;
  }
}

void print_puzzle(vector<vector<string>>& squares, vector<int>& path)
{
  for(int i = 0; i < NUM_SQUARES; i++)
  {
    cout << "(";
    for(int j = 0; j < 4; j++)
    {
      if((i / SQUARES_PER_LINE) % 2)
        cout << squares[path[(((i / SQUARES_PER_LINE) + 1) * SQUARES_PER_LINE) - (i % SQUARES_PER_LINE) - 1]][j];
      else
        cout << squares[path[i]][j];
      if(j < 3)
        cout << ",";
    }
    cout << ")";

    if((i + 1) % SQUARES_PER_LINE)
      cout << ";";
    else
      cout << endl;
  }
}

int main(void)
{
  vector<vector<string>> squares(NUM_SQUARES);

  // Create the squares vector
  string current_line;
  for(int i = 0; i < NUM_SQUARES; i++)
  {
    // Read the input
    cin >> current_line;
    // Remove first parentheses
    current_line.erase(0, 1);
    // Remove last parentheses
    current_line.pop_back();
    // Split string at commas and add them to current square
    stringstream ss(current_line);
    string token;
    while (getline(ss,token, ','))
      squares[i].push_back(token);
  }

  vector<bool> visited(NUM_SQUARES, false);
  vector<int> path;

  int start_square = find_top_left_square(squares);
  solve_puzzle(squares, visited, path, start_square, 1);
  print_puzzle(squares, path);

  return 0;
}
