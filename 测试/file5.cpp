//2551746 ´óÊı¾İ ³ÂçìÃú
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream out("out.txt", ios::out);
    out << "ABC\0\x61\x62\x63" << endl;
    out.close();

    return 0;
}