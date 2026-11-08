/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(os_alpine, "alpine");

void
os_alpine_prepare ()
{
  chef_prep_this_dish (os_alpine, gs);

  chef_set_recipe_created_on   (this, "2023-09-24");
  chef_set_recipe_last_updated (this, "2026-07-22");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Yangmoooo");

  chef_set_os_scope (this);

  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://dl-cdn.alpinelinux.org/alpine", FeedByPrepare},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/alpine", FeedByPrepare},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/alpine", FeedByPrepare},
  {&Zju,              "https://mirrors.zju.edu.cn/alpine", FeedByPrepare},
  {&Lzuoss,           "https://mirrors.lzu.edu.cn/alpine", FeedByPrepare},
  {&Ali,              "https://mirrors.aliyun.com/alpine", FeedByPrepare},
  {&Tencent,          "https://mirrors.tencent.com/alpine", FeedByPrepare},
  {&Huawei,           "https://mirrors.huaweicloud.com/alpine", FeedByPrepare}
  def_sources_end()

  chef_set_rest_smURL_with_postfix (this, "/latest-stable/releases/x86_64/netboot/initramfs-lts");
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

  chsrc_use_this_source (os_alpine);

  char* cmd = xy_strcat (3,
            "sed -i 's#https\\?://dl-cdn.alpinelinux.org/alpine#", source.url, "#g' /etc/apk/repositories"
            );
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apk update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
