/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-26>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-02 更新
 * @note 源并不完整，且未经测试是否有效
 */
static SourceInfo
os_fedora_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/fedora"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/fedora"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/fedora"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/fedora"},
  {&Tencent,       "https://mirrors.tencent.com/fedora"},
  {&Netease,       "https://mirrors.163.com/fedora"},
  {&Sohu,          "https://mirrors.sohu.com/fedora"}
};
def_sources_n(os_fedora);


/**
 * @note fedora 29 及以下版本暂不支持
 */
void
os_fedora_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_fedora);
  chsrc_confirm_source (&source);

  chsrc_note2 ("Fedora 29 及以下版本暂不支持");

  chsrc_backup ("/etc/yum.repos.d/fedora.repo");
  chsrc_backup ("/etc/yum.repos.d/fedora-updates.repo");

  char* cmd = xy_strjoin (9, "sed -e 's|^metalink=|#metalink=|g' ",
         "-e 's|^#baseurl=http://download.example/pub/fedora/linux/|baseurl=",
         source.url,
         "|g' ",
         "-i.bak ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-modular.repo ",
         "/etc/yum.repos.d/fedora-updates.repo ",
         "/etc/yum.repos.d/fedora-updates-modular.repo");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_log2 ("已替换文件 /etc/yum.repos.d/fedora.repo");
  chsrc_log2 ("已新增文件 /etc/yum.repos.d/fedora-modular.repo");
  chsrc_log2 ("已替换文件 /etc/yum.repos.d/fedora-updates.repo");
  chsrc_log2 ("已新增文件 /etc/yum.repos.d/fedora-updates-modular.repo");

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

def_target_noget(os_fedora);
