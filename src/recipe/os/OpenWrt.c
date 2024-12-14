/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo  <yangmoooo@outlook.com>
 *               | happy game <happygame1024@gmail.com>
 *               |
 * Created On    : <2024-08-08>
 * Last Modified : <2024-12-14>
 * ------------------------------------------------------------*/

/**
 * @update 2024-12-14
 */
static Source_t os_openwrt_sources[] =
{
  {&UpstreamProvider, "https://downloads.openwrt.org"},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/openwrt"},
  {&Ali,              "https://mirrors.aliyun.com/openwrt"},
  {&Tencent,          "https://mirrors.cloud.tencent.com/openwrt"},
  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/openwrt"},
  {&Tuna,             "https://mirror.tuna.tsinghua.edu.cn/openwrt"},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/openwrt"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/openwrt"},
  {&Pku,              "https://mirrors.pku.edu.cn/openwrt"},
  {&Sustech,          "https://mirrors.sustech.edu.cn/openwrt"},
};

def_sources_n(os_openwrt);


#define OS_OpenWRT_SourceConfig "/etc/opkg/distfeeds.conf"

void
os_openwrt_getsrc (char *option)
{
  chsrc_view_file (OS_OpenWRT_SourceConfig);
}

/**
 * @consult
 *    1. https://mirror.tuna.tsinghua.edu.cn/help/openwrt/
 *    2. https://github.com/RubyMetric/chsrc/issues/153#issuecomment-2543077933
 */
void
os_openwrt_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_openwrt);

  chsrc_backup (OS_OpenWRT_SourceConfig);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/releases@", source.url, "/releases@g' " OS_OpenWRT_SourceConfig);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);
  chsrc_run ("opkg update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
  chsrc_conclude (&source);
}


void
os_openwrt_resetsrc (char *option)
{
  os_openwrt_setsrc (option);
}


Feature_t
os_openwrt_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}


def_target_gsrf(os_openwrt);