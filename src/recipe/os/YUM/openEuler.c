/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_openeuler, "openeuler");

void
os_openeuler_prelude ()
{
  chef_prep_this (os_openeuler, gs);

  chef_set_created_on   (this, "2023-09-06");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_chef (this, NULL);
  chef_set_sauciers (this, 3, "@ccmywish", "@Yangmoooo", "@happy-game");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "https://repo.openeuler.org/",               FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/openeuler/",     FeedByPrelude},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/openeuler/",    FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/openeuler/",    FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/openeuler/", FeedByPrelude},
  {&Tencent,          "https://mirrors.tencent.com/openeuler/",    FeedByPrelude}
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/openeuler/", FeedByPrelude},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/openeuler/",  FeedByPrelude}
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/openeuler/", FeedByPrelude}
  def_sources_end()

  chef_set_sources_speed_measure_url_with_postfix (this, "https://repo.openeuler.org/openEuler-24.03-LTS/ISO/x86_64/openEuler-24.03-LTS-netinst-x86_64-dvd.iso");
}


/**
 * chsrc get openeuler
 */
void
os_openeuler_getsrc (char *option)
{
  chsrc_view_file (OS_openEuler_SourceList);
}


/**
 * chsrc set openeuler
 */
void
os_openeuler_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_use_this_source (os_openeuler);

  chsrc_backup (OS_openEuler_SourceList);

  // 替换 baseurl=<<URL>>/openEuler-xx.xx/...
  // openEuler-xx.xx 为 openEuler 版本号
  // sed -E 's!^baseurl=.*?/openEuler-([^/]+)!baseurl=$(source.url)/openEuler-\1/!g' OS_openEuler_SourceList
  char* cmd = xy_strcat (6, "sed ",
         "-i -E 's!^baseurl=.*?/openEuler-([^/]+)!baseurl=",
         source.url,
         "openEuler-\\1",
         "!g' ",
         OS_openEuler_SourceList);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * chsrc reset openeuler
 */
void
os_openeuler_resetsrc (char *option)
{
  os_openeuler_setsrc (option);
}
