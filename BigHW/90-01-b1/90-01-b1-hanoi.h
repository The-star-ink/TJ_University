//2551746 大数据 陈珈铭
#pragma once

/* 将所有const常量定义的头文件包含进来 */
#include "../include/cmd_console_tools.h"
#include "../include/cmd_hdc_tools.h"
#include "90-01-b1-hanoi_const_value.h"

/* ------------------------------------------------------------------------------------------------------

 本文件功能：
1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp
能相互访问函数的函数声明
2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
------------------------------------------------------------------------------------------------------
*/
int menu_main(int min = 0, int max = 9);
void easy_mode(int mode_num);
void hdc_mode(const int mode);
void hannoi_out(int n, char src, char tmp, char dst, int mode_num);