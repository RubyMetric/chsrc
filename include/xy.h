/** ------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * -------------------------------------------------------------
 * File          : xy.h
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 *               | Heng Guo   <2085471348@qq.com>
 * Contributors  : Null Nil   <null@nil.com>
 *               |
 * Created on    : <2023-08-28>
 * Last modified : <2024-07-09>
 *
 * xy: 襄阳、咸阳
 * Corss-Platform C utilities for CLI applications in Ruby flavor
 * ------------------------------------------------------------*/

#ifndef XY_H
#define XY_H

#define _XY_Version      "v0.1.3.0-2024/07/09"
#define _XY_Maintain_URL "https://gitee.com/RubyMetric/chsrc/blob/main/include/xy.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #define NDEBUG

#ifdef _WIN32
  #define xy_on_windows true
  #define xy_on_linux false
  #define xy_on_macos false
  #define xy_on_bsd false
  #define xy_os_devnull "nul"
  #include <windows.h>
  #define xy_useutf8() SetConsoleOutputCP (65001)

#elif defined(__linux__) || defined(__linux)
  #define xy_on_windows false
  #define xy_on_linux true
  #define xy_on_macos false
  #define xy_on_bsd false
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()

#elif defined(__APPLE__)
  #define xy_on_windows false
  #define xy_on_linux false
  #define xy_on_macos true
  #define xy_on_bsd false
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
  #define xy_on_windows false
  #define xy_on_linux false
  #define xy_on_macos false
  #define xy_on_bsd true
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()
#endif

void putf (double n)         { printf ("%f\n", n);   }
void puti (long long n)      { printf ("%lld\n", n); }
void putb (bool n)           { if (n) puts ("true"); else puts ("false"); }
void print (const char *s)   { printf ("%s", s);  }
void println (const char *s) { printf ("%s\n", s);}
void say (const char *s)     { printf ("%s\n", s);}

#define assert_str(a, b) assert (xy_streql ((a), (b)))

#define xy_unsupport   assert(!"Unsuppoted")
#define xy_unimplement assert(!"Unimplemented temporarily")
#define xy_unreach     assert(!"This code shouldn't be reached")

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
 * 将str中所有的pat字符串替换成replace，返回一个全新的字符串
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

static char *
xy_strjoin (unsigned int count, ...)
{
  size_t al_fixed = 128;
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
      // 是否需要重新分配
      bool need_realloc = false;

      str = va_arg (args, const char *);
      al_need += strlen (str);
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

static char *
xy_strdup (const char *str)
{
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

  // -2 把中间%s减掉
  size_t len = strlen (color_fmt_str) - 2;
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
 * remarkably 系列输出受 pip 启发，为了输出方便，使用xy.h的程序应该基于此再定义自己的 app_info_remarkbaly()
 */
#define xy_log_remarkably(prompt1,prompt2,content)   _xy_log_remarkably(_XY_Log_Plain,  prompt1,prompt2,content)
#define xy_succ_remarkably(prompt1,prompt2,content)  _xy_log_remarkably(_XY_Log_Success,prompt1,prompt2,content)
#define xy_info_remarkably(prompt1,prompt2,content)  _xy_log_remarkably(_XY_Log_Info,   prompt1,prompt2,content)
#define xy_warn_remarkably(prompt1,prompt2,content)  _xy_log_remarkably(_XY_Log_Warn,   prompt1,prompt2,content)
#define xy_error_remarkably(prompt1,prompt2,content) _xy_log_remarkably(_XY_Log_Error,  prompt1,prompt2,content)

static void
_xy_log_remarkably (int level, const char *prompt1, const char *prompt2, const char *content)
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
 * 执行cmd，返回某行输出结果，并对已经遍历过的行执行iter_func
 *
 * @param  cmd        要执行的命令
 * @param   n         指定命令执行输出的结果行中的某一行，0 表示最后一行，n (n>0) 表示第n行
 *                    该函数会返回这一行的内容
 * @param  iter_func  对遍历时经过的行的内容，进行函数调用
 *
 * @note 返回的字符串最后面一般有换行符号
 *
 * 由于目标行会被返回出来，所以 iter_func() 并不执行目标行，只会执行遍历过的行
 */
static char *
xy_run (const char *cmd,  unsigned long n,  void (*iter_func) (const char *))
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
      ret = buf;
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

#define xy_win_powershell_profile _xy_win_powershell_profile ()
#define xy_win_powershellv5_profile _xy_win_powershellv5_profile ()
static char *
_xy_win_powershell_profile ()
{
  return xy_2strjoin (
      xy_os_home, "\\Documents\\PowerShell\\Microsoft.PowerShell_profile.ps1");
}

char *
_xy_win_powershellv5_profile ()
{
  return xy_2strjoin (
      xy_os_home,
      "\\Documents\\WindowsPowerShell\\Microsoft.PowerShell_profile.ps1");
}

/**
 * @note Windows上，`path` 不要夹带变量名，因为最终 access() 不会帮你转换
 */
static bool
xy_file_exist (const char *path)
{
  const char *newpath = path;
  if (xy_on_windows)
    {
      if (xy_str_start_with (path, "~"))
        {
          newpath = xy_2strjoin (xy_os_home, path + 1);
        }
    }
  return access (newpath, 0) ? false : true;
}

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
 * 该函数即使在非Windows下也可调用，作用是删除路径左右两边多出来的空白符
 */
static char *
xy_uniform_path (const char *path)
{
  char *new = xy_str_strip (path); // 防止开发者多写了空白符

  // 这个函数仅在Windows上才进行替换
  if (xy_on_windows)
    {
      if (xy_str_start_with (new, "~/"))
        {
          // 或 %USERPROFILE%
          new = xy_strjoin (3, xy_os_home, "\\",
                            xy_str_delete_prefix (new, "~/"));
        }
      new = xy_str_gsub (new, "/", "\\");
    }

  return new;
}

static char *
xy_parent_dir (const char *path)
{
  char *dir = xy_uniform_path (path);
  char *last = NULL;
  if (xy_on_windows)
    {
      last = strrchr (dir, '\\');
      *last = '\0';
    }
  else
    {
      last = strrchr (dir, '/');
      *last = '\0';
    }
  return dir;
}

#endif
