#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;
ifstream in("input.txt");
ofstream out("output.txt");
int main() {
	long long n, m, s;
	in >> n >> m >> s;
	long long N = n + m;
	vector<long long>all_coins;
	long long customer_money = 0;
	long long temp;
	for (int i = 0; i < n; i++) {
		in >> temp;
		all_coins.push_back(temp);
		customer_money += temp;
	}
	for (int i = 0; i < m; i++) {
		in >> temp;
		all_coins.push_back(temp);
	}
	if (customer_money < s) {
		out << "No";
		return 0;
	}
	if (customer_money == s) {
		out << "Yes";
		return 0;
	}
	long long S = customer_money - s;
	set<long long>good_numbers;
	good_numbers.insert(0);
	for (int i = 0; i < N; i++) {
		long long f = good_numbers.size();
		auto x = good_numbers.begin();
		set<long long>tmp;
		while (f--) {
			long long new_good = *(x)+all_coins[i];
			if (new_good == S) {
				out << "Yes";
				return 0;
			}
			x++;
			tmp.insert(new_good);
		}
		good_numbers.insert(tmp.begin(), tmp.end());
	}
	out << "No";
	return 0;
}