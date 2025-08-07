/** ------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * -------------------------------------------------------------
 * File Name     : xy.c
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 *               |
 * Created On    : <2023-08-30>
 * Last Modified : <2025-08-08>
 *
 * Test xy.h
 * ------------------------------------------------------------*/

#include "xy.h"

int
main (int argc, char const *argv[])
{
  xy_useutf8 ();

  println (xy_os_depend_str ("Hello, Windows!", "Hello, Unix!"));

  println (3);
  double dbl = 3.1415;
  println (dbl);
  say (xy_2strjoin ("Xi", "'an"));
  say (xy_2pathjoin("C:\\Users\\", "\\Documents\\"));
  say (xy_2pathjoin("/home/user/", "/documents/"));
  say (xy_2pathjoin("C:\\\\Program Files\\\\", "\\\\Test\\\\"));
  say (xy_pathjoin(3, "X:\\a b", "c", "d"));
  say (xy_pathjoin(4, "E:\\", "\\a", "b\\c\\", "/d"));
  say (xy_pathjoin(2, "./a", "b"));
  say (xy_strjoin(2, "Xi", "'an"));
  say (xy_strjoin  (3, "屈身守分，", "以待天时，", "不可与命争也"));
  say (xy_strjoin  (4, "水落鱼梁浅，", "天寒梦泽深。", "羊公碑字在，", "读罢泪沾襟。"));
  say (xy_strjoin  (6, "楚山横地出，", "汉水接天回。", "冠盖非新里，", "章华即旧台。", "习池风景异，", "归路满尘埃。"));

  print (xy_str_to_bold      ("粗体"));
  print (xy_str_to_faint     ("浅体"));
  print (xy_str_to_italic    ("斜体"));
  print (xy_str_to_underline ("下划线"));
  print (xy_str_to_blink     ("闪烁"));
  print (xy_str_to_cross     ("删除线"));

  print (xy_str_to_red     ("红色"));
  print (xy_str_to_green   ("绿色"));
  print (xy_str_to_yellow  ("黄色"));
  print (xy_str_to_blue    ("蓝色"));
  print (xy_str_to_magenta ("紫色"));
  print (xy_str_to_purple  ("紫色"));
  print (xy_str_to_cyan    ("青色"));
  br();

  xy_log   ("普通", "输出普通内容");
  xy_succ  ("成功", "输出成功内容");
  xy_info  ("信息", "输出信息内容");
  xy_warn  ("警告", "输出警告内容");
  xy_error ("错误", "输出错误内容");

  xy_log_brkt   ("xy.h", "普通", "咸阳油泼面筋道十足辣子香");
  xy_succ_brkt  ("xy.h", "成功", "西安花干鸡蛋肉夹馍已出炉");
  xy_info_brkt  ("xy.h", "信息", "襄阳牛肉面搭配黄酒更美味");
  xy_warn_brkt  ("xy.h", "警告", "兰州牛肉面，而非兰州拉面");
  xy_error_brkt ("xy.h", "错误", "西安肉丸胡辣汤里没有肉丸");

  assert (xy_streql    ("abc", "abc"));
  assert (xy_streql_ic ("AbC", "abc"));
  assert (false == xy_streql ("abc", "abC"));
  assert (true  == xy_streql_ic ("abc", "abC"));

  assert (false == xy_str_end_with ("abcdef", "abcdefg"));
  assert (xy_str_end_with ("abcdef", "def"));

  assert (xy_str_end_with ("abcdef", "bcdef"));
  assert (xy_str_end_with ("abcdef", "abcdef"));
  assert (xy_str_end_with ("abcdef", ""));

  assert (false == xy_str_start_with ("abcdef", "abcdefg"));
  assert (xy_str_start_with ("abcdef", "abc"));
  assert (xy_str_start_with ("abcdef", "abcde"));
  assert (xy_str_start_with ("abcdef", "abcdef"));
  assert (xy_str_start_with ("abcdef", ""));

  assert_str ("abcdefg", xy_str_delete_suffix ("abcdefg", "cdef"));
  assert_str ("abcdefg", xy_str_delete_suffix ("abcdefg", "cdefgh"));
  assert_str ("abcdefg", xy_str_delete_suffix ("abcdefg", ""));
  assert_str ("abcd",    xy_str_delete_suffix ("abcdefg", "efg"));

  assert_str ("abcdefg", xy_str_delete_prefix ("abcdefg", "cdef"));
  assert_str ("abcdefg", xy_str_delete_prefix ("abcdefg", "0abcde"));
  assert_str ("abcdefg", xy_str_delete_prefix ("abcdefg", ""));
  assert_str ("defg",    xy_str_delete_prefix ("abcdefg", "abc"));

  assert_str ("defdef",   xy_str_gsub ("abcdefabcdef", "abc", ""));    // 删除
  assert_str ("6def6def", xy_str_gsub ("abcdefabcdef", "abc", "6")); // 缩小
  assert_str ("XIANGdefXIANGdef",
              xy_str_gsub ("abcdefabcdef", "abc", "XIANG")); // 扩张
  assert_str ("DEFdefDEFdef",
              xy_str_gsub ("abcdefabcdef", "abc", "DEF")); // 等量


  assert (xy_file_exist ("./doc/image/chsrc.png"));
  assert (xy_dir_exist ("~"));
  if (xy_on_windows)
    {
      assert (xy_file_exist (xy_win_powershell_profile));
      assert (true == xy_file_exist (xy_win_powershellv5_profile));
      assert (xy_dir_exist ("C:\\Users"));
      assert (xy_dir_exist ("C:\\Program Files\\"));
    }
  else
    {
      /**
       * debuild 过程会创建虚拟的 HOME 环境，导致检查 .bashrc 的测试会失败，所以我们先检查一下 .profile
       * 如果没有，则大概率也没有 .bashrc
       */
      if (xy_file_exist ("~/.profile"))
        {
          assert (xy_file_exist (xy_bashrc));
        }
      assert (xy_dir_exist ("/etc"));
    }

  println (xy_normalize_path (" \n ~/haha/test/123 \n\r "));
  assert_str (xy_normalize_path ("~/haha/test"), xy_parent_dir (" ~/haha/test/123"));

  xy_succ ("测试完成", "xy.h 测试全部通过");

  // xy_unimplemented();
  // xy_unsupported();
  // xy_unreached();
  return 0;
}
