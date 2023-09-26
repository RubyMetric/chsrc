/** ------------------------------------------------------------
 * File          : xy.h
 * License       : MIT
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-08-28>
 * Last modified : <2023-09-26>
 *
 * xy:
 *
 *   y = f(x)
 *
 *   Corss-Platform C utilities for CLI applications in Ruby flavor
 *
 *   该文件采用 MIT 许可证，请查阅 LICENSE.txt 文件
 * ------------------------------------------------------------*/

#ifndef XY_H
#define XY_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h> // For access()

// #define NDEBUG

#ifdef _WIN32
  #define xy_on_windows true
  #define xy_on_linux   false
  #define xy_on_macos   false
  #define xy_on_bsd     false
  #define xy_os_devnull "nul"
  #include <windows.h>
  #define xy_useutf8() SetConsoleOutputCP(65001)

#elif defined(__linux__) || defined(__linux)
  #define xy_on_windows false
  #define xy_on_linux   true
  #define xy_on_macos   false
  #define xy_on_bsd     false
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()

#elif defined(__APPLE__)
  #define xy_on_windows false
  #define xy_on_linux   false
  #define xy_on_macos   true
  #define xy_on_bsd     false
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
  #define xy_on_windows false
  #define xy_on_linux   false
  #define xy_on_macos   false
  #define xy_on_bsd     true
  #define xy_os_devnull "/dev/null"
  #define xy_useutf8()
#endif


void putf(double n)    {printf("%f\n",  n);}
void puti(long long n) {printf("%lld\n", n);}
void putb(bool n)      {if(n) puts("true"); else puts("false");}

#define xy_arylen(x) (sizeof(x) / sizeof(x[0]))


static inline void*
xy_malloc0 (size_t size)
{
  void* ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}


#define XY_Log_Info     00001
#define XY_Log_Success  00001<<1
#define XY_Log_Warn     00001<<2
#define XY_Log_Error    00001<<3

#define xy_success(str)  _xy_log (XY_Log_Success, str)
#define xy_info(str)     _xy_log (XY_Log_Info,    str)
#define xy_warn(str)     _xy_log (XY_Log_Warn,    str)
#define xy_error(str)    _xy_log (XY_Log_Error,   str)

static void
_xy_log (int level, const char* str)
{
  char* color_fmt_str = NULL;

  bool to_stderr = false;

  if (level & XY_Log_Info) {
    color_fmt_str = "\033[34m%s\033[0m";   // 蓝色
  }
  else if (level & XY_Log_Success) {
    color_fmt_str = "\033[32m%s\033[0m";   // 绿色
  }
  else if (level & XY_Log_Warn) {
    color_fmt_str = "\033[33m%s\033[0m\n"; // 黄色
    to_stderr = true;
  }
  else if (level & XY_Log_Error) {
    color_fmt_str = "\033[31m%s\033[0m\n"; // 红色
    to_stderr = true;
  }
  else {
    //xy_assert ("CAN'T REACH!");
  }

  // -2 把中间%s减掉
  size_t len = strlen(color_fmt_str) -2;
  char* buf = malloc(strlen(str) + len + 1);

  sprintf (buf, color_fmt_str, str);
  if (to_stderr) {
    fprintf(stderr, "%s",buf);
  } else {
    puts(buf);
  }
  free(buf);
}



/**
 * 将str中所有的src字符替换成dest,并返回一个全新的字符串
 * 现在已经废弃不用
 */
static char*
xy_strch (const char* str, char src,const char* dest)
{
  size_t str_len  = strlen(str);
  size_t dest_len = strlen(dest);
  size_t size = str_len*dest_len;
  char* ret  = (char*)malloc(size);
  int i=0;
  int j=0;
  while(i<str_len) {
    if(str[i]==src) {
      int k=0;
      while(k<dest_len){
        ret[j++] = dest[k++];
      }
      i++;
    }
    else {
      ret[j++] = str[i++];
    }
  }
  ret[j] = 0;
  return ret;
}


static char*
xy_2strjoin (const char* str1, const char* str2)
{
  size_t len  = strlen(str1);
  size_t size = len + strlen(str2) + 1;
  char* ret  = malloc(size);
  strcpy(ret, str1);
  strcpy(ret+len, str2);
  return ret;
}


