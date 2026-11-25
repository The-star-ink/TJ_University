//2551746 大数据 陈珈铭
#include "90-01-b2-color_linez.h"
#include <conio.h>
#include <iostream>
/* 加入必要的头文件、命名空间、你觉得应该属于工具类的其它函数 */

/***************************************************************************
  函数名称：
  功    能：显示提示信息
  输入参数：
  返 回 值：
  说    明：如果读不懂，暂时放着，等指针和引用学习完成后再阅读
***************************************************************************/
void to_be_continued(const char *prompt, const int X, const int Y) {
  cct_setcolor(); // 恢复缺省颜色
  cct_gotoxy(X, Y);

  if (prompt)
    cout << prompt << "，按回车键继续...";
  else
    cout << "按回车键继续...";

  /* 忽略除回车键外的所有输入（注意：_getch的回车是\r，而getchar是\n）*/
  while (_getch() != '\r')
    ;
  cout << endl;
  // cct_cls();

  return;
}
