//2551746 大数据 陈珈铭
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <new>
using namespace std;

/* 报名者信息 */
struct student {
  int no;       // 报名号
  char *name;   // 姓名（姓名+数字当做一个完整的姓名处理）
  char *school; // 毕业学校
};

int main() {
  ifstream fin;
  ofstream fout;
  student *stu = NULL;
  student tmp;
  int N = 0, M = 0;
  int i = 0, j = 0;
  int no;
  char name[64], school[64];

  /* 初始化随机数种子 */
  srand((unsigned)time(NULL));

  /* 打开数据文件 */
  fin.open("stulist.txt");
  if (!fin) {
    cout << "文件打开失败" << endl;
    return -1;
  }

  /* 第 1 行为招生人数 N，第 2 行为报名总人数 M */
  fin >> N >> M;

  /* 按需申请连续空间当数组用 */
  stu = new (nothrow) student[M];
  if (stu == NULL) {
    fin.close();
    return -1;
  }

  /* 读取所有报名者信息，姓名/毕业学校按实际长度二次申请，不浪费空间 */
  for (i = 0; i < M; i++) {
    fin >> no >> name >> school;

    stu[i].no = no;
    stu[i].name = new (nothrow) char[strlen(name) + 1];
    if (stu[i].name == NULL) {
      fin.close();
      return -1;
    }
    strcpy(stu[i].name, name);

    stu[i].school = new (nothrow) char[strlen(school) + 1];
    if (stu[i].school == NULL) {
      fin.close();
      return -1;
    }
    strcpy(stu[i].school, school);
  }
  fin.close();

  /* 随机抽取 N 人：部分 Fisher-Yates 洗牌，原地交换，保证结果不重复 */
  for (i = 0; i < N; i++) {
    j = i + rand() % (M - i);
    tmp = stu[i];
    stu[i] = stu[j];
    stu[j] = tmp;
  }

  /* 将抽中的 N 人写入 result.txt，每行一个学生，基本信息之间用一个空格分隔 */
  fout.open("result.txt");
  if (!fout) {
    for (i = 0; i < M; i++) {
      delete[] stu[i].name;
      delete[] stu[i].school;
    }
    delete[] stu;
    return -1;
  }

  for (i = 0; i < N; i++) {
    fout << stu[i].no << ' ' << stu[i].name << ' ' << stu[i].school << endl;
  }
  fout.close();

  /* 屏幕上给简单的提示性输出 */
  cout << "随机抽取完成，共抽取 " << N << " 人，结果见 result.txt" << endl;

  /* 释放动态申请的内存 */
  for (i = 0; i < M; i++) {
    delete[] stu[i].name;
    delete[] stu[i].school;
  }
  delete[] stu;

  return 0;
}

/*
 * 选择理由：
 * 选用“连续空间当数组用”的方式。
 * 1) 随机抽取 N 人本质上是“随机访问 + 去重”。数组配合“部分 Fisher-Yates 洗牌”，
 *    只需在 [i, M-1] 中随机取一个下标并与 stu[i] 原地交换，O(N) 时间即可完成，
 *    而且不需要任何额外数组，天然保证不重复；
 * 2) 若用链表，随机删除结点需要先从头遍历定位到第 k 个结点，每抽 1 人平均遍历
 *    M/2 个结点，总代价 O(N*M)，且指针操作繁琐、易错；
 * 3) 虽然报名总人数 M 在运行前未知，但结构体数组是按 M 个元素一次申请的，属于
 *    “按需申请”；姓名/毕业学校长度不一，故这两个成员用 char* 再按实际长度二次
 *    申请，避免固定长度字符数组造成的空间浪费。
 */
