/** ------------------------------------------------------------
 * Copyright © 2023-2025 曾奥然, 郭恒
 * SPDX-License-Identifier: MIT
 * -------------------------------------------------------------
 * Lib Authors   :  曾奥然 <ccmywish@qq.com>
 *               |   郭恒  <2085471348@qq.com>
 * Contributors  : Mikachu2333 <mikachu.23333@zohomail.com>
 *               | juzeon <skyjuzheng@gmail.com>
 *               | BingChunMoLi <bingchunmoli@bingchunmoli.com>
 *               |
 * Created On    : <2023-08-28>
 * Last Modified : <2025-10-28>
 *
 *
 *                     xy: 襄阳、咸阳
 *
 * 为跨平台命令行应用程序准备的 C11 实用函数和宏 (utilities)
 *
 * 该库的特点是混合多种编程语言风味 (绝大多数为 Ruby)，每个 API
 * 均使用 @flavor 标注其参考依据
 * ------------------------------------------------------------*/

#ifndef XY_H
#define XY_H

#define _XY_Version       "v0.2.2.0-2025/10/28"
#define _XY_Maintain_URL  "https://github.com/RubyMetric/chsrc/blob/dev/lib/xy.h"
#define _XY_Maintain_URL2 "https://gitee.com/RubyMetric/chsrc/blob/dev/lib/xy.h"

#if defined(__STDC__) && __STDC_VERSION__ < 201112L
#   error "xy.h requires C11 or later, please use a new compiler which at least supports C11"
#endif

#if defined(__STDC__) && __STDC_VERSION__ < 201710L
#   warning "xy.h recommends a C17 or later compiler"
#endif

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> // opendir() closedir()

#if defined(__STDC__) && __STDC_VERSION__ >= 202311
  #define XY_Deprecate_This(msg) [[deprecated(msg)]]
#elif defined(__GNUC__) || defined(__clang__)
  #define XY_Deprecate_This(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
  #define XY_Deprecate_This(msg) __declspec(deprecated(msg))
#else
  #define XY_Deprecate_This(msg)
#endif



/* 全局变量 与 全局状态 */
struct
{
  bool enable_color;

  bool inited;

  bool on_windows;
  bool on_linux;
  bool on_macos;
  bool on_bsd;
  bool on_android;

  char *os_devnull;
}
xy =
{
  .enable_color = true,

  /* 由 xy_init() 赋值 */

  .inited = false,

  .on_windows = false,
  .on_linux = false,
  .on_macos = false,
  .on_bsd = false,
  .on_android = false,

  .os_devnull = NULL
};



#ifdef _WIN32
  #define XY_Build_On_Windows 1

  #include <windows.h>
  #include <shlobj.h>

#elif defined(__linux__) || defined(__linux)
  #define XY_Build_On_Linux 1
  #define XY_Build_On_Unix  1

#elif defined(__APPLE__)
  #define XY_Build_On_macOS 1
  #define XY_Build_On_Unix  1

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
  #define XY_Build_On_BSD  1
  #define XY_Build_On_Unix 1
#endif



/**
 *  assert() 会被 NDEBUG 关闭，但我们也没有必要强制开启它，还是留给用户定义
 */
// #undef NDEBUG
#define xy_noop() ((void)0)

#define assert_str(a, b) assert (xy_streql ((a), (b)))

#define xy_throw(reason) assert(!reason)

/**
 * @depreacated 避免消极用语
 *
 * @flavor Perl, PHP, Raku
 */
// #define die xy_throw

/**
 * @depreacated 避免消极用语
 */
// #define xy_panic xy_throw

#define xy_unsupported()    xy_throw("Unsuppoted")
#define xy_unimplemented()  xy_throw("Unimplemented temporarily")
#define xy_unreached()      xy_throw("This code shouldn't be reached")
#define xy_cant_be_null(p)  if(!p) xy_throw("This pointer can't be null")



static void _xy_print_int    (int n) {printf ("%d", n);}
static void _xy_print_long   (long n) {printf ("%ld", n);}
static void _xy_print_long_long (long long n) {printf ("%lld", n);}
static void _xy_print_float  (float n) {printf ("%f", n);}
static void _xy_print_double (double n) {printf ("%f", n);}
static void _xy_print_bool   (bool b) {printf("%s", (b) ? "true" : "false");}
static void _xy_print_str    (char *str) {printf ("%s", str);}
static void _xy_print_const_str (const char *str) {printf ("%s", str);}

static void _xy_println_int    (int n) {printf ("%d\n", n);}
static void _xy_println_long   (long n) {printf ("%ld\n", n);}
static void _xy_println_long_long (long long n) {printf ("%lld\n", n);}
static void _xy_println_float  (float n) {printf ("%f\n", n);}
static void _xy_println_double (double n) {printf ("%f\n", n);}
static void _xy_println_bool   (bool b) {printf("%s\n", (b) ? "true" : "false");}
static void _xy_println_str    (char *str) {printf ("%s\n", str);}
static void _xy_println_const_str (const char *str) {printf ("%s\n", str);}

/**
 * @flavor Ruby, Python
 */
#define print(x) _Generic((x), \
  int:       _xy_print_int,  \
  long:      _xy_print_long, \
  long long: _xy_print_long_long, \
  float:     _xy_print_float,  \
  double:    _xy_print_double, \
  bool:      _xy_print_bool, \
  char *:    _xy_print_str,  \
  const char *:   _xy_print_const_str, \
  default:   xy_throw("Unsupported type for print()!") \
)(x)

/**
 * @flavor JVM family, Rust
 */
