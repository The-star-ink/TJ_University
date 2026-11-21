#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  if (!(cin >> T))
    return 0;

  while (T--) {
    int n, m;
    cin >> n >> m;

    vector<int> par(n + 1, 0);
    vector<vector<int>> ch(n + 1);
    for (int i = 1; i < n; i++) {
      int a, b;
      cin >> a >> b;
      par[b] = a; // a 是 b 的父亲
      ch[a].push_back(b);
    }

    int root = 1; // 没当过孩子的点就是根
    for (int v = 1; v <= n; v++)
      if (par[v] == 0) {
        root = v;
        break;
      }

    vector<int> dep(n + 1, 0), stk{root};
    while (!stk.empty()) {
      int u = stk.back();
      stk.pop_back();
      for (int v : ch[u]) {
        dep[v] = dep[u] + 1;
        stk.push_back(v);
      }
    }

    int LOG = 1;
    while ((1 << LOG) <= n)
      LOG++;
    vector<vector<int>> up(LOG, vector<int>(n + 1, 0));
    for (int v = 1; v <= n; v++)
      up[0][v] = par[v];
    for (int k = 1; k < LOG; k++)
      for (int v = 1; v <= n; v++)
        up[k][v] = up[k - 1][up[k - 1][v]];

    auto lca = [&](int x, int y) {
      if (dep[x] < dep[y])
        swap(x, y);
      int d = dep[x] - dep[y];
      for (int k = 0; k < LOG; k++)
        if (d >> k & 1)
          x = up[k][x];
      if (x == y)
        return x; // 一个是另一个的祖先
      for (int k = LOG - 1; k >= 0; k--)
        if (up[k][x] != up[k][y]) {
          x = up[k][x];
          y = up[k][y];
        }
      return up[0][x];
    };

    while (m--) {
      int x, y;
      cin >> x >> y;
      cout << lca(x, y) << '\n';
    }
  }
  return 0;
}
