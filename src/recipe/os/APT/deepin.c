/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-26>
 * Last Modified : <2024-08-16>
 *
 * 名称为小写deepin，而非Deepin
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-06 更新
 * @note 源并不完整，且未经测试是否有效
 */
static SourceInfo
os_deepin_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/deepin"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/deepin"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/deepin"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/deepin"},
  {&Tencent,       "https://mirrors.tencent.com/deepin"},
  {&Netease,       "https://mirrors.163.com/deepin"},
  {&Sohu,          "https://mirrors.sohu.com/deepin"}
};
def_sources_n(os_deepin);


void
os_deepin_getsrc(char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

/**
 * HELP: 未经测试
 */
void
os_deepin_setsrc (char *option)
{
  chsrc_ensure_root();

  chsrc_yield_source_and_confirm (os_deepin);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/deepin/?@",
                              source.url,
                              "@g\' " OS_Apt_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_conclude (&source, ChsrcTypeUntested);
}

def_target(os_deepin);
