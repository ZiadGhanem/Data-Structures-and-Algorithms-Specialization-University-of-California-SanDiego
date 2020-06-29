#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <cfloat>

using namespace std;

typedef vector<vector<double>> matrix;

// Convert  each  inequality  in  the  set  of  constraints  to  an  equation  by  adding  slack  variables.
matrix create_slack_matrix(int n, int m, matrix& A, vector<double>&b, vector<double>&c)
{
  matrix slack_matrix(n + 1);

  // Add objective function row
  // First column for constant
  slack_matrix[0].push_back(0);
  // Next m columns for non basic variables
  for(int column = 0; column < m; column++)
    slack_matrix[0].push_back(c[column]);
  // Next n columns for basic variables initialized to zero
  for(int column = 0; column < n; column++)
    slack_matrix[0].push_back(0);

  // Add constraint equations rows
  for(int row = 1; row < n + 1; row ++)
  {
    // First column for constant
    slack_matrix[row].push_back(b[row - 1]);
    // Next m columns for non basic variables
    for(int column = 0; column < m; column ++)
      slack_matrix[row].push_back(- A[row - 1][column]);
    // Next n columns for basic variables initialized to zero
    for(int column = 0; column < n; column++)
      slack_matrix[row].push_back(0);
  }

  return slack_matrix;
}

bool get_positive_coefficient(matrix& slack_matrix, int* positive_coefficient_index)
{
  *positive_coefficient_index = -1;
  double biggest_coefficient = 0;
  // Loop over all coefficients of variables in objective function and get most positive coefficient
  // Start from index 1 as index 0 is the constant
  for(int i = 1; i < slack_matrix[0].size(); i++)
  {
    // If the current coefficient is a positive coefficient set its index
    if(slack_matrix[0][i] > biggest_coefficient)
    {
      biggest_coefficient = slack_matrix[0][i];
      *positive_coefficient_index = i;
    }
  }

  // If there was no positive coefficient return false, else return true
  return (*positive_coefficient_index == -1)? false : true;
}


int get_tightest_constraint(matrix& slack_matrix, int positive_coefficient_column)
{
  double tighest_constraint = DBL_MAX;
  int tightest_constraint_row = -1;
  // Get the all possible constraints and return the minimum constraint
  for(int i = 1; i < slack_matrix.size(); i++)
  {
    if(slack_matrix[i][positive_coefficient_column] != 0)
    {
      double current_constraint = - slack_matrix[i][0] / slack_matrix[i][positive_coefficient_column];
      if((current_constraint > 0) && (tighest_constraint > current_constraint))
      {
        tighest_constraint = current_constraint;
        tightest_constraint_row = i;
      }
    }
  }

  return tightest_constraint_row;
}


pair<int, vector<double>> allocate_ads(int n, int m, matrix A, vector<double> b, vector<double> c)
{
  // Create the initial simplex tableau.
  matrix slack_matrix = create_slack_matrix(n, m, A, b, c);

  int positive_coefficient_column;

  // Set the initial non basic variable of each row
  unordered_map<int, int> basic_variable;
  for(int i = 1; i < n + 1; i++)
    basic_variable[i] = m + i;

  // Select a variable with the most positive coefficient in the objective function
  while(get_positive_coefficient(slack_matrix, &positive_coefficient_column))
  {

    cout << endl;
    for(int i = 0 ; i < slack_matrix.size(); i++)
    {
      for(int j = 0; j < slack_matrix[i].size(); j++)
        cout << slack_matrix[i][j] << " ";
      cout << endl;
    }
    cout << endl;

    // Increase the variable as much as possible without increasing any non negativity constraint
    // Find the tighest constraint
    int tightest_constraint_row = get_tightest_constraint(slack_matrix, positive_coefficient_column);

    // Unbounded solution
    if(tightest_constraint_row == -1)
      return {1, {}};

    // Get the initial coefficient of the non-basic variable
    double initial_value = slack_matrix[tightest_constraint_row][positive_coefficient_column];

    // Loop over all columns of the tighest constraint row
    for(int column = 0; column < n + m + 1; column++)
    {
      // If it is the used non basic variable then switch it with the basic variable
      // Else divide it by the the initial coefficient of the non-basic variable
      if(column == basic_variable[tightest_constraint_row])
        slack_matrix[tightest_constraint_row][basic_variable[tightest_constraint_row]] = 1.0 / initial_value;
      else if(column == positive_coefficient_column)
          slack_matrix[tightest_constraint_row][positive_coefficient_column] = 0;
      else
        slack_matrix[tightest_constraint_row][column] /= -initial_value;
    }


    // Replace the non basic variables in objective function and constraint inequalities except the tighest constraint inequality row
    for(int row = 0; row < n + 1; row++)
    {
      if(row != tightest_constraint_row)
      {
        initial_value = slack_matrix[row][positive_coefficient_column];
        for(int column = 0; column < n + m + 1; column++)
        {
          if(column != positive_coefficient_column)
            slack_matrix[row][column] += initial_value * slack_matrix[tightest_constraint_row][column];
          else
            slack_matrix[row][column] = 0;
        }
      }
    }

    // Switch the roles of basic and non-basic variables
    basic_variable[tightest_constraint_row] = positive_coefficient_column;
  }

  cout << endl;
  for(int i = 0 ; i < slack_matrix.size(); i++)
  {
    for(int j = 0; j < slack_matrix[i].size(); j++)
      cout << slack_matrix[i][j] << " ";
    cout << endl;
  }
  cout << endl;


  vector<double> solution(m, 0);
  for(auto it: basic_variable)
  {
    if(it.second <= m)
      solution[it.second - 1] = slack_matrix[it.first][0];
  }

  return make_pair(0, solution);

}

int main(){
  int n, m;
  cin >> n >> m;
  matrix A(n, vector<double>(m));
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

  pair<int, vector<double>> ans = allocate_ads(n, m, A, b, c);

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
