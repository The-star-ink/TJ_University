//2551746 ´óÊı¾İ ³ÂçìÃú
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream out("out.txt", ios::out);

    out << "hello" << endl;

    out.close();

    return 0;
}