static char*
xy_strjoin (unsigned int count, ...)
{
  size_t al_fixed = 128;
  char* ret = calloc(1, al_fixed);
  // 已分配次数
  int al_times = 1;
  // 当前已分配量
  size_t al_cur = al_fixed;

  const char* str = NULL;
  // 需要分配的量
  size_t al_need = 0;
  // 用于 strcpy() 到 ret 的哪个位置
  char* cur  = ret + 0;

  va_list args;
  va_start(args, count);

  for(int i=0; i<count; i++)
  {
    // 是否需要重新分配
    bool need_realloc = false;

    str = va_arg(args, const char*);
    al_need += strlen(str);
    while (al_need > al_cur) {
      al_times += 1; al_cur = al_times * al_fixed;
      need_realloc = true;
    }
    // printf("al_times %d, al_need %zd, al_cur %zd\n", al_times, al_need, al_cur);
    if (need_realloc) {
      ptrdiff_t diff = cur - ret;
      ret = realloc(ret, al_cur);
      cur = ret + diff;
    }
    if (NULL==ret) {
      xy_error ("xy: No availble memory!"); return NULL;
    }
    strcpy(cur, str);
    // puts(ret);
    cur += strlen(str);
  }
  va_end(args);

  *cur = '\0';
  return ret;
}


static char*
xy_strdup(const char* str)
{
  size_t len = strlen(str);
  char* new = xy_malloc0(len+1);
  strcpy(new, str);
  return new;
}


#define XY_Str_Bold      1
#define XY_Str_Faint     2
#define XY_Str_Italic    3
#define XY_Str_Underline 4
#define XY_Str_Blink     5
#define XY_Str_Cross     9

#define xy_str_to_bold(str)      _xy_str_to_terminal_style(XY_Str_Bold,     str)
#define xy_str_to_faint(str)     _xy_str_to_terminal_style(XY_Str_Faint,    str)
#define xy_str_to_italic(str)    _xy_str_to_terminal_style(XY_Str_Italic,   str)
#define xy_str_to_underline(str) _xy_str_to_terminal_style(XY_Str_Underline,str)
#define xy_str_to_blink(str)     _xy_str_to_terminal_style(XY_Str_Blink,    str)
#define xy_str_to_cross(str)     _xy_str_to_terminal_style(XY_Str_Cross,    str)

#define XY_Str_Red      31
#define XY_Str_Green    32
#define XY_Str_Yellow   33
#define XY_Str_Blue     34
#define XY_Str_Magenta  35
#define XY_Str_Cyan     36

#define xy_str_to_red(str)     _xy_str_to_terminal_style(XY_Str_Red,    str)
#define xy_str_to_green(str)   _xy_str_to_terminal_style(XY_Str_Green,  str)
#define xy_str_to_yellow(str)  _xy_str_to_terminal_style(XY_Str_Yellow, str)
#define xy_str_to_blue(str)    _xy_str_to_terminal_style(XY_Str_Blue,   str)
#define xy_str_to_magenta(str) _xy_str_to_terminal_style(XY_Str_Magenta,str)
#define xy_str_to_purple        xy_str_to_magenta
#define xy_str_to_cyan(str)    _xy_str_to_terminal_style(XY_Str_Cyan,   str)

static char*
_xy_str_to_terminal_style(int style, const char* str)
{
  char* color_fmt_str = NULL;
  if (XY_Str_Red==style)
  {
    color_fmt_str = "\e[31m%s\e[0m"; // 红色
  }
  else if (XY_Str_Green==style)
  {
    color_fmt_str = "\e[32m%s\e[0m"; // 绿色
  }
  else if (XY_Str_Yellow==style)
  {
    color_fmt_str = "\e[33m%s\e[0m"; // 黄色
  }
  else if (XY_Str_Blue==style)
  {
    color_fmt_str = "\e[34m%s\e[0m"; // 蓝色
  }
  else if (XY_Str_Magenta==style)
  {
    color_fmt_str = "\e[35m%s\e[0m"; // 蓝色
  }
  else if (XY_Str_Cyan==style)
  {
    color_fmt_str = "\e[36m%s\e[0m"; // 蓝色
  }


  else if (XY_Str_Bold==style)
  {
    color_fmt_str = "\e[1m%s\e[0m";
  }
  else if (XY_Str_Faint==style)
  {
    color_fmt_str = "\e[2m%s\e[0m";
  }
  else if (XY_Str_Italic==style)
  {
    color_fmt_str = "\e[3m%s\e[0m";
  }
  else if (XY_Str_Underline==style)
  {
    color_fmt_str = "\e[4m%s\e[0m";
  }
  else if (XY_Str_Blink==style)
  {
    color_fmt_str = "\e[5m%s\e[0m";
  }
  else if (XY_Str_Cross==style)
  {
    color_fmt_str = "\e[9m%s\e[0m";
  }

  // -2 把中间%s减掉
  size_t len = strlen(color_fmt_str) -2;
  char* buf = malloc(strlen(str) + len + 1);
  sprintf (buf, color_fmt_str, str);
  return buf;
}


static bool
xy_streql(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0 ? true : false;
}


