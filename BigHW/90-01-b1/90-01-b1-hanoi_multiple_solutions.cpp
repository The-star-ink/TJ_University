//2551746 大数据 陈珈铭
#include "../include/cmd_console_tools.h"
#include "../include/cmd_hdc_tools.h"
#include "90-01-b1-hanoi_const_value.h"
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <math.h>

/* ----------------------------------------------------------------------------------

         本文件功能：
        1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

         本文件要求：
        1、不允许定义外部全局变量（const及#define不在限制范围内）
        2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
        3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
        4、按需加入系统头文件、自定义头文件、命名空间等

   -----------------------------------------------------------------------------------
 */
using namespace std;

static int step;
static int tower[3][MAX_LAYER] = {0};
static int num[3] = {0};
static int sleep_time = 0;

/***************************************************************************
  函数名称：rectangle_X
  功    能：确定绘制方块的X坐标
  输入参数：int order：柱子编号（1-3：A-C）
                   int n : 盘编号（0-11）
  返 回 值：int型返回所求的X坐标
  说    明：0对应立柱，11对应底盘，与HDC_COLOR相对应
***************************************************************************/
int rectangle_X(const int order, const int n) {
  int X = 0;
  X += HDC_Start_X + (11 - n) * HDC_Base_Width;
  // A柱上所求盘的X坐标
  X += (23 * HDC_Base_Width + HDC_Underpan_Distance) * (order - 1);
  // BC柱的偏移
  return X;
}

/***************************************************************************
  函数名称：rectangle_Y
  功    能：确定绘制方块的Y坐标
  输入参数：int floor : 盘数量（0-11）
  返 回 值：int型返回所求的X坐标
  说    明：11对应立柱，0对应底盘，与num[]中当前柱数量相对应
***************************************************************************/
int rectangle_Y(const int floor) {
  int Y;
  Y = HDC_Start_Y - floor * HDC_Base_High;
  if (floor == 11)
    Y -= HDC_Base_High;
  return Y;
}

/***************************************************************************
  函数名称：rectangle_W
  功    能：确定绘制方块的宽度
  输入参数：int n : 盘编号（0-11）
  返 回 值：int型返回所求的W宽度
  说    明：0对应立柱，11对应底盘
***************************************************************************/
int rectangle_W(const int n) {
  int W;
  W = HDC_Base_Width;
  // A柱上所求盘的X坐标
  W += 2 * HDC_Base_Width * n;
  // BC柱的偏移
  return W;
}

/***************************************************************************
  函数名称：move_up
  功    能：将指定方块向上移动
  输入参数：int n：指定对象编号
                   int x：指定对象x值
                   int y：指定对象y值
                   int w：指定对象宽度
                   int h：指定对象高度
                   int speed：指定对象移动速度
                   int y_min：最上高度限制(y最小)
                   int tower_num：指定对象所属柱体(1-3)
  返 回 值：
  说    明：
***************************************************************************/
void move_up(int n, int x, int y, int w, int h, int speed, int y_min,
             int tower_num) {
  if (y <= y_min)
    return;
  hdc_rectangle(x, y + h - speed, w, speed, HDC_COLOR[0]);
  if (y + h >= rectangle_Y(11) + speed)
    hdc_rectangle(rectangle_X(tower_num, 0), y + h - speed, rectangle_W(0),
                  speed, HDC_COLOR[11]);
  hdc_rectangle(x, y - speed, w, speed, HDC_COLOR[n]);
  if (step <= 7) {
    Sleep(sleep_time);
    if (sleep_time == 0) {
      while (_getch() != '\r')
        ;
    }
  }
  move_up(n, x, y - speed, w, h, speed, y_min, tower_num);
}

