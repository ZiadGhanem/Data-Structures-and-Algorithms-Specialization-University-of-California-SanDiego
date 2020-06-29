#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cfloat>
#include <math.h>

using namespace std;

const double EPS = 1e-3;
const double INF = 1.0e+9;

typedef vector<double> Column;
typedef vector<double> Row;
typedef vector<Row> Matrix;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row):
        column(column),
        row(row)
    {}

    int column;
    int row;
};

Position SelectPivotElement(
  const Matrix &a,
  vector <bool> &used_rows,
  vector <bool> &used_columns) {
    // This algorithm selects the first free element.
    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;

    while(used_rows[pivot_element.column])
      ++pivot_element.column;


    // Select the largest value
    double max = 0;
    for(int row = pivot_element.row; row < a.size(); row++)
    {
      if(fabs(a[row][pivot_element.column]) > fabs(max))
      {
        max = a[row][pivot_element.column];
        pivot_element.row = row;
      }
    }

    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, vector <bool> &used_rows, Position &pivot_element) {
    swap(a[pivot_element.column], a[pivot_element.row]);
    swap(b[pivot_element.column], b[pivot_element.row]);
    swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Write your code here

    // Cache initial pivot value as it will be overwritter
    double pivot = a[pivot_element.row][pivot_element.row];
    // Divide the pivot row by the pivot value so pivot becomes one
    for(int i = 0; i < a[pivot_element.row].size(); i++)
      a[pivot_element.row][i] /= pivot;

    b[pivot_element.row] /= pivot;

    // Make the pivot column in each row except the pivot row equal to zero
    for(int i = 0; i < a.size(); i++)
    {
      if(i != pivot_element.row)
      {
        double multiplier = - a[i][pivot_element.column];
        for(int j = 0; j < a[i].size(); j++)
          a[i][j] += (a[pivot_element.row][j] * multiplier);

        b[i] += (b[pivot_element.row] * multiplier);
      }
    }
}

void MarkPivotElementUsed(const Position &pivot_element, vector <bool> &used_rows, vector <bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

Column GaussianElimination(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    vector <bool> used_columns(size, false);
    vector <bool> used_rows(size, false);
    for (int step = 0; step < size; ++step) {
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);
    }

    return b;
}

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

// Make all combinations of size k of numbers
// from 0 to n.
vector<vector<int> > makeCombi(int n, int k)
{
    vector<vector<int> > ans;
    vector<int> tmp;
    makeCombiUtil(ans, tmp, n - 1, 0, k);
    return ans;
}

bool check_solution(Matrix& A, Column& b, Column& current_solution)
{
  for(int i = 0; i < A.size(); i++)
  {
    double current_sum = 0;
    for(int j = 0; j < current_solution.size(); j++)
    {
      // Verify the number is valid
      if(!isfinite(current_solution[j]) || isnan(current_solution[j]))
        return false;

      current_sum += A[i][j] * current_solution[j];
    }

    // Verify that it satisfies the constraint
    if(current_sum - b[i] > EPS)
      return false;
  }

  return true;
}

pair<int, vector<double>> solve_diet_problem(
    int n,
    int m,
    Matrix A,
    Column b,
    Row c) {

  // Add variable >= 0 inequalities
  for(int i = 0; i < m; i++)
  {
    vector<double> temp(m, 0);
    // -1 because we want it <= instead of >=
    temp[i] = -1;
    A.push_back(temp);
    b.push_back(0);
  }

  // Add bouding inequality
  A.push_back(vector<double> (m, 1));
  b.push_back(INF);

  // Maximum Objective Function value
  double max_value =  -DBL_MAX;

  // If there is a solution
  bool solution_found = false;

  // Best solution found
  Column best_solution;

  // Make all combinations of which consist of m inequalities of the total n inequalities
  vector<vector<int>> subsets = makeCombi(n + m + 1, m);

  for(auto subset: subsets)
  {
    // Create equation of current combination
    Matrix current_a;
    Column current_b;
    for(auto inequality: subset)
    {
      current_a.push_back(A[inequality]);
      current_b.push_back(b[inequality]);
    }
    Equation current_equation(current_a, current_b);

    // Perform Gaussian Elimination on current combination
    Column current_solution = GaussianElimination(current_equation);

    // Verify that the solution satisfies the constraints
    if(check_solution(A, b, current_solution))
    {
      solution_found = true;

      // Check if the current combination improves the Objective Function value
      double current_value = 0;
      for(int i = 0; i < m; i++)
        current_value += c[i] * current_solution[i];

      if(current_value > max_value)
      {
        max_value = current_value;
        best_solution = current_solution;
      }
    }
  }

  // If no solution was found
  if(!solution_found)
    return {-1, {}};
  // If the solution was close to infinity
  double total_sum = 0;
  for(int i = 0; i < best_solution.size(); i++)
    total_sum += best_solution[i];
  if(total_sum + EPS >= INF)
    return {1, {}};

  // Return solution
  return {0, best_solution};
}

int main(){
  int n, m;
  cin >> n >> m;
  Matrix A(n, vector<double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
    }
  }
  vector<double> b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }
  vector<double> c(m);
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }

  pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);

  switch (ans.first) {
    case -1:
      printf("No solution\n");
      break;
    case 0:
      printf("Bounded solution\n");
      for (int i = 0; i < m; i++) {
        printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
      }
      break;
    case 1:
      printf("Infinity\n");
      break;
  }
  return 0;
}
