/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 * Created On    : <2023-09-29>
 * Last Modified : <2025-07-21>
 *
 * Raspberry Pi OS 树莓派操作系统，以前称为 Raspbian
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-29
 */
static Source_t os_raspberrypi_sources[] =
{
  // https://archive.raspberrypi.org/ until Debian "bullseye" release
  {&UpstreamProvider, "https://archive.raspberrypi.com/",              DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/raspberrypi/",    DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/raspberrypi/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/raspberrypi/",      DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/raspberrypi/",      DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/raspberrypi/", DelegateToMirror},
  {&Sustech,          "https://mirrors.sustech.edu.cn/raspberrypi/",   DelegateToMirror}
};
def_sources_n(os_raspberrypi);


void
os_raspberrypi_getsrc (char *option)
{
  chsrc_view_file (OS_RaspberryPi_SourceList);
}


void
os_raspberrypi_setsrc (char *option)
{
  chsrc_ensure_root(); // HELP: 不确定是否需要

  chsrc_yield_source_and_confirm (os_raspberrypi);

  chsrc_backup (OS_RaspberryPi_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url,
                            "@g' " OS_RaspberryPi_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_raspberrypi_resetsrc (char *option)
{
  os_raspberrypi_setsrc (option);
}


Feature_t
os_raspberrypi_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;

  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(os_raspberrypi);
