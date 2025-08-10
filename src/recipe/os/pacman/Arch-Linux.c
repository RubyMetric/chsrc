/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_arch);

#define OS_Pacman_MirrorList "/etc/pacman.d/mirrorlist"
#define OS_Pacman_ArchLinuxCN_MirrorList "/etc/pacman.conf"

void
os_arch_prelude ()
{
  use_this(os_arch);
  chef_allow_gs(os_arch);

  chef_set_created_on   (this, "2023-09-05");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_authors (this, 2, "Aoran Zeng", "ccmywish@qq.com",
                             "Heng Guo",   "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "happy game", "happygame1024@gmail.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note (this,
    "可额外使用 chsrc set archlinuxcn 来更换 Arch Linux CN Repository 源",
    "You can additionally use chsrc set archlinuxcn to change Arch Linux CN Repository source");

  /**
   * @note 不要给后面加 / ，因为ARM情况下，还要额外加一个 arm 后缀
   */
  def_sources_begin()
  {&upstream,         "https://repo.archlinux.org",            DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/archlinux",  DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/archlinux", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/archlinux", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/archlinux", DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/archlinux", DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/archlinux", DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com/archlinux", DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/archlinux", DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,          "https://mirrors.sohu.com/archlinux",   DelegateToMirror}
  def_sources_end()
}


void
os_arch_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_MirrorList);
}


/**
 * @consult
 *   1. https://mirrors.tuna.tsinghua.edu.cn/help/archlinux/
 *   2. https://mirrors.tuna.tsinghua.edu.cn/help/archlinuxarm/
 */
void
os_arch_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_arch);

  chsrc_backup (OS_Pacman_MirrorList);

  bool  is_x86 = false;
  char *to_write = NULL;
  char *arch = chsrc_get_cpuarch ();

  if (strncmp(arch, "x86_64", 6)==0)
    {
      is_x86 = true;
      to_write = xy_strjoin (3, "Server = ", source.url, "/$repo/os/$arch");
    }
  else
    {
      is_x86 = false;
      to_write = xy_strjoin (3, "Server = ", source.url, "arm/$arch/$repo");
    }

  /* 配置文件中，越前面的优先级越高 */
  chsrc_prepend_to_file (to_write, OS_Pacman_MirrorList);

  if (is_x86)
    {
      chsrc_run ("pacman -Syyu", RunOpt_No_Last_New_Line);
    }
  else
    {
      chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}



/** ------------------------------------------------------------
 * archlinuxcn target
 * ------------------------------------------------------------*/

def_target(os_archlinuxcn);

void
os_archlinuxcn_prelude ()
{
  use_this(os_archlinuxcn);
  chef_allow_gs(os_archlinuxcn);

  chef_set_created_on   (this, "2023-09-05");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-07-03");

  chef_set_authors (this, 2, "Aoran Zeng", "ccmywish@qq.com", "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "happy game", "happygame1024@gmail.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note (this,
    "可额外使用 chsrc set arch 来更换 Arch Linux 源",
    "You can additionally use chsrc set arch to change Arch Linux source");


  def_sources_begin()
  {&upstream,         "https://repo.archlinuxcn.org/",            DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/archlinuxcn/",  DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/archlinuxcn/", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/archlinuxcn/", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/", DelegateToMirror},
  {&Tencent,          "https://mirrors.cloud.tencent.com/archlinuxcn/",    DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/archlinuxcn/", DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/archlinux-cn/", DelegateToMirror}
  def_sources_end()
}


void
os_archlinuxcn_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_MirrorList);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/archlinuxcn/
 * @update 2025-05-24
 */
void
os_archlinuxcn_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_archlinuxcn);

  chsrc_backup (OS_Pacman_ArchLinuxCN_MirrorList);

  char *arch = chsrc_get_cpuarch ();

  /* 检查是否已存在 archlinuxcn 配置段 */
  char *check_cmd = "grep -q '\\[archlinuxcn\\]' " OS_Pacman_ArchLinuxCN_MirrorList;
  int ret = system(check_cmd);

  if (ret == 0)
    {
      char *sed_cmd = xy_strjoin (4, "sed -i '/\\[archlinuxcn\\]/{n;s|^Server = .*|Server = ",
                                  source.url, "$arch|;}' ", OS_Pacman_ArchLinuxCN_MirrorList);
      chsrc_run (sed_cmd, RunOpt_Default);
    }
  else
    {
      char *archlinuxcn_config = xy_strjoin (3, "\n[archlinuxcn]\nServer = ", source.url, "$arch\n");
      chsrc_append_to_file (archlinuxcn_config, OS_Pacman_ArchLinuxCN_MirrorList);
    }

  chsrc_run ("pacman-key --lsign-key \"farseerfc@archlinux.org\"", RunOpt_Dont_Abort_On_Failure); // 此命令可能会失败, 但对换源没有影响
  chsrc_run ("pacman -Sy archlinuxcn-keyring", RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}
