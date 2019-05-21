#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>
#include <set>
#include <ctime>
#include <map>

using namespace std;
int n, m, k, t;
struct coord {
    coord(int i, int j) : i(i), j(j) {}

    int i;
    int j;
};

inline void add_neighbors(int cur_ind);

void print_matrix();

int** colors;
vector<vector<int>> ids;
vector<vector<coord>> IDS;
vector<int> moves;
vector<stack<int>> sosed;
int final_color;
vector<bool>used_ids;
vector<bool>id_which_we_shouldnt_add;
void gen_test();

FILE *outFile;

inline void set_id_recur(int i, int j, int cur_id) {
    ids[i][j] = cur_id;
    IDS[ids[i][j]].push_back(coord(i, j));
    if (i + 1 < n && colors[i + 1][j] == colors[i][j] && ids[i + 1][j] == 0) {
        set_id_recur(i + 1, j, cur_id);
    }
    if (i - 1 >= 0 && colors[i - 1][j] == colors[i][j] && ids[i - 1][j] == 0) {
        set_id_recur(i - 1, j, cur_id);
    }
    if (j + 1 < m && colors[i][j + 1] == colors[i][j] && ids[i][j + 1] == 0) {
        set_id_recur(i, j + 1, cur_id);
    }
    if (j - 1 >= 0 && colors[i][j - 1] == colors[i][j] && ids[i][j - 1] == 0) {
        set_id_recur(i, j - 1, cur_id);
    }
}
inline void add_mas(){
    colors = new int*[n];
    for (int i=0;i<n;i++){
        colors[i]=new int[m];
    }

}

int main() {
    FILE *inFile;
    outFile = fopen("A:\\OneDrive\\Algorithms\\Task 30 Flood It! (10)\\floodit.out", "w");
    inFile = fopen("A:\\OneDrive\\Algorithms\\Task 30 Flood It! (10)\\floodit.in", "r");
    gen_test();
    clock_t start = clock();
    fscanf(inFile, "%d%d%d%d", &n, &m, &k, &t);
    add_mas();
    ids.resize(n, vector<int>(m));
    IDS.resize(n * m + 1);
    moves.resize(t);
    sosed.resize(k+1);
    used_ids.resize(n*m+1);
    id_which_we_shouldnt_add.resize(n*m+1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(inFile, "%d", &colors[i][j]);
        }
    }

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nReading: %.5f\n", elapsed);

    int cur_id = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (ids[i][j] == 0) {
                set_id_recur(i, j, ++cur_id);
            }
        }
    }


    for (auto &q:moves)
        fscanf(inFile, "%d", &q);
    final_color = moves[t - 1];

    stop = clock();
    elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nCount ids: %.5f\n", elapsed);


    used_ids[ids[0][0]] = 1;
    id_which_we_shouldnt_add[ids[0][0]] = 1;
    add_neighbors(1);
    int ids_count = IDS.size();
    int count_of_used = 1;
    bool is_all_area_used = false;
    if (count_of_used >= ids_count) {
        is_all_area_used = true;
    }
    bool flag = 0;
    for (int i = 0; i < moves.size() && is_all_area_used == 0; i++) {
        while (!sosed[moves[i]].empty()) {
            int cur = sosed[moves[i]].top();
            sosed[moves[i]].pop();
            used_ids[cur] = true;
            add_neighbors(cur);
            count_of_used++;
            if (count_of_used >= ids_count) {
                is_all_area_used = 1;
                break;
            }
        }
    }

    print_matrix();

    stop = clock();
    elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    return 0;
}


inline void add_neighbors(int cur_ind) {
    for (int i = 0; i < IDS[cur_ind].size(); i++) {
        int cur_i = IDS[cur_ind][i].i;
        int cur_j = IDS[cur_ind][i].j;
        if (cur_i + 1 < n && !used_ids[ids[cur_i + 1][cur_j]] && !id_which_we_shouldnt_add[ids[cur_i + 1][cur_j]]) {
            id_which_we_shouldnt_add[ids[cur_i + 1][cur_j]] = 1;
            sosed[colors[cur_i + 1][cur_j]].push(ids[cur_i + 1][cur_j]);
        }
        if (cur_i - 1 >= 0 && !used_ids[ids[cur_i - 1][cur_j]] && !id_which_we_shouldnt_add[ids[cur_i - 1][cur_j]]) {
            id_which_we_shouldnt_add[ids[cur_i - 1][cur_j]] = 1;
            sosed[colors[cur_i - 1][cur_j]].push(ids[cur_i - 1][cur_j]);
        }
        if (cur_j + 1 < m && !used_ids[ids[cur_i][cur_j + 1]] && !id_which_we_shouldnt_add[ids[cur_i][cur_j + 1]]) {
            id_which_we_shouldnt_add[ids[cur_i][cur_j + 1]] = 1;
            sosed[colors[cur_i][cur_j + 1]].push(ids[cur_i][cur_j + 1]);
        }
        if (cur_j - 1 >= 0 && !used_ids[ids[cur_i][cur_j - 1]] && !id_which_we_shouldnt_add[ids[cur_i][cur_j - 1]]) {
            id_which_we_shouldnt_add[ids[cur_i][cur_j - 1]] = 1;
            sosed[colors[cur_i][cur_j - 1]].push(ids[cur_i][cur_j - 1]);
        }
    }
}

void gen_test() {
    ofstream f1("A:\\OneDrive\\Algorithms\\Task 30 Flood It! (10)\\floodit.in");
    srand(time(0));
    int n = 1000, m = 1000, k = 150, t = 100000;
    f1 << n << " " << m << " " << k << " " << t << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            f1 << rand() % k + 1 << " ";
        }
        f1 << "\n";
    }
    for (int i = 0; i < t; i++) {
        f1 << rand() % k + 1 << " ";
    }
    f1.close();
}

void print_matrix() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (used_ids[ids[i][j]] == 0)
                fprintf(outFile, "%d%c", colors[i][j], ' ');
            else
                fprintf(outFile, "%d%c", final_color, ' ');
        }
        fprintf(outFile, "%s", "\n");
    }
}