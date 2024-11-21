/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 *               | Yangmoooo <yangmoooo@outlook.com>
 * Created On    : <2023-09-06>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

/**
 * @update 2024-09-14
 */
static Source_t
os_openeuler_sources[] = {
  {&UpstreamProvider,       "https://repo.openeuler.org/"},
  {&Ali,            "https://mirrors.aliyun.com/openeuler/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/openeuler/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/openeuler/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/openeuler/"},
  {&Tencent,        "https://mirrors.tencent.com/openeuler/"},
  // {&Tencent_Intra,  "https://mirrors.tencentyun.com/openeuler/"},
  {&Netease,        "https://mirrors.163.com/openeuler/"},
  {&Sohu,           "https://mirrors.sohu.com/openeuler/"}
};
def_sources_n(os_openeuler);

/**
 * HELP: 未经测试
 */
void
os_openeuler_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_openeuler);

  chsrc_backup (OS_openEuler_SourceList);

  char *towrite = xy_strjoin (3, "s#http://repo.openeuler.org#",
                              source.url,
                              "#\'< /etc/yum.repos.d/openEuler.repo.bak");

  chsrc_overwrite_file (towrite, OS_openEuler_SourceList);

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_conclude (&source, SetsrcType_Auto);
}

def_target_s(os_openeuler);
