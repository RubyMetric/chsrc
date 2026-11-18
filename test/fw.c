/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : @ccmywish
 * Contributors  : @Mikachu2333
 * Created On    : <2024-12-14>
 * Last Modified : <2026-10-29>
 *
 * 该文件最好启用 DEBUG mode 编译
 * ------------------------------------------------------------*/

#define Chsrc_Version "chsrc-framework-tester"

#include "../src/framework/base.c"
#include "../src/framework/OS.c"

#ifndef XY_Build_On_Windows
#include <sys/wait.h>
#endif

int
main (int argc, char const *argv[])
{
  if (argc == 2 && xy_streql (argv[1], "--version"))
    {
      say (Chsrc_Version);
      return 0;
    }

  xy_init ();

  if (argc == 2 && xy_streql (argv[1], "--ensure-root"))
    {
      chsrc_ensure_root ();
      return Exit_OK;
    }

  /* 正常执行必须接受管理员/root，拒绝普通用户且不能崩溃。 */
  char *root_cmd = xy_strcat (3, "\"", argv[0], "\" --ensure-root");
  int root_status = xy_system (root_cmd);
#ifndef XY_Build_On_Windows
  assert (WIFEXITED (root_status));
  root_status = WEXITSTATUS (root_status);
#endif
  assert (root_status == (chsrc_is_root () ? Exit_OK : Exit_UserCause));

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
  chsrc_backup (tmpfile);
  assert (chsrc_check_file (tmpfile_bk));
  remove (tmpfile_bk);

  chsrc_append_to_file ("Line4\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line2\n\nLine3\nLine4\n");

  chsrc_prepend_to_file ("Line1 \n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line1 \nLine2\n\nLine3\nLine4\n");

  chsrc_overwrite_file ("Line999 \nLine998\nLine997\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  /* Dry Run 下不得修改或删除文件，也不得创建备份 */
  ProgMode.DryRunMode = true;

  chsrc_ensure_root ();

  chsrc_overwrite_file ("DryRun\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  chsrc_append_to_file ("DryRun\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  chsrc_prepend_to_file ("DryRun\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  remove (tmpfile_bk);
  chsrc_backup (tmpfile);
  assert (!xy_file_exist (tmpfile_bk));

  assert (chsrc_delete_file (tmpfile));
  assert (xy_file_exist (tmpfile));

  char *missing_dir = xy_2strcat (tmpfile, ".missing");
  char *missing_file = xy_path_join (missing_dir, "config");
  chsrc_ensure_dir (missing_dir);
  chsrc_overwrite_file ("DryRun\n", missing_file);
  chsrc_append_to_file ("DryRun\n", missing_file);
  assert (!xy_dir_exist (missing_dir));
  assert (!xy_file_exist (missing_file));

  char *write_cmd = xy_strcat (3, "echo changed > \"", tmpfile, "\"");
  chsrc_run (write_cmd, RunOpt_Default);
  assert (0 == chsrc_run_directly (write_cmd));
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  /* 文件名含有不存在的目录；若试运行仍创建临时脚本，此调用会失败。 */
  chsrc_run_as_script_file (write_cmd, "missing/dry-run-script", ".sh", "sh \"", "Dry Run script");
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  ProgMode.DryRunMode = false;

  char *unicode_file = NULL;
  FILE *unicode_tmp = chsrc_make_tmpfile ("测试删除", ".txt", false, &unicode_file);
  fclose (unicode_tmp);
  assert (xy_file_exist (unicode_file));
  assert (chsrc_delete_file (unicode_file));
  assert (!xy_file_exist (unicode_file));
  assert (!chsrc_delete_file (unicode_file));

  chsrc_log (xy_2strcat ("CPU arch = ", chsrc_get_cpuarch ()));
  print ("chsrc: CPU cores = ");
  println (chsrc_get_cpucore ());

  remove (tmpfile);

  // chsrc_run_in_inline_pwsh_shell ("Write-Host \"Hello from inline PowerShell\"");

  if (xy.on_windows)
    {
      chsrc_view_env ("OS", "windir", NULL);
      chsrc_run_as_pwsh_file ("Write-Host \"Hello from PowerShell (v7) file\"");
      chsrc_run_as_powershellv5_file ("Write-Host \"Hello from PowerShell v5 file\"");
      chsrc_run_as_powershell_file ("Write-Host \"Hello from PowerShell file\"");
    }
  else
    {
      chsrc_view_env ("PWD", NULL);
      chsrc_run_as_sh_file ("echo Hello from sh file");
      chsrc_run_as_bash_file ("echo Hello from Bash file");
    }

  chsrc_run ("echo " Chsrc_Version " test pass!", RunOpt_Dont_Notify_On_Success);
  return 0;
}
