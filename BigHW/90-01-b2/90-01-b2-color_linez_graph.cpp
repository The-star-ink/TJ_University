//2551746 大数据 陈珈铭
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "90-01-b2-color_linez.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

/* ---------------------------------------------------------------------------
   版面参数：与 90-b2-demo 演示版保持一致
   ---------------------------------------------------------------------------
 */
#define BALL_CH "〇"       /* demo 中格子里使用的字符 */
#define FONT_NAME "新宋体" /* 作业要求的字体 */

const int WIDE = 2;      /* 一个汉字（中文制表符）占 2 个半角列 */
const int BOARD_ROW = 1; /* 棋盘左上角所在行（第 0 行放“屏幕”信息） */
const int BOARD_COL = 0; /* 棋盘左上角所在列 */

const int LINE_BG = COLOR_HWHITE; /* 边框背景：亮白 */
const int LINE_FG = COLOR_BLACK;  /* 边框前景：黑 */
const int CELL_FG = COLOR_HWHITE; /* 格内字符：亮白 */
const int TEXT_BG = COLOR_BLACK;  /* 提示文字背景 */
const int TEXT_FG = COLOR_WHITE;  /* 提示文字前景 */

/* 与 demo 一致的 7 种球色（亮色系，作为格子背景色，球字符用亮白） */
const int BALL_BG[COLOR_NUM] = {COLOR_HBLACK, COLOR_HBLUE, COLOR_HGREEN,
                                COLOR_HCYAN,  COLOR_HRED,  COLOR_HPINK,
                                COLOR_HYELLOW};

/***************************************************************************
  函数名称：cl_intToStr
  功    能：把非负整数转换成字符串（避免使用 printf/sprintf）
  输入参数：v：待转换的整数；out：保存结果的字符数组
  返 回 值：无
***************************************************************************/
static void cl_intToStr(int v, char *out) {
  char tmp[12];
  int i = 0;
  if (v == 0) {
    out[0] = '0';
    out[1] = '\0';
    return;
  }
  while (v > 0) {
    tmp[i++] = char('0' + v % 10);
    v /= 10;
  }
  for (int j = 0; j < i; ++j)
    out[j] = tmp[i - 1 - j];
  out[i] = '\0';
}

/***************************************************************************
  函数名称：cl_appendStr
  功    能：把一个字符串接到另一个字符串末尾（代替 strcat，避免安全告警）
  输入参数：dst：目标串；src：要追加的串
  返 回 值：无
***************************************************************************/
static void cl_appendStr(char *dst, const char *src) {
  while (*dst != '\0')
    ++dst;
  while (*src != '\0')
    *dst++ = *src++;
  *dst = '\0';
}

/***************************************************************************
  函数名称：cl_isFontName
  功    能：比较两个字体名是否完全相同
  输入参数：face：控制台当前字体名；name：待比较的字体名
  返 回 值：true 表示相同
***************************************************************************/
static bool cl_isFontName(const wchar_t *face, const wchar_t *name) {
  while (*name != 0 && *face == *name) {
    ++face;
    ++name;
  }
  return (*name == 0) && (*face == 0);
}

/***************************************************************************
  函数名称：cl_ensureFont
  功    能：保证控制台字体为“新宋体”，但不改变字号、不改变窗口大小
  输入参数：无
  返 回 值：无
  说    明：字号沿用当前控制台已有的高度，因此窗口内文字大小保持不变
***************************************************************************/
static void cl_ensureFont() {
  HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_FONT_INFOEX cfi = {0};
  int high = 16; /* 取不到当前字号时使用的兜底高度 */

  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  if (GetCurrentConsoleFontEx(hout, FALSE, &cfi) != 0 && cfi.dwFontSize.Y > 0)
    high = cfi.dwFontSize.Y;
  if (cl_isFontName(cfi.FaceName, L"新宋体") ||
      cl_isFontName(cfi.FaceName, L"NSimSun"))
    return; /* 已经是新宋体，不需要改动 */

  cct_setfontsize(FONT_NAME, high, 0); /* 只换字体，字号沿用原来的 */
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  if (GetCurrentConsoleFontEx(hout, FALSE, &cfi) != 0 &&
      !cl_isFontName(cfi.FaceName, L"新宋体") &&
      !cl_isFontName(cfi.FaceName, L"NSimSun"))
    cct_setfontsize("NSimSun", high, 0); /* 中文名设置失败时用英文名再试一次 */
}

