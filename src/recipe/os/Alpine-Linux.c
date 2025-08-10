/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static SourceProvider_t os_alpine_upstream =
{
  def_upstream, "http://dl-cdn.alpinelinux.org/alpine",
  {NotSkip, NA, NA, "https://dl-cdn.alpinelinux.org/alpine/latest-stable/releases/x86_64/alpine-standard-3.21.0-x86_64.iso", ACCURATE}
};

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

  chef_has_getfn();
  chef_has_setfn();
  // chef_has_resetsrc();
  this.cap_locally = CanNot;
  this.cap_locally_explain = NULL;
  this.can_english = true;
  this.can_user_define = false;
  this.note = NULL;

  def_upstream_provider(os_alpine_upstream);
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
