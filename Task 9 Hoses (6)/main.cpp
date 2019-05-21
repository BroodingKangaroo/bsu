#include <iostream>
#include <fstream>
#include <stack>

using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");

int main() {
    double n, cur;
    fin >> n;
    stack<int> a;
    for (int i = 0; i < n; i++) {
        fin >> cur >> cur >> cur;
        if (!a.empty() && cur == a.top())
            a.pop();
        else a.push(cur);
    }
    if (!a.empty())fout << "No";
    else fout << "Yes";
    return 0;
}