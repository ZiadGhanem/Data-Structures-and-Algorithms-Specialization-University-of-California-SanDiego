#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct Query {
    string type, name;
    int number;
};

vector<Query> read_queries() {
    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const vector<string>& result) {
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i] << "\n";
}

/*
vector<string> process_queries(const vector<Query>& queries) {
    vector<string> result;
    // Keep list of all existing (i.e. not deleted yet) contacts.
    vector<Query> contacts;
    for (size_t i = 0; i < queries.size(); ++i)
        if (queries[i].type == "add") {
            bool was_founded = false;
            // if we already have contact with such number,
            // we should rewrite contact's name
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    contacts[j].name = queries[i].name;
                    was_founded = true;
                    break;
                }
            // otherwise, just add it
            if (!was_founded)
                contacts.push_back(queries[i]);
        } else if (queries[i].type == "del") {
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    contacts.erase(contacts.begin() + j);
                    break;
                }
        } else {
            string response = "not found";
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    response = contacts[j].name;
                    break;
                }
            result.push_back(response);
        }
    return result;
}
*/

vector<string> process_queries(const vector<Query>& queries)
{
  unordered_map<int, string> hash_table;
  vector<string> result;

  for (size_t i = 0; i < queries.size(); ++i)
  {
    // If the query type was add
    if (queries[i].type == "add")
    {
      auto it = hash_table.find(queries[i].number);
      // Search in the hash table for the number
      if(it == end(hash_table))
      {
        // If not found then add it to table
        hash_table.insert(make_pair(queries[i].number, queries[i].name));
      }
      else
      {
        // Change the contact's name to new name
        it->second = queries[i].name;
      }
    }
    else if (queries[i].type == "del")
    {
      auto it = hash_table.find(queries[i].number);
      // If number is available in hash tale then delete it
      if(it != end(hash_table))
      {
        hash_table.erase(it);
      }
    }
    else
    {
      string response = "not found";
      auto it = hash_table.find(queries[i].number);
      if(it != end(hash_table))
      {
        response = it->second;
      }
      result.push_back(response);
    }
  }

  return result;
}

int main() {
    write_responses(process_queries(read_queries()));
    return 0;
}
