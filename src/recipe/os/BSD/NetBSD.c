/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-05>
 * Last Modified : <2025-07-31>
 * ------------------------------------------------------------*/

/**
 * @update 2025-07-31
 */
static Source_t os_netbsd_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
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
};
def_sources_n(os_netbsd);


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

  chsrc_yield_source_and_confirm (os_netbsd);

  chsrc_backup ("/usr/pkg/etc/pkgin/repositories.conf");

  char *arch = chsrc_get_cpuarch ();
  char *vercmd  = "cat /etc/os-release | grep \"VERSION=\" | grep -Po \"[8-9].[0-9]+\"";
  char *version = xy_run (vercmd, 0);

  char *url = xy_strjoin (5, chef_ensure_trailing_slash (source.url), arch, "/", version, "/All");
  chsrc_overwrite_file (url, "/usr/pkg/etc/pkgin/repositories.conf");

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


Feature_t
os_netbsd_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = false;

  f.note = NULL;
  return f;
}

def_target_gsf(os_netbsd);
