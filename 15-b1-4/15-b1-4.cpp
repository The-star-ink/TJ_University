//2551746 大数据 陈珈铭
#include <fstream>
#include <iostream>
#include <new>
using namespace std;

/* 有关的结构体定义如下，不允许更改 */
struct student {
  int no;        // 学号，不考虑 0开头
  char name[17]; // 姓名，最长 4个汉字（无生僻字，均为双字节 GB汉字）
  int score;     // 成绩，不考虑小数点
  int rank;      // 名次
};

int main() {
  ifstream fin;
  student *stu = NULL;
  student tmp;
  int n = 0;
  int i = 0, j = 0;
  int max, min;

  /* 打开数据文件 */
  fin.open("student.txt");
  if (!fin) {
    cout << "文件打开失败" << endl;
    return -1;
  }

  /* 读取学生人数 */
  fin >> n;

  /* 按需申请连续空间，并判断申请是否成功 */
  stu = new (nothrow) student[n];
  if (stu == NULL) {
    fin.close();
    return -1;
  }

  /* 读取所有学生信息 */
  for (i = 0; i < n; i++) {
    fin >> stu[i].no >> stu[i].name >> stu[i].score;
  }
  fin.close();

  /* 第一步：按成绩从高到低排序（选择法），用于计算名次 */
  for (i = 0; i < n - 1; i++) {
    max = i;
    for (j = i + 1; j < n; j++) {
      if (stu[j].score > stu[max].score)
        max = j;
    }
    if (max != i) {
      tmp = stu[i];
      stu[i] = stu[max];
      stu[max] = tmp;
    }
  }

  /* 计算名次：成绩相同则名次相同，下一名次跳过并列名次 */
  for (i = 0; i < n; i++) {
    if (i == 0)
      stu[i].rank = 1;
    else if (stu[i].score == stu[i - 1].score)
      stu[i].rank = stu[i - 1].rank;
    else
      stu[i].rank = i + 1;
  }

  /* 第二步：按名次从高到低（名次小在前）排序，名次相同者按学号从小到大排序（选择法）
   */
  for (i = 0; i < n - 1; i++) {
    min = i;
    for (j = i + 1; j < n; j++) {
      if (stu[j].rank < stu[min].rank ||
          (stu[j].rank == stu[min].rank && stu[j].no < stu[min].no))
        min = j;
    }
    if (min != i) {
      tmp = stu[i];
      stu[i] = stu[min];
      stu[min] = tmp;
    }
  }

  /* 按名次从高到低依次输出 */
  for (i = 0; i < n; i++) {
    cout << stu[i].no << ' ' << stu[i].name << ' ' << stu[i].score << ' '
         << stu[i].rank << endl;
  }

  /* 释放动态申请的内存 */
  delete[] stu;

  return 0;
}
