#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;
const ll BASE = 1000000000LL; // 基数 10^9：每个元素存 9 位十进制数字

// 大整数：d[0] 为最低的 9 位组，d.back() 为最高位组
struct BigInt {
  vector<ll> d;
  BigInt(ll v = 0) {
    if (v == 0)
      d.push_back(0);
    else {
      while (v) {
        d.push_back(v % BASE);
        v /= BASE;
      }
    }
  }
};

// 大数 × 小整数
BigInt mulSmall(const BigInt &a, ll k) {
  if (k == 0)
    return BigInt(0);
  BigInt r;
  r.d.assign(a.d.size(), 0);
  ll carry = 0;
  for (size_t i = 0; i < a.d.size(); ++i) {
    ll cur = a.d[i] * k + carry;
    r.d[i] = cur % BASE;
    carry = cur / BASE;
  }
  while (carry) {
    r.d.push_back(carry % BASE);
    carry /= BASE;
  }
  return r;
}

// 大数 + 大数
BigInt addBig(const BigInt &a, const BigInt &b) {
  BigInt r;
  size_t n = max(a.d.size(), b.d.size());
  r.d.assign(n, 0);
  ll carry = 0;
  for (size_t i = 0; i < n; ++i) {
    ll va = i < a.d.size() ? a.d[i] : 0;
    ll vb = i < b.d.size() ? b.d[i] : 0;
    ll cur = va + vb + carry;
    r.d[i] = cur % BASE;
    carry = cur / BASE;
  }
  if (carry)
    r.d.push_back(carry);
  return r;
}

// 输出：最高组不带前导 0，其余组补足 9 位
void printBig(const BigInt &a) {
  size_t hi = a.d.size() - 1;
  cout << a.d[hi];
  for (size_t j = hi; j > 0; --j) {
    cout << setw(9) << setfill('0') << a.d[j - 1];
  }
  cout << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, A;
  while (cin >> N >> A) { // 处理多行输入，直到 EOF
    BigInt p(1);          // p = A^i，从 A^0 开始
    BigInt sum(0);        // 累加结果
    for (int i = 1; i <= N; ++i) {
      p = mulSmall(p, A);                // p = A^i
      sum = addBig(sum, mulSmall(p, i)); // sum += i * A^i
    }
    printBig(sum);
  }
  return 0;
}
