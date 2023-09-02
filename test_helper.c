/* --------------------------------------------------------------
* File          : test_helper.c
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-30>
* Last modified : <2023-09-02>
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
  puts(xy_2strjoin("Xi",    "'an"));
  puts(xy_strjoin (2, "Xi", "'an"));
  puts(xy_strjoin(3, "屈身守分，",   "以待天时，",  "不可与命争也"));
  puts(xy_strjoin(4, "水落鱼梁浅，", "天寒梦泽深。", "羊公碑字在，", "读罢泪沾襟。"));
  puts(xy_strjoin(6, "楚山横地出，", "汉水接天回。", "冠盖非新里，", "章华即旧台。", "习池风景异，", "归路满尘埃。"));

  xy_success("成功：输出成功内容");
  xy_info("信息: 输出信息内容");
  xy_warn("警告：输出警告内容");
  xy_error("错误：输出错误内容");
  return 0;
}
