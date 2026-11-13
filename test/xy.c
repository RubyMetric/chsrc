/** ------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * -------------------------------------------------------------
 * Test File     : xy.c
 * Test Authors  : 曾奥然       <ccmywish@qq.com>
 *               | Mikachu2333 <linkchou@yandex.com>
 * Contributors  : Nul None    <nul@none.org>
 * Created On    : <2023-08-30>
 * Last Modified : <2026-11-11>
 *
 * Test xy.h
 * ------------------------------------------------------------*/

#include "xy.h"

void
callback_print_str_for_seq (void *str, void *DUMMY)
{
  print ((char *) str);
}

void
callback_print_str_for_map (const char *key, void *value, void *DUMMY)
{
  println (xy_strcat (4, "  ", key, " => ", (char *) value));
}


int
main (int argc, char const *argv[])
{
  xy_init ();

  // 关闭颜色
  // xy.enable_color = false;

  print ("Windows: ");     println(xy.on_windows);
  print ("Cygwin Like: "); println(xy.on_cygwin_like);
  print ("MSYS: ");        println(xy.on_msys);

  println (xy_os_depend_str ("Hello, Windows!", "Hello, Unix!"));

  println (3);
  double dbl = 3.1415;
  println (dbl);
  say (xy_2strcat ("Xi", "'an"));
  say (xy_strcat  (2, "Xi", "'an"));
  say (xy_strcat  (3, "屈身守分，", "以待天时，", "不可与命争也"));
  say (xy_strcat  (4, "水落鱼梁浅，", "天寒梦泽深。", "羊公碑字在，", "读罢泪沾襟。"));
  say (xy_strcat  (6, "楚山横地出，", "汉水接天回。", "冠盖非新里，", "章华即旧台。", "习池风景异，", "归路满尘埃。"));

  print (xy_str2bold      ("粗体"));
  print (xy_str2faint     ("浅体"));
  print (xy_str2italic    ("斜体"));
  print (xy_str2underline ("下划线"));
  print (xy_str2blink     ("闪烁"));
  print (xy_str2cross     ("删除线"));

  print (xy_str2red     ("红色"));
  print (xy_str2green   ("绿色"));
  print (xy_str2yellow  ("黄色"));
  print (xy_str2blue    ("蓝色"));
  print (xy_str2magenta ("紫色"));
  print (xy_str2purple  ("紫色"));
  print (xy_str2cyan    ("青色"));
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
  assert (xy_streql_ic ("abc", "abc"));
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

  assert_str ("0",          xy_int2str (0));
  assert_str ("12345",      xy_int2str (12345));
  assert_str ("-2147483648", xy_int2str (INT_MIN));

  assert (0       == xy_str2int (NULL));
  assert (0       == xy_str2int (""));
  assert (12345   == xy_str2int ("12345"));
  assert (-12345  == xy_str2int ("-12345"));
  assert (INT_MAX == xy_str2int ("2147483647"));
  assert (INT_MIN == xy_str2int ("-2147483648"));
  assert (0       == xy_str2int ("123abc"));

  assert (0.0f == xy_str2float (NULL));
  assert (0.0f == xy_str2float (""));
  assert (3.1399f < xy_str2float ("3.14"));
  assert (xy_str2float ("3.14") < 3.1401f);
  assert (-3.1401f < xy_str2float ("-3.14"));
  assert (xy_str2float ("-3.14") < -3.1399f);
  assert (0.0f == xy_str2float ("3.14abc"));

  assert_str ("defdef",   xy_str_gsub ("abcdefabcdef", "abc", ""));    // 删除
  assert_str ("6def6def", xy_str_gsub ("abcdefabcdef", "abc", "6")); // 缩小
  assert_str ("XIANGdefXIANGdef",
              xy_str_gsub ("abcdefabcdef", "abc", "XIANG")); // 扩张
  assert_str ("DEFdefDEFdef",
              xy_str_gsub ("abcdefabcdef", "abc", "DEF")); // 等量


  assert (xy_file_exist ("./doc/image/chsrc.png"));
  assert (xy_dir_exist ("~"));
  assert (!xy_file_exist (NULL));
  assert (!xy_dir_exist (NULL));
  assert (!xy_path_exist (NULL));
  assert (!xy_stat_is_file (NULL, NULL));
  assert (!xy_stat_is_dir (NULL, NULL));

  enum XyStatError error = XY_Stat_Ok;
  assert (!xy_stat_is_file (NULL, &error) && XY_Stat_Invalid_Path == error);
  assert (xy_stat_is_file ("./doc/image/chsrc.png", &error) && XY_Stat_Ok == error);
  assert (!xy_stat_is_dir (NULL, &error) && XY_Stat_Invalid_Path == error);
  assert (xy_stat_is_dir ("~", &error) && XY_Stat_Ok == error);

  assert (!xy_stat_is_file ("~", &error) && XY_Stat_Not_File == error);
  assert (!xy_stat_is_dir ("./doc/image/chsrc.png", &error) && XY_Stat_Not_Directory == error);
  assert (xy_stat_is_file (" ./doc/image/chsrc.png \t", &error) && XY_Stat_Ok == error);
  assert (!xy_stat_is_file ("~", NULL));
  assert (!xy_stat_is_dir ("./doc/image/chsrc.png", NULL));

  assert (!xy_stat_is_file ("", &error) && XY_Stat_Invalid_Path == error);
  assert (!xy_stat_is_dir ("", &error) && XY_Stat_Invalid_Path == error);
  assert (!xy_stat_is_file (" \t\n", &error) && XY_Stat_Invalid_Path == error);
  assert (!xy_stat_is_dir (" \t\n", &error) && XY_Stat_Invalid_Path == error);
  assert (!xy_stat_is_file ("./doc/image/xy-stat-nonexistent", &error) && XY_Stat_Not_Found == error);
  assert (!xy_stat_is_dir ("./doc/image/xy-stat-nonexistent", &error) && XY_Stat_Not_Found == error);
  assert (!xy_stat_is_file ("./doc/image/chsrc.png/missing", &error));
#ifdef XY_Build_On_Windows
  assert (XY_Stat_Not_Found == error || XY_Stat_Not_Directory == error);
#else
  assert (XY_Stat_Not_Directory == error);
#endif
  enum XyStatError component_error = error;
  assert (!xy_stat_is_dir ("./doc/image/chsrc.png/missing", &error) && component_error == error);

#ifdef XY_Build_On_Windows
  assert (xy_path_exist ("NUL"));
  assert (!xy_stat_is_file ("NUL", &error) && XY_Stat_Not_File == error);
  assert (!xy_stat_is_dir ("NUL", &error) && XY_Stat_Not_Directory == error);
  assert (!xy_stat_is_file ("invalid<path", &error) && XY_Stat_Invalid_Path == error);
  assert (!xy_stat_is_dir ("invalid<path", &error) && XY_Stat_Invalid_Path == error);
  assert (XY_Stat_Access_Denied == _xy_stat_error (ERROR_ACCESS_DENIED));
  assert (XY_Stat_Link_Loop == _xy_stat_error (ERROR_CANT_RESOLVE_FILENAME));
  assert (XY_Stat_Name_Too_Long == _xy_stat_error (ERROR_FILENAME_EXCED_RANGE));
  assert (XY_Stat_IO_Error == _xy_stat_error (ERROR_IO_DEVICE));
  assert (XY_Stat_Busy == _xy_stat_error (ERROR_SHARING_VIOLATION));
  assert (XY_Stat_Resource_Limit == _xy_stat_error (ERROR_TOO_MANY_OPEN_FILES));
#else
  assert (xy_path_exist ("/dev/null"));
  assert (!xy_stat_is_file ("/dev/null", &error) && XY_Stat_Not_File == error);
  assert (!xy_stat_is_dir ("/dev/null", &error) && XY_Stat_Not_Directory == error);
  assert (XY_Stat_Access_Denied == _xy_stat_error (EACCES));
  assert (XY_Stat_Link_Loop == _xy_stat_error (ELOOP));
  assert (XY_Stat_Name_Too_Long == _xy_stat_error (ENAMETOOLONG));
  assert (XY_Stat_IO_Error == _xy_stat_error (EIO));
  assert (XY_Stat_Busy == _xy_stat_error (EBUSY));
  assert (XY_Stat_Resource_Limit == _xy_stat_error (EMFILE));
#endif
  assert (XY_Stat_Query_Error == _xy_stat_error (ULONG_MAX));

  char *home_path = xy_normalize_path ("~");
  assert_str (home_path, xy_os_home);
  free (home_path);

#ifdef XY_Build_On_Windows
  char *appdata = xy_env_get ("APPDATA");
  if (appdata)
    {
      char *normal_appdata = xy_normalize_path (appdata);
      char *expanded_appdata = xy_normalize_path ("%APPDATA%");
      assert_str (expanded_appdata, normal_appdata);
      assert (xy_dir_exist ("%APPDATA%") == xy_dir_exist (appdata));
      free (expanded_appdata);
      free (normal_appdata);
      free (appdata);
    }
#endif
  if (xy.on_windows)
    {
      say (xy_normalize_path ("~"));
      say (xy_normalize_path ("~/"));
      // xy_parent_dir() 得到的结果一定是不包含尾斜杠的目录名
      assert_str (xy_path_display (xy_parent_dir (xy_normalize_path ("~"))), "C:\\Users");
      assert_str (xy_path_display (xy_parent_dir (xy_normalize_path ("~/"))), "C:\\Users");
      assert (xy_dir_exist ("C:\\Users"));

      say (xy_win_powershell_profile);
      say (xy_win_powershellv5_profile);
      assert (xy_file_exist (xy_win_powershell_profile));
      assert (true == xy_file_exist (xy_win_powershellv5_profile));

      assert_str (xy_path_display (xy_normalize_path ("C:\\a bc\\def\\")), "C:\\a bc\\def\\");
      assert_str (xy_normalize_path ("a/b c/d"), "a\\b c\\d");
      assert_str (xy_normalize_path ("a/b c/d/"), "a\\b c\\d\\");
      assert_str (xy_parent_dir ("a/b c/d"), "a\\b c");
      assert_str (xy_parent_dir ("a/b c\\d/"), "a\\b c");
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
  assert_str (".", xy_parent_dir ("abc"));

  if (xy.on_windows)
    {
      assert_str ("C:\\Users\\chsrc", xy_path_display (xy_path_join ("C:\\Users",   "chsrc")));
    }
  else
    {
      assert_str ("/usr/local/bin", xy_path_join ("/usr/local",  "bin"));
      assert_str ("/usr/local/lib", xy_path_join ("/usr/local/", "lib"));
    }



  XySeq_t *seq = xy_seq_new ();
  xy_seq_push (seq, "Hello");
  xy_seq_push (seq, "World");
  assert_str ("Hello", xy_seq_at (seq, 0));
  assert_str ("World", xy_seq_at (seq, 1));
  // assert_str ("World", xy_seq_at (seq, 2)); // out-of-rage throw
  xy_seq_each (seq, callback_print_str_for_seq, NULL); br();
  xy_seq_pop (seq);
  assert (1 == xy_seq_len (seq));


  XyMap_t *map = xy_map_new ();
  xy_map_set (map, "Hello", "World");
  xy_map_set (map, "你好",  "世界");
  assert_str ("World", xy_map_get (map, "Hello"));
  assert_str ("世界", xy_map_get (map, "你好"));
  xy_map_set (map, "Hello", "chsrc");
  assert_str ("chsrc", xy_map_get (map, "Hello"));
  assert (2 == xy_map_len (map));
  echo ("{");
    xy_map_each (map, callback_print_str_for_map, NULL);
  echo ("}");

  xy_succ ("测试完成", "xy.h 测试全部通过");

  // xy_unimplemented();
  // xy_unsupported();
  // xy_unreached();
  return 0;
}
