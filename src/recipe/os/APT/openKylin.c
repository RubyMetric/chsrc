/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * openKylin直接基于Linux内核开发，属于和Debian、openSUSE、Fedora、Arch
 * 同一级别的、根社区发布的系统
 * ------------------------------------------------------------*/

def_target(os_openkylin, "openkylin");

void
os_openkylin_prelude ()
{
  chef_prep_this (os_openkylin, gsr);

  chef_set_created_on   (this, "2023-09-06");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-29");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_sauciers (this, 1, "@ccmywish");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "https://archive.openkylin.top/openkylin/", DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/openkylin/",    DelegateToMirror},
  {&Netease,          "https://mirrors.163.com/openkylin/",       DelegateToMirror}
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

  chsrc_use_this_source (os_openkylin);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strcat (3, "sed -E -i 's@https?://.*/openkylin/?@", source.url, "@g'" OS_Apt_SourceList);
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
