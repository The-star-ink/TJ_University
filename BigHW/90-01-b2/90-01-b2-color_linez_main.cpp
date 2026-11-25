//2551746 大数据 陈珈铭
#include "90-01-b2-color_linez.h"
using namespace std;
/* 空文件，给出的目的是为了防止把文件名弄错 */

int main() {

  /* 这是一段用于验证cmd_console_tools.cpp版本是否正确的代码，判断正确的具体标准在作业文档中
   */
  cct_showstr(10, 2, "请输出雪花的大小", COLOR_BLACK, COLOR_WHITE, 2);
  cout << endl << endl;
  cct_showstr(10, 4, "╔═══汉诺塔═══╗", COLOR_BLACK, COLOR_WHITE, 1);
  cct_showstr(10, 5, "╔═══汉诺塔 ═══╗", COLOR_WHITE, COLOR_BLACK, 1);
  cct_showstr(10, 6, "╔═══H汉诺塔 ═══╗", COLOR_RED, COLOR_HYELLOW, 1);

  cct_showstr(10, 8, "**╔═╦═╗中", COLOR_CYAN, COLOR_YELLOW, 1);
  cct_showstr(10, 9, "中║测║试║**", COLOR_HBLUE, COLOR_HPINK, 1);
  cct_showstr(10, 10, "**╠═╬═╣**", COLOR_YELLOW, COLOR_HGREEN, 1);
  cct_showstr(10, 11, "中║ab║12║中", COLOR_HPINK, COLOR_HCYAN, 1);
  cct_showstr(10, 12, "**╚═╩═╝中", COLOR_GREEN, COLOR_HBLUE, 1);

  /* 本函数位于 mine_sweeper_tools.cpp 中 */
  to_be_continued("请确认上面的输出没有乱码字符相互重叠现象，如果有，说明cmd_"
                  "console_tools.cpp不对");

  // 给出后续的正常内容
  cct_cls();
  int mode = 0;
  bool exact_mode = true;
  while (1) {
    while (exact_mode) {
      cout << "--------------------------------------------" << endl;
      cout << "A.内部数组，随机生成初始5个球" << endl;
      cout << "B.内部数组，随机生成60%的球，寻找移动路径" << endl;
      cout << "C.内部数组，完整版" << endl;
      cout << "D.画出n*n的框架（无分隔线），随机显示5个球" << endl;
      cout << "E.画出n*n的框架（有分隔线），随机显示5个球" << endl;
      cout << "F.n*n的框架，60%的球，支持鼠标，完成一次移动" << endl;
      cout << "G.cmd图形界面完整版" << endl;
      cout << "Q.退出" << endl;
      cout << "------------------------------------------" << endl;
      cout << "[请选择:]";
      int temp = menu_main('A', 'q');
      if (temp > 32) {
        temp -= 32;
      }
      if (temp == 17) // 按Q/q退出
        return 0;
      else if (1 <= temp && temp <= 7) {
        mode = temp;
        break;
      }
    }
    if (mode != 5) {
      cout << "无需实现" << endl;
    } else {
      cl_runMenuE();
    }
    cct_cls();
  }
}