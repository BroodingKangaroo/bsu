//#include <iostream>
//#include <vector>
//#include <queue>
//#include <fstream>
//#include <algorithm>
//#include <functional>
//#include <iterator>
//#include <map>
//#define INF 1234567891
//#define LL long long
//using namespace std;
//ifstream fin("input.txt");
//ofstream fout("output.txt");
//int n, m, temp;
//LL F[651][651];
//LL Cap[651][651];
//LL Cost[651][651];
//struct Edge
//{
//	Edge(int from1, int to1, int flow1, int capacity1, LL cost1) :from(from1), to(to1), flow(flow1), capacity(capacity1), cost(cost1) {}
//	bool operator <(Edge a) {
//		return this->from < a.from;
//	}
//	int from;
//	int to;
//	int flow = 0;
//	int capacity;
//	LL cost;
//};
//
//vector<Edge>Network;
//vector<bool>used;
//vector<int>phi;
//vector<vector<pair<LL, int>>>vvv;
//vector<int> Dijkstra() {
//
//	int N = n + m + 2;
//	int start = 0, finish = n + m + 1; // номера истока и стока
//
//	fill(used.begin(), used.end(), false);
//	vector<LL>d(N, INF);
//	d[0] = 0;
//	priority_queue <pair<LL, LL>, vector<pair<LL, LL>>, greater<pair<int, int>>>q;
//
//	vector<int>pred(n + m + 2, -1);
//	q.push(make_pair(0, 0));
//	while (!q.empty()) {
//		auto from = q.top(); used[from.second] = 1;
//		q.pop();
//		if (from.first > d[from.second])continue;
//		for (auto to : vvv[from.second]) {
//			if (!used[to.second] && d[to.second] > d[from.second] + to.first + phi[from.second] - phi[to.second]) {
//				if (F[from.second][to.second] < Cap[from.second][to.second]) {
//					d[to.second] = d[from.second] + to.first + phi[from.second] - phi[to.second];
//					pred[to.second] = from.second;
//					q.push(make_pair(d[to.second], to.second));
//				}
//			}
//		}
//	}
//	for (int i = 0; i < N; i++) {
//		if (used[i])phi[i] += d[i];
//		else phi[i] += d[finish];
//	}
//	vector<int>path;
//	int cur = finish;
//	path.push_back(finish);
//	while (pred[cur] != -1) {
//		cur = pred[cur];
//		path.push_back(cur);
//	}
//	reverse(path.begin(), path.end());
//	return path;
//
//}
//
//int main() {
//	fin >> n >> m;
//	used.resize(n + m + 2);
//	phi.resize(n + m + 2);
//	vector<int>pr(n);
//	for (int i = 0; i < n; i++) {
//		fin >> pr[i];
//	}
//	vector<int>per(m);
//	for (int i = 0; i < m; i++) {
//		fin >> per[i];
//	}
//	int N = n + m + 2;
//	for (int i = 1; i <= n; i++) {
//		for (int j = 1; j <= m; j++) {
//			fin >> temp;
//			Cap[i][n + j] = INF;
//			Cost[i][n + j] = temp;
//			F[i][n + j] = 0;
//
//			Cost[n + j][i] = -temp;
//			Cap[n + j][i] = 0;
//			F[n + j][i] = 0;
//
//			Network.emplace_back(Edge(i, n + j, 0, INF, temp));
//			Network.emplace_back(Edge(n + j, i, 0, 0, -temp));
//		}
//	}
//	for (int i = 1; i <= n; i++) {
//		Cap[0][i] = pr[i - 1];
//		Cost[0][i] = 0;
//		F[0][i] = 0;
//
//		Cap[i][0] = pr[i - 1];
//		Cost[i][0] = 0;
//		F[i][0] = 0;
//		Network.emplace_back(Edge(0, i, 0, pr[i - 1], 0));
//		Network.emplace_back(Edge(i, 0, 0, pr[i - 1], 0));
//	}
//	for (int i = 1; i <= m; i++) {
//		Cap[i + n][n + m + 1] = per[i - 1];
//		Cost[i + n][n + m + 1] = 0;
//		F[i + n][n + m + 1] = 0;
//
//		Cap[n + m + 1][i + n] = per[i - 1];
//		Cost[n + m + 1][i + n] = 0;
//		F[n + m + 1][i + n] = 0;
//		Network.emplace_back(Edge(i + n, n + m + 1, 0, per[i - 1], 0));
//		Network.emplace_back(Edge(n + m + 1, i + n, 0, per[i - 1], 0));
//	}
//
//	vvv.resize(n + m + 2);
//	for (int i = 0; i < Network.size(); i++) {
//		vvv[Network[i].from].emplace_back(make_pair(Network[i].cost, Network[i].to));
//	}
//	while (1) {
//		vector<int>path = Dijkstra();
//		if (used[n + m + 1] == 0)break;
//
//		int mmin = INF;
//		for (int i = 0; i < path.size() - 1; i++) {
//			mmin = min(mmin, int(Cap[path[i]][path[i + 1]] - F[path[i]][path[i + 1]]));
//		}
//		for (int i = 0; i < path.size() - 1; i++) {
//			F[path[i]][path[i + 1]] += mmin;
//			F[path[i + 1]][path[i]] -= mmin;
//		}
//	}
//	LL result = 0;
//	for (int i = 0; i < 651; i++) {
//		for (int j = 0; j < 651; j++) {
//			if (Cost[i][j] > 0)
//				result += F[i][j] * Cost[i][j];
//		}
//	}
//	fout << result;
//
//	return 0;
//}
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
//ifstream fin("input.txt");
//ofstream fout("output.txt");
//int minX[30];
//int minY[30];
//int maxX[30];
//int maxY[30];
//struct coord {
//	coord() {
//
//	}
//	int x;
//	int y;
//};

int main() {
	//int a, n;
	//fin >> a >> n;
	//vector<coord> coords;

	//for (int i = 0; i < n; i++) {
	//	fin >> coords[i].x >> coords[i].y;
	//}
	//for (int i = 0; i < a; i++) {
	//	for (int j = 0; j < n; j++) {
	//		minX[i] = min(minX[i], coords[j].y);
	//		minY[i] = min(minY[i], coords[j].x);
	//		maxX[i] = max(maxX[i], coords[j].y);
	//		maxY[i] = max(maxY[i], coords[j].x);
	//	}
	//}

	//sort(coords.begin(), coords.end());



	return 0;
}