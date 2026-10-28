//2551746 大数据 陈珈铭
#include <fstream>
#include <iomanip>
#include <iostream>

namespace {

const int kBytesPerLine = 16; // 每行显示的字节数
const int kGroupBytes = 8;    // 第 8 个字节之后插入 '-'
const int kNameBufSize = 300; // 文件名缓冲区大小（含结尾 '\0'）

// 打印"文件名以下形式均可以"的说明，参数为标准错误流
void showUsage(std::ostream &err) {
  err << "文件名以下形式均可以:" << std::endl;
  err << "    a.txt                     : 不带路径形式" << std::endl;
  err << "    ..\\data\\b.dat             : 相对路径形式" << std::endl;
  err << "    C:\\Windows\\System32\\c.dat : 绝对相对路径形式" << std::endl;
}

// 输出一行
//   out    ：目标流（std::cout）
//   offset ：本行首字节在文件中的偏移
//   buf    ：本行数据，前 count 个字节有效
//   count  ：本行实际字节数，1 ~ 16
void dumpLine(std::ostream &out, unsigned long offset, const unsigned char *buf,
              int count) {
  out << std::hex << std::uppercase << std::setfill('0');
  out << std::setw(8) << offset << ":  "; // ① ②

  for (int i = 0; i < count; ++i) { // ③
    if (i == kGroupBytes) {
      out << "- ";
    }
    out << std::setw(2) << static_cast<unsigned int>(buf[i]) << ' ';
  }

  // 不足 16 个字节时补齐十六进制区，保证 ASCII 区总从第 63 列开始。
  // 注意：'-' 那 2 个字符是在 48 个字符之外额外插入的，不能占用字节槽位，
  //       所以 count <= 8 时要单独补 2 个空格。
  for (int i = count; i < kBytesPerLine; ++i) {
    if (i == kGroupBytes) {
      out << "  ";
    }
    out << "   ";
  }

  out << ' '; // ④

  for (int i = 0; i < count; ++i) { // ⑤
    const unsigned int ch = static_cast<unsigned int>(buf[i]);
    out << ((ch > 32U && ch < 127U) ? static_cast<char>(ch) : '.');
  }

  out << std::dec << std::setfill(' ') << std::endl;
}

} // namespace

int main() {
  showUsage(std::cerr);
  std::cerr << "请输入文件名 : ";

  char fileName[kNameBufSize] = {0};
  std::cin.getline(fileName, static_cast<std::streamsize>(sizeof(fileName)));
  // getline 会自动去掉行尾的 '\n'，不需要再手工处理

  // 必须加 ios::binary：文本方式会做字符转换（Windows 下读入时把 0D 0A 收缩成
  // 0A，遇到 0x1A 还当作文件结束符），会导致字节数变少、偏移错位。
  std::ifstream in(fileName, std::ios::binary);
  if (!in.is_open()) {
    std::cout << "输入文件" << fileName << "打开失败!" << std::endl;
    return -1;
  }

  unsigned char buf[kBytesPerLine] = {0};
  unsigned long offset = 0;

  while (true) {
    in.read(reinterpret_cast<char *>(buf),
            static_cast<std::streamsize>(kBytesPerLine));
    const std::streamsize got = in.gcount();
    if (got <= 0) {
      break; // 已到文件尾，空文件也不会多输出一行
    }

    dumpLine(std::cout, offset, buf, static_cast<int>(got));
    offset += static_cast<unsigned long>(got);

    if (got < static_cast<std::streamsize>(kBytesPerLine)) {
      break; // 不足一行，说明文件已到末尾
    }
  }

  in.close(); // 程序中打开的文件由程序自行关闭
  return 0;
}