/***************************************************************************
  函数名称：move_down
  功    能：将指定方块向上移动
  输入参数：int n：指定对象编号
                   int x：指定对象x值
                   int y：指定对象y值
                   int w：指定对象宽度
                   int h：指定对象高度
                   int speed：指定对象移动速度
                   int y_max：目标对象y值(y最大)
                   int tower_num：指定对象所属柱体(1-3)
  返 回 值：
  说    明：
***************************************************************************/
void move_down(int n, int x, int y, int w, int h, int speed, int y_max,
               int tower_num) {
  if (y >= y_max) {
    return;
  }
  hdc_rectangle(x, y, w, speed, HDC_COLOR[0]);
  if (y >= rectangle_Y(11))
    hdc_rectangle(rectangle_X(tower_num, 0), y, rectangle_W(0), speed,
                  HDC_COLOR[11]);
  hdc_rectangle(x, y + h, w, speed, HDC_COLOR[n]);
  if (step <= 7) {
    Sleep(sleep_time);
    if (sleep_time == 0) {
      while (_getch() != '\r')
        ;
    }
  }
  move_down(n, x, y + speed, w, h, speed, y_max, tower_num);
}

/***************************************************************************
  函数名称：move_level
  功    能：将指定方块左右移动
  输入参数：int n：指定对象编号
                   int begin_X：指定对象起始X坐标
                   int end_X：指定对象目标X坐标
                   int speed：指定对象移动速度
                   const short direction：移动方向（正为右，负为左）
  返 回 值：
  说    明：
***************************************************************************/
void move_level(int n, int begin_X, int end_X, int speed,
                const short direction) {
  if (abs(begin_X - end_X) < speed)
    return;
  if (direction == 1) {
    hdc_rectangle(begin_X, HDC_Top_Y, speed, HDC_Base_High, HDC_COLOR[0]);
    hdc_rectangle(begin_X + rectangle_W(n), HDC_Top_Y, speed, HDC_Base_High,
                  HDC_COLOR[n]);
  } else if (direction == -1) {
    hdc_rectangle(begin_X + rectangle_W(n) - speed, HDC_Top_Y, speed,
                  HDC_Base_High, HDC_COLOR[0]);
    hdc_rectangle(begin_X - speed, HDC_Top_Y, speed, HDC_Base_High,
                  HDC_COLOR[n]);
  }
  if (step <= 7) {
    Sleep(sleep_time);
    if (sleep_time == 0) {
      while (_getch() != '\r')
        ;
    }
  }
  move_level(n, begin_X + direction * speed, end_X, speed, direction);
}

/***************************************************************************
  函数名称：input_num
  功    能：处理输入及输入错误
  输入参数：提示输出字符串、最小值、最大值、超界提示字符
  返 回 值：int型
  说    明：
***************************************************************************/
int input_num(const char txt_out[], int min = 1, int max = MAX_LAYER,
              const char error_out[] = '\0') {
  int num_out;
  while (true) {
    cout << txt_out << endl;
    cin >> num_out;
    cin.clear();
    while (cin.get() != '\n')
      ;
    if (num_out >= min && num_out <= max) {
      break;
    }
  }
  return num_out;
}

/***************************************************************************
  函数名称：input_char
  功    能：处理输入及输入错误
  输入参数：提示输出字符串、最大值、最小值
  返 回 值：char型
  说    明：
***************************************************************************/
char input_char(const char txt_out[], char min = 'A', char max = 'C') {
  char char_out;
  while (true) {
    cout << txt_out << endl;
    cin >> char_out;
    cin.clear();
    while (cin.get() != '\n')
      ;
    if (char_out >= min && char_out <= max) {
      break;
    } else if (char_out >= min + 32 && char_out <= max + 32) {
      char_out -= 32;
      break;
    }
  }
  return char_out;
}

/***************************************************************************
  函数名称：all_input
  功    能：输入层数floors、起始柱src、目标柱dst和缓冲柱tmp
  输入参数：*floors
  返 回 值：无
***************************************************************************/
void all_input(int *floors, char *src, char *dst, char *tmp) {
  cout << "请输入汉诺塔的层数(1-" << MAX_LAYER << ")：";
  *floors = input_num("");
  *src = input_char("请输入起始柱(A-C)");
  while (1) {
    *dst = input_char("请输入目标柱(A-C)");
    if (*src != *dst)
      break;
    else
      cout << "目标柱" << *src << "不能与起始柱" << *dst << "相同" << endl;
  }
  *tmp = 'A' + 'B' + 'C' - *src - *dst;
}

