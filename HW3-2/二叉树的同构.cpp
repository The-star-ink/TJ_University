// 二叉树的同构 + 深度
//
// 判断同构：把每棵子树化成「规范编号」——相同编号 <=> 两棵子树同构。
//   空树编号 0；对结点 u，取左右孩子编号，小的放前、大的放后（左右可互换），
//   用 (字符, 小编号, 大编号) 到编号表里查，查不到就分配一个新编号。
//   于是两棵树同构 <=> 根结点编号相同。字符可以重复，判等时字符也参与编号，
//   不会误判。
// 深度：结点数意义下的树高，1 + max(左高, 右高)，同样在逆后序里一次算完。
//
// 全部用显式栈 / 数组完成，避免 n 很大（链状树）时递归爆栈。
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

int nextId = 1;                       // 0 留给空树
map<tuple<char, int, int>, int> memo; // (字符, 小编号, 大编号) -> 规范编号

struct Tree {
  vector<char> lab;
  vector<int> L, R;
  int root = -1;
  vector<int> cid; // 规范编号
  vector<int> h;   // 高度（结点数）
};

void build(Tree &t, int n) {
  t.lab.resize(n);
  t.L.assign(n, -1);
  t.R.assign(n, -1);
  vector<char> isChild(n, 0);

  for (int i = 0; i < n; i++) {
    string c, a, b;
    cin >> c >> a >> b;
    t.lab[i] = c[0];
    if (a != "-") {
      t.L[i] = stoi(a);
      isChild[t.L[i]] = 1;
    }
    if (b != "-") {
      t.R[i] = stoi(b);
      isChild[t.R[i]] = 1;
    }
  }
  for (int i = 0; i < n; i++) // 没当过孩子的就是根
    if (!isChild[i]) {
      t.root = i;
      break;
    }

  t.cid.assign(n, 0);
  t.h.assign(n, 0);
  if (t.root == -1)
    return;

  // 逆后序：先根、再（右、左）压栈得到的序列反过来，保证孩子先于父亲
  vector<int> stk{t.root}, seq;
  while (!stk.empty()) {
    int u = stk.back();
    stk.pop_back();
    seq.push_back(u);
    if (t.L[u] != -1)
      stk.push_back(t.L[u]);
    if (t.R[u] != -1)
      stk.push_back(t.R[u]);
  }
  reverse(seq.begin(), seq.end());

  for (int u : seq) {
    int a = (t.L[u] == -1) ? 0 : t.cid[t.L[u]];
    int b = (t.R[u] == -1) ? 0 : t.cid[t.R[u]];
    if (a > b)
      swap(a, b); // 左右互换不改变编号
    auto key = make_tuple(t.lab[u], a, b);
    auto it = memo.find(key);
    if (it == memo.end())
      it = memo.emplace(key, nextId++).first;
    t.cid[u] = it->second;

    int hl = (t.L[u] == -1) ? 0 : t.h[t.L[u]];
    int hr = (t.R[u] == -1) ? 0 : t.h[t.R[u]];
    t.h[u] = 1 + max(hl, hr);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n1, n2;
  if (!(cin >> n1))
    return 0;
  Tree t1;
  build(t1, n1);
  cin >> n2;
  Tree t2;
  build(t2, n2);

  bool same;
  if (n1 != n2)
    same = false; // 结点数不同必然不同构
  else if (n1 == 0)
    same = true;
  else
    same = (t1.cid[t1.root] == t2.cid[t2.root]);

  int d1 = (t1.root == -1) ? 0 : t1.h[t1.root];
  int d2 = (t2.root == -1) ? 0 : t2.h[t2.root];

  cout << (same ? "Yes" : "No") << '\n' << d1 << '\n' << d2 << '\n';
  return 0;
}
