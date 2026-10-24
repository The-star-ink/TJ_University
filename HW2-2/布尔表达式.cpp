// 布尔表达式求值：V=true, F=false, ! 非, & 与, | 或
// 优先级：! > & > |
// 用双栈（值栈 + 运算符栈）按算符优先法求值
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 运算符优先级，'(' 视为优先级最低的分界符
int prec(char op) {
  if (op == '!')
    return 3;
  if (op == '&')
    return 2;
  if (op == '|')
    return 1;
  return 0; // '('
}

// 取出运算符栈顶做一次运算，结果压回值栈
void applyTop(vector<bool> &vals, vector<char> &ops) {
  char op = ops.back();
  ops.pop_back();
  if (op == '!') { // 单目：只弹一个值
    bool a = vals.back();
    vals.pop_back();
    vals.push_back(!a);
  } else { // 双目：弹两个值
    bool b = vals.back();
    vals.pop_back();
    bool a = vals.back();
    vals.pop_back();
    vals.push_back(op == '&' ? (a && b) : (a || b));
  }
}

// 去掉空格等无关字符，并把全角符号转成半角
string clean(const string &line) {
  string s;
  s.reserve(line.size());
  for (size_t i = 0; i < line.size();) {
    unsigned char c = (unsigned char)line[i];
    if (c == 0xEF && i + 2 < line.size()) { // UTF-8 三字节全角字符
      string t = line.substr(i, 3);
      if (t == "\xEF\xBC\x81")
        s += '!'; // ！
      else if (t == "\xEF\xBC\x88")
        s += '('; // （
      else if (t == "\xEF\xBC\x89")
        s += ')'; // ）
      else if (t == "\xEF\xBC\x86")
        s += '&'; // ＆
      else if (t == "\xEF\xBD\x9C")
        s += '|'; // ｜
      else if (t == "\xEF\xBC\xB6")
        s += 'V'; // Ｖ
      else if (t == "\xEF\xBC\xA6")
        s += 'F'; // Ｆ
      i += 3;
      continue;
    } else if (c == 0xA3 && i + 1 < line.size()) { // GBK 全角字符（两字节）
      unsigned char d = (unsigned char)line[i + 1];
      if (d == 0xA1)
        s += '!'; // ！
      else if (d == 0xA6)
        s += '&'; // ＆
      else if (d == 0xA8)
        s += '('; // （
      else if (d == 0xA9)
        s += ')'; // ）
      else if (d == 0xFC)
        s += '|'; // ｜
      else if (d == 0xD6)
        s += 'V'; // Ｖ
      else if (d == 0xC6)
        s += 'F'; // Ｆ
      i += 2;
      continue;
    }
    if (c == 'V' || c == 'F' || c == '&' || c == '|' || c == '!' || c == '(' ||
        c == ')')
      s += (char)c;
    // 其余字符（空格、制表符、\r 等）一律丢弃
    i++;
  }
  return s;
}

bool evaluate(const string &s) {
  vector<bool> vals; // 值栈
  vector<char> ops;  // 运算符栈

  for (char c : s) {
    if (c == 'V' || c == 'F') {
      vals.push_back(c == 'V');
      // 操作数一到位，紧挨在它前面的 ! 立刻结算;
      // 连续多个 !! 要逐个弹掉（!!V 相当于 !(!V)）
      while (!ops.empty() && ops.back() == '!')
        applyTop(vals, ops);
    } else if (c == '!') {
      ops.push_back(c);
    } else if (c == '&' || c == '|') {
      // 先把栈顶所有优先级不低于当前运算符的都算掉，再入栈
      while (!ops.empty() && ops.back() != '(' && prec(ops.back()) >= prec(c))
        applyTop(vals, ops);
      ops.push_back(c);
    } else if (c == '(') {
      ops.push_back(c);
    } else if (c == ')') {
      // 把括号内剩余的运算全部结算
      while (!ops.empty() && ops.back() != '(')
        applyTop(vals, ops);
      if (!ops.empty())
        ops.pop_back(); // 弹出 '('
      // 括号整体算完后，紧挨在 '(' 前面的 ! 作用于整个括号
      while (!ops.empty() && ops.back() == '!')
        applyTop(vals, ops);
    }
  }
  while (!ops.empty())
    applyTop(vals, ops);
  return vals.back();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // 输出前缀：题面写的是 "Expression"+序号+": "，样例表格里写成了"表达式1："
  // 如果你们的判题要求中文前缀，把这里改成 true（注意源文件需保存为 GBK 编码）
  const bool CHINESE_PREFIX = false;

  string line;
  int idx = 0;
  while (getline(cin, line)) { // 一行一个表达式，读到 EOF 结束
    string s = clean(line);
    if (s.empty())
      continue; // 跳过空行
    idx++;
    bool ok = evaluate(s);
    if (CHINESE_PREFIX)
      cout << "表达式" << idx << "：";
    else
      cout << "Expression " << idx << ": ";
    cout << (ok ? 'V' : 'F') << '\n';
  }
  return 0;
}