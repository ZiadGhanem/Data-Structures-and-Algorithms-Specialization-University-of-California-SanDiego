#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class Solver
{
	string s;
	public:
		Solver(string s) : s(s)
		{
			hashes.resize(s.length());
			hashes[0] = x * (s[0] - 'a' + 1);
			for(int i = 1; i < s.length(); i++)
				hashes[i] = ((x * hashes[i - 1]) + (s[i] - 'a' + 1)) % m1;
		}

		bool ask(int a, int b, int l)
		{
			if(a == b)
				return true;

			unsigned long long y = 1;
		  for(int i = 0; i < l; i++)
		    y = (y * x) % m1;

			unsigned long long value_1 = (hashes[a + l] - (y * hashes[a]));
			unsigned long long value_2 = (hashes[b + l] - (y * hashes[b]));
			return ((value_1 % m1 == value_2 % m1) && (value_1 % m2 == value_2 % m2));
		}

	private:
		unsigned long long m1 = 1000000007,
		m2 = 1000000009,
		x = 27;

		vector<unsigned long long> hashes;
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0);

	string s;
	int q;
	cin >> s >> q;
	Solver solver(s);
	for (int i = 0; i < q; i++) {
		int a, b, l;
		cin >> a >> b >> l;
		cout << (solver.ask(a, b, l) ? "Yes\n" : "No\n");
	}
}
