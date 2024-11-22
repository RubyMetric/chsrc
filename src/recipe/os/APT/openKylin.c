/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-06>
 * Last Modified : <2024-08-16>
 *
 * openKylin直接基于Linux内核开发，属于和Debian、openSUSE、Fedora、Arch
 * 同一级别的、根社区发布的系统
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-29 更新
 */
static Source_t os_openkylin_sources[] =
{
  {&UpstreamProvider, "https://archive.openkylin.top/openkylin/"},
  {&Ali,              "https://mirrors.aliyun.com/openkylin/"},
  {&Netease,          "https://mirrors.163.com/openkylin/"},
};
def_sources_n(os_openkylin);

void
os_openkylin_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

void
os_openkylin_setsrc (char *option)
{
  chsrc_ensure_root();

  chsrc_yield_source_and_confirm (os_openkylin);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/openkylin/?@", source.url, "@g'" OS_Apt_SourceList);
  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}

def_target(os_openkylin);
