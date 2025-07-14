/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-12-14>
 * Last Modified : <2025-07-14>
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
  xy_useutf8();

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
  chsrc_warn2 ("chsrc_warn2");
  chsrc_error2 ("chsrc_error2");
  chsrc_debug2 ("fw", "chsrc_debug2");
  chsrc_verbose2 ("chsrc_verbose2");

  // 现在 chsrc_check_program() 无法检测本目录文件了
  assert (chsrc_check_program ("curl"));
  assert (chsrc_check_program_quietly ("curl"));
  assert (chsrc_check_program_quietly_when_exist ("curl"));
  chsrc_ensure_program ("curl");

  #define bkup "README.md.bak"

  chsrc_backup ("README.md");
  assert (chsrc_check_file (bkup));

  chsrc_ensure_dir ("test");

  chsrc_append_to_file  ("append",  bkup);
  if (!xy_on_windows)
    {
      chsrc_prepend_to_file ("prepend", bkup);
    }
  chsrc_overwrite_file  ("overwrite", bkup);
  chsrc_view_file (bkup);

  chsrc_log (xy_2strjoin ("CPU arch = ", chsrc_get_cpuarch ()));
  print ("chsrc: CPU cores = ");
  println (chsrc_get_cpucore ());

  if (xy_on_windows)
    {
      chsrc_run ("del " bkup, RunOpt_No_Last_New_Line);
    }
  else
    {
      chsrc_run ("rm " bkup, RunOpt_No_Last_New_Line);
    }

  char *tmpfile = NULL;
  FILE *tmp = chsrc_make_tmpfile ("tmpfile", ".txt", true, &tmpfile);
  fclose (tmp);
  remove (tmpfile);

  // chsrc_run_in_inline_pwsh_shell ("Write-Host \"Hello from inline PowerShell\"");

  if (xy_on_windows)
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