#define println(x) _Generic((x), \
  int:       _xy_println_int,  \
  long:      _xy_println_long, \
  long long: _xy_println_long_long, \
  float:     _xy_println_float,  \
  double:    _xy_println_double, \
  bool:      _xy_println_bool, \
  char *:    _xy_println_str,  \
  const char *:   _xy_println_const_str, \
  default:   xy_throw("Unsupported type for println()/say()!") \
)(x)
/* @flavor Raku, Perl */
#define say println
/* @flavor PHP */
#define echo println
/**
 * @flavor HTML
 */
void br ()                   { puts (""); }
void p (const char *s)       { printf ("%s\n", s); }


#define xy_c_array_len(arr) (sizeof (arr) / sizeof (arr[0]))


static inline void *
xy_malloc0 (size_t size)
{
  void *ptr = malloc (size);
  memset (ptr, 0, size);
  return ptr;
}


/**
 * @brief 替换指针内容并自动释放旧内存
 *
 * @param pptr    指向要被替换内存区域的指针的指针
 *                *pptr 可为 NULL
 * @param new_mem 新的内存区域
 */
static inline void
xy_ptr_replace (char **pptr, char *new_mem)
{
  xy_cant_be_null (pptr);

  if (*pptr)
    free (*pptr);

  *pptr = new_mem;
}


/******************************************************
 *                      String
 ******************************************************/

/**
 * @brief 将 str 中所有的 pat 字符串替换成 replace，返回一个全新的字符串
 *
 * @flavor Ruby: String#gsub
 *
 * @param str     原字符串
 * @param pat     要替换的字符串
 * @param replace 替换成的字符串
 *
 * @return 替换后的新字符串
 */
static char *
xy_str_gsub (const char *str, const char *pat, const char *replace)
{
  size_t replace_len = strlen (replace);
  size_t pat_len = strlen (pat);

  int unit = replace_len - pat_len;
  if (unit <= 0)
    unit = 0;

  size_t len = strlen (str);

  const char *cur = str;
  int count = 0;

  while (cur < str + len)
    {
      char *fnd = strstr (cur, pat);
      if (fnd)
        {
          count++;
          cur = fnd + pat_len;
        }
      else
        break;
    }
  // puti(count); DEBUG 匹配次数

  char *ret = malloc (unit * count + len + 1);
  char *retcur = ret;

  cur = str;
  while (cur < str + len)
    {
      char *fnd = strstr (cur, pat);
      if (fnd)
        {
          ptrdiff_t diff = fnd - cur;
          strncpy (retcur, cur, diff);
          cur = fnd + pat_len;

          retcur += diff;
          strcpy (retcur, replace);
          retcur += replace_len;
        }
      else
        break;
    }
  strcpy (retcur, cur);

  return ret;
}


/**
 * @flavor 见 xy_strcat()
 */
static char *
xy_2strcat (const char *str1, const char *str2)
{
  size_t len = strlen (str1);
  size_t size = len + strlen (str2) + 1;
  char *ret = malloc (size);
  strcpy (ret, str1);
  strcpy (ret + len, str2);
  return ret;
}


/**
 * @brief 将多个字符串连接成一个字符串
 *
 * @flavor C语言存在 strcat()，然而限制比较大，我们重新实现

 *   'concat' 这个API广泛应用于包括 Ruby、JavaScript、JVM family、C#
 *
 *   但由于 xy_str_concat() 显著长于 xy_strcat()，而这个 API 在 chsrc 中
 *   又大量使用，所以我们选择后者这个更简短的形式
 *
 * @param count 连接的字符串数量
 * @param ...   连接的字符串
 *
 * @return 拼接的新字符串
 */
static char *
xy_strcat (unsigned int count, ...)
{
  size_t al_fixed = 256;
  char *ret = calloc (1, al_fixed);
  // 已分配次数
  int al_times = 1;
  // 当前已分配量
  size_t al_cur = al_fixed;

  const char *str = NULL;
  // 需要分配的量
  size_t al_need = 0;
  // 用于 strcpy() 到 ret 的哪个位置
  char *cur = ret + 0;

  va_list args;
  va_start (args, count);

  for (int i = 0; i < count; i++)
    {
      bool need_realloc = false;

      str = va_arg (args, const char *);
      al_need += strlen (str) + 1;
      while (al_need > al_cur)
        {
          al_times += 1;
          al_cur = al_times * al_fixed;
          need_realloc = true;
        }
      // printf("al_times %d, al_need %zd, al_cur %zd\n", al_times, al_need,
      // al_cur);
      if (need_realloc)
        {
          ptrdiff_t diff = cur - ret;
          ret = realloc (ret, al_cur);
          cur = ret + diff;
        }
      if (NULL == ret)
        {
          fprintf (stderr, "xy.h: No availble memory!");
          return NULL;
        }
      strcpy (cur, str);
      // puts(ret);
      cur += strlen (str);
    }
  va_end (args);

  *cur = '\0';
  return ret;
}


/**
 * @brief 复制一个字符串，返回复制的新字符串
 *
 * @param str 要复制的字符串
 *
 * @return 复制的新字符串
 */
static char *
xy_strdup (const char *str)
{
  if (!str)
    {
      fprintf (stderr, "xy.h: xy_strdup() called with NULL!\n");
      return NULL;
    }

  size_t len = strlen (str);
  char *new = xy_malloc0 (len + 1);
  strcpy (new, str);
  return new;
}

#define _XY_Str_Bold      1
#define _XY_Str_Faint     2
#define _XY_Str_Italic    3
#define _XY_Str_Underline 4
#define _XY_Str_Blink     5
#define _XY_Str_Cross     9

