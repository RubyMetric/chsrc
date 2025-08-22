/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_alpine, "alpine");

void
os_alpine_prelude ()
{
  chef_prep_this (os_alpine, gs);

  chef_set_created_on   (this, "2023-09-24");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-09-14");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Yangmoooo");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://dl-cdn.alpinelinux.org/alpine", FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/alpine", FeedByPrelude},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/alpine", FeedByPrelude},
  {&Sustech,          "https://mirrors.sustech.edu.cn/alpine", FeedByPrelude},
  {&Zju,              "https://mirrors.zju.edu.cn/alpine", FeedByPrelude},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/alpine", FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/alpine", FeedByPrelude},
  {&Tencent,          "https://mirrors.cloud.tencent.com/alpine", FeedByPrelude},
  {&Huawei,           "https://mirrors.huaweicloud.com/alpine", FeedByPrelude}
  def_sources_end()

  chef_set_sources_speed_measure_url_with_postfix (this, "/latest-stable/releases/x86_64/alpine-standard-3.21.0-x86_64.iso");
}


void
os_alpine_getsrc (char *option)
{
  chsrc_view_file ("/etc/apk/repositories");
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/alpine/
 */
void
os_alpine_setsrc (char *option)
{
  // chsrc_ensure_root(); // HELP: 不确定是否需要root

  use_this_source(os_alpine);

  char* cmd = xy_strcat (3,
            "sed -i 's#https\\?://dl-cdn.alpinelinux.org/alpine#", source.url, "#g' /etc/apk/repositories"
            );
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apk update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
