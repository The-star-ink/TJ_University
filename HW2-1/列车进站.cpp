#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool check(const string &in, const string &out) {
  if (in.size() != out.size())
    return false;

  stack<char> st; // 真正的栈,模拟"车站"
  int j = 0;      // 指向出站序列当前要匹配的位置

  for (char c : in) { // 列车按入站序列依次来到
    st.push(c);       // 进车站(箭头 a)
    while (!st.empty() && j < (int)out.size() && st.top() == out[j]) {
      st.pop(); // 从车站到出口(箭头 c)
      j++;      // 匹配出站序列的下一个
    }
  }
  return j == (int)out.size(); // 出站序列全部匹配完 = 合法
}

int main() {
  string in, out;
  cin >> in;           // 第1行:入站序列
  while (cin >> out) { // 读到 EOF 结束
    cout << (check(in, out) ? "yes" : "no") << '\n';
  }
  return 0;
}