#define xy_str_to_bold(str)      _xy_str_to_terminal_style (_XY_Str_Bold, str)
#define xy_str_to_faint(str)     _xy_str_to_terminal_style (_XY_Str_Faint, str)
#define xy_str_to_italic(str)    _xy_str_to_terminal_style (_XY_Str_Italic, str)
#define xy_str_to_underline(str) _xy_str_to_terminal_style (_XY_Str_Underline, str)
#define xy_str_to_blink(str)     _xy_str_to_terminal_style (_XY_Str_Blink, str)
#define xy_str_to_cross(str)     _xy_str_to_terminal_style (_XY_Str_Cross, str)

#define _XY_Str_Red     31
#define _XY_Str_Green   32
#define _XY_Str_Yellow  33
#define _XY_Str_Blue    34
#define _XY_Str_Magenta 35
#define _XY_Str_Cyan    36

#define xy_str_to_red(str)     _xy_str_to_terminal_style (_XY_Str_Red, str)
#define xy_str_to_green(str)   _xy_str_to_terminal_style (_XY_Str_Green, str)
#define xy_str_to_yellow(str)  _xy_str_to_terminal_style (_XY_Str_Yellow, str)
#define xy_str_to_blue(str)    _xy_str_to_terminal_style (_XY_Str_Blue, str)
#define xy_str_to_magenta(str) _xy_str_to_terminal_style (_XY_Str_Magenta, str)
#define xy_str_to_purple        xy_str_to_magenta
#define xy_str_to_cyan(str)    _xy_str_to_terminal_style (_XY_Str_Cyan, str)

static char *
_xy_str_to_terminal_style (int style, const char *str)
{
  char *color_fmt_str = NULL;

  if (!xy.enable_color)
    {
      color_fmt_str = "%s";
      goto new_str;
    }

  switch (style)
    {
    case _XY_Str_Red:
      color_fmt_str = "\e[31m%s\e[0m"; break;
    case _XY_Str_Green:
      color_fmt_str = "\e[32m%s\e[0m"; break;
    case _XY_Str_Yellow:
      color_fmt_str = "\e[33m%s\e[0m"; break;
    case _XY_Str_Blue:
      color_fmt_str = "\e[34m%s\e[0m"; break;
    case _XY_Str_Magenta:
      color_fmt_str = "\e[35m%s\e[0m"; break;
    case _XY_Str_Cyan:
      color_fmt_str = "\e[36m%s\e[0m"; break;
    case _XY_Str_Bold:
      color_fmt_str = "\e[1m%s\e[0m"; break;
    case _XY_Str_Faint:
      color_fmt_str = "\e[2m%s\e[0m"; break;
    case _XY_Str_Italic:
      color_fmt_str = "\e[3m%s\e[0m"; break;
    case _XY_Str_Underline:
      color_fmt_str = "\e[4m%s\e[0m"; break;
    case _XY_Str_Blink:
      color_fmt_str = "\e[5m%s\e[0m"; break;
    case _XY_Str_Cross:
      color_fmt_str = "\e[9m%s\e[0m"; break;
    }


  size_t len = 0;
new_str:
  // -2 把中间%s减掉
  len = strlen (color_fmt_str) - 2;
  char *buf = malloc (strlen (str) + len + 1);
  sprintf (buf, color_fmt_str, str);
  return buf;
}


static bool
xy_streql (const char *str1, const char *str2)
{
  if (NULL==str1 || NULL==str2)
    {
      return false;
    }
  return strcmp (str1, str2) == 0 ? true : false;
}

static bool
xy_streql_ic (const char *str1, const char *str2)
{
  if (NULL == str1 || NULL == str2)
    {
      return false;
    }

  size_t len1 = strlen (str1);
  size_t len2 = strlen (str2);
  if (len1 != len2)
    {
      return false;
    }

  for (size_t i = 0; i < len1; i++)
    {
      if (tolower (str1[i]) != tolower (str2[i]))
        {
          return false;
        }
    }
  return true;
}


/**
 * @flavor Ruby: String#end_with?
 */
static bool
xy_str_end_with (const char *str, const char *suffix)
{
  size_t len1 = strlen (str);
  size_t len2 = strlen (suffix);

  if (0 == len2)
    return true; // 空字符串直接返回
  if (len1 < len2)
    return false;

  const char *cur1 = str + len1 - 1;
  const char *cur2 = suffix + len2 - 1;

  for (int i = 0; i < len2; i++)
    {
      if (*cur1 != *cur2)
        return false;
      cur1--;
      cur2--;
    }
  return true;
}

/**
 * @flavor Ruby: String#start_with?
 */
static bool
xy_str_start_with (const char *str, const char *prefix)
{
  if (NULL==str || NULL==prefix)
    {
      return false;
    }

  size_t len1 = strlen (str);
  size_t len2 = strlen (prefix);

  if (0 == len2)
    return true; // 空字符串直接返回
  if (len1 < len2)
    return false;

  const char *cur1 = str;
  const char *cur2 = prefix;

  for (int i = 0; i < len2; i++)
    {
      if (*cur1 != *cur2)
        return false;
      cur1++;
      cur2++;
    }
  return true;
}

/**
 * @flavor Ruby: String#delete_prefix
 */
static char *
xy_str_delete_prefix (const char *str, const char *prefix)
{
  bool yes = xy_str_start_with (str, prefix);
  if (!yes)
    {
      return xy_strdup (str);
    }
  else
    {
      size_t len = strlen (prefix);
      return xy_strdup (str + len);
    }
}

/**
 * @flavor Ruby: String#delete_suffix
 */
static char *
xy_str_delete_suffix (const char *str, const char *suffix)
{
  char *new = xy_strdup (str);
  bool yes = xy_str_end_with (str, suffix);
  if (!yes)
    return new;

  size_t len1 = strlen (str);
  size_t len2 = strlen (suffix);
  char *cur = new + len1 - len2;
  *cur = '\0';
  return new;
}

