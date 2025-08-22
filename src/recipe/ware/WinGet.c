/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(wr_winget, "winget");

void
wr_winget_prelude ()
{
  chef_prep_this (wr_winget, gsr);

  chef_set_created_on   (this, "2024-06-07");
  chef_set_last_updated (this, "2025-08-17");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Mikachu2333");

  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://cdn.winget.microsoft.com/cache",    NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/winget-source", NULL},
  {&Ustc,             "https://mirrors.ustc.edu.cn/winget-source", NULL},
  {&Nju,              "https://mirror.nju.edu.cn/winget-source",   NULL},
  {&Nyist,            "https://mirror.nyist.edu.cn/winget-source", NULL},
  {&Xjtu,             "https://mirrors.xjtu.edu.cn/winget-source", NULL}
  def_sources_end()
}


void
wr_winget_getsrc (char *option)
{
  chsrc_run ("winget source list", RunOpt_Default);
}


/**
 * @consult https://mirrors.ustc.edu.cn/help/winget-source.html
 */
void
wr_winget_setsrc (char *option)
{
  chsrc_use_this_source (wr_winget);

  // 2025.8.17 此前用户可能隐式使用默认源导致 remove 失败，故使用 Dont_Abort
  chsrc_run ("winget source remove winget", RunOpt_Dont_Abort_On_Failure);
  // 2025.8.18 执行两次相同的命令后继续设置，无报错，换源成功。具体原因不明
  chsrc_run ("winget source remove winget", RunOpt_Dont_Abort_On_Failure);
  chsrc_run (xy_strcat (3, "winget source add winget ", source.url, " --trust-level trusted"), RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
wr_winget_resetsrc (char *option)
{
  chsrc_run ("winget source reset winget", RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
