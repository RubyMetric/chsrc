/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_openwrt, "openwrt/opkg/LEDE");

void
os_openwrt_prelude ()
{
  chef_prep_this (os_openwrt, gsr);

  chef_set_created_on   (this, "2024-08-08");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-12-14");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@Yangmoooo", "@happy-game");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "https://downloads.openwrt.org", DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/openwrt", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/openwrt", DelegateToMirror},
  {&Tencent,          "https://mirrors.cloud.tencent.com/openwrt", DelegateToMirror},
  {&Tuna,             "https://mirror.tuna.tsinghua.edu.cn/openwrt", DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/openwrt", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/openwrt", DelegateToMirror},
  {&Pku,              "https://mirrors.pku.edu.cn/openwrt", DelegateToMirror},
  {&Sustech,          "https://mirrors.sustech.edu.cn/openwrt", DelegateToMirror}
  def_sources_end()

  chef_set_provider_speed_measure_url (&UpstreamProvider, "https://downloads.openwrt.org/releases/23.05.5/targets/x86/64/openwrt-sdk-23.05.5-x86-64_gcc-12.3.0_musl.Linux-x86_64.tar.xz");
}


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

  use_this_source(os_openwrt);

  chsrc_backup (OS_OpenWRT_SourceConfig);

  char *cmd = xy_strcat (3, "sed -E -i 's@https?://.*/releases@", source.url, "/releases@g' " OS_OpenWRT_SourceConfig);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);
  chsrc_run ("opkg update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_openwrt_resetsrc (char *option)
{
  os_openwrt_setsrc (option);
}