/**
 * @flavor Ruby: String#strip
 */
static char *
xy_str_strip (const char *str)
{
  xy_cant_be_null (str);

  const char *start = str;
  while (*start && strchr ("\n\r\v\t\f ", *start))
    start++;

  if ('\0' == *start)
    return xy_strdup ("");

  const char *end = start + strlen (start) - 1;
  while (end >= start && strchr ("\n\r\v\t\f ", *end))
    end--;

  size_t len = (size_t) (end - start + 1);
  char *ret = xy_malloc0 (len + 1);
  memcpy (ret, start, len);
  ret[len] = '\0';
  return ret;
}

typedef struct
{
  bool   found;
  size_t begin;
  size_t end;
}
XyStrFindResult_t;

/**
 * @brief 查找子串，返回是否命中以及子串在原串中的起止位置（0 基，end 为闭区间）
 */
static XyStrFindResult_t
xy_str_find (const char *str, const char *substr)
{
  XyStrFindResult_t result = { .found = false, .begin = 0, .end = 0 };

  if (!str || !substr)
    return result;

  if ('\0' == substr[0])
    {
      result.found = true;
      return result;
    }

  const char *pos = strstr (str, substr);
  if (!pos)
    return result;

  result.found = true;
  result.begin = (size_t) (pos - str);
  result.end = result.begin + strlen (substr) - 1;
  return result;
}

/**
 * @brief 获取字符串下一行的内容
 *
 * @note 将忽略开头的换行，截取至下一个换行前（不含换行符）
 */
static char *
xy_str_next_nonempty_line (const char *str)
{
  if (!str)
    return xy_strdup ("");

  const char *cur = str;
  while (*cur == '\n')
    cur++;

  if ('\0' == *cur)
    return xy_strdup ("");

  const char *newline = strchr (cur, '\n');
  size_t len = newline ? (size_t) (newline - cur) : strlen (cur);

  char *ret = xy_malloc0 (len + 1);
  strncpy (ret, cur, len);
  ret[len] = '\0';
  return ret;
}



/**
 * @brief 读取文件内容并返回字符串，失败时返回空字符串
 *
 * @note 已处理 \r\n 和 \r，返回的字符串均为 \n 换行
 *
 * @flavor Ruby: IO::read
 */
static char *
xy_file_read (const char *path)
{
  FILE *fp = fopen (path, "rb");
  if (!fp)
    return xy_strdup ("");

  if (fseek (fp, 0, SEEK_END) != 0)
    {
      fclose (fp);
      return xy_strdup ("");
    }

  long size = ftell (fp);
  if (size < 0)
    {
      fclose (fp);
      return xy_strdup ("");
    }
  rewind (fp);

  char *buf = xy_malloc0 ((size_t) size + 1);
  if (!buf)
    {
      fclose (fp);
      return xy_strdup ("");
    }

  size_t read_bytes = fread (buf, 1, (size_t) size, fp);
  if (read_bytes < (size_t) size && ferror (fp))
    {
      fclose (fp);
      free (buf);
      return xy_strdup ("");
    }

  fclose (fp);
  buf[read_bytes] = '\0';

  char *formatted_str = xy_str_gsub (buf, "\r\n", "\n");
  xy_ptr_replace (&formatted_str, xy_str_gsub (formatted_str, "\r", "\n"));

  free (buf);

  return formatted_str;
}



/******************************************************
 *                      Logging
 ******************************************************/

#define _XY_Log_Plain   000000001
#define _XY_Log_Success 000000001 << 1
#define _XY_Log_Info    000000001 << 2
#define _XY_Log_Warn    000000001 << 3
#define _XY_Log_Error   000000001 << 4

#define xy_log(prompt, str)  _xy_log (_XY_Log_Plain,   prompt, str)
#define xy_succ(prompt,str)  _xy_log (_XY_Log_Success, prompt, str)
#define xy_info(prompt,str)  _xy_log (_XY_Log_Info,    prompt, str)
#define xy_warn(prompt,str)  _xy_log (_XY_Log_Warn,    prompt, str)
#define xy_error(prompt,str) _xy_log (_XY_Log_Error,   prompt, str)

static void
_xy_log (int level, const char *prompt, const char *content)
{
  char *str = NULL;

  bool to_stderr = false;

  /**
   * 'app: content'
   */
  if (level & _XY_Log_Plain)
    {
      str = xy_strcat (3, prompt,  ": ", content);
    }
  else if (level & _XY_Log_Success)
    {
      str = xy_strcat (3, prompt,  ": ", xy_str_to_green (content));
    }
  else if (level & _XY_Log_Info)
    {
      str = xy_strcat (3, prompt,  ": ", xy_str_to_blue (content));
    }
  else if (level & _XY_Log_Warn)
    {
      str = xy_strcat (3, prompt,  ": ", xy_str_to_yellow (content));
      to_stderr = true;
    }
  else if (level & _XY_Log_Error)
    {
      str = xy_strcat (3, prompt,  ": ", xy_str_to_red (content));
      to_stderr = true;
    }
  else
    {
      xy_unreached();
    }

  if (to_stderr)
    {
      fprintf (stderr, "%s\n", str);
    }
  else
    {
      puts (str);
    }
  free (str);
}


/**
 * @flavor brkt 系列输出受 Python 的 pip 启发，为了输出方便，使用 xy.h 的程序应该
 *
 *  1.若想完全自定义颜色和输出位置：
 *
 *    应基于下述 xy_log_brkt_to 定义自己的输出函数
 *
 *  2.若想自定义颜色，而保持输出到stdout：
 *
 *    应基于下述 xy_log_brkt 定义
 *
 *  3.若对log无细致要求，想要快速使用log功能：
 *
 *    应基于下述 xy_<level>_brkt 定义自己的 app_<level>_brkt()，或直接使用 xy_<level>_brkt
 */

