//2551746 大数据 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

class TString {
private:
  char *content; // 指向字符串的指针
  int len;       // 字符串的长度，不含尾零
  // 除上面的两个private数据成员外，不再允许添加任何类型的数据成员

public:
  TString(const char *s = NULL);
  TString(const TString &other); // 拷贝构造函数（深拷贝）
  ~TString();
  const char *get_content() const;

  /* 根据需要补充其它的成员函数的定义(体外实现) */
  void set(const char *s);        // 用一个C字符串改变对象的值
  void set(const TString &other); // 用另一个对象改变对象的值
  int length() const;             // 取内容长度（不含尾零）
  TString &
  operator=(const TString &other); // 赋值运算符（深拷贝，防浅拷贝二次释放）
};

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：1、构造函数体外实现，不准动
            2、严格来说，在无返回类型的构造函数中用new(nothrow)是不对的，应该try-catch-throw
***************************************************************************/
TString::TString(const char *s) {
  if (s == NULL || strlen(s) == 0) {
    content = NULL;
    len = 0;
  } else {
    len = int(strlen(s)); // x64 编译下必须强制类型转换，否则会报错
    content = new (nothrow) char[len + 1];
    if (content == NULL)
      exit(-1); // 申请不到直接结束整个程序（严格来说这种处理不允许）
    strcpy(content, s);
  }
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：析构函数体外实现，不准动
***************************************************************************/
TString::~TString() {
  if (content)
    delete[] content;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：get_content()函数的体外实现，不准动
***************************************************************************/
const char *TString::get_content() const {
  if (content == NULL)
    return "<NULL>";
  else
    return content;
}

/* 在此给出其它成员函数的体外实现部分 */

/***************************************************************************
  函数名称：TString(const TString &other)
  功    能：拷贝构造函数，用已存在的对象初始化新对象
  输入参数：other - 被复制的对象（常引用，不修改它）
  返 回 值：无
  说    明：1、必须深拷贝：为新对象单独申请内存，两个对象各自释放自己的内存，
               否则默认的浅拷贝会让两个对象指向同一块内存，析构时二次释放导致崩溃
            2、other 内容为空时，新对象也为空（content = NULL，len = 0）
***************************************************************************/
TString::TString(const TString &other) {
  if (other.content == NULL || other.len == 0) {
    content = NULL;
    len = 0;
  } else {
    len = other.len;
    content = new (nothrow) char[len + 1];
    if (content == NULL)
      exit(-1); // 申请不到直接结束整个程序，与构造函数保持一致
    strcpy(content, other.content); // 连同尾零一起复制
  }
}

/***************************************************************************
  函数名称：set(const char *s)
  功    能：用C字符串（字符串常量、字符数组、字符指针）改变对象的值
  输入参数：s - 源字符串，可以为 NULL 或空串，此时对象内容变为空
  返 回 值：无
  说    明：1、先按新内容申请内存并复制，成功后再释放旧内存，
               这样即使申请失败，原对象的内容也不会被破坏
            2、s 与对象自身内容指向同一块内存时（如 s.set(s.get_content())）
               直接返回，避免"先释放、后复制"的悬空访问
            3、内容为空时统一置 content = NULL、len = 0，与构造函数一致
***************************************************************************/
void TString::set(const char *s) {
  char *p = NULL;
  int n = 0;

  if (s == content) // 自赋值，内存重叠，直接返回
    return;

  if (s != NULL && s[0] != '\0') {
    n = int(strlen(s));
    p = new (nothrow) char[n + 1];
    if (p == NULL) {
      return; // 申请失败，保持对象原值不变
    }
    strcpy(p, s);
  }

  if (content)
    delete[] content;
  content = p;
  len = n;
}

/***************************************************************************
  函数名称：set(const TString &other)
  功    能：用另一个对象的内容改变本对象的值
  输入参数：other - 源对象
  返 回 值：无
  说    明：1、参数用常引用，避免值传递时调用拷贝构造函数
            2、s.set(s) 这种自赋值要直接返回，否则会释放掉自己还要用的内存
            3、其余处理与 set(const char *) 相同
***************************************************************************/
void TString::set(const TString &other) {
  char *p = NULL;
  int n = 0;

  if (this == &other) // 自赋值
    return;

  if (other.content != NULL && other.len > 0) {
    n = other.len;
    p = new (nothrow) char[n + 1];
    if (p == NULL) {
      return; // 申请失败，保持对象原值不变
    }
    strcpy(p, other.content);
  }

  if (content)
    delete[] content;
  content = p;
  len = n;
}

/***************************************************************************
  函数名称：length()
  功    能：取对象内容的长度
  输入参数：无
  返 回 值：内容的字符个数（不含尾零），内容为空时返回 0
  说    明：常成员函数，const 对象也能调用
***************************************************************************/
int TString::length() const { return len; }

/***************************************************************************
  函数名称：operator=(const TString &other)
  功    能：赋值运算符重载，把一个对象赋给另一个已存在的对象
  输入参数：other - 源对象
  返 回 值：*this 的引用，以支持 a = b = c 这样的连续赋值
  说    明：1、默认的赋值运算符是浅拷贝，会造成两个对象共用一块内存、二次释放，
               所以必须自己实现深拷贝
            2、先判断自赋值 this == &other，再处理"释放旧内存"的顺序
            3、main 中没有直接用到，但按"三法则"与拷贝构造函数配套提供更完整
***************************************************************************/
TString &TString::operator=(const TString &other) {
  char *p = NULL;
  int n = 0;

  if (this == &other)
    return *this;

  if (other.content != NULL && other.len > 0) {
    n = other.len;
    p = new (nothrow) char[n + 1];
    if (p == NULL)
      return *this; // 申请失败，保持对象原值不变
    strcpy(p, other.content);
  }

  if (content)
    delete[] content;
  content = p;
  len = n;

  return *this;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数不准动
***************************************************************************/
int main() {
  if (1) {
    TString s1;              // s1为NULL
    TString s2("teststr2");  // s2为"teststr2"
    TString s3 = "teststr3"; // s3为"teststr3"
    TString s4 = NULL;       // s4为NULL
    TString s5 = "";         // s5为NULL

    cout << "定义对象并初始化测试(NULL及字符串常量初始化)" << endl;

    cout << "s1应该是<NULL>，  实际输出：" << s1.get_content() << endl;
    cout << "s2应该是teststr2，实际输出：" << s2.get_content() << endl;
    cout << "s3应该是teststr3，实际输出：" << s3.get_content() << endl;
    cout << "s4应该是<NULL>，  实际输出：" << s4.get_content() << endl;
    cout << "s5应该是<NULL>，  实际输出：" << s5.get_content() << endl;

    cout << endl << "任意键继续..." << endl;
    getchar();
  }

  if (1) {
    TString s0 = "teststr1";
    const char *str1 = "teststr2";
    const char str2[] = "teststr3";
    const char *str3 = NULL;
    const char str4[] = "";
    TString s1 = s0, s2 = str1, s3 = str2, s4 = str3, s5 = str4;

    cout << "定义对象并初始化测试(TString对象及字符指针、字符数组)" << endl;

    cout << "s1应该是teststr1，实际输出：" << s1.get_content() << endl;
    cout << "s2应该是teststr2，实际输出：" << s2.get_content() << endl;
    cout << "s3应该是teststr3，实际输出：" << s3.get_content() << endl;
    cout << "s4应该是<NULL>，  实际输出：" << s4.get_content() << endl;
    cout << "s5应该是<NULL>，  实际输出：" << s5.get_content() << endl;

    cout << endl << "任意键继续..." << endl;
    getchar();
  }

  if (1) {
    TString s0 = "hello";
    char str1[] = "tong\0\0\0\0";
    const char *str2 = "ji";
    const char *str3 = NULL;
    const char *str4 = "";

    cout << "通过set函数将字符串赋值给对象的测试" << endl;

    cout << "s0应为hello， 实际输出：" << s0.get_content() << endl;

    s0.set(str1);
    cout << "s0应为tong，  实际输出：" << s0.get_content() << endl;

    s0.set(str2);
    cout << "s0应为ji，    实际输出：" << s0.get_content() << endl;

    s0.set(str3);
    cout << "s0应为<NULL>，实际输出：" << s0.get_content() << endl;

    s0.set(str4);
    cout << "s0应为<NULL>，实际输出：" << s0.get_content() << endl;

    s0.set(strcat(str1, str2));
    cout << "s0应为tongji，实际输出：" << s0.get_content() << endl;

    s0.set(strcpy(str1, str2));
    cout << "s0应为ji，    实际输出：" << s0.get_content() << endl;

    cout << endl << "任意键继续..." << endl;
    getchar();
  }

  if (1) {
    TString s0 = "hello";
    TString s1 = "tongji";
    TString s2;
    TString s3(s2);

    cout << "通过set函数将一个对象的字符串赋值给另一个对象的测试" << endl;

    cout << "s0应为hello， 实际输出：" << s0.get_content() << endl;

    s0.set(s1);
    cout << "s0应为tongji，实际输出：" << s0.get_content() << endl;

    s0.set(s2);
    cout << "s0应为<NULL>，实际输出：" << s0.get_content() << endl;

    s0.set(s3);
    cout << "s0应为<NULL>，实际输出：" << s0.get_content() << endl;

    cout << endl << "任意键继续..." << endl;
    getchar();
  }

  if (1) {
    TString s1 = "hello";
    TString s2;
    char str1[] = "tong\0\0\0\0";
    const char *str2 = "ji";

    cout << "求长度测试(length()函数)" << endl;

    cout << "s1为hello， 长度应为5，实际：" << s1.length() << endl;
    cout << "s2为<NULL>，长度应为0，实际：" << s2.length() << endl;

    s2.set(strcat(str1, str2));
    cout << "s2为tongji，长度应为6，实际：" << s2.length() << endl;

    cout << endl << "任意键继续..." << endl;
    getchar();
  }

  return 0;
}
