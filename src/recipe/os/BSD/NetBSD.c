/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_netbsd, "netbsd");

void
os_netbsd_prelude ()
{
  chef_prep_this (os_netbsd, gs);

  chef_set_created_on   (this, "2023-09-05");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-31");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@ccmywish", "@G_I_Y");
  chef_set_sauciers (this, 0);


  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,  NULL,                                                 DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/pkgsrc/packages/NetBSD/", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/pkgsrc/packages/NetBSD/",  DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/pkgsrc/packages/NetBSD/", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/pkgsrc/packages/NetBSD/", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/pkgsrc/packages/NetBSD/",    DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/pkgsrc/packages/NetBSD/",  DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/pkgsrc/packages/NetBSD/", DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/pkgsrc/packages/NetBSD/", DelegateToMirror},
  {&Netease,          "https://mirrors.163.com/pkgsrc/packages/NetBSD/",     DelegateToMirror},
  {&Sohu,             "https://mirrors.sohu.com/pkgsrc/packages/NetBSD/",    DelegateToMirror}
  def_sources_end()
}


void
os_netbsd_getsrc (char *option)
{
  chsrc_view_file ("/usr/pkg/etc/pkgin/repositories.conf");
}


/**
 * @consult
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/pkgsrc/
 * 2. https://book.bsdcn.org/di-27-zhang-netbsd/di-27.2-jie-huan-yuan-yu-bao-guan-li-qi.html
 *
 * 根据 @ykla
 *    NetBSD 默认状态下没有 pkgsrc，用户可能安装了也可能没安装
 */
void
os_netbsd_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_netbsd);

  chsrc_backup ("/usr/pkg/etc/pkgin/repositories.conf");

  char *arch = chsrc_get_cpuarch ();
  char *vercmd  = "cat /etc/os-release | grep \"VERSION=\" | grep -Po \"[8-9].[0-9]+\"";
  char *version = xy_run (vercmd, 0);

  char *url = xy_strcat (5, chef_ensure_trailing_slash (source.url), arch, "/", version, "/All");
  chsrc_overwrite_file (url, "/usr/pkg/etc/pkgin/repositories.conf");

  chsrc_conclude (&source);
}
