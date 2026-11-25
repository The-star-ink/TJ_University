//2551746 大数据 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 有关链表的结构体定义如下，不允许更改 */
struct student {
  int no;        // 学号，不考虑 0开头
  char name[17]; // 姓名，最长 4个汉字（无生僻字，均为双字节 GB汉字）
  int score;     // 成绩，不考虑小数点
  struct student *next;
};

int main() {
  FILE *fp = NULL;
  struct student *head = NULL, *tail = NULL, *p = NULL, *q = NULL;
  int no, score;
  char name[17];

  /* 打开数据文件 */
  fp = fopen("list.txt", "r");
  if (fp == NULL) {
    printf("文件打开失败\n");
    return -1;
  }

  /* 每读一个学生，就申请一个结点存放信息，新读取的学生放在链表尾部 */
  while (fscanf(fp, "%d %16s %d", &no, name, &score) == 3) {
    if (no == 9999999)
      break; /* 读到结束标记，不加入链表 */

    p = (struct student *)malloc(sizeof(struct student));
    if (p == NULL) {
      fclose(fp);
      return -1; /* 申请失败，题目要求直接 return -1，不考虑释放 */
    }

    p->no = no;
    strcpy(p->name, name);
    p->score = score;
    p->next = NULL;

    if (head == NULL)
      head = p;
    else
      tail->next = p;
    tail = p;
  }
  fclose(fp);

  /* 遍历链表，打印所有学生信息 */
  p = head;
  while (p != NULL) {
    printf("%d %s %d\n", p->no, p->name, p->score);
    p = p->next;
  }

  /* 遍历完成后，释放链表 */
  p = head;
  while (p != NULL) {
    q = p->next;
    free(p);
    p = q;
  }

  return 0;
}
