/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-02>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static SourceInfo
os_debian_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/debian"},
  {&Volcengine,    "https://mirrors.volces.com/debian"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/debian"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/debian"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/debian"},
  {&Tencent,       "https://mirrors.tencent.com/debian"},
  {&Netease,       "https://mirrors.163.com/debian"},
  {&Sohu,          "https://mirrors.sohu.com/debian"}
};
def_sources_n(os_debian);


void
os_debian_getsrc (char *option)
{
  if (chsrc_check_file (OS_Debian_SourceList_DEB822))
    {
      chsrc_view_file (OS_Debian_SourceList_DEB822);
      return;
    }

  if (chsrc_check_file (OS_Apt_SourceList))
    {
      chsrc_view_file (OS_Apt_SourceList);
      return;
    }

  chsrc_error2 ("缺少源配置文件！但仍可直接通过 chsrc set debian 来添加使用新的源");
  return;
}

void
os_debian_setsrc_for_deb822 (char *option)
{
  SourceInfo source;
  chsrc_yield_source (os_debian);
  chsrc_confirm_source (&source);

  chsrc_note2 ("如果遇到无法拉取 HTTPS 源的情况，我们会使用 HTTP 源并需要您运行:");
  puts ("apt install apt-transport-https ca-certificates");

  chsrc_backup (OS_Debian_SourceList_DEB822);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/debian/?@", source.url, "@g' " OS_Debian_SourceList_DEB822);
  chsrc_run (cmd, RunOpt_Default);

  // debian-security 源和其他源不一样
  cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/debian-security/?@", source.url, "-security@g' " OS_Debian_SourceList_DEB822);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}


/**
 * Debian 10 Buster 以上版本默认支持 HTTPS 源。如果遇到无法拉取 HTTPS 源的情况，请先使用 HTTP 源并安装
 * apt install apt-transport-https ca-certificates
 */
void
os_debian_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (OS_Debian_SourceList_DEB822))
    {
      chsrc_note2 ("将基于新格式换源");
      os_debian_setsrc_for_deb822 (option);
      return;
    }


  // Docker环境下，Debian镜像可能不存在该文件
  bool sourcelist_exist = ensure_apt_sourcelist (OS_Is_Debian_Literally);

  SourceInfo source;
  chsrc_yield_source (os_debian);
  chsrc_confirm_source (&source);

  chsrc_note2 ("如果遇到无法拉取 HTTPS 源的情况，我们会使用 HTTP 源并需要您运行:");
  puts ("apt install apt-transport-https ca-certificates");

  // 不存在的时候，用的是我们生成的无效文件，不要备份
  if (sourcelist_exist)
    {
      chsrc_backup (OS_Apt_SourceList);
    }

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/debian/?@", source.url, "@g\' " OS_Apt_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

def_target(os_debian);
