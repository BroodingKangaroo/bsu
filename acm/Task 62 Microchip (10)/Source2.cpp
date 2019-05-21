#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <queue>
#include <stack>
#include <cmath>
#pragma comment(linker, "/STACK:106777216")
#define INF 1000000000
using namespace std;
ifstream f1("input.txt");
ofstream fout("output.txt");


vector<int> min_x(30, INF);
vector<int> min_y(30, INF);
vector<int> max_x(30, 0);
vector<int> max_y(30, 0);
vector<int> best_endings;
int count1 = 0;
int a, n;
int min_cost = INF;
string min_dir;

struct coord {
	bool operator<(const coord& a) {
		return this->x < a.x;
	}

	bool operator==(const coord& a) {
		return (this->x == a.x && this->y == a.y);
	}

	int x;
	int y;
};

struct state {
	int num;
	int max_y_right = 0;
	int min_y_right = INF;
	int min_y_up = INF;
	int max_y_down = 0;
	string dir;
	int cost;

	state(int k, int max_right, int min_right, int min_up, int max_down, int cost, string dir) : num(k),
		max_y_right(max_right),
		min_y_right(min_right),
		min_y_up(min_up),
		max_y_down(max_down),
		cost(cost), dir(dir) {}

	bool operator<(state a) {
		return this->num < a.num;
	}
};
struct cmpStatesByCost {
	bool operator()(const state& a, const state& b) const {

/*		if ((a.cost - precalc[a.num])/a.num != (b.cost - precalc[b.num])/b.num) {
			return (a.cost - precalc[a.num])/a.num > (b.cost - precalc[b.num])/b.num;
		}*/
		if (a.num != b.num) {
			return a.num < b.num;
		}
		//else if (a.cost != b.cost)return a.cost > b.cost;
		else if (a.min_y_right != b.min_y_right) return a.min_y_right < b.min_y_right;
		else if (a.max_y_right != b.max_y_right) {
			return a.max_y_right < b.max_y_right;
		}
		else if (a.max_y_down != b.max_y_down) {
			return a.max_y_down < b.max_y_down;
		}
		else {
			return a.min_y_up < b.min_y_up;
		}
	}
};


void get_min_cost(state cur_state);

int max_size = 0;

void take_states_from_the_queue();

void get_the_best_endings();
bool general_flag = 0;
vector<coord> coords;
void get_answer();
int main() {
	f1 >> a >> n;
	best_endings.resize(n + 1);
	coords.resize(n);
	for (int i = 0; i < n; i++) {
		f1 >> coords[i].x >> coords[i].y;
	}
	for (int i = 1; i < a; i++) {
		for (int j = 0; j < n; j++) {
			if (coords[j].x == i)
				min_y[i] = min(min_y[i], coords[j].y);
			if (coords[j].y == i)
				min_x[i] = min(min_x[i], coords[j].x);
			if (coords[j].x == i)
				max_y[i] = max(max_y[i], coords[j].y);
			if (coords[j].y == i)
				max_x[i] = max(max_x[i], coords[j].x);
		}
	}

	for (int i = 0; i < a; i++) {
		if (min_x[i] == INF)min_x[i] = 0;
		if (min_y[i] == INF)min_y[i] = 0;
		if (max_x[i] == INF)max_x[i] = 0;
		if (max_y[i] == INF)max_y[i] = 0;
	}

	auto temp_coords = coords;
	sort(coords.begin(), coords.end());
	clock_t start = clock();

	get_the_best_endings();
	//get_min_cost(state(0, 0, INF, INF, 0, 0, ""));
	get_answer();
	cout << min_cost << "\n";
	//get_answer();
	fout << min_cost << endl;
	for (auto q : temp_coords) {
		for (int i = 0; i < n; i++) {
			if (q == coords[i]) {
				int d = min_dir[i] - '0';
				if (d == 1) {
					fout << "RIGHT\n";
				}
				else if (d == 2) {
					fout << "UP\n";
				}
				else if (d == 3) {
					fout << "LEFT\n";
				}
				else if (d == 4) {
					fout << "DOWN\n";
				}
			}
		}
	}

	clock_t stop = clock();
	double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("\nTime elapsed: %.5f\n", elapsed);
	cout << "recursion: " << count1 << "\n";
	return 0;
}

