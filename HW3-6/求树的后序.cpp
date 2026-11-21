#include <iostream>
#include <string>
using namespace std;

string pre, in, post;
bool ok;

void build(int pl, int pr, int il, int ir) { // 区间均为左闭右开
  if (!ok || pl >= pr)
    return;
  char root = pre[pl];
  int k = -1;
  for (int i = il; i < ir; i++)
    if (in[i] == root) {
      k = i;
      break;
    }
  if (k == -1) {
    ok = false;
    return;
  } // 根不在中序段里 -> 非法

  int leftLen = k - il;
  build(pl + 1, pl + 1 + leftLen, il, k); // 左子树
  if (!ok)
    return;
  build(pl + 1 + leftLen, pr, k + 1, ir); // 右子树
  if (!ok)
    return;
  post.push_back(root); // 左右处理完再放根 -> 后序
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string a, b;
  while (cin >> a >> b) { // 每行一对，读到 EOF
    pre = a;
    in = b;
    post.clear();
    ok = (pre.size() == in.size());
    if (ok)
      build(0, (int)pre.size(), 0, (int)in.size());
    cout << (ok ? post : string("Error")) << '\n';
  }
  return 0;
}
