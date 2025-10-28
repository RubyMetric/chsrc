/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_anolis, "anolis/openanolis");

void
os_anolis_prelude ()
{
  chef_prep_this (os_anolis, s);

  chef_set_created_on   (this, "2023-09-24");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-06-12");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_deny_english(this);
  chef_deny_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "https://mirrors.openanolis.cn/anolis", DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/anolis",  DelegateToMirror},
  {&Hust,             "https://mirrors.hust.edu.cn/anolis", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult: https://mirrors.hust.edu.cn/docs/anolis
 */
void
os_anolis_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_use_this_source (os_anolis);

  char *cmd = xy_strcat (3, "sed -i.bak -E 's|https?://(mirrors\\.openanolis\\.cn/anolis)|", source.url, "|g' /etc/yum.repos.d/*.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("dnf makecache", RunOpt_Default);
  chsrc_run ("dnf update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
