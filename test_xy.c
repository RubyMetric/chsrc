/** ------------------------------------------------------------
 * File          : test_xy.c
 * License       : MIT
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-08-30>
 * Last modified : <2023-09-27>
 *
 * test_xy:
 *
 *   测试 xy.h
 * ------------------------------------------------------------*/

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


  print(xy_str_to_bold("粗体"));
  print(xy_str_to_faint("浅体"));
  print(xy_str_to_italic("斜体"));
  print(xy_str_to_underline("下划线"));
  print(xy_str_to_blink("闪烁"));
  print(xy_str_to_cross("删除线"));

  print(xy_str_to_red("红色"));
  print(xy_str_to_green("绿色"));
  print(xy_str_to_yellow("黄色"));
  print(xy_str_to_blue("蓝色"));
  print(xy_str_to_magenta("紫色"));
  print(xy_str_to_purple("紫色"));
  print(xy_str_to_cyan("青色"));
  puts("");

  assert(false == xy_str_end_with("abcdef", "abcdefg"));
  assert(xy_str_end_with("abcdef", "def"));

  assert(xy_str_end_with("abcdef", "bcdef"));
  assert(xy_str_end_with("abcdef", "abcdef"));
  assert(xy_str_end_with("abcdef", ""));

  assert(false == xy_str_start_with("abcdef", "abcdefg"));
  assert(xy_str_start_with("abcdef", "abc"));
  assert(xy_str_start_with("abcdef", "abcde"));
  assert(xy_str_start_with("abcdef", "abcdef"));
  assert(xy_str_start_with("abcdef", ""));

  assert_str("abcdefg",
    xy_str_delete_suffix("abcdefg", "cdef"));
  assert_str("abcdefg",
    xy_str_delete_suffix("abcdefg", "cdefgh"));
  assert_str("abcdefg",
    xy_str_delete_suffix("abcdefg", ""));
  assert_str("abcd",
    xy_str_delete_suffix("abcdefg", "efg"));

  assert_str("abcdefg",
    xy_str_delete_prefix("abcdefg", "cdef"));
  assert_str("abcdefg",
    xy_str_delete_prefix("abcdefg", "0abcde"));
  assert_str("abcdefg",
    xy_str_delete_prefix("abcdefg", ""));
  assert_str("defg",
    xy_str_delete_prefix("abcdefg", "abc"));

  assert_str("defdef",
    xy_str_gsub("abcdefabcdef", "abc", ""));  // 删除
  assert_str("6def6def",
    xy_str_gsub("abcdefabcdef", "abc", "6")); // 缩小
  assert_str("XIANGdefXIANGdef",
    xy_str_gsub("abcdefabcdef", "abc", "XIANG")); // 扩张
  assert_str("DEFdefDEFdef",
    xy_str_gsub("abcdefabcdef", "abc", "DEF"));   // 等量

  xy_success("成功：输出成功内容");
  xy_info("信息: 输出信息内容");
  xy_warn("警告：输出警告内容");
  xy_error("错误：输出错误内容");

  assert(xy_file_exist("chsrc.png"));
  assert(xy_file_exist(xy_win_powershell_profile));
  assert(false==xy_file_exist(xy_win_powershellv5_profile));
  xy_success("测试全部通过");
  return 0;
}
