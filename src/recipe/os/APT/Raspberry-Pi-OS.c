/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Raspberry Pi OS 树莓派操作系统，以前称为 Raspbian
 * ------------------------------------------------------------*/

def_target(os_raspberrypi);

void
os_raspberrypi_prelude ()
{
  use_this(os_raspberrypi);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-29");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Yangmoooo", "yangmoooo@outlook.com");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  // https://archive.raspberrypi.org/ until Debian "bullseye" release
  {&upstream,       "https://archive.raspberrypi.com/",              DelegateToUpstream},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/raspberrypi/",    DelegateToMirror},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/raspberrypi/", DelegateToMirror},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/raspberrypi/",      DelegateToMirror},
  {&Ustc,           "https://mirrors.ustc.edu.cn/raspberrypi/",      DelegateToMirror},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/raspberrypi/", DelegateToMirror},
  {&Sustech,        "https://mirrors.sustech.edu.cn/raspberrypi/",   DelegateToMirror}
  def_sources_end()
}


void
os_raspberrypi_getsrc (char *option)
{
  chsrc_view_file (OS_RaspberryPi_SourceList);
}


void
os_raspberrypi_setsrc (char *option)
{
  chsrc_ensure_root(); // HELP: 不确定是否需要

  chsrc_yield_source_and_confirm (os_raspberrypi);

  chsrc_backup (OS_RaspberryPi_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url,
                            "@g' " OS_RaspberryPi_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_raspberrypi_resetsrc (char *option)
{
  os_raspberrypi_setsrc (option);
}