static void
xy_log_brkt_to (const char *prompt, const char *content, FILE *stream)
{
  char *str = xy_strcat (4, "[", prompt, "] ", content);
  fprintf (stream, "%s\n", str);
  free (str);
}

#define xy_log_brkt(prompt1,prompt2,content)   _xy_log_brkt(_XY_Log_Plain,  prompt1,prompt2,content)
#define xy_succ_brkt(prompt1,prompt2,content)  _xy_log_brkt(_XY_Log_Success,prompt1,prompt2,content)
#define xy_info_brkt(prompt1,prompt2,content)  _xy_log_brkt(_XY_Log_Info,   prompt1,prompt2,content)
#define xy_warn_brkt(prompt1,prompt2,content)  _xy_log_brkt(_XY_Log_Warn,   prompt1,prompt2,content)
#define xy_error_brkt(prompt1,prompt2,content) _xy_log_brkt(_XY_Log_Error,  prompt1,prompt2,content)

static void
_xy_log_brkt (int level, const char *prompt1, const char *prompt2, const char *content)
{
  char *str = NULL;

  bool to_stderr = false;

  if (level & _XY_Log_Plain)
    {
      str = xy_strcat (6, "[", prompt1, " ", prompt2, "] ", content);
    }
  else if (level & _XY_Log_Success)
    {
      /* [app 成功]  [app success] */
      str = xy_strcat (6,
        "[", xy_str_to_green (prompt1), " ", xy_str_to_bold (xy_str_to_green (prompt2)), "] ", xy_str_to_green (content));
    }
  else if (level & _XY_Log_Info)
    {
      /* [app 信息]  [app info]
         [app 提示]  [app notice]
      */
      str = xy_strcat (6,
        "[", xy_str_to_blue (prompt1), " ", xy_str_to_bold (xy_str_to_blue (prompt2)), "] ", xy_str_to_blue (content));
    }
  else if (level & _XY_Log_Warn)
    {
      /* [app 警告]  [app warn] */
      str = xy_strcat (6,
        "[", xy_str_to_yellow (prompt1), " ", xy_str_to_bold (xy_str_to_yellow (prompt2)), "] ", xy_str_to_yellow (content));
      to_stderr = true;
    }
  else if (level & _XY_Log_Error)
    {
      /* [app 错误]  [app error] */
      str = xy_strcat (6,
        "[", xy_str_to_red (prompt1), " ", xy_str_to_bold (xy_str_to_red (prompt2)), "] ", xy_str_to_red (content));
      to_stderr = true;
    }
  else
    {
      xy_unreached();
    }

  if (to_stderr)
    {
      fprintf (stderr, "%s\n", str);
    }
  else
    {
      puts (str);
    }
  free (str);
}



/******************************************************
 *                      cross OS
 ******************************************************/

static char *
xy_quiet_cmd (const char *cmd)
{
  char *ret = NULL;

  if (xy.on_windows)
    ret = xy_2strcat (cmd, " >nul 2>nul ");
  else
    ret = xy_2strcat (cmd, " 1>/dev/null 2>&1 ");

  return ret;
}


/**
 * @brief 执行 `cmd`，返回某行输出结果，并对已经遍历过的行执行 `func`
 *
 * @param  cmd   要执行的命令
 * @param   n    指定命令执行输出的结果行中的某一行，0 表示最后一行，n (n>0) 表示第n行
 * @param  func  对遍历时经过的行的内容，进行函数调用，如果返回 true，则提前停止遍历
 *
 * @return
 *   1. 第 `n` 行的内容
 *   或
 *   2. `func` 调用后返回为 true 的行
 *
 * @note
 *   1. 由于目标行第 `n` 行会被返回出来，所以 `func` 并不执行目标行，只会执行遍历过的行
 *   或
 *   2. 由于 `func` 调用后返回为 true 的行会被返回出来，所以该返回出的行也被 `func` 执行过了
 */
static char *
xy_run_iter_lines (const char *cmd,  unsigned long n,  bool (*func) (const char *))
{
  const int size = 512;
  char *buf = (char *) malloc (size);

  FILE *stream = popen (cmd, "r");
  if (stream == NULL)
    {
      fprintf (stderr, "xy: 命令执行失败\n");
      return NULL;
    }

  char *ret = NULL;
  unsigned long count = 0;

  while (true)
    {
      if (NULL == fgets (buf, size, stream))
        break;
      /* 存在换行的总是会把换行符读出来，删掉 */
      ret = xy_str_delete_suffix (buf, "\n");
      count += 1;
      if (n == count)
        break;
      if (func)
        {
           if (func (ret))
            break;
        }
    }

  pclose (stream);
  return ret;
}

static char *
xy_run (const char *cmd, unsigned long n)
{
  return xy_run_iter_lines (cmd, n, NULL);
}


/**
 * @brief 执行命令，仅返回命令的执行状态
 */
int
xy_run_get_status (char *cmd)
{
  char * command = xy_quiet_cmd (cmd);

  int status = system (command);
  return status;
}


/**
 * @brief 捕获命令的输出
 *
 * @param[in]  cmd    要执行的命令
 * @param[out] output 捕获的标准输出，
 *                    为NULL时表示不关心stdout输出，但依然允许stderr输出
 *
 * @return 返回命令的执行状态
 */
