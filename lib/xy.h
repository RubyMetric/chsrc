/** ------------------------------------------------------------
 * Copyright © 2023-2025 Aoran Zeng, Heng Guo
 * SPDX-License-Identifier: MIT
 * -------------------------------------------------------------
 * Lib Name      :   xy.h
 * Lib Authors   :  曾奥然  <ccmywish@qq.com>
 *               |   郭恒   <2085471348@qq.com>
 * Contributors  :   juzeon <skyjuzheng@gmail.com>
 *               | Mikachu2333 <mikachu.23333@zohomail.com>
 *               |
 * Created On    : <2023-08-28>
 * Last Modified : <2025-08-20>
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

#define _XY_Version       "v0.1.6.0-2025/08/18"
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

#if defined(__STDC__) && __STDC_VERSION__ >= 202311
  #define XY_Deprecate_This(msg) [[deprecated(msg)]]
#elif defined(__GNUC__) || defined(__clang__)
  #define XY_Deprecate_This(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
  #define XY_Deprecate_This(msg) __declspec(deprecated(msg))
#else
  #define XY_Deprecate_This(msg)
#endif


/* Global */
bool xy_enable_color = true;

// #define NDEBUG

#ifdef _WIN32
  #define XY_On_Windows 1
  #define xy_on_windows true
  #define xy_on_linux false
  #define xy_on_macos false
  #define xy_on_bsd false
  #define xy_os_devnull "nul"
  #include <windows.h>
  #include <shlobj.h>
  #define xy_useutf8() SetConsoleOutputCP (65001)

#elif defined(__linux__) || defined(__linux)
  #define XY_On_Linux 1
  #define xy_on_windows false
  #define xy_on_linux true
  #define xy_on_macos false
  #define xy_on_bsd false
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()

#elif defined(__APPLE__)
  #define XY_On_macOS 1
  #define xy_on_windows false
  #define xy_on_linux false
  #define xy_on_macos true
  #define xy_on_bsd false
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
  #define XY_On_BSD 1
  #define xy_on_windows false
  #define xy_on_linux false
  #define xy_on_macos false
  #define xy_on_bsd true
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()
#endif

#define assert_str(a, b) assert (xy_streql ((a), (b)))

#define xy_unsupported()    assert(!"Unsuppoted")
#define xy_unimplemented()  assert(!"Unimplemented temporarily")
#define xy_unreached()      assert(!"This code shouldn't be reached")
#define xy_noop()           (void)0

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

#define print(x) _Generic((x), \
  int:       _xy_print_int,  \
  long:      _xy_print_long, \
  long long: _xy_print_long_long, \
  float:     _xy_print_float,  \
  double:    _xy_print_double, \
  bool:      _xy_print_bool, \
  char *:    _xy_print_str,  \
  const char *:   _xy_print_const_str, \
  default:   assert(!"Unsupported type for print()!") \
)(x)

#define println(x) _Generic((x), \
  int:       _xy_println_int,  \
  long:      _xy_println_long, \
  long long: _xy_println_long_long, \
  float:     _xy_println_float,  \
  double:    _xy_println_double, \
  bool:      _xy_println_bool, \
  char *:    _xy_println_str,  \
  const char *:   _xy_println_const_str, \
  default:   assert(!"Unsupported type for println()/say()!") \
)(x)
/* @flavor Perl/Raku */
#define say println
/* @flavor PHP */
#define echo println
/* @flavor HTML */
void br ()                   { puts (""); }
void p (const char *s)       { printf ("%s\n", s); }

#define xy_arylen(x) (sizeof (x) / sizeof (x[0]))

static inline void *
xy_malloc0 (size_t size)
{
  void *ptr = malloc (size);
  memset (ptr, 0, size);
  return ptr;
}


/******************************************************
 *                      String
 ******************************************************/

/**
 * @brief 将 str 中所有的 pat 字符串替换成 replace，返回一个全新的字符串；也可用作删除、缩小、扩张
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

static char *
xy_2strjoin (const char *str1, const char *str2)
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
 * @param count 连接的字符串数量
 * @param ...   连接的字符串
 *
 * @return 拼接的新字符串
 */
