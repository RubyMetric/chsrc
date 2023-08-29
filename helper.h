/* --------------------------------------------------------------
* File          : helper.h
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-28>
* Last modified : <2023-08-29>
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

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

char*
xy_strjoin (const char* str1, const char* str2)
{
  size_t len = strlen(str1) + strlen(str2) + 1;
  char* ret  = malloc(len);
  strcat(ret, str1);
  strcat(ret, str2);
  return ret;
}
