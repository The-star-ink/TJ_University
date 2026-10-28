//2551746 大数据 陈珈铭
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main()
{   
    ofstream out("out.txt", ios::out);
    out << "hello
" << endl; //模拟Windows格式
    out.close();

    char str[80];
    ifstream in("out.txt", ios::in);
    in.getline(str, 80);
    cout << strlen(str) << endl;
    cout << in.peek() << endl;
    in.close();

    return 0;
}
