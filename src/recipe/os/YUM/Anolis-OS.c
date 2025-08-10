/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_anolis);

void
os_anolis_prelude ()
{
  use_this(os_anolis);
  chef_allow_s(os_anolis);

  chef_set_created_on   (this, "2023-09-24");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-06-12");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&upstream,  "https://mirrors.openanolis.cn/anolis", DelegateToUpstream},
  {&Ali,       "https://mirrors.aliyun.com/anolis",  DelegateToMirror},
  {&Hust,      "https://mirrors.hust.edu.cn/anolis", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult: https://mirrors.hust.edu.cn/docs/anolis
 */
void
os_anolis_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this(os_anolis);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  char *cmd = xy_strjoin (3, "sed -i.bak -E 's|https?://(mirrors\\.openanolis\\.cn/anolis)|", source.url, "|g' /etc/yum.repos.d/*.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("dnf makecache", RunOpt_Default);
  chsrc_run ("dnf update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
