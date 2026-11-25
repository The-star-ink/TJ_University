// 2551746 大数据 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include "../include/class_aat.h"
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// 如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS // 不使用lib才有效

/* ---------------------------------------------------------------
         允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：aat_disp_width
  功    能：计算字符串在控制台上占用的显示列数
  输入参数：s - 待计算的字符串
  返 回 值：占用的列数
  说    明：GBK编码下一个汉字占2列（首字节0x81~0xFE，尾字节0x40~0xFE），
                        西文字符占1列，这样打印表格时列才能对齐
 ***************************************************************************/
static int aat_disp_width(const string &s) {
  int width = 0;

  for (size_t i = 0; i < s.size(); i++) {
    unsigned char ch = (unsigned char)s[i];

    if (ch < 0x80) {
      width += 1;
      continue;
    }

    /* 双字节字符（汉字/全角符号），两个字节算2列 */
    if ((i + 1) < s.size()) {
      unsigned char ch2 = (unsigned char)s[i + 1];
      if (ch2 >= 0x40 && ch2 <= 0xFE)
        i++;
    }
    width += 2;
  }

  return width;
}

/***************************************************************************
  函数名称：aat_int2str
  功    能：int转string
  输入参数：value - 待转换的值
  返 回 值：转换后的字符串
  说    明：null
 ***************************************************************************/
static string aat_int2str(const int value) {
  ostringstream oss;
  oss << value;
  return oss.str();
}

/***************************************************************************
  函数名称：aat_double2str
  功    能：double转string
  输入参数：value - 待转换的值
  返 回 值：转换后的字符串
  说    明：null
 ***************************************************************************/
static string aat_double2str(const double value) {
  ostringstream oss;
  oss.imbue(std::locale::classic()); // 关键：防止被全局 locale 变成 "0,000000"
  oss << std::fixed << std::setprecision(6) << value;
  return oss.str();
}

/***************************************************************************
  函数名称：aat_ipaddr2str
  功    能：将 0x7f000001 形式的IP地址转为 "127.0.0.1"
  输入参数：ipaddr - 待转换的IP地址
  返 回 值：点分十进制形式的字符串
  说    明：null
 ***************************************************************************/
static string aat_ipaddr2str(const u_int ipaddr) {
  char buf[24];

  sprintf(buf, "%u.%u.%u.%u", (ipaddr >> 24) & 0xFF, (ipaddr >> 16) & 0xFF,
          (ipaddr >> 8) & 0xFF, ipaddr & 0xFF);

  return string(buf);
}

/***************************************************************************
  函数名称：aat_str2int
  功    能：将字符串严格转换为int（整串必须都是合法的十进制整数）
  输入参数：str - 待转换的字符串
                        value - 转换结果
  返 回 值：true - 转换成功，false - 转换失败
  说    明："12abc"、"abc"、""、超出int范围 均视为失败
 ***************************************************************************/
static bool aat_str2int(const char *str, int &value) {
  if (str == NULL || *str == '\0')
    return false;

  char *end = NULL;
  errno = 0;
  long result = strtol(str, &end, 10);

  if (errno == ERANGE)
    return false;
  if (end == str || *end != '\0')
    return false;
  if (result < (long)INT_MIN || result > (long)INT_MAX)
    return false;

  value = (int)result;
  return true;
}

/***************************************************************************
  函数名称：aat_str2double
  功    能：将字符串严格转换为double（整串必须都是合法的浮点数）
  输入参数：str - 待转换的字符串
                        value - 转换结果
  返 回 值：true - 转换成功，false - 转换失败
  说    明：null
 ***************************************************************************/
static bool aat_str2double(const char *str, double &value) {
  if (str == NULL || *str == '\0')
    return false;

  char *end = NULL;
  errno = 0;
  double result = strtod(str, &end);

  if (errno == ERANGE)
    return false;
  if (end == str || *end != '\0')
    return false;
  if (!(result >= -1e308 && result <= 1e308)) // 排除 inf/nan
    return false;

  value = result;
  return true;
}

/***************************************************************************
  函数名称：aat_str2ipaddr
  功    能：将 "192.168.80.230" 形式的字符串转为 u_int（0xC0A850E6）
  输入参数：str - 待转换的字符串
                        ipaddr - 转换结果
  返 回 值：true - 格式正确，false - 格式错误
  说    明：必须严格为 xxx.xxx.xxx.xxx 的形式，四段，每段 0~255
 ***************************************************************************/
