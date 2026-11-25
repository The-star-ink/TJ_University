//2551746 大数据 陈珈铭
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "../include/cmd_console_tools.h"
#include <iostream>
#define COLOR_LINEZ_H
using namespace std;

/* ---- 允许的全局常变量（不使用全局变量/可写数组/指针） ---- */
const int MIN_SIZE = 7;
const int MAX_SIZE = 9;
const int BALL_NUM = 5;
const int COLOR_NUM = 7;

/* 11 个符号的下标：0 左上 1 上中 2 右上 3 左中 4 十字
                   5 右中 6 左下 7 下中 8 右下 9 竖 10 横 */
const int SYM_LT = 0, SYM_TT = 1, SYM_RT = 2;
const int SYM_LM = 3, SYM_CC = 4, SYM_RM = 5;
const int SYM_LB = 6, SYM_BT = 7, SYM_RB = 8;
const int SYM_V = 9, SYM_H = 10;
const int SYM_COUNT = 11;

/* 四种中文线型，每种 11 个制表符（可与你的附件对照后替换） */
const char *const LINE_TYPE_TABLE[4][SYM_COUNT] = {
    {"╔", "╦", "╗", "╠", "╬", "╣", "╚", "╩", "╝", "║", "═"}, /* 双线   */
    {"┌", "┬", "┐", "├", "┼", "┤", "└", "┴", "┘", "│", "─"}, /* 单线   */
    {"┏", "┳", "┓", "┣", "╋", "┫", "┗", "┻", "┛", "┃", "━"}, /* 粗单线 */
    {"╒", "╤", "╕", "╞", "╪", "╡", "╘", "╧", "╛", "│", "─"}  /* 混合线 */
};
const int LINE_TYPE_DEFAULT = 0; /* 菜单 E 选用的线型，可改 0~3 */

/* 几何参数：中文制表符占 2 列，故以“半角列/字符行”为单位 */
const int BOARD_TOP = 3;  /* 边框起始行（第 1 行留作提示信息） */
const int BOARD_LEFT = 2; /* 边框起始列 */
const int BORDER_W = 3;   /* 一条竖线占的列数 */
const int CELL_W = 1;     /* 单元格内部宽（列） */
const int CELL_STEP = BORDER_W + CELL_W;

const char *const BALL_CHAR = "●";
const int BALL_COLOR[COLOR_NUM] = {4, 2, 6, 1, 5, 3, 14}; /* 7 种球色 */

/* ---- 函数原型 ---- */
void cl_initConsole();
int cl_inputSize(bool hang);
void cl_genBalls(int n, int ballRow[], int ballCol[], int ballColor[]);
void cl_drawBoard(int n, int lineType, const int ballRow[], const int ballCol[],
                  const int ballColor[]);
void cl_showScreenInfo(int rows, int cols);
bool cl_waitEnd();
void cl_runMenuE();
void to_be_continued(const char *prompt, const int X = 0, const int Y = 22);
int menu_main(char min, char max);
