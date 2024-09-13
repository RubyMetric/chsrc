/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 *               | Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-03>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

/**
 * @update 2024-09-14
 */
static SourceInfo
os_openbsd_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/OpenBSD/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/OpenBSD/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/OpenBSD/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/OpenBSD/"},
  {&Tencent,        "https://mirrors.tencent.com/OpenBSD/"},
  // {&Tencent_Intra,  "https://mirrors.tencentyun.com/OpenBSD/"},
  {&Netease,        "https://mirrors.163.com/OpenBSD/"},
  {&Sohu,           "https://mirrors.sohu.com/OpenBSD/"}
};
def_sources_n(os_openbsd);


void
os_openbsd_getsrc (char *option)
{
  chsrc_view_file ("/etc/installurl");
}

/**
 * 参考：
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/openbsd/
 * 2. https://book.bsdcn.org/di-26-zhang-openbsd/di-26.2-jie-pei-zhi.html
 */
void
os_openbsd_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_openbsd);

  chsrc_backup ("/etc/installurl");
  chsrc_overwrite_file (source.url, "/etc/installurl");

  chsrc_conclude (&source, ChsrcTypeUntested);
}

def_target(os_openbsd);