/***************************************************************************
  函数名称：tower_init
  功    能：初始化tower和num组
  输入参数：
  返 回 值：无
***************************************************************************/
void tower_init(const int floors, const char src) {
  // 初始化起始柱的盘子
  num[0] = 0, num[1] = 0, num[2] = 0;
  if (src == 'A') {
    for (int i = floors; i >= 1; i--) {
      tower[0][num[0]] = i;
      tower[1][num[1]] = 0;
      tower[2][num[2]] = 0;
      num[0]++;
    }
  } else if (src == 'B') {
    for (int i = floors; i >= 1; i--) {
      tower[0][num[0]] = 0;
      tower[1][num[1]] = i;
      tower[2][num[2]] = 0;
      num[1]++;
    }
  } else { // src == 'C'
    for (int i = floors; i >= 1; i--) {
      tower[0][num[0]] = 0;
      tower[1][num[1]] = 0;
      tower[2][num[2]] = i;
      num[2]++;
    }
  }
}

/***************************************************************************
函数名称：move_disk
功    能：对tower中运行的数组进行同步
输入参数：int n：层数
                char src：起始柱
                char tmp：中间柱
                char dst：目标柱
返 回 值：int类型disk，即操作的盘
***************************************************************************/
int move_disk(int n, char src, char dst) {
  int disk;
  // 从源柱取出盘子
  if (src == 'A') {
    num[0]--;
    disk = tower[0][num[0]];
  } else if (src == 'B') {
    num[1]--;
    disk = tower[1][num[1]];
  } else { // *src == 'C'
    num[2]--;
    disk = tower[2][num[2]];
  }

  // 放到目标柱
  if (dst == 'A') {
    tower[0][num[0]] = disk;
    num[0]++;
  } else if (dst == 'B') {
    tower[1][num[1]] = disk;
    num[1]++;
  } else { // *dst == 'C'
    tower[2][num[2]] = disk;
    num[2]++;
  }
  return disk;
}

/***************************************************************************
  函数名称：mode_348_out(
  功    能：用于功能3&4的横向输出
  输入参数：int n：层数
                        char src：起始柱
                        char tmp：中间柱
                        char dst：目标柱
                        const int mode_num
  返 回 值：无
***************************************************************************/
void mode_348_out(char src, char dst, char tmp) {
  // 打印A柱
  cout << "A: ";
  for (int i = 0; i < num[0]; i++) {
    cout << tower[0][i] << " ";
  }
  for (int i = num[0]; i < MAX_LAYER; i++) {
    cout << "  ";
  }

  // 打印B柱
  cout << "B: ";
  for (int i = 0; i < num[1]; i++) {
    cout << tower[1][i] << " ";
  }
  for (int i = num[1]; i < MAX_LAYER; i++) {
    cout << "  ";
  }

  // 打印C柱
  cout << "C:";
  for (int i = 0; i < num[2]; i++) {
    cout << tower[2][i] << " ";
  }
  for (int i = num[2]; i < MAX_LAYER; i++) {
    cout << "  ";
  }
  cout << endl;
}

/***************************************************************************
  函数名称：vertical_out_init
  功    能：初始化立式输出
  输入参数：int x：初始x位置
                  int y：初始y位置
                  int floors：层数
                  char src：起始柱
                  char dst：目标柱
                  char tmp：中间柱
  返 回 值：无
***************************************************************************/
void vertical_out_init(int x, int y, const int floors, const char src,
                       const char dst, const char tmp) {
  cct_cls();
  cct_gotoxy(x, y);
  cout << 'A' << setw(Underpan_Distance + 1) << 'B'
       << setw(Underpan_Distance + 1) << 'C';
  cct_gotoxy(x - 2, y - 1);
  cout << setw(Underpan_Distance * 2 + 7) << setfill('=') << "";
  cout << setfill(' ');
  cct_gotoxy(x - Underpan_A_X_OFFSET, y - Underpan_A_Y_OFFSET);
  for (int i = floors; i > 0; i--) {
    cct_gotoxy(x - 1 + (src - 'A') * (Underpan_Distance + 1),
               y - 2 - floors + i);
    cout << setw(2) << i;
  }
  cct_gotoxy(x - Underpan_A_X_OFFSET, y - Underpan_A_Y_OFFSET);
  cout << "初始：";
  mode_348_out(src, dst, tmp);
}

