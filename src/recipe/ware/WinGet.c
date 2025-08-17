/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(wr_winget, "winget");

void
wr_winget_prelude ()
{
  use_this(wr_winget);
  chef_allow_gsr(wr_winget);

  chef_set_created_on   (this, "2024-06-07");
  chef_set_last_updated (this, "2025-08-17");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 1, "Mikachu2333", "mikachu.23333@zohomail.com");

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
  use_this_source (wr_winget);

  // 2025.8.17 此前用户可能隐式使用默认源导致 remove 失败，故使用 Dont_Abort
  // 我也不知道为啥执行两次就可以了，但是确实是能用了……
  chsrc_run ("winget source remove winget", RunOpt_Dont_Abort_On_Failure);
  chsrc_run ("winget source remove winget", RunOpt_Dont_Abort_On_Failure);
  chsrc_run (xy_strjoin (3, "winget source add winget ", source.url, " --trust-level trusted"), RunOpt_Default);

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