static bool aat_str2ipaddr(const char *str, u_int &ipaddr) {
  if (str == NULL)
    return false;

  u_int part[4];
  int num = 0;
  const char *p = str;

  while (true) {
    if (num >= 4) // 多于4段
      return false;
    if (*p < '0' || *p > '9') // 每段必须以数字开头
      return false;

    u_int value = 0;
    int digits = 0;

    while (*p >= '0' && *p <= '9') {
      value = value * 10 + (u_int)(*p - '0');
      digits++;
      p++;
      if (digits > 3 || value > 255)
        return false;
    }

    part[num++] = value;

    if (*p == '.') {
      p++;
      if (*p == '\0') // 以'.'结尾
        return false;
      continue;
    }
    if (*p == '\0') // 正常结束
      break;

    return false; // 出现了非法字符
  }

  if (num != 4) // 不足4段
    return false;

  ipaddr = (part[0] << 24) | (part[1] << 16) | (part[2] << 8) | part[3];
  return true;
}

/***************************************************************************
  函数名称：aat_is_dash_args
  功    能：判断某个参数是否是 "--" 开头的可变参数
  输入参数：str - 待判断的参数
  返 回 值：true - 是，false - 不是
  说    明：null
 ***************************************************************************/
static bool aat_is_dash_args(const char *str) {
  return (str != NULL && str[0] == '//' && str[1] == '//');
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：无参构造函数：初始化一个空的参数项
  输入参数：无
  返 回 值：无
  说    明：null
                        参数表中最后一项用它作为结束标志（extargs_type==null）
 ***************************************************************************/
args_analyse_tools::args_analyse_tools() {
  this->args_name = "";
  this->extargs_type = ST_EXTARGS_TYPE::null;
  this->extargs_num = 0;

  this->extargs_bool_default = false;

  this->extargs_int_default = 0;
  this->extargs_int_min = 0;
  this->extargs_int_max = 0;
  this->extargs_int_set = NULL;

  this->extargs_double_default = 0.0;
  this->extargs_double_min = 0.0;
  this->extargs_double_max = 0.0;
  this->extargs_double_set = NULL;

  this->extargs_string_default = "";
  this->extargs_string_set = NULL;

  this->extargs_ipaddr_default = 0;

  this->args_existed = 0;
  this->extargs_int_value = 0;
  this->extargs_double_value = 0.0;
  this->extargs_string_value = "";
  this->extargs_ipaddr_value = 0;

  this->extargs_set_num = 0;

  memset(this->pad, 0, sizeof(this->pad));
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：boolean型参数（只关心是否出现，如--help）
  输入参数：name    - 参数名称，形如"--help"
                        type    - 参数类型，必须是boolean
                        ext_num - 附加参数个数，必须为0
                        def     - 缺省值，只用于显示
  返 回 值：无
  说    明：null
                        类型或附加参数个数不对时打印错误信息，并保持null类型
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const ST_EXTARGS_TYPE type,
                                       const int ext_num, const bool def)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::boolean) {
    cout << "args_analyse构造函数类型错1(type=" << (int)type << ")" << endl;
    return; // 类型错时保持null类型，防止后续误用
  }
  if (ext_num != 0) {
    cout << "args_analyse的附加参数个数必须为0(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;
  this->extargs_bool_default = def;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：int型参数，带取值范围[min,max]
  输入参数：name    - 参数名称
                        type    - int_with_default(超范围置缺省值) 或
int_with_error(超范围报错) ext_num - 附加参数个数，必须为1 def     - 缺省值 _min
- 最小值 _max    - 最大值 返 回 值：无 说    明：null
                        参数不出现时，get_int()读到的是def
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const ST_EXTARGS_TYPE type,
                                       const int ext_num, const int def,
                                       const int _min, const int _max)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::int_with_default &&
      type != ST_EXTARGS_TYPE::int_with_error) {
    cout << "args_analyse构造函数类型错2(type=" << (int)type << ")" << endl;
    return;
  }
  if (ext_num != 1) {
    cout << "args_analyse的附加参数个数必须为1(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;

  this->extargs_int_min =
      (_min <= _max) ? _min : _max; // 容错：上下限写反时自动交换
  this->extargs_int_max = (_min <= _max) ? _max : _min;
  this->extargs_int_default = def;

  /* 参数不出现时，读到的是缺省值 */
  this->extargs_int_value = def;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：int型参数，取值只能是集合中的某个元素
  输入参数：name           - 参数名称
                        type           - int_with_set_default 或
int_with_set_error ext_num        - 附加参数个数，必须为1 def_of_set_pos -
缺省值在集合中的下标（超范围时缺省值为0） set            -
集合，以INVALID_INT_VALUE_OF_SET结尾 返 回 值：无 说    明：null
                        集合只保存指针，不复制内容，调用者要保证集合的生命期
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const enum ST_EXTARGS_TYPE type,
                                       const int ext_num,
                                       const int def_of_set_pos,
                                       const int *const set)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::int_with_set_default &&
      type != ST_EXTARGS_TYPE::int_with_set_error) {
    cout << "args_analyse构造函数类型错3(type=" << (int)type << ")" << endl;
    return;
  }
  if (ext_num != 1) {
    cout << "args_analyse的附加参数个数必须为1(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;
  this->extargs_int_set = (int *)set; // 集合由调用者保证生命期

  /* 统计集合的有效元素个数（结尾必须是 INVALID_INT_VALUE_OF_SET） */
  this->extargs_set_num = 0;
  if (set != NULL) {
    while (this->extargs_set_num < AAT_SET_MAX_NUM &&
           set[this->extargs_set_num] != INVALID_INT_VALUE_OF_SET)
      this->extargs_set_num++;
  }

  /* def_of_set_pos 是集合中的下标，超出范围时缺省值为0 */
  this->extargs_int_default = 0;
  if (set != NULL && def_of_set_pos >= 0 &&
      def_of_set_pos < this->extargs_set_num)
    this->extargs_int_default = set[def_of_set_pos];
  this->extargs_int_value = this->extargs_int_default;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：string型参数（含IP地址型参数）
  输入参数：name    - 参数名称
                        type    - str/ipaddr_with_default/ipaddr_with_error
                        ext_num - 附加参数个数，必须为1
                        def     - 缺省值（IP地址型时形如"192.168.80.230"）
  返 回 值：无
  说    明：null
                        IP地址型的缺省值会被转成u_int保存，空串表示0.0.0.0
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const ST_EXTARGS_TYPE type,
                                       const int ext_num, const string def)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::str &&
      type != ST_EXTARGS_TYPE::ipaddr_with_default &&
      type != ST_EXTARGS_TYPE::ipaddr_with_error) {
    cout << "args_analyse构造函数类型错4(type=" << (int)type << ")" << endl;
    return;
  }
  if (ext_num != 1) {
    cout << "args_analyse的附加参数个数必须为1(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;
  this->extargs_string_default = def;
  this->extargs_string_value = def;

  if (type == ST_EXTARGS_TYPE::ipaddr_with_default) {
    u_int ipaddr = 0;
    if (aat_str2ipaddr(def.c_str(), ipaddr) == true)
      this->extargs_ipaddr_default = ipaddr;
    else
      this->extargs_ipaddr_default = 0; // 缺省值非法时按0.0.0.0处理
    this->extargs_ipaddr_value = this->extargs_ipaddr_default;
  } else if (type == ST_EXTARGS_TYPE::ipaddr_with_error) {
    this->extargs_ipaddr_default = 0; // 无缺省值
    this->extargs_ipaddr_value = 0;
  }
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：string型参数，取值只能是集合中的某个元素
  输入参数：name           - 参数名称
                        type           - str_with_set_default 或
str_with_set_error ext_num        - 附加参数个数，必须为1 def_of_set_pos -
缺省值在集合中的下标（超范围时缺省值为空串） set            - 集合，以空串""结尾
  返 回 值：无
  说    明：null
                        集合只保存指针，不复制内容，调用者要保证集合的生命期
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const ST_EXTARGS_TYPE type,
                                       const int ext_num,
                                       const int def_of_set_pos,
                                       const string *const set)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::str_with_set_default &&
      type != ST_EXTARGS_TYPE::str_with_set_error) {
    cout << "args_analyse构造函数类型错5(type=" << (int)type << ")" << endl;
    return;
  }
  if (ext_num != 1) {
    cout << "args_analyse的附加参数个数必须为1(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;
  this->extargs_string_set = (string *)set; // 集合由调用者保证生命期

  /* 统计集合的有效元素个数（结尾必须是空串 ""） */
  this->extargs_set_num = 0;
  if (set != NULL) {
    while (this->extargs_set_num < AAT_SET_MAX_NUM &&
           set[this->extargs_set_num].empty() == false)
      this->extargs_set_num++;
  }

  /* def_of_set_pos 是集合中的下标，超出范围时缺省值为空串 */
  this->extargs_string_default = "";
  if (set != NULL && def_of_set_pos >= 0 &&
      def_of_set_pos < this->extargs_set_num)
    this->extargs_string_default = set[def_of_set_pos];

  this->extargs_string_value = this->extargs_string_default;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：double型参数，带取值范围[min,max]
  输入参数：name    - 参数名称
                        type    - double_with_default 或 double_with_error
                        ext_num - 附加参数个数，必须为1
                        def     - 缺省值
                        _min    - 最小值
                        _max    - 最大值
  返 回 值：无
  说    明：null
                        参数不出现时，get_double()读到的是def
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const ST_EXTARGS_TYPE type,
                                       const int ext_num, const double def,
                                       const double _min, const double _max)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::double_with_default &&
      type != ST_EXTARGS_TYPE::double_with_error) {
    cout << "args_analyse构造函数类型错6(type=" << (int)type << ")" << endl;
    return;
  }
  if (ext_num != 1) {
    cout << "args_analyse的附加参数个数必须为1(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;

  this->extargs_double_min =
      (_min <= _max) ? _min : _max; // 容错：上下限写反时自动交换
  this->extargs_double_max = (_min <= _max) ? _max : _min;
  this->extargs_double_default = def;

  /* 参数不出现时，读到的是缺省值 */
  this->extargs_double_value = def;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools
  功    能：构造函数：double型参数，取值只能是集合中的某个元素
  输入参数：name           - 参数名称
                        type           - double_with_set_default 或
double_with_set_error ext_num        - 附加参数个数，必须为1 def_of_set_pos -
缺省值在集合中的下标（超范围时缺省值为0） set            -
集合，以INVALID_DOUBLE_VALUE_OF_SET结尾 返 回 值：无 说    明：null
                        集合只保存指针，不复制内容，调用者要保证集合的生命期
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char *name,
                                       const enum ST_EXTARGS_TYPE type,
                                       const int ext_num,
                                       const int def_of_set_pos,
                                       const double *const set)
    : args_analyse_tools() {
  this->args_name = (name == NULL) ? "" : name;

  if (type != ST_EXTARGS_TYPE::double_with_set_default &&
      type != ST_EXTARGS_TYPE::double_with_set_error) {
    cout << "args_analyse构造函数类型错7(type=" << (int)type << ")" << endl;
    return;
  }
  if (ext_num != 1) {
    cout << "args_analyse的附加参数个数必须为1(type=" << (int)type << ")"
         << endl;
    return;
  }

  this->extargs_type = type;
  this->extargs_num = ext_num;
  this->extargs_double_set = (double *)set; // 集合由调用者保证生命期

  /* 统计集合的有效元素个数（结尾必须是 INVALID_DOUBLE_VALUE_OF_SET） */
  this->extargs_set_num = 0;
  if (set != NULL) {
    while (this->extargs_set_num < AAT_SET_MAX_NUM &&
           set[this->extargs_set_num] != INVALID_DOUBLE_VALUE_OF_SET)
      this->extargs_set_num++;
  }

  /* def_of_set_pos 是集合中的下标，超出范围时缺省值为0 */
  this->extargs_double_default = 0.0;
  if (set != NULL && def_of_set_pos >= 0 &&
      def_of_set_pos < this->extargs_set_num)
    this->extargs_double_default = set[def_of_set_pos];

  this->extargs_double_value = this->extargs_double_default;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_tools::~args_analyse_tools
  功    能：析构函数
  输入参数：无
  返 回 值：无
  说    明：null
                        集合只是指针，不负责释放；string等成员由编译器自动析构
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools() {
  /* 集合只是指针，不负责释放；string等成员由编译器自动析构 */
}

/* ---------------------------------------------------------------
         允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：get_extargs_type_str
  功    能：返回参数类型的名称字符串
  输入参数：
  返 回 值：类型名称
  说    明：打印参数表时使用
 ***************************************************************************/
const char *args_analyse_tools::get_extargs_type_str() const {
  switch (this->extargs_type) {
  case ST_EXTARGS_TYPE::boolean:
    return "Bool";
  case ST_EXTARGS_TYPE::int_with_default:
    return "IntWithDefault";
  case ST_EXTARGS_TYPE::int_with_error:
    return "IntWithError";
  case ST_EXTARGS_TYPE::int_with_set_default:
    return "IntSETWithDefault";
  case ST_EXTARGS_TYPE::int_with_set_error:
    return "IntSETWithError";
  case ST_EXTARGS_TYPE::double_with_default:
    return "DoubleWithDefault";
  case ST_EXTARGS_TYPE::double_with_error:
    return "DoubleWithError";
  case ST_EXTARGS_TYPE::double_with_set_default:
    return "DoubleSETWithDefault";
  case ST_EXTARGS_TYPE::double_with_set_error:
    return "DoubleSETWithError";
  case ST_EXTARGS_TYPE::str:
    return "String";
  case ST_EXTARGS_TYPE::str_with_set_default:
    return "StringSETWithDefault";
  case ST_EXTARGS_TYPE::str_with_set_error:
    return "StringSETWithError";
  case ST_EXTARGS_TYPE::ipaddr_with_default:
    return "IPAddrWithDefault";
  case ST_EXTARGS_TYPE::ipaddr_with_error:
    return "IPAddrWithError";
  default:
    return "UnDefineed";
  }
}

/***************************************************************************
  函数名称：get_default_str
  功    能：返回缺省值的字符串形式
  输入参数：
  返 回 值：缺省值字符串（没有缺省值时返回 "/"）
  说    明：打印参数表时使用
 ***************************************************************************/
const string args_analyse_tools::get_default_str() const {
  switch (this->extargs_type) {
  case ST_EXTARGS_TYPE::boolean:
    return this->extargs_bool_default ? "true" : "false";
  case ST_EXTARGS_TYPE::int_with_default:
  case ST_EXTARGS_TYPE::int_with_set_default:
    return aat_int2str(this->extargs_int_default);
  case ST_EXTARGS_TYPE::double_with_default:
  case ST_EXTARGS_TYPE::double_with_set_default:
    return aat_double2str(this->extargs_double_default);
  case ST_EXTARGS_TYPE::str:
  case ST_EXTARGS_TYPE::str_with_set_default:
    return this->extargs_string_default.empty() ? string("/")
                                                : this->extargs_string_default;
  case ST_EXTARGS_TYPE::ipaddr_with_default:
    return aat_ipaddr2str(this->extargs_ipaddr_default);
  default:
    return "/";
  }
}

/***************************************************************************
  函数名称：get_value_str
  功    能：返回当前值的字符串形式
  输入参数：
  返 回 值：当前值字符串
  说    明：打印参数表时使用
 ***************************************************************************/
const string args_analyse_tools::get_value_str() const {
  if (this->args_existed == 0) {
    return "/";
  };
  switch (this->extargs_type) {
  case ST_EXTARGS_TYPE::boolean:
    return this->existed() ? "true" : "false";
  case ST_EXTARGS_TYPE::int_with_default:
  case ST_EXTARGS_TYPE::int_with_error:
  case ST_EXTARGS_TYPE::int_with_set_default:
  case ST_EXTARGS_TYPE::int_with_set_error:
    return aat_int2str(this->extargs_int_value);
  case ST_EXTARGS_TYPE::double_with_default:
  case ST_EXTARGS_TYPE::double_with_error:
  case ST_EXTARGS_TYPE::double_with_set_default:
  case ST_EXTARGS_TYPE::double_with_set_error:
    return aat_double2str(this->extargs_double_value);
  case ST_EXTARGS_TYPE::str:
  case ST_EXTARGS_TYPE::str_with_set_default:
  case ST_EXTARGS_TYPE::str_with_set_error:
    return this->extargs_string_value.empty() ? string("<NULL>")
                                              : this->extargs_string_value;
  case ST_EXTARGS_TYPE::ipaddr_with_default:
  case ST_EXTARGS_TYPE::ipaddr_with_error:
    return aat_ipaddr2str(this->extargs_ipaddr_value);
  default:
    return "/";
  }
}

/***************************************************************************
  函数名称：get_range_set_str
  功    能：返回取值范围/取值集合的字符串形式
  输入参数：
  返 回 值：范围或集合字符串（没有范围/集合时返回 "-"）
  说    明：打印参数表时使用
 ***************************************************************************/
const string args_analyse_tools::get_range_set_str() const {
  string result;
  int i;

  switch (this->extargs_type) {
  case ST_EXTARGS_TYPE::int_with_default:
  case ST_EXTARGS_TYPE::int_with_error:
    return "[" + aat_int2str(this->extargs_int_min) + ".." +
           aat_int2str(this->extargs_int_max) + "]";

  case ST_EXTARGS_TYPE::int_with_set_default:
  case ST_EXTARGS_TYPE::int_with_set_error:
    for (i = 0; i < this->extargs_set_num; i++) {
      if (i > 0)
        result += "/";
      result += aat_int2str(this->extargs_int_set[i]);
    }
    return result;

  case ST_EXTARGS_TYPE::double_with_default:
  case ST_EXTARGS_TYPE::double_with_error:
    return "[" + aat_double2str(this->extargs_double_min) + ".." +
           aat_double2str(this->extargs_double_max) + "]";

  case ST_EXTARGS_TYPE::double_with_set_default:
  case ST_EXTARGS_TYPE::double_with_set_error:
    for (i = 0; i < this->extargs_set_num; i++) {
      if (i > 0)
        result += "/";
      result += aat_double2str(this->extargs_double_set[i]);
    }
    return result;

  case ST_EXTARGS_TYPE::str_with_set_default:
  case ST_EXTARGS_TYPE::str_with_set_error:
    for (i = 0; i < this->extargs_set_num; i++) {
      if (i > 0)
        result += "/";
      result += this->extargs_string_set[i];
    }
    return result;

  default:
    return "/";
  }
}

/***************************************************************************
/***************************************************************************
  函数名称：get_name
  功    能：外取参数：返回参数名称（形如"--help"）
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_name() const { return this->args_name; }

/***************************************************************************
/***************************************************************************
  函数名称：existed
  功    能：外取参数：返回参数是否出现过（0-没有，1-出现过）
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const { return !!this->args_existed; }

/***************************************************************************
/***************************************************************************
  函数名称：get_int
  功    能：外取参数：返回int型附加参数的值
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const {
  return this->extargs_int_value;
}

/***************************************************************************
/***************************************************************************
  函数名称：get_double
  功    能：外取参数：返回double型附加参数的值
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const double args_analyse_tools::get_double() const {
  return this->extargs_double_value;
}

/***************************************************************************
/***************************************************************************
  函数名称：get_string
  功    能：外取参数：返回string型附加参数的值
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const {
  return this->extargs_string_value;
}

/***************************************************************************
/***************************************************************************
  函数名称：get_ipaddr
  功    能：外取参数：返回IP地址型的值（以"127.0.0.1"为例，返回0x7f000001）
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const {
  return this->extargs_ipaddr_value;
}

/***************************************************************************
/***************************************************************************
  函数名称：get_str_ipaddr
  功    能：外取参数：返回IP地址型的字符串形式（如"127.0.0.1"）
  输入参数：无
  返 回 值：值
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const {
  return aat_ipaddr2str(this->extargs_ipaddr_value);
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_process
  功    能：对命令行的可变参数进行分析（取出附加值、检查合法性）
  输入参数：argc           - argv的元素个数
                        argv           -
命令行参数（argv[0]是可执行文件名，从argv[1]开始分析） args           -
参数表（以type==null的项作为结束标志） follow_up_args - 0:后面没有固定参数
1:后面还有固定参数 返 回 值：-1 - 处理过程出错（错误信息已在函数内打印） >0 -
可变参数处理完成后，当前停在argv[]的第几个下标位置 说    明：友元函数
                        命令行格式约定：先--开头的可变参数（顺序任意），最后才是固定参数
 ***************************************************************************/
int args_analyse_process(const int argc, const char *const *const argv,
                         args_analyse_tools *const args,
                         const int follow_up_args) {
  int i;

  if (args == NULL || argv == NULL)
    return -1;

  /* 没有可变参数，固定参数从argv[1]开始，若没有固定参数则返回argc */
  if (argc <= 1)
    return 1;

  i = 1;
  while (i < argc) {
    const char *opt = argv[i];

    if (opt == NULL)
      break;

    /* 不是以'//'开头的，说明可变参数已经结束，后面都是固定参数 */
    if (opt[0] != '//')
      break;

    /* 只支持 "--" 开头的可变参数（-t 这种单减号形式本工具集不支持） */
    if (opt[1] != '//') {
      cout << "参数[" << opt << "]格式非法(不是--开头的有效内容)." << endl;
      return -1;
    }

    /* 在参数表中查找该参数 */
    int idx = -1;
    for (int n = 0; args[n].extargs_type != ST_EXTARGS_TYPE::null; n++) {
      if (args[n].args_name == opt) {
        idx = n;
        break;
      }
    }
    if (idx < 0) {
      cout << "参数[" << opt << "]格式非法(不是--开头的有效内容)." << endl;
      return -1;
    }

    /* 同一个参数不允许多次出现 */
    if (args[idx].args_existed != 0) {
      cout << "参数[" << opt << "]重复." << endl;
      return -1;
    }

    /* 取出附加参数（bool型没有附加参数，其余类型最多只有1个） */
    const int ext_num = (args[idx].extargs_type == ST_EXTARGS_TYPE::boolean)
                            ? 0
                            : args[idx].extargs_num;
    const char *value = NULL;

    if (ext_num > 0) {
      if ((i + 1) >= argc) {
        cout << "参数[" << opt << "]缺少附加参数." << endl;
        return -1;
      }
      value = argv[i + 1];
      if (aat_is_dash_args(value) ==
          true) { // 后面紧接着另一个可变参数，说明本参数缺少附加参数
        cout << "参数[" << opt << "]的附加参数不足." << endl;
        return -1;
      }
    }

    args[idx].args_existed = 1; // 标记本参数已经出现过

    switch (args[idx].extargs_type) {
    case ST_EXTARGS_TYPE::boolean:
      i += 1;
      break;

    case ST_EXTARGS_TYPE::int_with_default:
    case ST_EXTARGS_TYPE::int_with_error: {
      int v = 0;
      if (aat_str2int(value, v) == false) {
        cout << "参数[" << opt << "]的附加参数不是整数." << endl;
        return -1;
      }
      if (v < args[idx].extargs_int_min || v > args[idx].extargs_int_max) {
        if (args[idx].extargs_type == ST_EXTARGS_TYPE::int_with_error) {
          cout << "参数[" << opt << "]的附加参数值(" << value << ")非法."
               << endl;
          return -1;
        }
        v = args[idx].extargs_int_default; // 超范围，置缺省值
      }
      args[idx].extargs_int_value = v;
      i += 2;
      break;
    }

    case ST_EXTARGS_TYPE::int_with_set_default:
    case ST_EXTARGS_TYPE::int_with_set_error: {
      int v = 0;
      if (aat_str2int(value, v) == false) {
        cout << "参数[" << opt << "]的附加参数不是整数." << endl;
        return -1;
      }

      bool found = false;
      for (int n = 0; n < args[idx].extargs_set_num; n++) {
        if (args[idx].extargs_int_set[n] == v) {
          found = true;
          break;
        }
      }

      if (found == false) {
        if (args[idx].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
          cout << "参数[" << opt << "]的附加参数值(" << value << ")非法."
               << endl;
          return -1;
        }
        v = args[idx].extargs_int_default; // 不在集合中，置缺省值
      }
      args[idx].extargs_int_value = v;
      i += 2;
      break;
    }

    case ST_EXTARGS_TYPE::double_with_default:
    case ST_EXTARGS_TYPE::double_with_error: {
      double v = 0.0;
      if (aat_str2double(value, v) == false) {
        cout << "参数[" << opt << "]的附加参数不是浮点数." << endl;
        return -1;
      }
      if (v < args[idx].extargs_double_min ||
          v > args[idx].extargs_double_max) {
        if (args[idx].extargs_type == ST_EXTARGS_TYPE::double_with_error) {
          cout << "参数[" << opt << "]的附加参数值(" << value << ")非法."
               << endl;
          return -1;
        }
        v = args[idx].extargs_double_default; // 超范围，置缺省值
      }
      args[idx].extargs_double_value = v;
      i += 2;
      break;
    }

    case ST_EXTARGS_TYPE::double_with_set_default:
    case ST_EXTARGS_TYPE::double_with_set_error: {
      double v = 0.0;
      if (aat_str2double(value, v) == false) {
        cout << "参数[" << opt << "]的附加参数不是浮点数." << endl;
        return -1;
      }

      bool found = false;
      for (int n = 0; n < args[idx].extargs_set_num; n++) {
        if (fabs(args[idx].extargs_double_set[n] - v) < DOUBLE_DELTA) {
          found = true;
          v = args[idx].extargs_double_set[n]; // 取集合中的值，消除浮点误差
          break;
        }
      }

      if (found == false) {
        if (args[idx].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
          cout << "参数[" << opt << "]的附加参数值(" << value << ")非法."
               << endl;
          return -1;
        }
        v = args[idx].extargs_double_default; // 不在集合中，置缺省值
      }
      args[idx].extargs_double_value = v;
      i += 2;
      break;
    }

    case ST_EXTARGS_TYPE::str:
      args[idx].extargs_string_value = (value == NULL) ? "" : value;
      i += 2;
      break;

    case ST_EXTARGS_TYPE::str_with_set_default:
    case ST_EXTARGS_TYPE::str_with_set_error: {
      string s = (value == NULL) ? "" : value;

      bool found = false;
      for (int n = 0; n < args[idx].extargs_set_num; n++) {
        if (args[idx].extargs_string_set[n] == s) {
          found = true;
          break;
        }
      }

      if (found == false) {
        if (args[idx].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
          cout << "参数[" << opt << "]非法." << endl;
          return -1;
        }
        s = args[idx].extargs_string_default; // 不在集合中，置缺省值
      }
      args[idx].extargs_string_value = s;
      i += 2;
      break;
    }

    case ST_EXTARGS_TYPE::ipaddr_with_default:
    case ST_EXTARGS_TYPE::ipaddr_with_error: {
      u_int ipaddr = 0;
      if (aat_str2ipaddr(value, ipaddr) == false) {
        if (args[idx].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error) {
          cout << "参数[" << opt << "]非法." << endl;
          return -1;
        }
        ipaddr = args[idx].extargs_ipaddr_default; // 格式非法，置缺省值
      }
      args[idx].extargs_ipaddr_value = ipaddr;
      i += 2;
      break;
    }

    default: // 正常不会走到这里
      i += 1;
      break;
    }
  }

  /* follow_up_args==0 时，本应没有固定参数；有多余的参数也不在这里报错，
     由调用者根据返回值自行判断处理 */
  (void)follow_up_args;

  return i;
}

/***************************************************************************
/***************************************************************************
  函数名称：args_analyse_print
  功    能：打印参数分析的结果（参数表）
  输入参数：args - 参数表（以type==null的项作为结束标志）
  返 回 值：0 - 成功，-1 - 参数表为空
  说    明：友元函数
                        每行开头一个空格，每列宽度为列名与该列所有内容的最大宽度，
                        列之间一个空格，分隔线(---)的长度按列的宽度动态计算
 ***************************************************************************/
int args_analyse_print(const args_analyse_tools *const args) {
  if (args == NULL) {
    cout << "参数表为空，无法打印" << endl;
    return -1;
  }

  /* 以 type==null 作为参数表的结束标志 */
  int num = 0;
  while (args[num].extargs_type != ST_EXTARGS_TYPE::null)
    num++;

  if (num <= 0) {
    cout << " 没有需要分析的参数" << endl;
    return 0;
  }

  const int col = 6; // name/type/default/exists/value/range-set
  vector<vector<string>> cells(num + 1, vector<string>(col));

  /* 第0行是列名 */
  cells[0][0] = "name";
  cells[0][1] = "type";
  cells[0][2] = "default";
  cells[0][3] = "exists";
  cells[0][4] = "value";
  cells[0][5] = "range/set";

  /* 其余各行是各个参数的内容 */
  for (int i = 0; i < num; i++) {
    const args_analyse_tools &a = args[i];

    cells[i + 1][0] = a.args_name.empty() ? string("<NULL>") : a.args_name;
    cells[i + 1][1] = a.get_extargs_type_str();
    cells[i + 1][2] = a.get_default_str();
    cells[i + 1][3] = aat_int2str(a.existed());
    cells[i + 1][4] = a.get_value_str();
    cells[i + 1][5] = a.get_range_set_str();
  }

  /* 计算每列宽度：列名与该列所有内容的最大宽度 */
  vector<int> width(col, 0);
  for (int r = 0; r < (int)cells.size(); r++) {
    for (int c = 0; c < col; c++) {
      int w = aat_disp_width(cells[r][c]);
      if (w > width[c])
        width[c] = w;
    }
  }

  /* 整行宽度 = 行首的1个空格 + 各列宽度 + 列之间的1个空格 */
  int line_width = 1;
  for (int c = 0; c < col; c++)
    line_width += width[c];
  line_width += (col - 1);

  /* 分隔线，长度随列宽动态变化 */
  string separator = string(line_width, '=');

  cout << separator << endl;
  for (int r = 0; r < (int)cells.size(); r++) {
    string line = " ";

    for (int c = 0; c < col; c++) {
      if (c > 0)
        line += " ";
      line += cells[r][c];

      /* 最后一列不补空格，避免行尾出现多余的空格 */
      if (c + 1 < col) {
        int w = aat_disp_width(cells[r][c]);
        if (w < width[c])
          line += string(width[c] - w, ' ');
      }
    }
    cout << line << endl;

    /* 列名行与内容行之间再加一条分隔线 */
    if (r == 0)
      cout << separator << endl;
  }
  cout << left << separator << endl << endl;

  return 0;
}

#endif // !ENABLE_LIB_COMMON_TOOLS
