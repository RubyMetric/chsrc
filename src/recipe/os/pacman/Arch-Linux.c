/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-05>
 * Last Modified : <2024-08-22>
 * ------------------------------------------------------------*/

/**
 * @update 2024-07-03
 * @note 不要给后面加 / ，因为ARM情况下，还要额外加一个 arm 后缀
 */
static Source_t os_arch_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Ali,              "https://mirrors.aliyun.com/archlinux"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/archlinux"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/archlinux"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/archlinux"},
  {&Tencent,          "https://mirrors.tencent.com/archlinux"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/archlinux"},
  {&Huawei,           "https://mirrors.huaweicloud.com/archlinux"}, // 不支持 archlinuxcn
  {&Netease,          "https://mirrors.163.com/archlinux"},         // archlinuxcn 的URL和其他镜像站不同
  // {&Sohu,          "https://mirrors.sohu.com/archlinux"}       // 不支持 archlinuxcn
},

/**
 * @update 2024-07-03
 * @note 根据 @zheng7fu2 建议，拆分 archlinuxcn 出来
 */
os_archlinuxcn_sources[] =
{
  {&UpstreamProvider, "https://repo.archlinuxcn.org/"},
  {&Ali,              "https://mirrors.aliyun.com/archlinuxcn/"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/archlinuxcn/"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/archlinuxcn/"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/"},
  {&Tencent,          "https://mirrors.cloud.tencent.com/archlinuxcn/"},
  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/archlinuxcn/"},
  {&Netease,          "https://mirrors.163.com/archlinux-cn/"}
};
def_sources_n(os_arch);
def_sources_n(os_archlinuxcn);


#define OS_Pacman_MirrorList "/etc/pacman.d/mirrorlist"
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

  chsrc_yield_source_and_confirm (os_arch);

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

  // 越前面的优先级越高
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


void
os_archlinuxcn_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_MirrorList);
}

/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/archlinuxcn/
 */
void
os_archlinuxcn_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_archlinuxcn);

  chsrc_backup (OS_Pacman_MirrorList);

  bool  arch_flag = false;
  char *arch = chsrc_get_cpuarch ();

  char *towrite = xy_strjoin (3, "[archlinuxcn]\nServer = ", source.url, "$arch");
  // 越前面的优先级越高
  chsrc_prepend_to_file (towrite, OS_Pacman_MirrorList);

  chsrc_run ("pacman-key --lsign-key \"farseerfc@archlinux.org\"", RunOpt_Dont_Abort_On_Failure);
  chsrc_run ("pacman -Sy archlinuxcn-keyring", RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}
#undef OS_Pacman_MirrorList


Feature_t
os_arch_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.can_english = true;
  f.can_user_define = true;

  f.note = "可额外使用 chsrc set archlinuxcn 来更换 Arch Linux CN Repository 源";
  return f;
}


Feature_t
os_archlinuxcn_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.can_english = true;
  f.can_user_define = true;

  f.note = "可额外使用 chsrc set arch 来更换 Arch Linux 源";
  return f;
}


def_target_gsf(os_arch);
def_target_gsf(os_archlinuxcn);
