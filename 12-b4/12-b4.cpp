// 2551746 大数据 陈珈铭
#include <cstring>
#include <fstream>
#include <iostream>

namespace {

const int kBytesPerLine = 16;  // 每行字节数
const int kAsciiColumn = 62;   // ASCII 区在行中的起始下标（0 基）
const int kLineBufSize = 1024; // 行缓冲区大小
const int kNameBufSize = 300;  // 文件名缓冲区大小

// 单个字符转 16 进制数值；不是 16 进制字符时返回 -1
int hexValue(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }
  return -1;
}

void showUsage(std::ostream &err) {
  err << "文件名以下形式均可以:" << std::endl;
  err << "    a.hex                     : 不带路径形式" << std::endl;
  err << "    ..\\data\\b.txt             : 相对路径形式" << std::endl;
  err << "    C:\\Windows\\System32\\c.hex : 绝对相对路径形式" << std::endl;
}

// 解析一行
//   line   ：已去掉行尾 '\r' '\n' 的一行
//   offset ：输出，本行首字节的偏移
//   buf    ：输出，最多 kBytesPerLine 个字节
//   count  ：输出，本行实际字节数
//   返回是否解析成功
bool parseLine(const char *line, unsigned long &offset, unsigned char *buf,
               int &count) {
  const int len = static_cast<int>(std::strlen(line));

  // ① 8 位 16 进制偏移 + 冒号
  if (len < 9 || line[8] != ':') {
    return false;
  }
  unsigned long value = 0;
  for (int i = 0; i < 8; ++i) {
    const int digit = hexValue(line[i]);
    if (digit < 0) {
      return false;
    }
    value = value * 16UL + static_cast<unsigned long>(digit);
  }
  offset = value;

  // ② 由行宽推算本行应有的字节数，用于在十六进制区里及时收手
  int expect = kBytesPerLine;
  if (len >= kAsciiColumn && line[kAsciiColumn - 1] == ' ') {
    expect = len - kAsciiColumn;
    if (expect < 1 || expect > kBytesPerLine) {
      expect = kBytesPerLine;
    }
  }

  // ③ 扫描十六进制区：跳过空格和 '-'，每 2 个 16 进制字符构成 1 个字节
  //    从下标 11 开始：0~7 是偏移，8 是冒号，9、10 是冒号后的 2 个空格。
  //    不能从 9 开始，否则这两个空格会被下面的"连续 2 个空格"规则误判为结束。
  count = 0;
  int i = 11;
  while (i < len && count < expect) {
    const char c = line[i];
    if (c == ' ') {
      // 连续 2 个及以上空格，说明十六进制区已结束（短行末尾是补位空格）
      if (i + 1 < len && line[i + 1] == ' ') {
        break;
      }
      ++i;
    } else if (c == '-') {
      ++i;
    } else {
      const int high = hexValue(c);
      const int low = (i + 1 < len) ? hexValue(line[i + 1]) : -1;
      if (high < 0 || low < 0) {
        break;
      }
      buf[count] = static_cast<unsigned char>(high * 16 + low);
      ++count;
      i += 2;
    }
  }

  return count > 0;
}

} // namespace

int main() {
  showUsage(std::cerr);
  std::cerr << "请输入要转换的hex格式文件名 : ";
  char hexName[kNameBufSize] = {0};
  std::cin.getline(hexName, static_cast<std::streamsize>(sizeof(hexName)));

  std::cerr << "请输入转换后的文件名        : ";
  char binName[kNameBufSize] = {0};
  std::cin.getline(binName, static_cast<std::streamsize>(sizeof(binName)));

  std::ifstream in(hexName, std::ios::binary);
  if (!in.is_open()) {
    std::cout << "输入文件" << hexName << "打开失败!" << std::endl;
    return -1;
  }

  std::ofstream out(binName, std::ios::binary | std::ios::trunc);
  if (!out.is_open()) {
    std::cout << "输出文件" << binName << "打开失败!" << std::endl;
    in.close();
    return -1;
  }

  char line[kLineBufSize] = {0};
  unsigned char buf[kBytesPerLine] = {0};
  unsigned long offset = 0;  // 本行应有的偏移（等于已还原的字节数）
  unsigned long written = 0; // 已还原的字节总数
  int lineNo = 0;
  int ret = 0;

  while (in.getline(line, static_cast<std::streamsize>(sizeof(line)))) {
    ++lineNo;

    // 去掉行尾可能残留的 '\r'（Windows 版 12-b3 重定向出的文件是 CR LF）
    int len = static_cast<int>(std::strlen(line));
    while (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n')) {
      line[--len] = '\0';
    }
    if (len == 0) {
      continue; // 空行直接跳过
    }

    int count = 0;
    unsigned long offsetInFile = 0;
    if (!parseLine(line, offsetInFile, buf, count)) {
      std::cout << "第 " << lineNo << " 行的下一行非法" << std::endl;
      ret = -1;
      break;
    }
    if (offsetInFile != offset) {
      std::cout << "第 " << lineNo << " 行最左侧的偏移量不是期望值0x"
                << std::hex << std::uppercase << offset << std::dec
                << std::endl;
      ret = -1;
      break;
    }

    out.write(reinterpret_cast<const char *>(buf),
              static_cast<std::streamsize>(count));
    if (!out) {
      std::cout << "输出文件" << binName << "写入失败!" << std::endl;
      ret = -1;
      break;
    }

    offset += static_cast<unsigned long>(count);
    written += static_cast<unsigned long>(count);
  }

  // 某行长度超过缓冲区时 getline 会置 failbit，这里单独识别真正的读错误
  if (ret == 0 && in.bad()) {
    std::cout << "输入文件" << hexName << "读取失败!" << std::endl;
    ret = -1;
  }

  in.close();
  out.close();

  // std::cerr << "共还原 " << written << " 个字节" << std::endl;
  return ret;
}