/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 *               |
 * Created On    : <2024-12-14>
 * Last Modified : <2024-12-14>
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

  br();
  chsrc_log  ("chsrc_log");
  chsrc_succ ("chsrc_succ");
  chsrc_info ("chsrc_info");
  chsrc_warn ("chsrc_warn");
  chsrc_error ("chsrc_error");
  chsrc_debug ("chsrc_debug");
  chsrc_verbose ("chsrc_verbose");

  chsrc_succ2 ("chsrc_succ2");
  chsrc_log2  ("chsrc_log2");
  chsrc_note2 ("chsrc_note2");
  chsrc_warn2 ("chsrc_warn2");
  chsrc_error2 ("chsrc_error2");
  chsrc_debug2 ("chsrc_debug2");
  chsrc_verbose2 ("chsrc_verbose2");


  assert (chsrc_check_program ("./fw"));
  assert (chsrc_check_program_quietly ("./fw"));
  assert (chsrc_check_program_quietly_when_exist ("./fw"));
  chsrc_ensure_program ("./fw");

  #define bkup "README.md.bak"

  chsrc_backup ("README.md");
  assert (chsrc_check_file (bkup));

  chsrc_ensure_dir ("test");

  chsrc_append_to_file  ("append",  bkup);
  chsrc_prepend_to_file ("prepend", bkup);
  chsrc_overwrite_file  ("overwrite", bkup);
  chsrc_view_file (bkup);

  chsrc_log (xy_2strjoin ("CPU arch = ", chsrc_get_cpuarch ()));
  printf ("chsrc: CPU cores = ");
  puti (chsrc_get_cpucore ());

  chsrc_run ("rm " bkup, RunOpt_No_Last_New_Line);
  chsrc_run ("echo " Chsrc_Version " test pass!", RunOpt_Dont_Notify_On_Success);
  return 0;
}
