//2551746 大数据 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// ---------------- 参数定义结构（要求(4)的核心） ----------------
struct ParamDef {
  const char *name;      // 参数名，如 "-l"
  int argCount;          // 附加参数个数：0 或 1（可扩展为任意个数）
  int minValue;          // 附加参数下限
  int maxValue;          // 附加参数上限（argCount==0 时，出现即取该值为“开启”）
  int defValue;          // 默认值
  const char *usageText; // Usage 中显示的文字片段，如 "-l 大小"
  int value;             // 运行期解析得到的当前值
};

// ---------------- 工具函数 ----------------

// 取可执行文件短名（去掉路径），让 Usage 自适应程序名
static const char *baseName(const char *path) {
  const char *base = path;
  if (path == NULL)
    return "";
  for (const char *p = path; *p != '\0'; ++p) {
    if (*p == '\\' || *p == '/')
      base = p + 1;
  }
  return base;
}

// 严格解析十进制整数；仅接受形如 [+-]?digits，成功返回 true
static bool parseInt(const char *s, int *out) {
  if (s == NULL || *s == '\0' || out == NULL)
    return false;
  const char *p = s;
  bool negative = false;
  if (*p == '+' || *p == '-') {
    negative = (*p == '-');
    ++p;
  }
  if (*p == '\0')
    return false;
  long long value = 0;
  while (*p != '\0') {
    if (*p < '0' || *p > '9')
      return false;
    value = value * 10 + (*p - '0');
    if (value > 2147483647LL)
      return false;
    ++p;
  }
  *out = negative ? -static_cast<int>(value) : static_cast<int>(value);
  return true;
}

// 校验点分十进制 IPv4：恰好 4 段、每段 0..255、无空段、无首尾多余的 '.'
static bool isValidIP(const char *s) {
  if (s == NULL || *s == '\0')
    return false;
  const char *p = s;
  int parts = 0;
  while (*p != '\0') {
    if (parts >= 4)
      return false; // 多于 4 段
    if (*p < '0' || *p > '9')
      return false; // 段首不是数字
    int value = 0;
    int digits = 0;
    while (*p >= '0' && *p <= '9') {
      value = value * 10 + (*p - '0');
      if (++digits > 3)
        return false; // 单段超过三位
      ++p;
    }
    if (value > 255)
      return false; // 段值越界
    ++parts;
    if (*p == '.') {
      ++p;
      if (*p == '\0')
        return false; // 结尾多余的 '.'
    } else if (*p != '\0') {
      return false; // 段后出现非法字符
    }
  }
  return parts == 4; // 必须恰好 4 段
}

// 按参数名查找，返回下标，未找到返回 -1
static int findParam(const ParamDef *defs, int count, const char *name) {
  for (int i = 0; i < count; ++i) {
    if (strcmp(defs[i].name, name) == 0)
      return i;
  }
  return -1;
}

// 自适应打印 Usage：参数列表完全由定义表生成，无一处硬编码
static void printUsage(const char *prog, const ParamDef *defs, int count) {
  printf("Usage: %s", prog);
  for (int i = 0; i < count; ++i) {
    printf(" [%s]", defs[i].usageText);
  }
  printf(" IP地址\n");
  for (int i = 0; i < count; ++i) {
    printf("  %-4s 附加参数%d, 范围[%d..%d], 默认值%d\n", defs[i].name,
           defs[i].argCount, defs[i].minValue, defs[i].maxValue,
           defs[i].defValue);
  }
}

// 打印解析通过后的结果
static void printResult(const ParamDef *defs, int count, const char *ip) {
  printf("参数检查通过\n");
  for (int i = 0; i < count; ++i) {
    printf("  参数%s = %d\n", defs[i].name, defs[i].value);
  }
  printf("  IP地址 = %s\n", ip);
}

// ---------------- 主程序 ----------------
int main(int argc, char *argv[]) {
  // ====== 唯一需要改动的位置：参数表 ======
  ParamDef params[] = {
      {"-l", 1, 32, 64000, 64, "-l 大小"},
      {"-n", 1, 1, 1024, 4, "-n 数量"},
      {"-t", 0, 0, 1, 0, "-t"},
  };
  const int paramCount = static_cast<int>(sizeof(params) / sizeof(params[0]));

  for (int i = 0; i < paramCount; ++i) {
    params[i].value = params[i].defValue;
  }

  const char *prog =
      (argc > 0 && argv[0] != NULL) ? baseName(argv[0]) : "14-b4-demo";

  // (2) 未带任何参数 -> 打印 Usage
  if (argc <= 1) {
    printUsage(prog, params, paramCount);
    return 0;
  }

  // 约定：最后一个参数为 IP 地址，且必须是合法 IP
  const char *ip = argv[argc - 1];
  if (!isValidIP(ip)) {
    printf("IP地址错误\n");
    return 0;
  }

  // 解析 IP 之前的选项
  for (int i = 1; i < argc - 1; ++i) {
    const char *cur = argv[i];

    if (cur[0] != '-') { // 不以 - 开头
      printf("不是以-开头的合法参数\n");
      return 0;
    }

    int idx = findParam(params, paramCount, cur);
    if (idx < 0) { // 未知参数
      printf("参数%s不存在\n", cur);
      return 0;
    }

    if (params[idx].argCount == 0) { // 无附加参数：出现即“开启”
      params[idx].value = params[idx].maxValue;
      continue;
    }

    // 需要附加参数：必须存在、不以 '-' 开头，且不能占用末尾的 IP
    for (int k = 0; k < params[idx].argCount; ++k) {
      if (i + 1 >= argc - 1 || argv[i + 1][0] == '-') {
        printf("参数%s没有后续参数\n", cur);
        return 0;
      }
      ++i; // 消费该附加参数
      int v = 0;
      if (parseInt(argv[i], &v)) { // 合法整数且在范围内才更新
        if (v >= params[idx].minValue && v <= params[idx].maxValue) {
          params[idx].value = v;
        }
      }
      // 非整数或越界：忽略，保持默认值/上一次的值
    }
  }

  printResult(params, paramCount, ip);
  return 0;
}
