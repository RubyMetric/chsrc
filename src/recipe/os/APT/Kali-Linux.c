/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_kali, "kali");

void
os_kali_prelude ()
{
  chef_prep_this (os_kali, gsr);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_sauciers (this, 2, "@Yangmoooo", "@happy-game");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_deny_english(this);
  chef_deny_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://http.kali.org/kali",          DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/kali", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/kali",    DelegateToMirror},
  {&Volcengine,       "https://mirrors.volces.com/kali",    DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/kali",   DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/kali",   DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/kali", DelegateToMirror}
  /* 不启用原因: 未与上游同步 */
  // {&Huawei,     "https://mirrors.huaweicloud.com/kali",   DelegateToMirror},
  def_sources_end()
}

void
os_kali_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

void
os_kali_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_use_this_source (os_kali);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strcat (3, "sed -E -i \'s@https?://.*/kali/?@",
                              source.url,
                             "@g\' " OS_Apt_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_kali_resetsrc (char *option)
{
  os_kali_setsrc (option);
}
