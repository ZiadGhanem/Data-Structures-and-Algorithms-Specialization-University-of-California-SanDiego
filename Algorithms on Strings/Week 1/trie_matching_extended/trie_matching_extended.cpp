/*
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int const Letters =    4;
int const NA      =   -1;

struct Node
{
	int next [Letters];
	bool patternEnd;

	Node ()
	{
		fill (next, next + Letters, NA);
		patternEnd = false;
	}
};

int letterToIndex (char letter)
{
	switch (letter)
	{
		case 'A': return 0; break;
		case 'C': return 1; break;
		case 'G': return 2; break;
		case 'T': return 3; break;
		default: assert (false); return -1;
	}
}

vector <int> solve (string text, int n, vector <string> patterns)
{
	vector <int> result;

	// write your code here

	return result;
}

int main (void)
{
	string t;
	cin >> text;

	int n;
	cin >> n;

	vector <string> patterns (n);
	for (int i = 0; i < n; i++)
	{
		cin >> patterns[i];
	}

	vector <int> ans;
	ans = solve (t, n, s);

	for (int i = 0; i < (int) ans.size (); i++)
	{
		cout << ans[i];
		if (i + 1 < (int) ans.size ())
		{
			cout << " ";
		}
		else
		{
			cout << endl;
		}
	}

	return 0;
}
*/

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace std;


struct Vertex
{
  unordered_map<char, Vertex*> children;
	bool end_pattern;
	Vertex()
	{
		end_pattern = false;
	}
};

Vertex* build_trie(const vector<string> & patterns)
{
  Vertex *root = new Vertex;

  Vertex* current_vertex = root;

  current_vertex = root;

  for(const string &pattern: patterns)
  {
    current_vertex = root;

    for(char character: pattern)
    {
      if(current_vertex->children.find(character) == end(current_vertex->children))
      {
        Vertex* new_vertex = new Vertex;
        current_vertex->children.insert(make_pair(character, new_vertex));
        current_vertex = new_vertex;
      }
      else
      {
        current_vertex = current_vertex->children[character];
      }
    }
		current_vertex->end_pattern = true;
  }
  return root;
}

vector <int> solve (const string& text, const vector <string>& patterns)
{
	vector <int> result;
	Vertex* patterns_trie = build_trie(patterns);
	Vertex* current_vertex = patterns_trie;
	for(int i = 0; i < text.length(); i++)
	{
		current_vertex = patterns_trie;

		for(int j = i; j < text.length(); j++)
		{
			if(current_vertex->children.find(text[j]) != end(current_vertex->children))
				current_vertex = current_vertex->children[text[j]];
			else
				break;

			if(current_vertex->end_pattern)
			{
				result.push_back(i);
				break;
			}
		}
	}

	return result;
}

string random_string(const int len) {

    string result="";
    static const char alphanum[] =
        "ACGT";

    for (int i = 0; i < len; ++i) {
        result+= alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return result;
}


vector<int> test(const string& text, const vector<string>& patterns)
{
    vector<int> pos;
    for (int i=0; i< text.size();i++){
        for (int j=0; j< patterns.size();j++){
            if ((patterns[j].size()+i-1)< text.size()){
                string sub=text.substr(i,patterns[j].size());
                if (sub.compare(patterns[j])==0){
                    pos.push_back(i);
                    break;
                }
            }
        }
    }
    sort(pos.begin(),pos.end());
    return pos;
}


int main (void)
{

	string text;
	cin >> text;

	int n;
	cin >> n;

	vector <string> patterns (n);
	for (int i = 0; i < n; i++)
	{
		cin >> patterns[i];
	}

	vector <int> ans;
	ans = solve (text, patterns);

	for (int i = 0; i < (int) ans.size (); i++)
	{
		cout << ans[i];
		if (i + 1 < (int) ans.size ())
		{
			cout << " ";
		}
		else
		{
			cout << endl;
		}
	}


	/*
	srand(time(0));

	while(true)
	{
		const string text = random_string((rand() % 1000) + 1);
		cout << "Text: " << text << endl;
		vector<string> patterns;
		cout << "Patterns:" << endl;
		int number_patterns = rand() % 100;
		for(int i = 0; i < number_patterns; i++)
		{
			string pattern = random_string((rand() % 100) + 1);
			patterns.push_back(pattern);
			cout << pattern << endl;
		}
		vector<int> brute_force_result = test(text, patterns);
		vector<int> result = solve(text, patterns);
		if(brute_force_result != result)
		{
			cout << "Brute Force result: ";
			for(int item: brute_force_result)
				cout << item << " ";
			cout << endl;

			cout << "Result: ";
			for(int item: result)
				cout << item << " ";
			cout << endl;

			cout << "Failed" << endl;
			break;
		}
		cout << endl << endl;
	}
	return 0;
	*/
}
