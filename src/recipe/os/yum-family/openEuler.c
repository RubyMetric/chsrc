/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-06>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-06 更新
 * @note 源并不完整，且未经测试是否有效
 */
static SourceInfo
os_openeuler_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/openeuler/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/openeuler/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/openeuler/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/openeuler/"},
  {&Tencent,        "https://mirrors.tencent.com/openeuler/"},
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

  SourceInfo source;
  chsrc_yield_source (os_openeuler);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_openEuler_SourceList);

  char *towrite = xy_strjoin (3, "s#http://repo.openeuler.org#",
                              source.url,
                              "#\'< /etc/yum.repos.d/openEuler.repo.bak");

  chsrc_overwrite_file (towrite, OS_openEuler_SourceList);

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

def_target_noget(os_openeuler);
