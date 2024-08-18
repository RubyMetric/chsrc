/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Heng Guo <2085471348@qq.com>
 * Contributors  : Nil Null  <nil@null.org>
 * Created On    : <2023-09-17>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static SourceInfo
os_opensuse_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/opensuse"},
  {&Volcengine,     "https://mirrors.volces.com/opensuse"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/opensuse"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/opensuse"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/opensuse"},
  {&Tencent,        "https://mirrors.tencent.com/opensuse"},
  {&Netease,        "https://mirrors.163.com/opensuse"},
  {&Sohu,           "https://mirrors.sohu.com/opensuse"}
};
def_sources_n(os_opensuse);


/**
 * HELP: 未经测试
 */
void
os_opensuse_setsrc (char *option)
{
  chsrc_ensure_root (); // HELP: 不知道是否需要确保root权限

  chsrc_yield_source_and_confirm (os_opensuse);

  char *source_nselect = "zypper mr -da";
  chsrc_run (source_nselect, RunOpt_Default);

  char *cmd1 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/repo/oss/' mirror-oss");
  char *cmd2 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/repo/non-oss/' mirror-non-oss");
  char *cmd3 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/oss/' mirror-update");
  char *cmd4 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/non-oss/' mirror-update-non-oss");
  char *cmd5 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/sle/' mirror-sle-update");
  char *cmd6 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/backports/' mirror-backports-update");

  chsrc_run (cmd1, RunOpt_Default);
  chsrc_run (cmd2, RunOpt_Default);
  chsrc_run (cmd3, RunOpt_Default);
  chsrc_run (cmd4, RunOpt_Default);

  chsrc_note2 ("leap 15.3用户还需要添加sle和backports源");
  chsrc_note2 ("另外请确保系统在更新后仅启用了六个软件源，可以使用 zypper lr 检查软件源状态");
  chsrc_note2 ("并使用 zypper mr -d 禁用多余的软件源");

  chsrc_run (cmd5, RunOpt_Default);
  chsrc_run (cmd6, RunOpt_Default);
  chsrc_conclude (&source, ChsrcTypeUntested);
}

def_target_s(os_opensuse);
