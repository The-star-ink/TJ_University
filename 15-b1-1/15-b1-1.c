//2551746 大数据 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* 有关的结构体定义如下，不允许更改 */
struct student {
  int no;        // 学号，不考虑 0开头
  char name[17]; // 姓名，最长 4个汉字（无生僻字，均为双字节 GB汉字）
  int score;     // 成绩，不考虑小数点
  int rank;      // 名次
};

int main() {
  FILE *fp = NULL;
  struct student *stu = NULL;
  struct student tmp;
  int n = 0;
  int i = 0, j = 0;
  int max, min;

  /* 打开数据文件 */
  fp = fopen("student.txt", "r");
  if (fp == NULL) {
    printf("文件打开失败\n");
    return -1;
  }

  /* 读取学生人数 */
  fscanf(fp, "%d", &n);

  /* 按需申请连续空间，并判断申请是否成功 */
  stu = (struct student *)malloc(n * sizeof(struct student));
  if (stu == NULL) {
    fclose(fp);
    return -1;
  }

  /* 读取所有学生信息 */
  for (i = 0; i < n; i++) {
    fscanf(fp, "%d %16s %d", &stu[i].no, stu[i].name, &stu[i].score);
  }
  fclose(fp);

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

  /* 第二步：重新按学号从小到大排序（选择法） */
  for (i = 0; i < n - 1; i++) {
    min = i;
    for (j = i + 1; j < n; j++) {
      if (stu[j].no < stu[min].no)
        min = j;
    }
    if (min != i) {
      tmp = stu[i];
      stu[i] = stu[min];
      stu[min] = tmp;
    }
  }

  /* 按学号从小到大依次输出 */
  for (i = 0; i < n; i++) {
    printf("%d %s %d %d\n", stu[i].no, stu[i].name, stu[i].score, stu[i].rank);
  }

  /* 释放动态申请的内存 */
  free(stu);

  return 0;
}
