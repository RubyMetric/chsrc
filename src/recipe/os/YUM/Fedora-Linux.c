/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 *               | happy game <happygame1024@gmail.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 *               |
 * Created On    : <2023-09-26>
 * Last Modified : <2025-07-21>
 *
 * 名称为 Fedora Linux
 * ------------------------------------------------------------*/

/**
 * @update 2025-06-20
 */
static Source_t os_fedora_sources[] =
{
  {&UpstreamProvider, "http://download.example/pub/fedora/linux", DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/fedora",        DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/fedora",       DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/fedora",       DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/fedora", DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/fedora",       DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/fedora",    DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/fedora",        DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/fedora",        DelegateToMirror}"
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

  chsrc_alert2 ("Fedora 38 及以下版本暂不支持");

  chsrc_backup ("/etc/yum.repos.d/fedora.repo");
  chsrc_backup ("/etc/yum.repos.d/fedora-updates.repo");

  // 取消对 baseurl 的注释
  char* cmd = xy_strjoin (5, "sed ",
         "-i 's|^#baseurl=|baseurl=",
         "|g' ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-updates.repo");
  chsrc_run (cmd, RunOpt_Default);

  // 替换
  // (1) baseurl=<<URL>>/releases/...
  // (2) baseurl=<<URL>>/updates/...
  cmd = xy_strjoin (7, "sed ",
         "-i -E 's!^baseurl=.*?/(releases|updates)/!baseurl=",
         source.url,
         "/\\1/",
         "!g' ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-updates.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_alert2 ("已更换baseurl, 但Fedora默认会优先使用metalink来匹配最快的源, 若在获取metadata时速度较慢可自行将其注释:");
  chsrc_log2 ("(1) /etc/yum.repos.d/fedora.repo");
  chsrc_log2 ("(2) /etc/yum.repos.d/fedora-updates.repo");

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_fedora_resetsrc (char *option)
{
  os_fedora_setsrc (option);
}


Feature_t
os_fedora_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.can_english = false;
  f.can_user_define = true;

  return f;
}

def_target_srf(os_fedora);
