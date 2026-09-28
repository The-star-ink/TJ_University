#include <algorithm>
#include <iostream>
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

// ---------- 多项式乘法：生成全部乘积项 -> 按指数排序 -> 合并同类项 ----------
// 时间复杂度 O(m*n*log(m*n))，最多约 420 万个乘积项
vector<Term> polyMul(const vector<Term> &a, const vector<Term> &b) {
  vector<pair<ll, ll>> raw; // first = 指数, second = 系数（待合并）
  raw.reserve(a.size() * b.size());
  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = 0; j < b.size(); ++j) {
      raw.push_back(
          make_pair(a[i].second + b[j].second, a[i].first * b[j].first));
    }
  }
  sort(raw.begin(), raw.end()); // 按指数递增排序
  vector<Term> r;
  r.reserve(raw.size());
  for (size_t i = 0; i < raw.size();) {
    ll e = raw[i].first, c = 0;
    while (i < raw.size() && raw[i].first == e) {
      c += raw[i].second;
      ++i;
    }
    if (c != 0)
      r.push_back(make_pair(c, e)); // 系数为 0 的项丢弃
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
