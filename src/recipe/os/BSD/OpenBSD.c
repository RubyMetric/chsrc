/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_openbsd, "openbsd");

void
os_openbsd_prelude ()
{
  use_this(os_openbsd);
  chef_allow_gs(os_openbsd);

  chef_set_created_on   (this, "2023-09-03");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-31");

  chef_set_authors (this, 2, "Heng Guo", "2085471348@qq.com", "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&upstream,         NULL,                                     DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/OpenBSD/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/OpenBSD/",   DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/OpenBSD/", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/OpenBSD/",    DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/OpenBSD/",  DelegateToMirror},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/OpenBSD/",   DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/OpenBSD/",   DelegateToMirror},
  {&Cqupt,            "https://mirrors.cqupt.edu.cn/openbsd/", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/OpenBSD/",   DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/OpenBSD/",  DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/OpenBSD/", DelegateToMirror},
  {&Netease,          "https://mirrors.163.com/OpenBSD/",      DelegateToMirror},
  {&Sohu,             "https://mirrors.sohu.com/OpenBSD/",     DelegateToMirror}
  def_sources_end()
}


void
os_openbsd_getsrc (char *option)
{
  chsrc_view_file ("/etc/installurl");
}


/**
 * @consult
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/openbsd/
 * 2. https://book.bsdcn.org/di-26-zhang-openbsd/di-26.2-jie-pei-zhi.html
 */
void
os_openbsd_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_openbsd);

  chsrc_backup ("/etc/installurl");
  chsrc_overwrite_file (source.url, "/etc/installurl");

  chsrc_conclude (&source);
}
