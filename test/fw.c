/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : 曾奥然 <ccmywish@qq.com>
 * Contributors  : Mikachu2333 <mikachu.23333@zohomail.com>
 * Created On    : <2024-12-14>
 * Last Modified : <2025-10-06>
 *
 * 该文件最好启用 DEBUG mode 编译
 * ------------------------------------------------------------*/

#define Chsrc_Version "Frameworker"

#include "../src/framework/core.c"

int
main (int argc, char const *argv[])
{
  if (argc == 2 && xy_streql (argv[1], "--version"))
    {
      say (Chsrc_Version);
      return 0;
    }

  xy_init ();

  br();
  chsrc_log  ("chsrc_log");
  chsrc_succ ("chsrc_succ");
  chsrc_info ("chsrc_info");
  chsrc_warn ("chsrc_warn");
  chsrc_error ("chsrc_error");
  chsrc_debug ("fw", "chsrc_debug");
  chsrc_verbose ("chsrc_verbose");

  chsrc_succ2 ("chsrc_succ2");
  chsrc_log2  ("chsrc_log2");
  chsrc_note2 ("chsrc_note2");
  chsrc_alert2 ("chsrc_alert2");
  chsrc_warn2 ("chsrc_warn2");
  chsrc_error2 ("chsrc_error2");
  chsrc_debug2 ("fw", "chsrc_debug2");
  chsrc_verbose2 ("chsrc_verbose2");

  // 现在 chsrc_check_program() 无法检测本目录文件了
  assert (chsrc_check_program ("curl"));
  assert (chsrc_check_program_quietly ("curl"));
  assert (chsrc_check_program_quietly_when_exist ("curl"));
  chsrc_ensure_program ("curl");

  chsrc_ensure_dir ("test");

  char *tmpfile_content = "Line2\n\nLine3\n";
  char *tmpfile_name = "test";
  char *tmpfile_ext = ".txt";
  char *tmpfile = "";

  FILE *tmp = chsrc_make_tmpfile (tmpfile_name, tmpfile_ext, true, &tmpfile);
  fwrite (tmpfile_content, sizeof (char), strlen (tmpfile_content), tmp);
  fclose (tmp);

  char *tmpfile_bk = xy_2strcat (tmpfile, ".bak");

  assert (xy_file_exist (tmpfile));
  chsrc_backup (tmpfile);\
  assert (chsrc_check_file (tmpfile_bk));
  remove (tmpfile_bk);

  chsrc_append_to_file ("Line4\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line2\n\nLine3\nLine4\n");

  chsrc_prepend_to_file ("Line1 \n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line1 \nLine2\n\nLine3\nLine4\n");

  chsrc_overwrite_file ("Line999 \nLine998\nLine997\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  chsrc_log (xy_2strcat ("CPU arch = ", chsrc_get_cpuarch ()));
  print ("chsrc: CPU cores = ");
  println (chsrc_get_cpucore ());

  remove (tmpfile);

  // chsrc_run_in_inline_pwsh_shell ("Write-Host \"Hello from inline PowerShell\"");

  if (xy.on_windows)
    {
      chsrc_run_as_pwsh_file ("Write-Host \"Hello from PowerShell file\"");
    }
  else
    {
      chsrc_run_as_sh_file ("echo Hello from sh file");
      chsrc_run_as_bash_file ("echo Hello from Bash file");
    }

  chsrc_run ("echo " Chsrc_Version " test pass!", RunOpt_Dont_Notify_On_Success);
  return 0;
}
