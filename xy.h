/* --------------------------------------------------------------
* File          : xy.h
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-28>
* Last modified : <2023-09-05>
*
* xy:
*
*   y = f(x)
* -------------------------------------------------------------*/

#ifndef XY_H
#define XY_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// #define NDEBUG


#ifdef _WIN32

  static bool xy_on_windows = true;
  static bool xy_on_linux   = false;
  static bool xy_on_macos   = false;
  static bool xy_on_bsds    = false;

  static char* xy_os_devnull = "nul";

  #include <windows.h>
  #define xy_useutf8() SetConsoleOutputCP(65001)

#elif  defined(__linux__) || defined(__linux)

  static bool xy_on_windows = false;
  static bool xy_on_linux   = true;
  static bool xy_on_macos   = false;
  static bool xy_on_bsds    = false;

  static char* xy_os_devnull = "/dev/null"

  #define xy_useutf8()

#elif defined(TARGET_OS_MAC) ||defined(__MACOSX__)

  static bool xy_on_windows = false;
  static bool xy_on_linux   = false;
  static bool xy_on_macos   = true;
  static bool xy_on_bsds    = false;

  // set xy_os_devnull

  #define xy_useutf8()

#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)

  static bool xy_on_windows = false;
  static bool xy_on_linux   = false;
  static bool xy_on_macos   = false;
  static bool xy_on_bsds    = true;

  // set xy_os_devnull

  #define xy_useutf8()

#endif

#define putf(n) printf("%f\n", n)
#define puti(n) printf("%d\n", n)
#define putb(n) if(n) puts("true"); else puts("false");

#define xy_arylen(x) (sizeof(x) / sizeof(x[0]))


static inline void*
xy_malloc0 (size_t size)
{
  void* ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}


#define XY_INFO     00001
#define XY_SUCCESS  00001<<1
#define XY_WARN     00001<<2
#define XY_ERROR    00001<<3

static void
xy_log (int level, const char* str)
{
  char* color_fmt_str = NULL;

  bool to_stderr = false;

  if (level & XY_INFO) {
    color_fmt_str = "\033[34m%s\033[0m";   // 蓝色
  }
  else if (level & XY_SUCCESS) {
    color_fmt_str = "\033[32m%s\033[0m";   // 绿色
  }
  else if (level & XY_WARN) {
    color_fmt_str = "\033[33m%s\033[0m\n"; // 黄色
    to_stderr = true;
  }
  else if (level & XY_ERROR) {
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
    fprintf(stderr, buf);
  } else {
    puts(buf);
  }
  free(buf);
}

#define xy_info(str)  xy_log (XY_INFO,  str)
#define xy_error(str) xy_log (XY_ERROR, str)


/**
 * 将str中所有的src字符替换成dest,并返回一个全新的字符串
 * 现在已经废弃不用
 */
static char*
xy_strch (const char* str, char src,const char* dest)
{
  size_t str_len  = strlen(str);
  size_t dest_len  = strlen(dest);
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


static void
xy_success (const char* str1)
{
  char color_fmt_str[] = "\033[32m%s\033[0m";
  // -2 把中间%s减掉，-1 把末尾nul减掉
  size_t len = sizeof(color_fmt_str) -2 -1;
  char* buf = malloc(strlen(str1) + len + 1);

  sprintf (buf, color_fmt_str, str1);
  puts(buf);
  free(buf);
}


static void
xy_warn (const char* str1)
{
  // 注意，我们这里相比于xy_suceess()多了一个换行符
  char color_fmt_str[] = "\033[33m%s\033[0m\n";
  // -2 把中间%s减掉，-1 把末尾nul减掉
  size_t len = sizeof(color_fmt_str) -2 -1;
  char* buf = malloc(strlen(str1) + len + 1);

  sprintf (buf, color_fmt_str, str1);
  fprintf(stderr, buf);
  free(buf);
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


char*
xy_strdup(const char* str)
{
  size_t len = strlen(str);
  char* new = xy_malloc0(len+1);
  strcpy(new, str);
}


bool
xy_streql(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0 ? true : false;
}


char*
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


bool
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

bool
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

char*
xy_str_delete_prefix (const char* str, const char* prefix)
{
  char* new = xy_strdup(str);
  bool yes = xy_str_start_with(str, prefix);
  if (!yes) return new;

  size_t len = strlen(prefix);
  char* cur = new + len;
  return cur;
}


char*
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

#endif
