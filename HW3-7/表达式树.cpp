#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Node {
  char c;
  int l, r;
};

vector<Node> tr;
string s;
int pos = 0;

int newNode(char c, int l, int r) {
  tr.push_back({c, l, r});
  return (int)tr.size() - 1;
}

int parseExpr();

int parseFactor() {
  if (s[pos] == '(') {
    ++pos; // 跳过 '('
    int v = parseExpr();
    ++pos; // 跳过 ')'
    return v;
  }
  return newNode(s[pos++], -1, -1);
}

int parseTerm() {
  int v = parseFactor();
  while (pos < (int)s.size() && (s[pos] == '*' || s[pos] == '/')) {
    char op = s[pos++];
    int w = parseFactor();
    v = newNode(op, v, w); // 左结合
  }
  return v;
}

int parseExpr() {
  int v = parseTerm();
  while (pos < (int)s.size() && (s[pos] == '+' || s[pos] == '-')) {
    char op = s[pos++];
    int w = parseTerm();
    v = newNode(op, v, w);
  }
  return v;
}

int height(int u) {
  if (u == -1)
    return 0;
  int a = height(tr[u].l), b = height(tr[u].r);
  return 1 + (a > b ? a : b);
}

void postOrder(int u, string &out) {
  if (u == -1)
    return;
  postOrder(tr[u].l, out);
  postOrder(tr[u].r, out);
  out.push_back(tr[u].c);
}

void draw(int u, int row, int col, vector<string> &canvas) {
  canvas[row][col] = tr[u].c;
  int h = height(u); // 以 u 为根的高
  if (tr[u].l != -1) {
    canvas[row + 1][col - 1] = '/'; // 父列偏左一格
    draw(tr[u].l, row + 2, col - (1 << (h - 2)), canvas);
  }
  if (tr[u].r != -1) {
    canvas[row + 1][col + 1] = '\\'; // 父列偏右一格
    draw(tr[u].r, row + 2, col + (1 << (h - 2)), canvas);
  }
}

long long eval(int u, map<char, long long> &val) {
  if (tr[u].l == -1)
    return val[tr[u].c];
  long long a = eval(tr[u].l, val), b = eval(tr[u].r, val);
  switch (tr[u].c) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  default:
    return a / b; // 整除，向零截断
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string expr;
  cin >> expr;
  int n;
  cin >> n;
  map<char, long long> val;
  for (int i = 0; i < n; i++) {
    char c;
    long long x;
    cin >> c >> x;
    val[c] = x;
  }

  s = expr;
  pos = 0;
  int root = parseExpr();

  string rpn;
  postOrder(root, rpn);
  cout << rpn << '\n';

  int H = height(root);
  int width = (1 << H) - 1;
  vector<string> canvas(2 * H - 1, string(width, ' '));
  draw(root, 0, width / 2, canvas);
  for (auto &line : canvas) {
    while (!line.empty() && line.back() == ' ')
      line.pop_back(); // 去行末空格
    cout << line << '\n';
  }

  cout << eval(root, val) << '\n';
  return 0;
}
