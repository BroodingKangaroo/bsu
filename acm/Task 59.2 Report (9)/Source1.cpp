#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <ctime>
using namespace std;


ifstream f1("report.in");
ofstream f2("report.out");

vector<int>get_answer(vector<int>a)
{
	int n = a.size();
	vector<int>answer(n, -1);
	vector<int> d(n + 1);
	d[0] = -INT_MAX;
	for (int i = 1; i <= n; ++i)
		d[i] = INT_MAX;
	for (int i = 0; i < n; i++)
	{
		int j = int(upper_bound(d.begin(), d.end(), a[i]) - d.begin());
		if (a[i] == d[j-1])
		{
			answer[i] = j - 1;
		}
		if (d[j - 1] < a[i] && a[i] < d[j]) {
			d[j] = a[i];
			answer[i] = j;
		}

	}
	return answer;
}
//ofstream f3("report.in");
int main() {

	//srand(time(0));
	//f3 << 10<<"\n";
	//for (int i=0;i<10;i++)
	//{
	//	f3 << rand() % 20 << "\n";
	//}
	//f3.close();
	int n;
	f1 >> n;

	vector<int>h(n);
	for (int i = 0; i < n; i++) {
		f1 >> h[i];
	}
	vector<int> inc = get_answer(h);

	reverse(h.begin(), h.end());
	//for (auto q : inc)
	//{
	//	cout << q << " ";
	//}
	//cout << "\n";
	vector<int> dec = get_answer(h);

	reverse(dec.begin(), dec.end());
	reverse(h.begin(), h.end());

	//for (auto q : dec)
	//{
	//	cout << q << " ";
	//}
	int max = -1;
	int num = -1;
	int num1 = -1;
	int len = -1;
	for (int i = 0; i < n; i++)
	{
		if (len < min(inc[i], dec[i]))
		{
			max = i;
			len = min(inc[i], dec[i]);
			num = inc[i];
			num1 = dec[i];
		}
	}
	//cout << "max:" << max << "\n";
	int temp = len, prev = -1;
	vector<int>answer;
	for (int i = max; i >= 0, num > 0, len > 0; --i) {
		if (i < 0)break;
		if (inc[i] == num && h[i] != prev) {
			if (answer.empty())
			{
				answer.push_back(i + 1);
				num--;
				len--;
				prev = h[i];
			}
			else if (h[answer[answer.size() - 1] - 1] > h[i]) {
				answer.push_back(i + 1);
				num--;
				len--;
				prev = h[i];
			}
		}
	}
	prev = -1;
	len = temp;
	reverse(answer.begin(), answer.end());
	num1--;
	len--;
	for (int i = max + 1; i < dec.size() && len>0; i++)
	{
		if (dec[i] == num1 && h[i] != prev)
		{
			if (i != max + 1 && h[answer[answer.size() - 1] - 1] > h[i]) {
				answer.push_back(i + 1);
				num1--;
				len--;
				prev = h[i];
			}
			else if (i == max + 1)
			{
				answer.push_back(i + 1);
				num1--;
				len--;
				prev = h[i];
			}
		}
	}

	{
		if (answer.size() <= 0)
		{
			f2 << 0 << "\n" << h[0];
		}
		else
		{
			f2 << answer.size() / 2 << "\n";
			for (auto q : answer)
			{
				f2 << q << " ";
			}
		}
	}
	return 0;
}