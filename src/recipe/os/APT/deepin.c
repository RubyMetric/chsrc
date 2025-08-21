/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_deepin, "deepin");

void
os_deepin_prelude ()
{
  use_this(os_deepin);
  chef_allow_gsr(os_deepin);

  chef_set_created_on   (this, "2023-09-26");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-09-14");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_contributors (this, 1, "@Yangmoooo");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "https://community-packages.deepin.com/deepin", DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/deepin",            DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/deepin",          DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/deepin",          DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/deepin",  DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/deepin",          DelegateToMirror}
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/deepin",     DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Netease,       "https://mirrors.163.com/deepin",             DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,          "https://mirrors.sohu.com/deepin",            DelegateToMirror}
  def_sources_end()
}


void
os_deepin_getsrc(char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

/**
 * HELP: 未经测试
 */
void
os_deepin_setsrc (char *option)
{
  chsrc_ensure_root();

  use_this_source(os_deepin);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strcat (3, "sed -E -i \'s@https?://.*/deepin/?@",
                              source.url,
                              "@g\' " OS_Apt_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_deepin_resetsrc (char *option)
{
  os_deepin_setsrc (option);
}