/***************************************************************************
  函数名称：cl_initConsole
  功    能：设置字体为“新宋体”，清屏并隐藏光标
  输入参数：无
  返 回 值：无
  说    明：按作业要求不改变窗口大小，也不改变窗口内的文字大小
***************************************************************************/
void cl_initConsole() {
  cl_ensureFont();
  cct_cls();
  cct_setcursor(CCT_CURSOR_INVISIBLE); /* 画框时不需要光标 */
  cct_setcolor(TEXT_BG, TEXT_FG);
}

/***************************************************************************
  函数名称：cl_inputOne
  功    能：显示提示并读入一个限定在 MIN_SIZE~MAX_SIZE 之间的整数
  输入参数：prompt：提示文字
  返 回 值：合法的整数
  说    明：用 cin.getline 读取整行，避免输入过长时越界
***************************************************************************/
static int cl_inputOne(const char *prompt) {
  char buf[32];
  int n = 0;
  for (;;) {
    cout << prompt;
    cin.getline(buf, sizeof(buf)); /* 最多读入 31 个字符，不会越界 */
    if (cin.fail()) {              /* 本行过长，丢弃剩余部分 */
      cin.clear();
      cin.ignore(1024, '\n');
    } else if (buf[0] >= '0' && buf[0] <= '9' && buf[1] == '\0') {
      n = buf[0] - '0'; /* 只接受一位数字 */
      if (n >= MIN_SIZE && n <= MAX_SIZE)
        break;
    }
    cout << "输入有误，请重新输入 7~9 之间的整数！" << endl;
  }
  return n;
}

/***************************************************************************
  函数名称：cl_inputSize
  功    能：读入行数或列数
  输入参数：hang：true 表示读入行数，false 表示读入列数
  返 回 值：合法的整数（MIN_SIZE~MAX_SIZE 之间）
  说    明：行数和列数是分开输入的，所以本函数按 hang 显示不同的提示
***************************************************************************/
int cl_inputSize(bool hang) {
  return cl_inputOne(hang ? "请输入行数(7-9)：" : "请输入列数(7-9)：");
}

/***************************************************************************
  函数名称：cl_genBallsRC
  功    能：在 rows*cols 范围内随机生成 BALL_NUM 个互不重叠的球及颜色
  输入参数：rows/cols：行数、列数；ballRow/ballCol/ballColor：球的行、列、颜色
  返 回 值：无
***************************************************************************/
static void cl_genBallsRC(int rows, int cols, int ballRow[], int ballCol[],
                          int ballColor[]) {
  static int call = 0; /* 静态局部变量：每调用一次换一个随机种子 */
  srand((unsigned)time(NULL) + (unsigned)(call++));
  for (int i = 0; i < BALL_NUM; ++i) {
    bool dup;
    int r = 0, c = 0;
    do {
      r = rand() % rows;
      c = rand() % cols;
      dup = false;
      for (int j = 0; j < i; ++j)
        if (ballRow[j] == r && ballCol[j] == c)
          dup = true;
    } while (dup);
    ballRow[i] = r;
    ballCol[i] = c;
    ballColor[i] = i; /* 5 个球颜色互不相同 */
  }
}

/***************************************************************************
  函数名称：cl_genBalls
  功    能：在 n*n 范围内随机生成 BALL_NUM 个互不重叠的球及颜色
  输入参数：n：行列值；ballRow/ballCol/ballColor：保存球的行、列、颜色
  返 回 值：无
***************************************************************************/
void cl_genBalls(int n, int ballRow[], int ballCol[], int ballColor[]) {
  cl_genBallsRC(n, n, ballRow, ballCol, ballColor);
}

