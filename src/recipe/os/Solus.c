/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-29>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-29 更新
 */
static SourceInfo
os_solus_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/solus/packages/shannon/eopkg-index.xml.xz"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/solus/packages/shannon/eopkg-index.xml.xz"},
  {&Nju,            "https://mirror.nju.edu.cn/solus/packages/shannon/eopkg-index.xml.xz"}
};
def_sources_n(os_solus);


/**
 * 参考: https://help.mirrors.cernet.edu.cn/solus/
 */
void
os_solus_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_solus);

  char *cmd = xy_2strjoin ("eopkg add-repo Solus ", source.url);
  chsrc_run (cmd, RunOpt_Default);
  chsrc_conclude (&source, ChsrcTypeAuto);
}

def_target_s(os_solus);
