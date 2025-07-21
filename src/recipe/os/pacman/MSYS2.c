/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 * Created On    : <2023-09-06>
 * Last Modified : <2025-06-20>
 * ------------------------------------------------------------*/

/**
 * @update 2025-06-20
 */
static Source_t os_msys2_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
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
};
def_sources_n(os_msys2);


/**
 * HELP: 未经测试
 */
void
os_msys2_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (os_msys2);

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

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


Feature_t
os_msys2_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_sf(os_msys2);
