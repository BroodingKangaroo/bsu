#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");

vector<int>DSU;

void tree_union(int root_1, int root_2) {
	if (abs(DSU[root_1]) <= abs(DSU[root_2])) {
		DSU[root_2] += DSU[root_1];
		DSU[root_2] = root_1;
	}
	else {
		DSU[root_1] += DSU[root_2];
		DSU[root_2] = root_1;
	}
}
int main() {
	int n, r, ans = 0;
	pair<int, int>pair_of_num;
	fin >> n >> r;
	DSU.resize(n + 1);
	fill(DSU.begin(), DSU.end(), -1);

	int tmp1, tmp2;
	
	while (r--) {
		fin >> pair_of_num.first >> pair_of_num.second;

		tmp1 = pair_of_num.first;
		
		while (DSU[tmp1] > 0) {
			tmp1 = DSU[tmp1];
		}

		tmp2 = pair_of_num.second;

		while (DSU[tmp2] > 0) {
			tmp2 = DSU[tmp2];
		}
		if (tmp1 == tmp2)
			ans++;
		else {
			tree_union(tmp1, tmp2);
		}
	}
	fout << ans;
	return 0;
}