static char*
xy_str_to_quietcmd (const char* cmd)
{
  char* ret = NULL;
#ifdef _WIN32
  ret = xy_2strjoin (cmd, " >nul 2>nul ");
#else
  ret = xy_2strjoin (cmd, " 1>/dev/null 2>&1 ");
#endif
  return ret;
}


static bool
xy_str_end_with (const char* str, const char* suffix)
{
  size_t len1 = strlen(str);
  size_t len2 = strlen(suffix);

  if (0==len2)     return true;  // 空字符串直接返回
  if (len1 < len2) return false;

  const char* cur1 = str + len1 - 1;
  const char* cur2 = suffix + len2 - 1;

  for (int i=0; i<len2; i++)
  {
    if (*cur1 != *cur2) return false;
    cur1--; cur2--;
  }
  return true;
}

static bool
xy_str_start_with (const char* str, const char* prefix)
{
  size_t len1 = strlen(str);
  size_t len2 = strlen(prefix);

  if (0==len2)     return true;  // 空字符串直接返回
  if (len1 < len2) return false;

  const char* cur1 = str;
  const char* cur2 = prefix;

  for (int i=0; i<len2; i++)
  {
    if (*cur1 != *cur2) return false;
    cur1++; cur2++;
  }
  return true;
}

static char*
xy_str_delete_prefix (const char* str, const char* prefix)
{
  char* new = xy_strdup(str);
  bool yes = xy_str_start_with(str, prefix);
  if (!yes) return new;

  size_t len = strlen(prefix);
  char* cur = new + len;
  return cur;
}


static char*
xy_str_delete_suffix (const char* str, const char* suffix)
{
  char* new = xy_strdup(str);
  bool yes = xy_str_end_with(str, suffix);
  if (!yes) return new;

  size_t len1 = strlen(str);
  size_t len2 = strlen(suffix);
  char* cur = new + len1 - len2;
  *cur = '\0';
  return new;
}


static char*
xy_str_strip (const char* str)
{
  const char* lf   = "\n";
  const char* crlf = "\r\n";

  char* new = xy_strdup(str);

  while (xy_str_start_with(new, lf)) {
    new = xy_str_delete_prefix(new, lf);
  }
  while (xy_str_start_with(new, crlf)) {
    new = xy_str_delete_prefix(new, crlf);
  }
  while (xy_str_end_with(new, lf)) {
    new = xy_str_delete_suffix(new, lf);
  }
  while (xy_str_end_with(new, crlf)) {
    new = xy_str_delete_suffix(new, crlf);
  }
  return new;
}


/**
 * 执行cmd后拿到cmd的执行结果 注意从外部free掉这段内存
 * 注意：执行结果后面有回车换行
 */
static char *
xy_getcmd(const char * cmd, bool (*func)(const char*))
{
  const int BUFSIZE = 1024;

  FILE *stream;
  char* buf = (char*)malloc(sizeof(char)*BUFSIZE);

  // 执行命令，并将输出保存到 stream 指针指向的文件中。
  stream = popen(cmd, "r");
  if (stream == NULL) {
    printf("命令执行失败。\n");
    return NULL;
  }

  // 从 stream 指针指向的文件中读取数据。
  char *ret;
  do {
    if(fgets(buf, sizeof(buf), stream)==NULL)
    {
      break;
    }
    if(func==NULL)
    {
      ret = buf;
    }
    else
    {
      if(func(buf))
      {
        ret = buf;
        break;
      }
    }
  }while(1);

  // 关闭 stream 指针。
  pclose(stream);
  return ret;
}


#define xy_os_home _xy_os_home()
static char*
_xy_os_home ()
{
  char* home = NULL;
  if (xy_on_windows)
    home = getenv("USERPROFILE");
  else
    home = getenv("HOME");
  return home;
}


#define xy_win_powershell_profile _xy_win_powershell_profile()
#define xy_win_powershellv5_profile _xy_win_powershellv5_profile()
static char*
_xy_win_powershell_profile ()
{
  return xy_2strjoin(xy_os_home, "\\Documents\\PowerShell\\Microsoft.PowerShell_profile.ps1");
}

char*
_xy_win_powershellv5_profile()
{
  return xy_2strjoin(xy_os_home, "\\Documents\\WindowsPowerShell\\Microsoft.PowerShell_profile.ps1");
}


/**
 * @note Windows上，`path` 不要夹带变量名，因为最终 access() 不会帮你转换
 */
static bool
xy_file_exist(char* path)
{
  char* newpath = path;
  if (xy_on_windows)
  {
    if (xy_str_start_with(path, "~")) {
      newpath = xy_2strjoin(xy_os_home, path+1);
    }
  }
  return access(newpath, 0) ? false : true;
}

#endif
