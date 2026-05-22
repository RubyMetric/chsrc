/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_kali, "kali");

void
os_kali_prelude ()
{
  chef_prep_this (os_kali, gsr);

  chef_set_recipe_created_on   (this, "2023-09-29");
  chef_set_recipe_last_updated (this, "2026-05-22");
  chef_set_sources_last_updated (this, "2026-05-22");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_sauciers (this, 3, "@Yangmoooo", "@happy-game", "@ccmywish");

  chef_set_os_scope (this);

  chef_deny_english(this);
  chef_deny_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://http.kali.org/kali",          DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/kali", DelegateToMirror},

  /* 不启用原因: (2026-05-22) https://github.com/RubyMetric/chsrc/issues/357 */
  // {&Ali,              "https://mirrors.aliyun.com/kali",    DelegateToMirror},

  {&Volcengine,       "https://mirrors.volces.com/kali",    DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/kali",   DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/kali",   DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/kali", DelegateToMirror}

  /* 不启用原因: 未与上游同步 */
  // {&Huawei,     "https://mirrors.huaweicloud.com/kali",   DelegateToMirror},
  def_sources_end()
}

/**
 * @note Kali Linux 的软件源配置文件路径时至今日 (2026-05-22) 仍然是 /etc/apt/sources.list
 *       而非 DEB822 格式的 /etc/apt/sources.list.d/kali.sources
 */
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
  chsrc_run ("apt-get update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_kali_resetsrc (char *option)
{
  os_kali_setsrc (option);
}
