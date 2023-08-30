/* --------------------------------------------------------------
* File          : helper.h
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-28>
* Last modified : <2023-08-30>
*
* helper:
*
*   helper functions and macros
* -------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
  #include <windows.h>
  #define xy_useutf8() SetConsoleOutputCP(65001)
#else
  #define xy_useutf8()
#endif

#define Array_Size(x) (sizeof(x) / sizeof(x[0]))


static inline void*
xy_malloc0 (size_t size)
{
  void* ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}


static char*
xy_strjoin (const char* str1, const char* str2)
{
  size_t len  = strlen(str1);
  size_t size = len + strlen(str2) + 1;
  char* ret  = malloc(size);
  strcpy(ret, str1);
  strcpy(ret+len, str2);
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
