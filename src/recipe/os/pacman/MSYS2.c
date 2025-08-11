/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_msys2, "msys2/msys");

void
os_msys2_prelude ()
{
  use_this(os_msys2);
  chef_allow_s(os_msys2);

  chef_set_created_on   (this, "2023-09-06");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_authors (this, 1, "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 1,
    "Aoran Zeng", "ccmywish@qq.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,  NULL,                               DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/msys2",  DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/msys2", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/msys2", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/msys2", DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/msys2", DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com/msys2", DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/msys2",  DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/msys2", DelegateToMirror}
  def_sources_end()
}


/**
 * HELP: 未经测试
 */
void
os_msys2_setsrc (char *option)
{
  use_this_source(os_msys2);

  chsrc_backup ("/etc/pacman.d/mirrorlist.mingw32");
  chsrc_backup ("/etc/pacman.d/mirrorlist.mingw64");
  chsrc_backup ("/etc/pacman.d/mirrorlist.msys");

  char *prev = xy_strjoin (3, "请针对你的架构下载安装此目录下的文件:",
                              source.url,
                             "distrib/<架构>/");
  chsrc_note2 (prev);

  char *cmd = xy_strjoin (3, "sed -i \"s#https\?://mirror.msys2.org/#",
                              source.url,
                             "#g\" /etc/pacman.d/mirrorlist* ");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source);
}