static int
xy_run_get_stdout (const char *cmd, char **output)
{
  int cap  = 8192; /* 假如1行100个字符，大约支持80行输出 */
  char *buf = (char *) xy_malloc0 (cap);

  FILE *stream = popen (cmd, "r");
  if (stream == NULL)
    {
      fprintf (stderr, "xy: 命令执行失败\n");
      return -1;
    }

  int size = 0;
  size_t n;
  while ((n = fread (buf + size, 1, cap - size, stream)) > 0) {
    size += n;
    if (size == cap)
      {
        cap *= 2;
        char *new_buf = realloc (buf, cap);
        buf = new_buf;
      }
    }
  buf[size] = '\0';

  int status = pclose (stream);

  if (output)
    *output = buf;

    return status;
}


 /**
 * @flavor 该函数同 just 中的 os_family()，只区分 windows, unix
 *
 * @return 返回 "windows" 或 "unix"
 */
#define xy_os_family _xy_os_family ()
static char *
_xy_os_family ()
{
  if (xy.on_windows)
    return "windows";
  else
    return "unix";
}


/**
 * @brief 该函数返回所在 os family 的对应字符串
 */
static const char *
xy_os_depend_str (const char *str_for_win, const char *str_for_unix)
{
  if (xy.on_windows)
    return str_for_win;
  else
    return str_for_unix;
}


/**
 * @brief 返回当前操作系统的 HOME 目录
 *
 * @note  Windows 上返回 %USERPROFILE%，Linux 等返回 $HOME
 * @warning Windows 上要警惕 Documents 等目录被移动位置的情况，避免使用本函数的 HOME 路径直接拼接 Documents，应参考 _xy_win_documents() 的实现
 */
#define xy_os_home _xy_os_home ()
static char *
_xy_os_home ()
{
  char *home = NULL;
  if (xy.on_windows)
    home = getenv ("USERPROFILE");
  else
    home = getenv ("HOME");
  return home;
}


/**
 * @brief 返回 Windows 上的 Documents 目录
 *
 * @note 警告，不可使用 HOME 目录直接拼接，若用户移动了 Documents，将导致错误
 * @warning 非 Windows 返回 NULL
 */
static char *
_xy_win_documents ()
{
#ifdef XY_Build_On_Windows
  char documents_path[MAX_PATH];
  HRESULT result = SHGetFolderPathA (NULL, CSIDL_MYDOCUMENTS, NULL,
                                     SHGFP_TYPE_CURRENT, documents_path);

  if (SUCCEEDED (result))
    return xy_strdup (documents_path);

  return xy_2strcat (xy_os_home, "\\Documents");
#else
  return NULL;
#endif
}

#define xy_win_powershell_profile _xy_win_powershell_profile ()
#define xy_win_powershellv5_profile _xy_win_powershellv5_profile ()

/**
 * @brief 返回 Windows 上 pwsh (>=v5) 的配置文件路径
 *
 * @warning 非 Windows 返回 NULL
 */
static char *
_xy_win_powershell_profile ()
{
  if (xy.on_windows)
    {
      char *documents_dir = _xy_win_documents ();
      char *profile_path = xy_2strcat (documents_dir, "\\PowerShell\\Microsoft.PowerShell_profile.ps1");
      free (documents_dir);
      return profile_path;
    }
  else
    return NULL;
}


/**
 * @brief 返回 Windows 上自带的 powershell (v5) 的配置文件路径
 *
 * @warning 非 Windows 返回 NULL
 */
static char *
_xy_win_powershellv5_profile ()
{
  if (xy.on_windows)
    {
      char *documents_dir = _xy_win_documents ();
      char *profile_path = xy_2strcat (documents_dir, "\\WindowsPowerShell\\Microsoft.PowerShell_profile.ps1");
      free (documents_dir);
      return profile_path;
    }
  else
    return NULL;
}

#define xy_zshrc  "~/.zshrc"
#define xy_bashrc "~/.bashrc"
#define xy_fishrc "~/.config/fish/config.fish"

/**
 * @note Windows上，`path` 不要夹带变量名，因为最终 access() 不会帮你转换
 */
static bool
xy_file_exist (const char *path)
{
  char *expanded_path = NULL;
  const char *check_path = path;

  if (xy_str_start_with (path, "~"))
    {
      expanded_path = xy_2strcat (xy_os_home, path + 1);
      check_path = expanded_path;
    }

  // 0 即 F_OK
  bool result = (0 == access (check_path, 0)) ? true : false;
  if (expanded_path) free (expanded_path);
  return result;
}

/**
 * @note `xy_file_exist()` 和 `xy_dir_exist()` 两个函数在所有平台默认都支持使用 '~'，
 *       但实现中都没有调用 `xy_normalize_path()`，以防万一，调用前可能需要用户手动调用它
 */
static bool
xy_dir_exist (const char *path)
{
  char *allocated_dir = NULL;
  const char *dir = path;
  if (xy.on_windows)
    {
      if (xy_str_start_with (path, "~"))
        {
          allocated_dir = xy_2strcat (xy_os_home, path + 1);
          dir = allocated_dir;
        }
    }

  if (xy.on_windows)
    {
#ifdef XY_Build_On_Windows
      // 也可以用 opendir() #include <dirent.h>
      DWORD attr = GetFileAttributesA (dir);

      bool result = false;
      if (attr == INVALID_FILE_ATTRIBUTES)
        {
          // Q: 我们应该报错吗？
          result = false;
        }
      else if (attr & FILE_ATTRIBUTE_DIRECTORY)
        {
          result = true;
        }
      else
        {
          result = false;
        }
      if (allocated_dir) free (allocated_dir);
      return result;
#endif
    }
  else
    {
      char *tmp_cmd = xy_2strcat ("test -d ", dir);
      int status = system (tmp_cmd);
      free (tmp_cmd);
      bool result = (0==status);
      if (allocated_dir) free (allocated_dir);
      return result;
    }

  return false;
}

