//2551746 大数据 陈珈铭
#include <cstring>
#include <fstream>
#include <iostream>
#include <new>
using namespace std;

/* 有关链表的结构体定义如下，不允许更改 */
struct student {
  int *no;    // 学号，不考虑 0开头
  char *name; // 姓名，无生僻字，均为双字节 GB汉字
  int *score; // 成绩，不考虑小数点
  struct student *next;
};

int main() {
  ifstream fin;
  student *head = NULL, *tail = NULL, *p = NULL, *q = NULL;
  int no, score;
  char name[17];

  /* 打开数据文件 */
  fin.open("list.txt");
  if (!fin) {
    cout << "文件打开失败" << endl;
    return -1;
  }

  /* 每读一个学生，就申请一个结点存放信息，新读取的学生放在链表尾部 */
  while (fin >> no >> name >> score) {
    if (no == 9999999)
      break; /* 读到结束标记，不加入链表 */

    /* 第一次申请：结点本身 */
    p = new (nothrow) student;
    if (p == NULL) {
      fin.close();
      return -1; /* 申请失败，题目要求直接 return -1，不考虑释放 */
    }

    /* 第二次申请：结点内各成员 */
    p->no = new (nothrow) int;
    if (p->no == NULL) {
      fin.close();
      return -1;
    }
    p->name = new (nothrow) char[strlen(name) + 1];
    if (p->name == NULL) {
      fin.close();
      return -1;
    }
    p->score = new (nothrow) int;
    if (p->score == NULL) {
      fin.close();
      return -1;
    }

    *(p->no) = no;
    strcpy(p->name, name);
    *(p->score) = score;
    p->next = NULL;

    if (head == NULL)
      head = p;
    else
      tail->next = p;
    tail = p;
  }
  fin.close();

  /* 遍历链表，打印所有学生信息 */
  p = head;
  while (p != NULL) {
    cout << *(p->no) << ' ' << p->name << ' ' << *(p->score) << endl;
    p = p->next;
  }

  /* 遍历完成后，释放链表（先释放成员，再释放结点） */
  p = head;
  while (p != NULL) {
    q = p->next;
    delete p->no;
    delete[] p->name;
    delete p->score;
    delete p;
    p = q;
  }

  return 0;
}
