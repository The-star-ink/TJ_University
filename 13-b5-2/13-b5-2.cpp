//2551746 大数据 陈珈铭
#include <iomanip>
#include <iostream>
using namespace std;

const char *sp = "=====================================";

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：每个数字宽度为8，右对齐
***************************************************************************/
template <typename T, int R, int C>
void matrix_print(const char *title, const T (&mat)[R][C]) {
  cout << title << "行=" << R << " 列=" << C << endl;

  for (int i = 0; i < R; ++i) {
    for (int j = 0; j < C; ++j)
      cout << setw(8) << right << mat[i][j];
    cout << endl;
  }

  return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
template <typename T3, typename T1, typename T2, int R, int C>
void matrix_addition(T3 (&dst)[R][C], T1 (&a)[R][C], T2 (&b)[R][C]) {
  cout << "源矩阵1 : 行=" << R << " 列=" << C << endl;

  for (int i = 0; i < R; ++i)
    for (int j = 0; j < C; ++j)
      dst[i][j] = static_cast<T3>(a[i][j] + b[i][j]);

  return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
template <typename T3, typename T1, int R1, int C1, typename T2, int C2>
void matrix_multiplication(T3 (&dst)[R1][C2], T1 (&a)[R1][C1],
                           T2 (&b)[C1][C2]) {
  cout << "源矩阵1 : 行=" << R1 << " 列=" << C1 << endl;

  for (int i = 0; i < R1; ++i)
    for (int j = 0; j < C2; ++j) {
      double s = 0.0; // 高精度累加器
      for (int k = 0; k < C1; ++k)
        s += a[i][k] * b[k][j];
      dst[i][j] = static_cast<T3>(s); // 只在最后落到目标类型（存进 int 会截断）
    }

  return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数不准更改
***************************************************************************/
int main() {
  int t1[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
  int t2[3][4] = {{12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
  int t3[4][2] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
  int t_add[3][4], t_mul[3][2];

  cout << sp << endl;
  matrix_print("加法运算，源矩阵1：", t1);
  matrix_print("加法运算，源矩阵2：", t2);
  matrix_addition(t_add, t1,
                  t2); // 将t1和t2的和放入t_add中，人工保证三个矩阵行列一致
  matrix_print("加法运算，和矩阵 ：", t_add);
  cout << sp << endl;
  matrix_print("乘法运算，源矩阵1：", t1);
  matrix_print("乘法运算，源矩阵2：", t3);
  matrix_multiplication(
      t_mul, t1, t3); // 将t1和t2的和放入t_add中，人工保证三个矩阵行列一致
  matrix_print("乘法运算，积矩阵 ：", t_mul);
  cout << sp << endl;

  double d1[2][4] = {{1.1, 2.2, 3.3, 4.4}, {5.5, 6.6, 7.7, 8.8}};
  double d2[2][4] = {{8.8, 7.7, 6.6, 5.5}, {4.4, 3.3, 2.2, 1.1}};
  double d_add[2][4];
  /* 不要问为什么矩阵乘法数据类型不同，故意的 */
  float f3[4][3] = {{12.12f, 11.11f, 10.10f},
                    {9.9f, 8.8f, 7.7f},
                    {6.6f, 5.5f, 4.4f},
                    {3.3f, 2.2f, 1.1f}};
  int i_mul[2][3];

  matrix_print("加法运算，源矩阵1：", d1);
  matrix_print("加法运算，源矩阵2：", d2);
  matrix_addition(d_add, d1,
                  d2); // 将d1和d2的和放入d_add中，人工保证三个矩阵行列一致
  matrix_print("加法运算，和矩阵 ：", d_add);
  cout << sp << endl;
  matrix_print("乘法运算，源矩阵1：", d1);
  matrix_print("乘法运算，源矩阵2：", f3);
  matrix_multiplication(
      i_mul, d1, f3); // 将t1和t2的和放入t_add中，人工保证三个矩阵行列一致
  matrix_print("乘法运算，积矩阵 ：", i_mul);
  cout << sp << endl;

  return 0;
}