/**
 * @brief 规范化路径
 *
 * @details
 *   1. 删除路径左右两边多出来的空白符
 *      - 防止通过间接方式得到的路径包含了空白字符 (如 grep 出来的结果)
 *      - 防止维护者多写了空白字符
 *   2. 将 ~ 转换为绝对路径
 *   3. 在Windows上，使用标准的 \ 作为路径分隔符
 */
static char *
xy_normalize_path (const char *path)
{
  char *new = xy_str_strip (path);

  if (xy_str_start_with (new, "~"))
    {
      char *tmp = xy_str_delete_prefix (new, "~");
      char *joined = xy_2strcat (xy_os_home, tmp);
      free (tmp);
      xy_ptr_replace (&new, joined);
    }

  if (xy.on_windows)
    {
      xy_ptr_replace (&new, xy_str_gsub (new, "/", "\\"));
    }
  return new;
}


/**
 * @brief 返回一个路径的父目录名
 *
 * @note
 *   - 返回的是真正的 "目录名" (就像文件名一样)，而不是 "路径"，所以一定是不含末尾斜杠的
 *   - 在Windows上，使用标准的 \ 作为路径分隔符
 */
static char *
xy_parent_dir (const char *path)
{
  char *dir = xy_normalize_path (path);

  /* 不管是否为Windows，全部统一使用 / 作为路径分隔符，方便后续处理 */
  xy_ptr_replace (&dir, xy_str_gsub (dir, "\\", "/"));

  if (xy_str_end_with (dir, "/"))
    xy_ptr_replace (&dir, xy_str_delete_suffix (dir, "/"));

  char *last = NULL;

  last = strrchr (dir, '/');
  if (!last)
    {
      /* 路径中没有一个 / 是很奇怪的，我们直接返回 . 作为当前目录 */
      return ".";
    }
  *last = '\0';

  /* Windows上重新使用 \ 作为路径分隔符 */
  if (xy.on_windows)
    {
      xy_ptr_replace (&dir, xy_str_gsub (dir, "/", "\\"));
    }
  return dir;
}



void
xy_detect_os ()
{
  // C:
  char *drive = getenv ("SystemDrive");
  if (drive)
    {
      char path[256];
      snprintf (path, sizeof (path), "%s\\Users", drive);
      DIR *d = opendir (path);
      if (d)
        {
          xy.on_windows = true;
          closedir (d);
          return;
        }
    }

  FILE *fp = fopen ("/proc/version", "r");
  if (fp)
    {
      char buf[256] = {0};
      fread (buf, 1, sizeof (buf) - 1, fp);
      fclose (fp);
      if (strstr (buf, "Linux"))
        {
          xy.on_linux = true;
          return;
        }
    }

  // @consult https://android.googlesource.com/platform/system/core/+/refs/heads/main/rootdir/init.environ.rc.in
  char *android_env = getenv ("ANDROID_ROOT");
  if (xy_str_find (android_env, "/system").found)
    {
      xy.on_android = true;
      return;
    }

  /* 判断 macOS */
  DIR *d = opendir ("/System/Applications");
  if (d)
    {
      closedir (d);
      d = opendir ("/Library/Apple");
      if (d)
        {
          xy.on_macos = true;
          closedir (d);
          return;
        }
    }

  /* 最后判断 BSD */
  fp = popen ("uname -s", "r");
  if (!fp)
    {
      DIR *bsd_dir = opendir ("/etc/rc.d");
      if (bsd_dir)
        {
          closedir (bsd_dir);
          xy.on_bsd = true;
          return;
        }
    }
  else
    {
      char buf[256];
      fgets (buf, sizeof (buf), fp);
      pclose (fp);
      if (strstr (buf, "BSD")  != NULL)
        xy.on_bsd = true;
        return;
    }

  if (!(xy.on_windows || xy.on_linux || xy.on_android || xy.on_macos || xy.on_bsd))
    xy_throw ("Unknown operating system");
}


void
xy_use_utf8 ()
{
#ifdef XY_Build_On_Windows
  SetConsoleOutputCP (65001);
#endif
}


/**
 * @note 该函数必须被首先调用，方能使用各个跨操作系统的函数
 */
void
xy_init ()
{
  xy_detect_os ();

  if (xy.on_windows)
    xy.os_devnull = "nul";
  else
    xy.os_devnull = "/dev/null";

  xy_use_utf8 ();

  xy.inited = true;
}


/******************************************************
 *                      Container
 ******************************************************/
typedef struct XySeqItem_t
{
  struct XySeqItem_t *prev;
  struct XySeqItem_t *next;

  void *data;
}
XySeqItem_t;

typedef struct XySeq_t
{
  XySeqItem_t *first_item;
  XySeqItem_t *last_item;

  uint32_t length;
}
XySeq_t;


XySeq_t*
xy_seq_new (void)
{
  XySeq_t *seq = xy_malloc0 (sizeof (XySeq_t));
  if (!seq) return NULL;

  seq->first_item = NULL;
  seq->last_item = NULL;
  seq->length = 0;

  return seq;
}

/**
 * @flavor Python: len()
 */
uint32_t
xy_seq_len (XySeq_t *seq)
{
  xy_cant_be_null (seq);
  return seq->length;
}

/**
 * @flavor Ruby: Enumerable#first
 */
void *
xy_seq_first (XySeq_t *seq)
{
  xy_cant_be_null (seq);
  return seq->first_item ? seq->first_item->data : NULL;
}

/**
 * @flavor Ruby: Enumerable#last
 */
