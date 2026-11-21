#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  int t = 0;
  while (getline(cin, line)) {
    while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
      line.pop_back();
    if (!line.empty() && line[0] == '#')
      break;
    if (line.empty())
      continue;

    ++t;
    vector<int> lc(1, -1), rs(1, -1), lastCh(1, -1);
    vector<int> stk;
    int cur = 0, dep = 0, h1 = 0;

    for (char c : line) {
      if (c == 'd') {
        int v = (int)lc.size();
        lc.push_back(-1);
        rs.push_back(-1);
        lastCh.push_back(-1);

        if (lc[cur] == -1)
          lc[cur] = v; // 第一个孩子 -> 左孩子
        else
          rs[lastCh[cur]] = v; // 其余孩子 -> 上一个孩子的右兄弟
        lastCh[cur] = v;

        stk.push_back(cur);
        cur = v;
        if (++dep > h1)
          h1 = dep;
      } else if (c == 'u') {
        if (!stk.empty()) {
          cur = stk.back();
          stk.pop_back();
          --dep;
        }
      }
    }

    int n = (int)lc.size();
    vector<int> bh(n, 1);
    for (int u = n - 1; u >= 0; --u) { // 孩子/兄弟编号都更大，倒序即自底向上
      int a = (lc[u] == -1) ? 0 : bh[lc[u]];
      int b = (rs[u] == -1) ? 0 : bh[rs[u]];
      bh[u] = 1 + max(a, b);
    }

    cout << "Tree " << t << ": " << h1 << " => " << bh[0] - 1 << '\n';
  }
  return 0;
}
