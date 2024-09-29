/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-05>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

/**
 * @update 2024-09-14
 */
static SourceInfo
os_netbsd_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/pkgsrc/packages/NetBSD/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Tencent,        "https://mirrors.tencent.com/pkgsrc/packages/NetBSD/"},
  // {&Tencent_Intra,  "https://mirrors.tencentyun.com/pkgsrc/packages/NetBSD/"},
  {&Netease,        "https://mirrors.163.com/pkgsrc/packages/NetBSD/"},
  {&Sohu,           "https://mirrors.sohu.com/pkgsrc/packages/NetBSD/"}
};
def_sources_n(os_netbsd);


void
os_netbsd_getsrc (char *option)
{
  chsrc_view_file ("/usr/pkg/etc/pkgin/repositories.conf");
}

/**
 * 参考:
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/pkgsrc/
 * 2. https://book.bsdcn.org/di-27-zhang-netbsd/di-27.2-jie-huan-yuan-yu-bao-guan-li-qi.html
 *
 * 根据 @ykla (https://github.com/ykla)
 *    NetBSD 默认状态下没有 pkgsrc，用户可能安装了也可能没安装
 *
 * HELP: 未经测试
 */
void
os_netbsd_setsrc (char *option)
{
  chsrc_ensure_root (); // HELP: 不知道是否需要确保root权限

  chsrc_yield_source_and_confirm (os_netbsd);

  chsrc_backup ("/usr/pkg/etc/pkgin/repositories.conf");

  char *arch = chsrc_get_cpuarch ();
  char *vercmd  = "cat /etc/os-release | grep \"VERSION=\" | grep -Po \"[8-9].[0-9]+\"";
  char *version = xy_run (vercmd, 0, NULL);

  char *url = xy_strjoin (5, source.url, arch, "/", version, "/All");
  chsrc_overwrite_file (url, "/usr/pkg/etc/pkgin/repositories.conf");

  chsrc_conclude (&source, SetsrcType_Untested);
}

def_target(os_netbsd);