int check_bad_state(int cur_cost, int k) {
	while (k < n) {
		int mmin = INF;
		auto cur = coords[k++];
		//right
		if (max_x[cur.y] == cur.x) {
			mmin = min(mmin, a - cur.x);
		}
		//left
		if (min_x[cur.y] == cur.x) {
			mmin = min(mmin, cur.x);
		}
		//up
		if (max_y[cur.x] == cur.y) {
			mmin = min(mmin, a - cur.y);
		}
		//down
		if (min_y[cur.x] == cur.y) {
			mmin = min(mmin, cur.y);
		}
		cur_cost += mmin;
	}
	return cur_cost;
}
void get_the_best_endings() {
	for (int i = 0; i < n; i++) {
		best_endings[i] = check_bad_state(0, i);
		cout << best_endings[i] << " ";
	}
};
priority_queue<state, vector<state>, cmpStatesByCost> myq;
void get_answer() {
	myq.push(state(0, 0, INF, INF, 0, 0, ""));
	while (!myq.empty()) {
		auto t = myq.top();
		if (t.num == n && general_flag==0) {
			min_cost = t.cost;
			min_dir = t.dir;
			general_flag = 1;
			break;
		}
		cout << myq.size() << " " << t.num << " " << count1 << "\n";
		if (count1 == 15) {
			cout << "qwe";
		}
		myq.pop();
		if (t.num<n&& t.cost + best_endings[t.num+1] < min_cost)
			get_min_cost(t);
	}
}
void get_min_cost(state cur_state) {
	count1++;

	int is_near_border = 0;
	int k = cur_state.num;
	int cost = cur_state.cost;
	if (k == n) {
		if (cost < min_cost) {
			min_cost = cost;
			min_dir = cur_state.dir;
		}
		return;
	}

	coord cur = coords[k];
	//check is_near_border
	if (a - cur.x <= 1 && max_x[cur.y] == cur.x) {
		is_near_border = 1;
	}
	else if (cur.x <= 1 && min_x[cur.y] == cur.x) {
		is_near_border = 3;
	}
	else if (a - cur.y <= 1 && max_y[cur.x] == cur.y) {
		is_near_border = 2;
	}
	else if (cur.y <= 1 && min_y[cur.x] == cur.y) {
		is_near_border = 4;
	}

	//right
	if (is_near_border == 0 || is_near_border == 1)
		if (max_x[cur.y] == cur.x && cost + a - cur.x + best_endings[k] < min_cost) {
			myq.push(state(k + 1, max(cur_state.max_y_right, cur.y), min(cur_state.min_y_right, cur.y),
				cur_state.min_y_up,
				cur_state.max_y_down, cost + a - cur.x, cur_state.dir + "1"));
		}
	//left
	if (is_near_border == 0 || is_near_border == 3)
		if (min_x[cur.y] == cur.x && cur.y < cur_state.min_y_up && cur.y > cur_state.max_y_down &&
			cost + cur.x + best_endings[k] < min_cost) {
			myq.push(state(k + 1, cur_state.max_y_right, cur_state.min_y_right, cur_state.min_y_up,
				cur_state.max_y_down, cost + cur.x, cur_state.dir + "3"));
		}
	//up
	if (is_near_border == 0 || is_near_border == 2)
		if (max_y[cur.x] == cur.y && cur.y > cur_state.max_y_right &&
			cost + a - cur.y + best_endings[k] < min_cost) {
			myq.push(state(k + 1, cur_state.max_y_right, cur_state.min_y_right, min(cur_state.min_y_up, cur.y),
				cur_state.max_y_down, cost + a - cur.y, cur_state.dir + "2"));
		}
	//down
	if (is_near_border == 0 || is_near_border == 4)
		if (min_y[cur.x] == cur.y && cur.y < cur_state.min_y_right && cost + cur.y + best_endings[k] < min_cost) {
			myq.push(state(k + 1, cur_state.max_y_right, cur_state.min_y_right, cur_state.min_y_up,
				max(cur_state.max_y_down, cur.y), cost + cur.y, cur_state.dir + "4"));
		}
	//while (!myq.empty()) {
	//	auto t = myq.top();
	//	myq.pop();
	//	if (t.cost + best_endings[t.num] < min_cost)
	//		get_min_cost(t);
	//}
}