/* --------------------------------------------------------------
* File          : test_helper.c
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-30>
* Last modified : <2023-08-30>
*
* test_helper:
*
*   测试 helper.h 中的辅助函数
* -------------------------------------------------------------*/

#include <stdio.h>
#include "helper.h"

int
main (int argc, char const *argv[])
{
  xy_useutf8();
  puts(xy_strjoin("中文输出",  "test"));
  puts(xy_strjoin("中文输出2",  "test"));

  xy_success("成功输出");
  xy_warn("警告输出");
  return 0;
}
