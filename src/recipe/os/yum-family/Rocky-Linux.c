/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-24>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static SourceInfo
os_rockylinux_sources[] = {
  {&Upstream,        NULL},
  {&Ali,            "https://mirrors.aliyun.com/rockylinux"},
  {&Volcengine,     "https://mirrors.volces.com/rockylinux"},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/rocky"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/rocky-linux"},
  {&Zju,            "https://mirrors.zju.edu.cn/rocky"},
  {&Lzuoss,         "https://mirror.lzu.edu.cn/rocky"},
  {&Sohu,           "https://mirrors.sohu.com/Rocky"},
  {&Netease,        "https://mirrors.163.com/rocky"}
};
def_sources_n(os_rockylinux);


/**
 * 参考: https://help.mirrors.cernet.edu.cn/rocky/
 */
void
os_rockylinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_rockylinux);
  chsrc_confirm_source (&source);

  char *cmd = xy_strjoin (3,
            "sed -e 's|^mirrorlist=|#mirrorlist=|g' "
            "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
            "-i.bak /etc/yum.repos.d/rocky-extras.repo /etc/yum.repos.d/rocky.repo"
            );
  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

def_target_noget(os_rockylinux);
