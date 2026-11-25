//2551746 大数据 陈珈铭
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

/* 检查文件是 Windows 格式、Linux 格式还是无法识别 */
int check_format(const char *path) {
  ifstream fin(path, ios::binary);
  char c, prev = 0;
  bool any_lf = false, has_crlf = false, has_lone_lf = false;

  if (!fin) {
    cout << "文件打开失败" << endl;
    return -1;
  }

  while (fin.get(c)) {
    if (c == '\n') { /* 0x0A */
      any_lf = true;
      if (prev == '\r') /* 0x0D */
        has_crlf = true;
      else
        has_lone_lf = true;
    }
    prev = c;
  }
  fin.close();

  if (!any_lf || (has_crlf && has_lone_lf)) {
    cout << "文件格式无法识别" << endl;
    return 0;
  }
  if (has_lone_lf) {
    cout << "Linux格式" << endl;
    return 0;
  }
  cout << "Windows格式" << endl;
  return 0;
}

/* 将 Windows 格式转换为 Linux 格式（0D 0A -> 0A） */
int convert_wtol(const char *inpath, const char *outpath) {
  ifstream fin(inpath, ios::binary);
  char c, prev = 0;
  bool any_lf = false, has_lone_lf = false;
  bool prev_cr = false;

  if (!fin) {
    cout << "文件打开失败" << endl;
    return -1;
  }

  /* 先判断格式：必须是 Windows 格式 */
  while (fin.get(c)) {
    if (c == '\n') {
      any_lf = true;
      if (prev != '\r')
        has_lone_lf = true;
    }
    prev = c;
  }
  fin.close();

  if (!any_lf || has_lone_lf) {
    cout << "文件格式无法识别" << endl;
    return 0;
  }

  /* 再转换：删除紧邻 0A 前面的 0D */
  fin.open(inpath, ios::binary);
  ofstream fout(outpath, ios::binary);
  if (!fin || !fout) {
    if (fin)
      fin.close();
    if (fout)
      fout.close();
    cout << "文件打开失败" << endl;
    return -1;
  }

  while (fin.get(c)) {
    if (prev_cr) {
      if (c == '\n')
        fout.put('\n');
      else {
        fout.put('\r');
        fout.put(c);
      }
      prev_cr = false;
    } else if (c == '\r') {
      prev_cr = true;
    } else {
      fout.put(c);
    }
  }
  if (prev_cr)
    fout.put('\r');

  fin.close();
  fout.close();
  return 0;
}

/* 将 Linux 格式转换为 Windows 格式（0A -> 0D 0A） */
int convert_ltow(const char *inpath, const char *outpath) {
  ifstream fin(inpath, ios::binary);
  char c, prev = 0;
  bool any_lf = false, has_crlf = false;

  if (!fin) {
    cout << "文件打开失败" << endl;
    return -1;
  }

  /* 先判断格式：必须是 Linux 格式 */
  while (fin.get(c)) {
    if (c == '\n') {
      any_lf = true;
      if (prev == '\r')
        has_crlf = true;
    }
    prev = c;
  }
  fin.close();

  if (!any_lf || has_crlf) {
    cout << "文件格式无法识别" << endl;
    return 0;
  }

  /* 再转换：每个 0A 前插入 0D */
  fin.open(inpath, ios::binary);
  ofstream fout(outpath, ios::binary);
  if (!fin || !fout) {
    if (fin)
      fin.close();
    if (fout)
      fout.close();
    cout << "文件打开失败" << endl;
    return -1;
  }

  while (fin.get(c)) {
    if (c == '\n') {
      fout.put('\r');
      fout.put('\n');
    } else {
      fout.put(c);
    }
  }

  fin.close();
  fout.close();
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc >= 3 && strcmp(argv[1], "--check") == 0) {
    return check_format(argv[2]);
  }
  if (argc >= 5 && strcmp(argv[1], "--convert") == 0) {
    if (strcmp(argv[2], "wtol") == 0)
      return convert_wtol(argv[3], argv[4]);
    if (strcmp(argv[2], "ltow") == 0)
      return convert_ltow(argv[3], argv[4]);
  }
  return -1;
}
