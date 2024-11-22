/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 *               |
 * Created On    : <2023-09-29>
 * Last Modified : <2024-11-21>
 * ------------------------------------------------------------*/

/**
 * @update 2024-11-21
 */
static Source_t os_linuxlite_sources[] =
{
  {&UpstreamProvider, "http://repo.linuxliteos.com/linuxlite/"},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/linuxliteos/"},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/linuxliteos/"},
  {&Nju,              "https://mirror.nju.edu.cn/linuxliteos/"}
};
def_sources_n(os_linuxlite);


void
os_linuxlite_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/linuxliteos/
 */
void
os_linuxlite_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_linuxlite);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url, "@g' " OS_Apt_SourceList);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Auto;
  chsrc_conclude (&source);
}

def_target(os_linuxlite);
