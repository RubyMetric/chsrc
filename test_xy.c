/* --------------------------------------------------------------
* File          : test_xy.c
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-30>
* Last modified : <2023-09-04>
*
* test_xy:
*
*   测试 xy.h
* -------------------------------------------------------------*/

#include "xy.h"

int
main (int argc, char const *argv[])
{
  xy_useutf8();
  puti(3);
  double dbl = 3.1415;
  putf(dbl);
  puts(xy_2strjoin("Xi",    "'an"));
  puts(xy_strjoin (2, "Xi", "'an"));
  puts(xy_strjoin(3, "屈身守分，",   "以待天时，",  "不可与命争也"));
  puts(xy_strjoin(4, "水落鱼梁浅，", "天寒梦泽深。", "羊公碑字在，", "读罢泪沾襟。"));
  puts(xy_strjoin(6, "楚山横地出，", "汉水接天回。", "冠盖非新里，", "章华即旧台。", "习池风景异，", "归路满尘埃。"));


  putb(xy_str_end_with("abcdef", "abcdefg"));
  putb(xy_str_end_with("abcdef", "def"));
  putb(xy_str_end_with("abcdef", "bcdef"));
  putb(xy_str_end_with("abcdef", "abcdef"));
  putb(xy_str_end_with("abcdef", ""));

  xy_success("成功：输出成功内容");
  xy_info("信息: 输出信息内容");
  xy_warn("警告：输出警告内容");
  xy_error("错误：输出错误内容");
  return 0;
}
