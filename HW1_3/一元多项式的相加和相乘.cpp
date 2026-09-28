#include <iostream>
#include <queue>
#include <vector>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> Term; // first = 系数, second = 指数

// ---------- 多项式加法：归并两个已按指数递增的列表 ----------
// 时间复杂度 O(m + n)
vector<Term> polyAdd(const vector<Term> &a, const vector<Term> &b) {
  vector<Term> r;
  r.reserve(a.size() + b.size());
  size_t i = 0, j = 0;
  while (i < a.size() && j < b.size()) {
    if (a[i].second < b[j].second) {
      r.push_back(a[i]);
      ++i;
    } else if (a[i].second > b[j].second) {
      r.push_back(b[j]);
      ++j;
    } else {
      ll c = a[i].first + b[j].first; // 同指数合并
      if (c != 0)
        r.push_back(make_pair(c, a[i].second));
      ++i;
      ++j;
    }
  }
  while (i < a.size())
    r.push_back(a[i++]);
  while (j < b.size())
    r.push_back(b[j++]);
  return r;
}

// ---------- 多项式乘法：k 路归并 + 小顶堆（内存优化版）----------
// 原理：乘积矩阵 M[i][j] = (A[i].c * B[j].c, A[i].e + B[j].e)，
//       固定 i 时指数随 j 递增 => 共有 m 个有序序列，
//       用小顶堆做 k 路归并，边弹出边合并同指数项，无需存全部乘积项。
// 时间复杂度 O(m*n*log(min(m,n)))，空间 O(min(m,n)) + 结果
struct Item {
  ll c, e;  // 系数、指数
  int i, j; // 来自 A[i] * B[j]
};
struct Cmp {
  bool operator()(const Item &x, const Item &y) const {
    return x.e > y.e; // 小顶堆：指数小的优先
  }
};

vector<Term> polyMul(const vector<Term> &x, const vector<Term> &y) {
  // 让 a 为较短者：堆的大小 = a.size() = min(m, n)
  const vector<Term> &a = (x.size() <= y.size()) ? x : y;
  const vector<Term> &b = (x.size() <= y.size()) ? y : x;
  if (a.empty() || b.empty())
    return vector<Term>();

  priority_queue<Item, vector<Item>, Cmp> pq;
  // 每行（i）当前项：与 B 的第 0 项相乘
  for (int i = 0; i < (int)a.size(); ++i) {
    pq.push(Item{a[i].first * b[0].first, a[i].second + b[0].second, i, 0});
  }

  vector<Term> r;
  r.reserve(a.size() + b.size()); // 结果最多 m+n-1 项
  while (!pq.empty()) {
    Item cur = pq.top();
    pq.pop();
    ll e = cur.e, c = cur.c;
    // 该行推进到下一列（指数更大，不可能再等于当前 e）
    if (cur.j + 1 < (int)b.size()) {
      pq.push(Item{a[cur.i].first * b[cur.j + 1].first,
                   a[cur.i].second + b[cur.j + 1].second, cur.i, cur.j + 1});
    }
    // 合并所有同指数的项（此刻它们相邻排在堆顶）
    while (!pq.empty() && pq.top().e == e) {
      Item nx = pq.top();
      pq.pop();
      c += nx.c;
      if (nx.j + 1 < (int)b.size()) {
        pq.push(Item{a[nx.i].first * b[nx.j + 1].first,
                     a[nx.i].second + b[nx.j + 1].second, nx.i, nx.j + 1});
      }
    }
    if (c != 0)
      r.push_back(make_pair(c, e)); // 系数为 0 丢弃
  }
  return r;
}

// 输出：指数递增，"系数 指数" 空格分隔；空多项式不输出
void printPoly(const vector<Term> &p) {
  if (p.empty())
    return;
  for (size_t i = 0; i < p.size(); ++i) {
    if (i)
      cout << ' ';
    cout << p[i].first << ' ' << p[i].second;
  }
  cout << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m;
  cin >> m;
  vector<Term> A;
  A.reserve(m);
  for (int i = 0; i < m; ++i) {
    ll p, e;
    cin >> p >> e;
    if (p != 0)
      A.push_back(make_pair(p, e)); // 过滤系数 0 的无意义项
  }

  int n;
  cin >> n;
  vector<Term> B;
  B.reserve(n);
  for (int i = 0; i < n; ++i) {
    ll p, e;
    cin >> p >> e;
    if (p != 0)
      B.push_back(make_pair(p, e)); // 过滤系数 0 的无意义项
  }

  int op;
  cin >> op;

  if (op == 0) {
    printPoly(polyAdd(A, B));
  } else if (op == 1) {
    printPoly(polyMul(A, B));
  } else { // op == 2：先输出加法结果一行，再输出乘法结果一行
    printPoly(polyAdd(A, B));
    printPoly(polyMul(A, B));
  }
  return 0;
}
