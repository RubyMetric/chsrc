/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 * Created On    : <2024-06-12>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static Source_t os_almalinux_sources[] =
{
  {&UpstreamProvider, "http://repo.almalinux.org/almalinux"},
  {&Ali,              "https://mirrors.aliyun.com/almalinux"},
  {&Volcengine,       "https://mirrors.volces.com/almalinux"},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/almalinux"},
  {&Zju,              "https://mirrors.zju.edu.cn/almalinux"},
  {&Nju,              "https://mirror.nju.edu.cn/almalinux"},
};
def_sources_n(os_almalinux);

/**
 * 参考: https://developer.aliyun.com/mirror/almalinux
 */
void
os_almalinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_almalinux);

  char *cmd = xy_strjoin (3,
    "sed -e 's|^mirrorlist=|#mirrorlist=|g' -e 's|^#\\s*baseurl=https://repo.almalinux.org/almalinux|baseurl=", source.url, "|g'  -i.bak  /etc/yum.repos.d/almalinux*.repo");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Auto;
  chsrc_conclude (&source);
}

def_target_s(os_almalinux);
