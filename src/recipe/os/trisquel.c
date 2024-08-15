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
os_trisquel_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/trisquel/"},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/trisquel/"},
  {&Nju,            "https://mirror.nju.edu.cn/trisquel/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/trisquel/"}
};
def_sources_n(os_trisquel);


void
os_trisquel_getsrc (char *option)
{
  chsrc_view_file (ETC_APT_SOURCELIST);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/trisquel/
 */
void
os_trisquel_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_trisquel);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/trisquel/?@", source.url, "@g' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

def_target(os_trisquel);
