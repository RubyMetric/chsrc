/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 * Created On    : <2023-09-26>
 * Last Modified : <2024-10-09>
 *
 * 名称为 Fedora Linux
 * ------------------------------------------------------------*/

/**
 * @update 2024-10-09
 */
static SourceInfo
os_fedora_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/fedora"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/fedora"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/fedora"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/fedora"},
  {&Tencent,       "https://mirrors.tencent.com/fedora"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/fedora"},
  {&Netease,       "https://mirrors.163.com/fedora"},
  {&Sohu,          "https://mirrors.sohu.com/fedora"}
};
def_sources_n(os_fedora);


/**
 * @note fedora 38 及以下版本暂不支持
 * 参考:
 * 1. https://mirrors.ustc.edu.cn/help/fedora.html
 */
void
os_fedora_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_fedora);

  chsrc_note2 ("Fedora 38 及以下版本暂不支持");

  chsrc_backup ("/etc/yum.repos.d/fedora.repo");
  chsrc_backup ("/etc/yum.repos.d/fedora-updates.repo");

  char* cmd = xy_strjoin (7, "sed ",
         "-e 's|^#baseurl=http://download.example/pub/fedora/linux/|baseurl=",
         source.url,
         "|g' ",
         "-i.bak ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-updates.repo");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_log2 ("已替换文件 /etc/yum.repos.d/fedora.repo");
  chsrc_log2 ("已替换文件 /etc/yum.repos.d/fedora-updates.repo");
  
  chsrc_note2 ("chsrc 已为您更换baseurl, 但fedora默认会优先使用metalink来匹配最快的源, 如您在获取metadata时速度较慢可自行将其注释");

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_conclude (&source, SetsrcType_Auto);
}

def_target_s(os_fedora);
