/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_linuxmint, "linuxmint");

void
os_linuxmint_prelude ()
{
  use_this(os_linuxmint);
  chef_allow_gs(os_linuxmint);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-11-21");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 1,
    "Happy Game", "happygame1024@gmail.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);


  /* @note 实际上镜像站里的内容和Ubuntu的不太一样 */
  def_sources_begin()
  {&upstream,       "http://packages.linuxmint.com", DelegateToUpstream},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/linuxmint/", DelegateToMirror},
  {&Ali,            "http://mirrors.aliyun.com/linuxmint-packages/",   DelegateToMirror},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/linuxmint/", DelegateToMirror},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/linuxmint/",          DelegateToMirror},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/linuxmint/",            DelegateToMirror},
  {&Jlu,            "https://mirrors.jlu.edu.cn/linuxmint/",            DelegateToMirror},
  {&Ustc,           "https://mirrors.ustc.edu.cn/linuxmint/",           DelegateToMirror},
  {&Bjtu,           "https://mirror.bjtu.edu.cn/linuxmint/",             DelegateToMirror},
  {&Zju,            "https://mirrors.zju.edu.cn/linuxmint/",             DelegateToMirror},
  {&Sustech,        "https://mirrors.sustech.edu.cn/linuxmint/",         DelegateToMirror},
  {&Iscas,          "https://mirror.iscas.ac.cn/linuxmint/",             DelegateToMirror},
  {&Scau,           "https://mirrors.scau.edu.cn/linuxmint/",            DelegateToMirror}
  /* 不启用原因：过慢 */
  // {&Netease,     "https://mirrors.163.com/linuxmint/packages/",       DelegateToMirror}
  def_sources_end()
}


void
os_linuxmint_getsrc (char *option)
{
  chsrc_view_file (OS_LinuxMint_SourceList);
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/linuxmint/
 */
void
os_linuxmint_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_linuxmint);

  chsrc_backup (OS_LinuxMint_SourceList);

  // deb xxx wilma main upstream import backport 为mint主要源, wilma为版本代号
  // 暂不实现自动替换基于debian或ubuntu的基础源
  char *version_codename = xy_run ("sed -nr 's/^VERSION_CODENAME=([^\"]+)/\1/p' " ETC_OS_RELEASE, 0);
  // sed -i '/<version_codename>/ s|http[^ ]*|<source.url>|g' OS_LinuxMint_SourceList
  char* cmd = xy_strjoin (5, "sed -i '/",
                          version_codename, "/ s|http[^ ]*|",
                          source.url, "|g' "  OS_LinuxMint_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_warn2 ("完成后请不要再使用 mintsources（自带的图形化软件源设置工具）进行任何操作，因为在操作后，无论是否有按“确定”，mintsources 均会覆写我们刚才换源的内容");
  chsrc_warn2 ("已自动更换mint主要源, 但mint也使用基于debian或ubuntu的基础源, 可参考对应的debian或ubuntu换源方法进行手动换源");
}