static char *
xy_strjoin (unsigned int count, ...)
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

  if (!xy_enable_color)
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
xy_streql_ic(const char *str1, const char *str2)
{
  if (NULL == str1 || NULL == str2)
    {
      return false;
    }

  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  if (len1 != len2)
    {
      return false;
    }

  for (size_t i = 0; i < len1; i++)
    {
      if (tolower(str1[i]) != tolower(str2[i]))
        {
          return false;
        }
    }
  return true;
}

static char *
xy_str_to_quietcmd (const char *cmd)
{
  char *ret = NULL;
#ifdef _WIN32
  ret = xy_2strjoin (cmd, " >nul 2>nul ");
#else
  ret = xy_2strjoin (cmd, " 1>/dev/null 2>&1 ");
#endif
  return ret;
}

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

static char *
xy_str_delete_prefix (const char *str, const char *prefix)
{
  char *new = xy_strdup (str);
  bool yes = xy_str_start_with (str, prefix);
  if (!yes)
    return new;

  size_t len = strlen (prefix);
  char *cur = new + len;
  return cur;
}

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

static char *
xy_str_strip (const char *str)
{
  char *new = xy_strdup (str);

  while (strchr ("\n\r\v\t\f ", new[0]))
    {
      new += 1;
    }

  size_t len = strlen (new);

  char *last = new + len - 1;

  while (strchr ("\n\r\v\t\f ", *last))
    {
      *last = '\0';
      last -= 1;
    }
  return new;
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
      str = xy_strjoin (3, prompt,  ": ", content);
    }
  else if (level & _XY_Log_Success)
    {
      str = xy_strjoin (3, prompt,  ": ", xy_str_to_green (content));
    }
  else if (level & _XY_Log_Info)
    {
      str = xy_strjoin (3, prompt,  ": ", xy_str_to_blue (content));
    }
  else if (level & _XY_Log_Warn)
    {
      str = xy_strjoin (3, prompt,  ": ", xy_str_to_yellow (content));
      to_stderr = true;
    }
  else if (level & _XY_Log_Error)
    {
      str = xy_strjoin (3, prompt,  ": ", xy_str_to_red (content));
      to_stderr = true;
    }
  else
    {
      // xy_assert ("CAN'T REACH!");
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
 * brkt 系列输出受 pip 启发，为了输出方便，使用 xy.h 的程序应该
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
  char *str = xy_strjoin (4, "[", prompt, "] ", content);
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
      str = xy_strjoin (6, "[", prompt1, " ", prompt2, "] ", content);
    }
  else if (level & _XY_Log_Success)
    {
      /* [app 成功]  [app success] */
      str = xy_strjoin (6,
        "[", xy_str_to_green (prompt1), " ", xy_str_to_bold (xy_str_to_green (prompt2)), "] ", xy_str_to_green (content));
    }
  else if (level & _XY_Log_Info)
    {
      /* [app 信息]  [app info]
         [app 提示]  [app notice]
      */
      str = xy_strjoin (6,
        "[", xy_str_to_blue (prompt1), " ", xy_str_to_bold (xy_str_to_blue (prompt2)), "] ", xy_str_to_blue (content));
    }
  else if (level & _XY_Log_Warn)
    {
      /* [app 警告]  [app warn] */
      str = xy_strjoin (6,
        "[", xy_str_to_yellow (prompt1), " ", xy_str_to_bold (xy_str_to_yellow (prompt2)), "] ", xy_str_to_yellow (content));
      to_stderr = true;
    }
  else if (level & _XY_Log_Error)
    {
      /* [app 错误]  [app error] */
      str = xy_strjoin (6,
        "[", xy_str_to_red (prompt1), " ", xy_str_to_bold (xy_str_to_red (prompt2)), "] ", xy_str_to_red (content));
      to_stderr = true;
    }
  else
    {
      // xy_assert ("CAN'T REACH!");
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
 *                      System
 ******************************************************/

/**
 * @brief 执行cmd，返回某行输出结果，并对已经遍历过的行执行iter_func
 *
 * @param  cmd        要执行的命令
 * @param   n         指定命令执行输出的结果行中的某一行，0 表示最后一行，n (n>0) 表示第n行
 * @param  iter_func  对遍历时经过的行的内容，进行函数调用
 *
 * @return            该函数会返回 参数 n 指定的该行的内容
 *
 * @note 返回的字符串最后面一般有换行符号
 *
 * 由于目标行会被返回出来，所以 iter_func() 并不执行目标行，只会执行遍历过的行
 */
static char *
xy_run_iter (const char *cmd,  unsigned long n,  void (*iter_func) (const char *))
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
      if (iter_func)
        {
          iter_func (buf);
        }
    }

  pclose (stream);
  return ret;
}

