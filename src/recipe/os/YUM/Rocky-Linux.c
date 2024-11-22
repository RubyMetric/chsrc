/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-24>
 * Last Modified : <2024-08-22>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static Source_t
os_rockylinux_sources[] = {
  {&UpstreamProvider,        NULL},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/rocky"},
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

  chsrc_yield_source_and_confirm (os_rockylinux);


  char *version_str = xy_run ("sed -nr 's/ROCKY_SUPPORT_PRODUCT_VERSION=(.*)/\\1/p' " ETC_OS_RELEASE, 0, NULL);
  version_str = xy_str_delete_suffix (version_str, "\n");
  double version = atof (version_str);

  char *cmd = NULL;

  if (version < 9)
    {
      cmd = xy_strjoin (3,
                      "sed -e 's|^mirrorlist=|#mirrorlist=|g' "
                      "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
                      "-i.bak /etc/yum.repos.d/Rocky-*.repo"
                      );
                      // Rocky-AppStream.repo
                      // Rocky-BaseOS.repo
                      // Rocky-Extras
                      // Rocky-PowerTools
    }
  else
    {
      cmd = xy_strjoin (3,
                      "sed -e 's|^mirrorlist=|#mirrorlist=|g' "
                      "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
                      "-i.bak /etc/yum.repos.d/rocky-extras.repo /etc/yum.repos.d/rocky.repo"
                      );
    }


  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Auto;
  chsrc_conclude (&source);
}


def_target_s(os_rockylinux);
