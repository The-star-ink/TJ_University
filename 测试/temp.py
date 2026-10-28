with open("out.txt","r") as n:
    num = int(n.read())

OUT = True
TEXT = r"""
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main()
{
    ofstream out("out.txt", ios::out);
    out << "ABC\x1\x2\xff\t\v\b\175()-=def"<<endl;
    out.close();

    ifstream in("out.txt", ios::in); //可加ios::binary
    int c=0;
    char ch;
    while((ch=in.get())!=EOF) {
        c++;
        }
    cout << c << endl;
    in.close();

    return 0;
}
"""
num += 1
with open(f"file{num}.cpp", "w") as f:
    f.write(TEXT)
    print(f"file{num}.cpp")
with open("out.txt","w") as n:
    n.write(str(num))