/***************************************************************************
  函数名称：mode_234_out
  功    能：用于功能3&4的横向输出
  输入参数：int n：层数
                        char src：起始柱
                        char tmp：中间柱
                        char dst：目标柱
                        const int mode_num
  返 回 值：无
***************************************************************************/
void mode_234_out(int n, char src, char dst, char tmp, const int mode_num) {
  cout << "第 " << setw(4) << step << " 步(" << setw(2) << n << "): " << src
       << "-->" << dst << " ";
  if (mode_num == 2)
    cout << endl;
  else
    mode_348_out(src, dst, tmp);
}

/***************************************************************************
  函数名称：mode_48
  功    能：实现功能4和功能8的立式更新
  输入参数：int n：层数
                        char src：起始柱
                        char tmp：中间柱
                        char dst：目标柱
                        const int mode_num
                        int x：要求的X坐标
                        int y：要求的Y坐标
  返 回 值：无
***************************************************************************/
void mode_489(int n, char src, char dst, char tmp, const int mode_num, int x,
              int y, int disk) {
  cct_gotoxy(x + (src - 'A') * (Underpan_Distance + 1) - 1,
             y - 2 - num[src - 'A']);
  cout << "  ";
  cct_gotoxy(x + (dst - 'A') * (Underpan_Distance + 1) - 1,
             y - 1 - num[dst - 'A']);
  cout << setw(2) << disk;

  cct_gotoxy(x - Underpan_A_X_OFFSET, y - Underpan_A_Y_OFFSET);
  mode_234_out(n, src, dst, tmp, mode_num);
  if (sleep_time > 0)
    Sleep(sleep_time);
  else {
    if (mode_num == 8 && step > 7)
      return;
    while (_getch() != '\r')
      ;
  }
}

/***************************************************************************
  函数名称：mode_5
  功    能：实现功能5
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void mode_5() {
  cct_gotoxy(Status_Line_X, Status_Line_Y);
  Sleep(100);
  const int win_width = 1400, win_high = 900; // 设定屏幕宽度
  const int win_bgcolor = 7, win_fgcolor = 0;

  hdc_init(win_bgcolor, win_fgcolor, win_width, win_high);
  // 用(背景色，前景色，宽度，高度）初始化窗口
  hdc_cls();

  // 绘制底盘
  for (int i = 1; i <= 3; i++) {
    hdc_rectangle(rectangle_X(i, 11), rectangle_Y(0), rectangle_W(11),
                  HDC_Base_High, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
  }
  // 绘制立柱
  for (int i = 1; i <= 3; i++) {
    hdc_rectangle(rectangle_X(i, 0), rectangle_Y(11), rectangle_W(0),
                  12 * HDC_Base_High, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
  }
}

/***************************************************************************
  函数名称：mode_78
  功    能：完成移动
  输入参数：
  返 回 值：
  说    明：在模式7下只显示第一步
***************************************************************************/
void mode_789(int n, char src, char dst, char tmp, const int mode_num,
              int disk) {
  if (mode_num == 7 && step > 1)
    return;
  const int src_ = (int)(src - 'A');
  const int dst_ = (int)(dst - 'A');

  int disk_x = rectangle_X(src_ + 1, disk);
  int disk_y = rectangle_Y(num[src_] + 1);
  move_up(disk, disk_x, disk_y, rectangle_W(disk), HDC_Base_High, HDC_Step_Y,
          HDC_Top_Y, src_ + 1);

  short direction = 1; // 正为右，负为左
  if (src_ > dst_)
    direction = -1;
  move_level(disk, rectangle_X(src_ + 1, disk),
             disk_x = rectangle_X(dst_ + 1, disk), HDC_Step_X, direction);

  disk_x = rectangle_X(dst_ + 1, disk);
  disk_y = rectangle_Y(num[dst_]);
  move_down(disk, disk_x, HDC_Top_Y, rectangle_W(disk), HDC_Base_High,
            HDC_Step_Y, disk_y, dst_ + 1);
}

