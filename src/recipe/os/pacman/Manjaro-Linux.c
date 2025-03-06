/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Heng Guo <2085471348@qq.com>
 * Contributors  : Nil Null <nil@null.org>
 * Created On    : <2023-09-06>
 * Last Modified : <2024-08-16>
 *
 * Manjaro Linux（或简称Manjaro）基于Arch Linux
 * ------------------------------------------------------------*/

/**
 * 似乎会弹出GUI，待确定
 */
void
os_manjaro_setsrc (char *option)
{
  chsrc_ensure_root ();
  char *cmd = "pacman-mirrors -i -c China -m rank";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (NULL);
}

Target_t os_manjaro_target = {NULL, os_manjaro_setsrc, NULL, NULL, 0};