void *
xy_seq_last (XySeq_t *seq)
{
  xy_cant_be_null (seq);
  return seq->last_item ? seq->last_item->data : NULL;
}

/**
 * @flavor Ruby: Array#at
 *
 * @note 序号从1开始
 *
 * @return 如果seq中并没有第n个数据，则返回NULL
 */
void *
xy_seq_at (XySeq_t *seq, int n)
{
  xy_cant_be_null (seq);

  if (0 == n) xy_throw ("The index must begin from 1, not 0");

  if (1 == n) return seq->first_item ? seq->first_item->data : NULL;

  XySeqItem_t *it = seq->first_item;
  for (uint32_t i = 1; i < n && it; i++)
    {
      it = it->next;
    }
  return it ? it->data : NULL;
}


/**
 * @flavor Perl: push
 */
void
xy_seq_push (XySeq_t *seq, void *data)
{
  xy_cant_be_null (seq);

  XySeqItem_t *it = xy_malloc0 (sizeof (XySeqItem_t));;

  it->data = data;
  it->prev = NULL;
  it->next = NULL;

  // 更新 item 间关系
  XySeqItem_t *l = seq->last_item;
  if (l)
    {
      it->prev = l;
      l->next = it;
    }

  // 更新 seq 信息
  seq->last_item = it;
  if (0==seq->length)
    seq->first_item = it;

  seq->length++;
}


/**
 * @flavor Perl: pop
 */
void *
xy_seq_pop (XySeq_t *seq)
{
  xy_cant_be_null (seq);

  if (0==seq->length) return NULL;

  // 更新 item 间关系
  XySeqItem_t *l = seq->last_item;
  XySeqItem_t *p = l->prev;
  // 考虑 seq 当前只有1项的情况
  if (p) p->next = NULL;
  l->prev = NULL;

  // 更新 seq 信息
  seq->last_item = p;
  // 考虑 seq 当前只有1项的情况
  if (!p) seq->first_item = NULL;
  seq->length--;

  void *data = l->data;
  free (l);
  return data;
}


/**
 * @flavor Ruby: Array#each
 */
void
xy_seq_each (XySeq_t *seq, void (*func) (void *, void *), void *user_data)
{
  xy_cant_be_null (seq);
  xy_cant_be_null (func);

  for (XySeqItem_t *it = seq->first_item; it; it = it->next)
    {
      func (it->data, user_data);
    }
}

/**
 * @flavor Ruby: Enumerable#find
 */
void *
xy_seq_find (XySeq_t *seq, bool (*func) (void *, void *), void *user_data)
{
  xy_cant_be_null (seq);
  xy_cant_be_null (func);

  for (XySeqItem_t *it = seq->first_item; it; it = it->next)
    {
      if (func (it->data, user_data))
        {
          return it->data;
        }
    }
  return NULL;
}



#define _XY_Map_Buckets_Count 97

struct _XyHashBucket_t
{
  struct _XyHashBucket_t *next;
  char *key;
  void *value;
};

typedef struct XyMap_t
{
  struct _XyHashBucket_t **buckets;

  uint32_t length;
}
XyMap_t;


XyMap_t *
xy_map_new ()
{
  XyMap_t *map = xy_malloc0 (sizeof (XyMap_t));
  map->buckets = xy_malloc0 (sizeof (struct _XyHashBucket_t *) * _XY_Map_Buckets_Count);

  map->length = 0;

  return map;
}


uint32_t
xy_map_len (XyMap_t *map)
{
  xy_cant_be_null (map);
  return map->length;
}


unsigned long
xy_hash (const char* str)
{
  unsigned long h = 5381;
  int c;
  while ((c = *str++))
    h = ((h << 5) + h) + c; /* h * 33 + c */
  return h;
}


/**
 * @flavor JavaScript: map.set
 */
void
xy_map_set (XyMap_t *map, const char *key, void *value)
{
  xy_cant_be_null (map);
  xy_cant_be_null (key);

  unsigned long hash = xy_hash (key);
  uint32_t index = hash % _XY_Map_Buckets_Count;

  // 若 key 已经存在
  struct _XyHashBucket_t *maybe = map->buckets[index];
  while (maybe)
    {
      if (xy_streql (maybe->key, key))
        {
          maybe->value = value;
          return;
        }
      maybe = maybe->next;
    }

  // 若 key 不存在
  struct _XyHashBucket_t *bucket = xy_malloc0 (sizeof (struct _XyHashBucket_t));

  bucket->key = xy_strdup (key);
  bucket->value = value;
  bucket->next = map->buckets[index];
  map->buckets[index] = bucket;

  map->length++;
}


/**
 * @flavor JavaScript: map.get
 */
void *
xy_map_get (XyMap_t *map, const char *key)
{
  xy_cant_be_null (map);
  xy_cant_be_null (key);

  unsigned long hash = xy_hash (key);
  uint32_t index = hash % _XY_Map_Buckets_Count;

  struct _XyHashBucket_t *maybe = map->buckets[index];
  while (maybe)
    {
      if (xy_streql (maybe->key, key))
        {
          return maybe->value;
        }
      maybe = maybe->next;
    }

  return NULL;
}

/**
 * @flavor Ruby: Hash#each
 */
void
xy_map_each (
  XyMap_t *map,
  void (*func) (const char *key, void *value, void *user_data),
  void *user_data)
{
  xy_cant_be_null (map);
  xy_cant_be_null (func);

  for (uint32_t i = 0; i < _XY_Map_Buckets_Count; i++)
    {
      struct _XyHashBucket_t *bucket = map->buckets[i];
      while (bucket)
        {
          func (bucket->key, bucket->value, user_data);
          bucket = bucket->next;
        }
    }
}


#endif
