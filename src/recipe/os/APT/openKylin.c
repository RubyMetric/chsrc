/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * openKylin直接基于Linux内核开发，属于和Debian、openSUSE、Fedora、Arch
 * 同一级别的、根社区发布的系统
 * ------------------------------------------------------------*/

def_target(os_openkylin);

void
os_openkylin_prelude ()
{
  use_this(os_openkylin);

  chef_set_created_on   (this, "2023-09-06");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-29");

  chef_set_authors (this, 1, "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Aoran Zeng", "ccmywish@qq.com");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_upstream("https://archive.openkylin.top/openkylin/");
  def_sources_begin()
  {&upstream,       "https://archive.openkylin.top/openkylin/", DelegateToUpstream},
  {&Ali,            "https://mirrors.aliyun.com/openkylin/",    DelegateToMirror},
  {&Netease,        "https://mirrors.163.com/openkylin/",       DelegateToMirror}
  def_sources_end()
}

void
os_openkylin_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

void
os_openkylin_setsrc (char *option)
{
  chsrc_ensure_root();

  chsrc_yield_source_and_confirm (os_openkylin);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/openkylin/?@", source.url, "@g'" OS_Apt_SourceList);
  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_openkylin_resetsrc (char *option)
{
  os_openkylin_setsrc (option);
}
