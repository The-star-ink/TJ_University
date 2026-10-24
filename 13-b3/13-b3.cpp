//2551746 计算机 陈珈铭
#include <iostream>
using namespace std;
int main() {
  const char *month[] = {"January",   "February", "March",    "April",
                         "May",       "June",     "July",     "August",
                         "September", "October",  "November", "December"};
  int n;
  cout << "请输入月份(1-12)" << endl;
  cin >> n;
  if (!cin || n < 1 || n > 12)
    cout << "Invalid" << endl;
  else
    cout << month[n - 1] << endl;
  return 0;
}