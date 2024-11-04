/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 *               | happy game <happygame1024@gmail.com>
 * Created On    : <2023-09-26>
 * Last Modified : <2024-11-04>
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
 *
 * @consult https://mirrors.ustc.edu.cn/help/fedora.html
 */
void
os_fedora_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_fedora);

  chsrc_note2 ("Fedora 38 及以下版本暂不支持");

  chsrc_backup ("/etc/yum.repos.d/fedora.repo");
  chsrc_backup ("/etc/yum.repos.d/fedora-updates.repo");

  // 取消注释 baseurl
  char* cmd = xy_strjoin (5, "sed ",
         "-i 's|^#baseurl=|baseurl=",
         "|g' ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-updates.repo");
  chsrc_run (cmd, RunOpt_Default);

  // fedora的换源涉及 /etc/yum.repos.d/fedora.repo和 /etc/yum.repos.d/fedora-updates.repo
  // 需要替换 baseurl=source.url/releases/... 和 baseurl=source.url/releases/...
  cmd = xy_strjoin (7, "sed ",
         "-i -E 's!^baseurl=.*?/(releases|updates)/!baseurl=",
         source.url,
         "/\\1/",
         "!g' ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-updates.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_note2 ("已为您更换baseurl, 但fedora默认会优先使用metalink来匹配最快的源, 如您在获取metadata时速度较慢可自行将其注释:");
  chsrc_log2 ("(1) /etc/yum.repos.d/fedora.repo");
  chsrc_log2 ("(2) /etc/yum.repos.d/fedora-updates.repo");

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_conclude (&source, SetsrcType_Auto);
}



FeatInfo
os_fedora_feat (char *option)
{
  FeatInfo f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.can_english = true;
  f.can_user_define = true;

  return f;
}

def_target_sf(os_fedora);

