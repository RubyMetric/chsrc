/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_openeuler);

void
os_openeuler_prelude ()
{
  use_this(os_openeuler);

  chef_set_created_on   (this, "2023-09-06");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_authors (this, 1, "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 3,
    "Aoran Zeng", "ccmywish@qq.com",
    "Yangmoooo", "yangmoooo@outlook.com",
    "happy game", "happygame1024@gmail.com");

  chef_allow_get();
  chef_allow_set();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_upstream("https://repo.openeuler.org/");
  def_sources_begin()
  {&upstream,  "https://repo.openeuler.org/",         DelegateToUpstream},
  {&Ali,       "https://mirrors.aliyun.com/openeuler/",     DelegateToMirror},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/openeuler/",    DelegateToMirror},
  {&Ustc,      "https://mirrors.ustc.edu.cn/openeuler/",    DelegateToMirror},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/openeuler/", DelegateToMirror},
  {&Tencent,   "https://mirrors.tencent.com/openeuler/",    DelegateToMirror}
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/openeuler/", DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/openeuler/",  DelegateToMirror}
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/openeuler/", DelegateToMirror}
  def_sources_end()

  chsrc_set_provider_speed_measure_url (&upstream, "https://repo.openeuler.org/openEuler-24.03-LTS/ISO/x86_64/openEuler-24.03-LTS-netinst-x86_64-dvd.iso");
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

  use_this(os_openeuler);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  chsrc_backup (OS_openEuler_SourceList);

  // 替换 baseurl=<<URL>>/openEuler-xx.xx/...
  // openEuler-xx.xx 为 openEuler 版本号
  // sed -E 's!^baseurl=.*?/openEuler-([^/]+)!baseurl=$(source.url)/openEuler-\1/!g' OS_openEuler_SourceList
  char* cmd = xy_strjoin (6, "sed ",
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


/**
 * chsrc ls openeuler
 */
chef_allow_get();
chef_allow_reset();
def_target(os_openeuler);
