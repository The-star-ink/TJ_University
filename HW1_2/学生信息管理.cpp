#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 学生信息
struct Student {
  string no;   // 学号
  string name; // 姓名
};

// 顺序表（用 vector 模拟，位置从 1 开始计数）
// 操作函数全部返回 int：成功返回 0 或位置，失败返回 -1

// 插入：在第 i 个位置（1 开始）插入学生 s
// 合法范围：1 <= i <= 表长 + 1
int insertStu(vector<Student> &seq, int i, const Student &s) {
  int len = (int)seq.size();
  if (i < 1 || i > len + 1)
    return -1;
  seq.insert(seq.begin() + (i - 1), s);
  return 0;
}

// 删除：删除第 j 个元素（1 开始）
// 合法范围：1 <= j <= 表长
int removeStu(vector<Student> &seq, int j) {
  int len = (int)seq.size();
  if (j < 1 || j > len)
    return -1;
  seq.erase(seq.begin() + (j - 1));
  return 0;
}

// 按姓名查找：返回第一个匹配的位置（1 开始），找不到返回 -1
int findBy(const vector<Student> &seq, const string &key, bool byName) {
  for (size_t i = 0; i < seq.size(); ++i) {
    if (byName ? seq[i].name == key : seq[i].no == key) {
      return (int)i + 1;
    }
  }
  return -1;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Student> seq;
  seq.reserve(n + 10);
  for (int i = 0; i < n; ++i) {
    Student s;
    cin >> s.no >> s.name;
    seq.push_back(s);
  }

  string op;
  while (cin >> op) {
    if (op == "insert") {
      int i;
      Student s;
      cin >> i >> s.no >> s.name;
      cout << insertStu(seq, i, s) << '\n';
    } else if (op == "remove") {
      int j;
      cin >> j;
      cout << removeStu(seq, j) << '\n';
    } else if (op == "check") {
      string mode, key;
      cin >> mode >> key;
      bool byName = (mode == "name");
      int pos = findBy(seq, key, byName);
      if (pos == -1) {
        cout << -1 << '\n';
      } else {
        cout << pos << ' ' << seq[pos - 1].no << ' ' << seq[pos - 1].name
             << '\n';
      }
    } else if (op == "end") {
      cout << (int)seq.size() << '\n';
      break;
    }
  }
  return 0;
}
