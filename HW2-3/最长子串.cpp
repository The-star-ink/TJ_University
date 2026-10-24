// 最长合法括号子串：输出长度和起始位置(0 起)，多个取起始位置最小的
// 用栈存下标，O(n)
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string s;
  if (!getline(cin, s)) { // 无输入
    cout << "0 0\n";
    return 0;
  }

  // 去掉行尾的 \r 和首尾空白/多余的引号
  while (!s.empty() &&
         (s.back() == '\r' || s.back() == '\n' || s.back() == ' ' ||
          s.back() == '\t' || s.back() == '"' || s.back() == '\''))
    s.pop_back();
  size_t b = 0;
  while (b < s.size() &&
         (s[b] == ' ' || s[b] == '\t' || s[b] == '"' || s[b] == '\''))
    b++;
  s = s.substr(b);

  // 只保留括号字符（防止数据里有别的干扰字符）
  vector<int> st; // 存下标；栈底元素是"最后一个无法匹配的位置"
  st.push_back(-1);
  int bestLen = 0, bestStart = 0;

  for (int i = 0; i < (int)s.size(); i++) {
    if (s[i] == '(') {
      st.push_back(i); // 左括号：等着被匹配
    } else if (s[i] == ')') {
      st.pop_back(); // 和最近的左括号配对
      if (st.empty()) {
        st.push_back(i); // 这个右括号非法，作为新的分界
      } else {
        int len = i - st.back(); // 以 i 结尾的最长合法子串长度
        int start = st.back() + 1;
        if (len > bestLen) { // 严格大于 -> 保留最先出现的
          bestLen = len;
          bestStart = start;
        }
      }
    }
  }

  cout << bestLen << ' ' << bestStart << '\n';
  return 0;
}