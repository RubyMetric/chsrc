/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 * Created On    : <2023-09-06>
 * Last Modified : <2025-07-21>
 *
 * openKylin直接基于Linux内核开发，属于和Debian、openSUSE、Fedora、Arch
 * 同一级别的、根社区发布的系统
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-29
 */
static Source_t os_openkylin_sources[] =
{
  {&UpstreamProvider, "https://archive.openkylin.top/openkylin/", DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/openkylin/",    DelegateToMirror},
  {&Netease,          "https://mirrors.163.com/openkylin/",       DelegateToMirror},
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

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_openkylin_resetsrc (char *option)
{
  os_openkylin_setsrc (option);
}


Feature_t
os_openkylin_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;

  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(os_openkylin);
