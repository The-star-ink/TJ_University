//2551746 ´óÊı¾İ ³ÂçìÃú
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  ofstream out("out.txt", ios::out);

  out << "hello" << endl;

  out.close();

  return 0;
}
