/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-29>
 * Last Modified : <2024-11-21>
 *
 * Trisquel基于Ubuntu开发，不含任何专有软件及专有固件，内核使用 Linux-libre
 * ------------------------------------------------------------*/

/**
 * @update 2024-11-21
 */
static Source_t os_trisquel_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/trisquel/"},
  {&Ali,              "https://mirrors.aliyun.com/trisquel/"},
  {&Nju,              "https://mirror.nju.edu.cn/trisquel/"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/trisquel/"},
  {&Iscas,            "https://mirror.iscas.ac.cn/trisquel/"}
};
def_sources_n(os_trisquel);


void
os_trisquel_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/trisquel/
 */
void
os_trisquel_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_trisquel);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/trisquel/?@", source.url, "@g' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}

def_target(os_trisquel);
