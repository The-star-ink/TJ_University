// 2551746 计算机 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>

/***************************************************************************
  函数名称：definite_integration
  功    能：用矩形法求定积分的通用函数（每个小矩形取右端点的高）
  输入参数：double (*f)(double)  被积函数指针
            double low          积分下限
            double high         积分上限
            int    n            区间划分数量
  返 回 值：定积分的近似值
  说    明：函数内部不允许出现任何选择语句（if / switch / 条件表达式），
            被积函数的选择完全由函数指针 f 完成，故本函数对 sin、cos、
            exp 通用
***************************************************************************/
double definite_integration(double (*f)(double), double low, double high,
                            int n) {
  int i;
  double dx = (high - low) / n;
  double sum = 0.0;

  for (i = 1; i <= n; i++) {
    sum += f(low + i * dx) * dx; /* 第 i 个矩形取右端点，宽 dx */
  }

  return sum;
}

int main() {
  int n;
  double low, high, value;

  printf("请输入sinxdx的下限、上限及区间划分数量\n");
  scanf("%lf %lf %d", &low, &high, &n);
  value =
      definite_integration(sin, low, high, n); /* sin 为 math.h 中已有函数 */
  printf("sinxdx[%g~%g/n=%d] : %g\n", low, high, n, value);

  printf("请输入cosxdx的下限、上限及区间划分数量\n");
  scanf("%lf %lf %d", &low, &high, &n);
  value =
      definite_integration(cos, low, high, n); /* cos 为 math.h 中已有函数 */
  printf("cosxdx[%g~%g/n=%d] : %g\n", low, high, n, value);

  printf("请输入e^xdx的下限、上限及区间划分数量\n");
  scanf("%lf %lf %d", &low, &high, &n);
  value =
      definite_integration(exp, low, high, n); /* exp 为 math.h 中已有函数 */
  printf("e^xdx[%g~%g/n=%d] : %g\n", low, high, n, value);

  return 0;
}