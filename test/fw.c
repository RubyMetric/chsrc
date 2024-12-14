/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 *               |
 * Created On    : <2024-12-14>
 * Last Modified : <2024-12-14>
 * ------------------------------------------------------------*/

#define Chsrc_Version "sid"

#include "../src/framework/core.c"

int
main (int argc, char const *argv[])
{
  br();
  chsrc_log  ("chsrc_log");
  chsrc_succ ("chsrc_succ");
  chsrc_info ("chsrc_info");
  chsrc_warn ("chsrc_warn");
  chsrc_error ("chsrc_error");
  chsrc_debug ("chsrc_debug");
  chsrc_verbose ("chsrc_verbose");

  chsrc_succ2 ("chsrc_succ2");
  chsrc_log2 ("chsrc_log2");
  chsrc_note2 ("chsrc_note2");
  chsrc_warn2 ("chsrc_warn2");
  chsrc_error2 ("chsrc_error2");
  chsrc_debug2 ("chsrc_debug2");
  chsrc_verbose2 ("chsrc_verbose2");

  chsrc_backup ("README.md");
  chsrc_succ2 ("Test framework successfully!");
  return 0;
}
