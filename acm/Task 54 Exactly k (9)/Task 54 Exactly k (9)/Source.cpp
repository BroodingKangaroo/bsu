#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");
vector<vector<int>>vvv;
vector<pair<int, int>>games;
vector<pair<int, int>>min_even_odd_length;
void bfs(int start) {
	// ------------------------------------------------------
	vector<bool>used_even(vvv.size());
	vector<bool>used_odd(vvv.size());
	fill(min_even_odd_length.begin(), min_even_odd_length.end(), pair<int, int>(INT_MAX, INT_MAX));
	queue<pair<int, int>>q; // first - number of vertex, second - length of this vertex
	// ------------------------------------------------------

	q.push(make_pair(start, 0));
	//min_even_odd_length[start].first = 0;
	//min_even_odd_length[start].second = 0;
	//used_odd[0] = true;
	while (!q.empty()) {
		int v = q.front().first;
		int length = q.front().second;
		q.pop();
		for (auto vv : vvv[v]) {
			if (used_even[vv] == false && (length + 1) % 2 == 0) {
				q.push(make_pair(vv, length + 1));
				used_even[vv] = true;
				if (min_even_odd_length[vv].first == INT_MAX)
					min_even_odd_length[vv].first = length + 1;
			}
			if (used_odd[vv] == false && (length + 1) % 2 == 1) {
				q.push(make_pair(vv, length + 1));
				used_odd[vv] = true;
				if (min_even_odd_length[vv].second == INT_MAX)
					min_even_odd_length[vv].second = length + 1;
			}
		}
	}
}


int main() {
	int n, m, q, from, to;
	fin >> n >> m;
	vvv.resize(n);
	min_even_odd_length.resize(n);
	for (int i = 0; i < m; i++) {
		fin >> from >> to;
		from--; to--;
		vvv[from].push_back(to);
		vvv[to].push_back(from);
	}
	fin >> q;
	games.resize(q);
	for (int i = 0; i < q; i++) {
		fin >> games[i].first >> games[i].second;
	}
	bfs(0);
	//for (int i = 0; i < n; i++) {
	//	cout << i << ": ";
	//	cout << "min even length: " << min_even_odd_length[i].first << " min odd length: " << min_even_odd_length[i].second << "\n";
	//}
	for (int i = 0; i < q; i++) {
		int to = games[i].first;
		int how_far = games[i].second;
		if (to == 1 && how_far == 0) {
			fout << "Yes\n";
			continue;
		}
		else if (how_far == 0 && to != 1) {
			fout << "No\n";
			continue;
		}
		if (how_far % 2 == 0 && min_even_odd_length[to - 1].first <= how_far) {
			fout << "Yes\n";
		}
		else if (how_far % 2 == 1 && min_even_odd_length[to - 1].second <= how_far) {
			fout << "Yes\n";
		}
		else {
			fout << "No\n";
		}
	}
	return 0;
}