static char *
xy_run (const char *cmd, unsigned long n)
{
  return xy_run_iter (cmd, n, NULL);
}




/******************************************************
 *                      cross OS
 ******************************************************/

 /**
 * @note 该函数同 just 中的 os_family()，只区分 windows, unix
 * @return 返回 "windows" 或 "unix"
 */
#define xy_os_family _xy_os_family ()
static char *
_xy_os_family ()
{
  if (xy_on_windows)
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
  if (xy_on_windows)
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
  if (xy_on_windows)
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
#ifdef XY_On_Windows
  char documents_path[MAX_PATH];
  HRESULT result = SHGetFolderPathA (NULL, CSIDL_MYDOCUMENTS, NULL,
                                     SHGFP_TYPE_CURRENT, documents_path);

  if (SUCCEEDED (result))
    return xy_strdup (documents_path);

  return xy_2strjoin (xy_os_home, "\\Documents");
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
  if (xy_on_windows)
    {
      char *documents_dir = _xy_win_documents ();
      char *profile_path = xy_2strjoin (documents_dir, "\\PowerShell\\Microsoft.PowerShell_profile.ps1");
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
  if (xy_on_windows)
    {
      char *documents_dir = _xy_win_documents ();
      char *profile_path = xy_2strjoin (documents_dir, "\\WindowsPowerShell\\Microsoft.PowerShell_profile.ps1");
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
  const char *new_path = path;
  if (xy_str_start_with (path, "~"))
    {
      new_path = xy_2strjoin (xy_os_home, path + 1);
    }
  // 0 即 F_OK
  return (0==access (new_path, 0)) ? true : false;
}

/**
 * @note `xy_file_exist()` 和 `xy_dir_exist()` 两个函数在所有平台默认都支持使用 '~'，
 *       但实现中都没有调用 `xy_normalize_path()`，以防万一，调用前可能需要用户手动调用它
 */
static bool
xy_dir_exist (const char *path)
{
  const char *dir = path;
  if (xy_on_windows)
    {
      if (xy_str_start_with (path, "~"))
        {
          dir = xy_2strjoin (xy_os_home, path + 1);
        }
    }

  if (xy_on_windows)
    {
#ifdef XY_On_Windows
      // 也可以用 opendir() #include <dirent.h>
      DWORD attr = GetFileAttributesA (dir);

      if (attr == INVALID_FILE_ATTRIBUTES)
        {
          // Q: 我们应该报错吗？
          return false;
        }
      else if (attr & FILE_ATTRIBUTE_DIRECTORY)
        {
          return true;
        }
      else
        {
          return false;
        }
#endif
    }
  else
    {
      int status = system (xy_2strjoin ("test -d ", dir));

      if (0==status)
        return true;
      else
        return false;
    }
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
      new = xy_2strjoin (xy_os_home, xy_str_delete_prefix (new, "~"));
    }

  if (xy_on_windows)
    return xy_str_gsub (new, "/", "\\");
  else
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
  dir = xy_str_gsub (dir, "\\", "/");

  if (xy_str_end_with (dir, "/"))
    dir = xy_str_delete_suffix (dir, "/");

  char *last = NULL;

  last = strrchr (dir, '/');
  if (!last)
    {
      /* 路径中没有一个 / 是很奇怪的，我们直接返回 . 作为当前目录 */
      return ".";
    }
  *last = '\0';

  /* Windows上重新使用 \ 作为路径分隔符 */
  if (xy_on_windows)
    return xy_str_gsub (dir, "/", "\\");
  else
    return dir;
}

#endif
