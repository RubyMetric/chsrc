/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_alpine);

void
os_alpine_prelude ()
{
  use_this(os_alpine);

  chef_set_created_on   (this, "2023-09-24");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-09-14");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Yangmoooo", "yangmoooo@outlook.com");

  chef_allow_get();
  chef_allow_set();
  // chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&upstream,        "http://dl-cdn.alpinelinux.org/alpine", DelegateToUpstream},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/alpine", DelegateToMirror},
  {&Sjtug_Zhiyuan,   "https://mirrors.sjtug.sjtu.edu.cn/alpine", DelegateToMirror},
  {&Sustech,         "https://mirrors.sustech.edu.cn/alpine", DelegateToMirror},
  {&Zju,             "https://mirrors.zju.edu.cn/alpine", DelegateToMirror},
  {&Lzuoss,          "https://mirror.lzu.edu.cn/alpine", DelegateToMirror},
  {&Ali,             "https://mirrors.aliyun.com/alpine", DelegateToMirror},
  {&Tencent,         "https://mirrors.cloud.tencent.com/alpine", DelegateToMirror},
  {&Huawei,          "https://mirrors.huaweicloud.com/alpine", DelegateToMirror}
  def_sources_end()

  chsrc_set_provider_speed_measure_url (&upstream, "https://dl-cdn.alpinelinux.org/alpine/latest-stable/releases/x86_64/alpine-standard-3.21.0-x86_64.iso");
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

  use_this(os_alpine);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  char* cmd = xy_strjoin (3,
            "sed -i 's#https\\?://dl-cdn.alpinelinux.org/alpine#", source.url, "#g' /etc/apk/repositories"
            );
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apk update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