/***************************************************************************
  函数名称：hanoi_out
  功    能：在汉诺塔移动过程中负责输出
  输入参数：int n：层数
                        char src：起始柱
                        char tmp：中间柱
                        char dst：目标柱
                        const int mode_num
  返 回 值：无
***************************************************************************/
void hannoi_out(int n, char src, char dst, char tmp, const int mode_num) {
  step++;
  int disk = move_disk(n, src, dst);
  switch (mode_num) {
  case 1:
    cout << " " << n << "# " << src << "-->" << dst << endl;
    break;
  case 2:
    mode_234_out(n, src, dst, tmp, mode_num);
    break;
  case 3:
    mode_234_out(n, src, dst, tmp, mode_num);
    break;
  case 4:
    mode_489(n, src, dst, tmp, mode_num,
             MenuItem4_Start_X + Underpan_A_X_OFFSET,
             MenuItem4_Start_Y + Underpan_A_Y_OFFSET, disk);
    break;
  case 7:
    mode_789(n, src, dst, tmp, mode_num, disk);
    break;
  case 8:
    mode_489(n, src, dst, tmp, mode_num,
             MenuItem8_Start_X + Underpan_A_X_OFFSET,
             MenuItem8_Start_Y + Underpan_A_Y_OFFSET, disk);
    mode_789(n, src, dst, tmp, mode_num, disk);
    break;
  }
}

/***************************************************************************
  函数名称：hanoi
  功    能：打印n层汉诺塔的移动顺序
  输入参数：int n：层数
                        char src：起始柱
                        char tmp：中间柱
                        char dst：目标柱
                        const int mode_num
  返 回 值：无
***************************************************************************/
void hanoi(int n, char src, char tmp, char dst, const int mode_num) {
  if (n == 0) {
    return;
  }
  hanoi(n - 1, src, dst, tmp, mode_num);
  hannoi_out(n, src, dst, tmp, mode_num);
  hanoi(n - 1, tmp, src, dst, mode_num);
}

/***************************************************************************
  函数名称：mode_9
  功    能：实现功能9
  输入参数：const int mode_num
                  const char final_floors：最终层数
                  const char final_dst：最终目标柱
  返 回 值：无
***************************************************************************/
void mode_9(const int mode_num, const int final_floors, const char final_dst) {
  while (1) {

    if (num[final_dst - 'A'] == final_floors) {
      cout << endl << setw(10) << "游戏结束！" << setw(20) << ' ';
      return;
    }
    cout << endl;
    char src;
    char dst;
    bool input_out_1 = true;
    bool input_out_2 = true;
    cct_gotoxy(MenuItem9_Start_X + 59, MenuItem9_Start_Y);
    cout << setw(30) << " ";
    cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y);
    cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";

    cin >> src;
    if (src == 'Q' || src == 'q') {
      cout << endl << setw(10) << "游戏终止！！！" << setw(20) << ' ';
      return;
    } else if (src >= 'A' && src <= 'C') {
      input_out_1 = false;
    } else if (src >= 'a' && src <= 'c') {
      src = (char)(src - 32);
      input_out_1 = false;
    }

    cin >> dst;
    if (dst >= 'A' && dst <= 'C') {
      input_out_2 = false;
    } else if (dst >= 'a' && dst <= 'c') {
      dst -= 32;
      input_out_2 = false;
    }
    if (src == dst)
      input_out_2 = true;

    cin.clear();
    while (cin.get() != '\n')
      ;

    if (input_out_1 || input_out_2)
      continue;
    cct_gotoxy(MenuItem9_Start_X + 60, MenuItem9_Start_Y);
    const int src_ = int(src - 'A');
    const int dst_ = int(dst - 'A');
    char tmp = src + dst - 'A' - 'B' - 'C';
    if (num[src_] == 0) {
      cout << "  源柱为空！";
      Sleep(500);
      continue;
    } else if (tower[src_][num[src_] - 1] > tower[dst_][num[dst_] - 1] &&
               num[dst_] != 0) {
      cout << "大盘压小盘，非法移动！";
      Sleep(500);
      continue;
    }

    step++;
    int n = tower[src_][num[src_]];
    int disk = move_disk(num[src_], src, dst);
    mode_489(n, src, dst, tmp, mode_num,
             MenuItem9_Start_X + Underpan_A_X_OFFSET,
             MenuItem9_Start_Y + Underpan_A_Y_OFFSET, disk);
    mode_789(n, src, dst, tmp, mode_num, disk);
  }
}