/***************************************************************************
  函数名称：cl_drawHLine
  功    能：画出第 r 条横线（r=0 为上边框，r=rows 为下边框，其余为分隔线）
  输入参数：rows/cols：行数、列数；lineType：线型；r：横线序号
  返 回 值：无
  说    明：一条横线 = 左端符号 + cols 段“横”符号 + 各连接点 + 右端符号
***************************************************************************/
static void cl_drawHLine(int rows, int cols, int lineType, int r) {
  const char *const *s = LINE_TYPE_TABLE[lineType];
  const char *lc = (r == 0) ? s[SYM_LT] : ((r == rows) ? s[SYM_LB] : s[SYM_LM]);
  const char *jc = (r == 0) ? s[SYM_TT] : ((r == rows) ? s[SYM_BT] : s[SYM_CC]);
  const char *rc = (r == 0) ? s[SYM_RT] : ((r == rows) ? s[SYM_RB] : s[SYM_RM]);
  int y = BOARD_ROW + r * 2;

  cct_showstr(BOARD_COL, y, lc, LINE_BG, LINE_FG); /* 左端 */
  for (int c = 0; c < cols; ++c) {
    int x = BOARD_COL + (c + 1) * CELL_STEP; /* 下一个竖线的列位置 */
    cct_showstr(x - WIDE, y, s[SYM_H], LINE_BG, LINE_FG); /* 一段横线 */
    cct_showstr(x, y, (c + 1 == cols) ? rc : jc, LINE_BG, LINE_FG); /* 连接点 */
  }
}

/***************************************************************************
  函数名称：cl_drawVLines
  功    能：画出第 r 行（内容行）上的 cols+1 条竖线
  输入参数：cols：列数；lineType：线型；r：内容行序号
  返 回 值：无
***************************************************************************/
static void cl_drawVLines(int cols, int lineType, int r) {
  const char *const *s = LINE_TYPE_TABLE[lineType];
  int y = BOARD_ROW + 1 + r * 2;

  for (int c = 0; c <= cols; ++c)
    cct_showstr(BOARD_COL + c * CELL_STEP, y, s[SYM_V], LINE_BG, LINE_FG);
}

/***************************************************************************
  函数名称：cl_findBall
  功    能：查找第 r 行第 c 列是否有球
  输入参数：r/c：行、列；ballRow/ballCol：球的位置
  返 回 值：球的下标，没有球返回 -1
***************************************************************************/
static int cl_findBall(int r, int c, const int ballRow[], const int ballCol[]) {
  for (int i = 0; i < BALL_NUM; ++i)
    if (ballRow[i] == r && ballCol[i] == c)
      return i;
  return -1;
}

/***************************************************************************
  函数名称：cl_drawCells
  功    能：画出第 r 行（内容行）上 cols 个格子里的字符：有球用球色，空格用白底
  输入参数：cols：列数；r：内容行序号；ballRow/ballCol/ballColor：球的信息
  返 回 值：无
***************************************************************************/
static void cl_drawCells(int cols, int r, const int ballRow[],
                         const int ballCol[], const int ballColor[]) {
  int y = BOARD_ROW + 1 + r * 2;

  for (int c = 0; c < cols; ++c) {
    int k = cl_findBall(r, c, ballRow, ballCol);
    int bg = (k < 0) ? LINE_BG : BALL_BG[ballColor[k] % COLOR_NUM];
    cct_showstr(BOARD_COL + c * CELL_STEP + WIDE, y, BALL_CH, bg, CELL_FG);
  }
}

/***************************************************************************
  函数名称：cl_drawBoardRC
  功    能：画出整个 rows*cols 的框架（含分隔线）及初始的五个彩球
  输入参数：rows/cols：行数、列数；lineType：线型；球的位置与颜色
  返 回 值：无
  说    明：逐行画“横线 + 竖线 + 格子内容”，行列数全部由循环控制
***************************************************************************/
static void cl_drawBoardRC(int rows, int cols, int lineType,
                           const int ballRow[], const int ballCol[],
                           const int ballColor[]) {
  for (int r = 0; r <= rows; ++r) {
    cl_drawHLine(rows, cols, lineType, r);
    if (r == rows)
      break;
    cl_drawVLines(cols, lineType, r);
    cl_drawCells(cols, r, ballRow, ballCol, ballColor);
  }
}

