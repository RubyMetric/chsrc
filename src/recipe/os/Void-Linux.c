/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 *               |
 * Created On    : <2023-09-24>
 * Last Modified : <2024-12-18>
 * ------------------------------------------------------------*/

static SourceProvider_t os_voidlinux_upstream =
{
  def_upstream, "https://repo-default.voidlinux.org",
  {NotSkip, NA, NA, "https://repo-default.voidlinux.org/live/20240314/void-live-x86_64-musl-20240314-xfce.iso"}
};

/**
 * @update 2024-12-18
 */
static Source_t os_voidlinux_sources[] =
{
  {&os_voidlinux_upstream, "https://repo-default.voidlinux.org"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/voidlinux"},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/voidlinux"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/voidlinux"}
};
def_sources_n(os_voidlinux);


void
os_voidlinux_getsrc (char *option)
{
  char* cmd = "xbps-query -L";
  chsrc_run (cmd, RunOpt_No_Last_New_Line);
}

/**
 * @consult https://help.mirrors.cernet.edu.cn/voidlinux/
 */
void
os_voidlinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_voidlinux);

  chsrc_ensure_dir ("/etc/xbps.d");
  char *cmd = "cp /usr/share/xbps.d/*-repository-*.conf /etc/xbps.d/";
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3,
            "sed -i 's|https://repo-default.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3,
            "sed -i 's|https://alpha.de.repo.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );

  chsrc_note2 ("若报错可尝试使用以下命令:");
  p(cmd);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}

def_target(os_voidlinux);