/***************************************************************************
  函数名称：mode_6789
  功    能：实现功能6789
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void mode_6789(const int mode_num) {
  int floors;
  char src;
  char dst;
  char tmp;
  all_input(&floors, &src, &dst, &tmp);
  tower_init(floors, src);
  if (mode_num >= 8)
    sleep_time = input_num(
        "请输入移动速度(0-20：0-按回车单步演示 1-20 : 延时1-20ms", 0, 20);
  cct_cls();
  if (mode_num >= 8)
    vertical_out_init(MenuItem8_Start_X + Underpan_A_X_OFFSET,
                      MenuItem8_Start_Y + Underpan_A_Y_OFFSET, floors, src, dst,
                      tmp);
  mode_5();
  const int src_ = (int)(src - 'A');
  // 绘制盘
  for (int i = 0; i < floors; i++) {
    hdc_rectangle(rectangle_X(src_ + 1, tower[src_][i]), rectangle_Y(i + 1),
                  rectangle_W(tower[src_][i]), HDC_Base_High,
                  HDC_COLOR[tower[src_][i]]);
    Sleep(HDC_Init_Delay);
  }
  if (mode_num >= 7) {
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << floors << " 层";
    if (mode_num == 8) {
      cout << "，延时设置为" << sleep_time << "ms（前7步，后面自动变为0ms）";
    }
    if (mode_num != 9)
      hanoi(floors, src, tmp, dst, mode_num);
    else
      mode_9(mode_num, floors, dst);
  }
}

/***************************************************************************
  函数名称：easy_mode
  功    能：处理menu中1-4的简易功能
  输入参数：const int mode_num
  返 回 值：无
***************************************************************************/
void easy_mode(const int mode_num) {
  int floors;
  char src;
  char dst;
  char tmp;
  all_input(&floors, &src, &dst, &tmp);
  if (mode_num != 1) {
    step = 0;
    if (mode_num != 2) {
      tower_init(floors, src);

      if (mode_num == 4) {
        sleep_time = input_num("请输入移动速度(0-200：0-按回车单步演示;"
                               "1-200: 延时1-200ms) ",
                               0, 200);
        vertical_out_init(MenuItem4_Start_X + Underpan_A_X_OFFSET,
                          MenuItem4_Start_Y + Underpan_A_Y_OFFSET, floors, src,
                          dst, tmp);
        cct_gotoxy(Status_Line_X, Status_Line_Y);
        cout << "从 " << dst << " 移动到 " << src << "，共 " << floors
             << " 层，延时设置为 " << sleep_time << "ms";
        if (sleep_time > 0)
          Sleep(sleep_time);
        else {
          while (_getch() != '\r')
            ;
        }
      }
    }
  }
  hanoi(floors, src, tmp, dst, mode_num);
}

/***************************************************************************
  函数名称：hdc_mode
  功    能：完成功能5-9
  输入参数：const int mode_num
  返 回 值：无
***************************************************************************/
void hdc_mode(const int mode_num) {
  step = 0;
  if (mode_num == 5) {
    // mode 5 画三个圆柱
    cct_cls();
    mode_5();
    return;
  }
  // mode 6 在起始柱上画n个盘子
  // mode 7 画出第一步移动
  // mode 8 全显示移动步骤
  mode_6789(mode_num);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

// hanoi相关函数中n和disk可能指代同一含义，始终相等，不影响函数正常运行，暂不解决；
// sleep重复出现，大量_getch()重复使用，可打包函数，不影响函数正常运行，简化空间较小，暂不解决；