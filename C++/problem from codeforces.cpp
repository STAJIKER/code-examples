#pragma comment(linker, "/STACK:64000000")
#include <iostream>
#include <vector>
using namespace std;
#define pb push_back
long long n, k, i, a, b, d[50001][501], ans = 0;
vector <long long> g[50001];
bool used[50001];
void dfs(int v, int p = -1)
{
	used[v] = true;
	d[v][0] = 1;
	for (int i = 0; i<g[v].size(); ++i)
	{
		int to = g[v][i];
		if (!used[to])
		{
			dfs(to, v);
			for (int j = 1; j <= k; ++j)
				d[v][j] += d[to][j - 1];
		}
	}
	ans += 2 * d[v][k];
	for (int i = 0; i<g[v].size(); ++i)
	{
		int to = g[v][i];
		if (to != p)
			for (int j = 1; j<k; ++j)
				ans += d[to][j - 1] * (d[v][k - j] - d[to][k - j - 1]);
	}
}
int main()
{
	cin >> n >> k;
	for (i = 1; i<n; ++i)
	{
		cin >> a >> b;
		g[a - 1].pb(b - 1);
		g[b - 1].pb(a - 1);
	}
	dfs(0);
	cout << ans / 2;
	return 0;
}