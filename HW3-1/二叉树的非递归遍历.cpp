// 二叉树的非递归遍历 —— 由中序遍历的栈操作序列还原二叉树，输出后序序列
//
// 思路：直接模拟课本 6.3 的非递归中序遍历：
//     p = root;
//     while (p || !stack.empty())
//         if (p) { push(p); p = p->left; }   // 一路向左压栈
//         else   { p = pop(); visit(p); p = p->right; }  //
//         出栈访问，转向右子树
//
// 由此得到两条对应关系：
//   · 读到 "push X"：X 就是当前指针 p。
//       - 若它是第一次 push，X 是根；
//       - 否则 X 一定挂在上一步「待定的位置」上（上一步 push 之后的 X.left，
//         或上一步 pop 之后的 X.right）。
//     压栈后，当前指针变为 X.left —— 位置待定。
//   · 读到 "pop"：弹出栈顶 X，访问 X，当前指针变为 X.right —— 位置待定。
//   若某个待定位置之后一直没被 push 填充，它就是空指针。
//
// 于是用「待定位置 = (父结点, 左/右)」边读边建树，再用栈做后序遍历即可。
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  if (!(cin >> n))
    return 0;

  vector<char> label; // 每个结点的字符
  vector<int> lc, rc; // 左右孩子，-1 表示空
  vector<int> st;     // 模拟中序遍历用的栈，存结点编号
  int root = -1;
  int pend = -1;      // 待定位置的父结点，-1 表示无（如根）
  char pendDir = 'L'; // 待定位置是父结点的左孩子还是右孩子

  for (int i = 0; i < 2 * n; i++) {
    string op;
    if (!(cin >> op))
      break;
    if (op == "push") {
      string s;
      cin >> s;
      int id = (int)label.size();
      label.push_back(s[0]);
      lc.push_back(-1);
      rc.push_back(-1);

      if (root == -1) {
        root = id; // 第一个压入的结点就是根
      } else if (pend != -1) {
        if (pendDir == 'L')
          lc[pend] = id;
        else
          rc[pend] = id;
      }

      st.push_back(id);
      pend = id;
      pendDir = 'L'; // 下一步可能压入 X.left
    } else {         // pop
      int x = st.back();
      st.pop_back();
      pend = x;
      pendDir = 'R'; // 下一步可能压入 X.right
    }
  }

  // 后序遍历（迭代写法，避免深递归）
  string ans;
  vector<pair<int, bool>> s2;
  if (root != -1)
    s2.push_back({root, false});
  while (!s2.empty()) {
    auto [u, visited] = s2.back();
    s2.pop_back();
    if (visited) {
      ans.push_back(label[u]);
    } else {
      s2.push_back({u, true});
      if (rc[u] != -1)
        s2.push_back({rc[u], false});
      if (lc[u] != -1)
        s2.push_back({lc[u], false});
    }
  }

  cout << ans << '\n';
  return 0;
}
