/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 * Created On    : <2023-09-29>
 * Last Modified : <2024-10-02>
 *
 * Raspberry Pi OS 树莓派操作系统，以前称为 Raspbian
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-29 更新
 */
static Source_t
os_raspberrypi_sources[] = {
  {&UpstreamProvider,       "https://archive.raspberrypi.com/"}, // https://archive.raspberrypi.org/ until Debian "bullseye" release
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/raspberrypi/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/raspberrypi/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/raspberrypi/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/raspberrypi/"},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/raspberrypi/"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/raspberrypi/"}
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

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}

def_target(os_raspberrypi);