/***************************************************************************
  函数名称：cl_drawBoard
  功    能：画出 n*n 的框架（含分隔线）及初始的五个彩球
  输入参数：n：行列值；lineType：线型；球的位置与颜色
  返 回 值：无
  说    明：方阵的情形，行列相等，直接转调 cl_drawBoardRC
***************************************************************************/
void cl_drawBoard(int n, int lineType, const int ballRow[], const int ballCol[],
                  const int ballColor[]) {
  cl_drawBoardRC(n, n, lineType, ballRow, ballCol, ballColor);
}

/***************************************************************************
  函数名称：cl_showScreenInfo
  功    能：在第 0 行显示“屏幕：xx行xx列”
  输入参数：rows：完整显示所需行数；cols：完整显示所需列数
  返 回 值：无
***************************************************************************/
void cl_showScreenInfo(int rows, int cols) {
  char buf[64], num[12];

  buf[0] = '\0';
  cl_appendStr(buf, "屏幕：");
  cl_intToStr(rows, num);
  cl_appendStr(buf, num);
  cl_appendStr(buf, "行");
  cl_intToStr(cols, num);
  cl_appendStr(buf, num);
  cl_appendStr(buf, "列");
  cct_showstr(BOARD_COL, 0, buf, TEXT_BG, TEXT_FG);
}

/***************************************************************************
  函数名称：cl_promptRow
  功    能：取得/设置“本小题结束”提示所在的行（紧贴棋盘下方）
  输入参数：无
  返 回 值：提示行号的引用，可直接赋值
  说    明：用静态局部变量保存，避免使用全局变量
***************************************************************************/
static int &cl_promptRow() {
  static int row = BOARD_ROW + MAX_SIZE * 2 + 1;
  return row;
}

/***************************************************************************
  函数名称：cl_waitEnd
  功    能：等待用户输入 End，输入 End 返回 true，否则返回 false
  输入参数：无
  返 回 值：true 表示可以返回菜单
  说    明：必须区分大小写地输入 End
***************************************************************************/
bool cl_waitEnd() {
  char buf[32];

  cct_setcolor(TEXT_BG, TEXT_FG);
  cct_showstr(BOARD_COL, cl_promptRow(), "本小题结束，请输入End继续           ",
              TEXT_BG, TEXT_FG);
  int x, y;
  cct_getxy(x, y);
  cct_gotoxy(x - 10, y);
  cin.getline(buf, sizeof(buf));
  if (cin.fail()) {
    cin.clear();
    cin.ignore(1024, '\n');
    return false;
  }
  for (int i = 0; i <= 3; i++) {
    if (buf[i] > 96)
      buf[i] -= 32;
  }
  return strcmp(buf, "END") == 0;
}

/***************************************************************************
  函数名称：cl_runMenuE
  功    能：菜单项 E 的总控：分行分列输入、生成球、画框架、等待 End 返回
  输入参数：无
  返 回 值：无
  说    明：行数和列数是分开输入的，因此允许 rows != cols 的矩形棋盘
***************************************************************************/
void cl_runMenuE() {
  int ballRow[BALL_NUM], ballCol[BALL_NUM], ballColor[BALL_NUM];
  int rows, cols, needRows, needCols;

  cl_initConsole();
  rows = cl_inputSize(true);  /* 先输入行数 */
  cols = cl_inputSize(false); /* 再输入列数 */
  cl_genBallsRC(rows, cols, ballRow, ballCol, ballColor);

  cct_cls();
  cl_promptRow() = BOARD_ROW + rows * 2 + 1; /* 提示紧贴在棋盘下方 */
  needRows =
      BOARD_TOP + (rows * 2 + 1) + 1; /* 完整显示所需行数（与 demo 一致） */
  needCols = (cols + 1) * BORDER_W +
             cols * CELL_W; /* 完整显示所需列数（与 demo 一致） */
  cl_showScreenInfo(needRows, needCols);
  cl_drawBoardRC(rows, cols, LINE_TYPE_DEFAULT, ballRow, ballCol, ballColor);

  while (!cl_waitEnd()) { /* 必须输入 End 才能返回菜单 */
    ;
  }

  cct_setcursor(CCT_CURSOR_VISIBLE_NORMAL); /* 恢复光标 */
  cct_setcolor(COLOR_BLACK, COLOR_WHITE);
  cct_cls(); /* 清屏，方便主菜单重新显示 */
}
