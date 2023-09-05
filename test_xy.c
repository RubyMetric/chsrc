/* --------------------------------------------------------------
* File          : test_xy.c
* License       : MIT
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-30>
* Last modified : <2023-09-05>
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


  puts(xy_str_to_bold("粗体"));
  puts(xy_str_to_faint("浅体"));
  puts(xy_str_to_italic("斜体"));
  puts(xy_str_to_underline("下划线"));
  puts(xy_str_to_blink("闪烁"));
  puts(xy_str_to_cross("删除线"));

  puts(xy_str_to_red("红色"));
  puts(xy_str_to_green("绿色"));
  puts(xy_str_to_yellow("黄色"));
  puts(xy_str_to_blue("蓝色"));
  puts(xy_str_to_magenta("紫色"));
  puts(xy_str_to_purple("紫色"));
  puts(xy_str_to_cyan("青色"));



  putb(xy_str_end_with("abcdef", "abcdefg")); // false
  putb(xy_str_end_with("abcdef", "def"));     // true
  putb(xy_str_end_with("abcdef", "bcdef"));   // true
  putb(xy_str_end_with("abcdef", "abcdef"));  // true
  putb(xy_str_end_with("abcdef", ""));        // true

  putb(xy_str_start_with("abcdef", "abcdefg")); // false
  putb(xy_str_start_with("abcdef", "abc"));     // true
  putb(xy_str_start_with("abcdef", "abcde"));   // true
  putb(xy_str_start_with("abcdef", "abcdef"));  // true
  putb(xy_str_start_with("abcdef", ""));        // true

  puts(xy_str_delete_suffix("abcdefg", "cdef"));  // 不变
  puts(xy_str_delete_suffix("abcdefg", "cdefgh"));// 不变
  puts(xy_str_delete_suffix("abcdefg", ""));      // 不变
  puts(xy_str_delete_suffix("abcdefg", "efg"));   // abcd


  puts(xy_str_delete_prefix("abcdefg", "cdef"));  // 不变
  puts(xy_str_delete_prefix("abcdefg", "0abcde"));// 不变
  puts(xy_str_delete_prefix("abcdefg", ""));      // 不变
  puts(xy_str_delete_prefix("abcdefg", "abc"));   // defg

  xy_success("成功：输出成功内容");
  xy_info("信息: 输出信息内容");
  xy_warn("警告：输出警告内容");
  xy_error("错误：输出错误内容");
  return 0;
}
