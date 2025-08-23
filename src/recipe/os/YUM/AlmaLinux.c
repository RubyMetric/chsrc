/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_almalinux, "alma/almalinux");

void
os_almalinux_prelude ()
{
  chef_prep_this (os_almalinux, s);

  chef_set_created_on   (this, "2024-06-12");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-08-22");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Yangmoooo");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://repo.almalinux.org/almalinux",  DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/almalinux", FeedByPrelude},
  {&Volcengine,       "https://mirrors.volces.com/almalinux", FeedByPrelude},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/almalinux", FeedByPrelude},
  {&Zju,              "https://mirrors.zju.edu.cn/almalinux",        FeedByPrelude},
  {&Nju,              "https://mirror.nju.edu.cn/almalinux",         FeedByPrelude}
  def_sources_end()

#define link "/9.6/isos/x86_64/AlmaLinux-9-latest-x86_64-minimal.iso"
  chef_set_sources_speed_measure_url_with_postfix (this, link);
  chef_set_provider_speed_measure_url (&UpstreamProvider, "https://raw.repo.almalinux.org/almalinux" link);
#undef link
}

/**
 * @consult: https://developer.aliyun.com/mirror/almalinux
 */
void
os_almalinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_use_this_source (os_almalinux);

  char *cmd = xy_strcat (3,
    "sed -e 's|^mirrorlist=|#mirrorlist=|g' -e 's|^#\\s*baseurl=https://repo.almalinux.org/almalinux|baseurl=", source.url, "|g'  -i.bak  /etc/yum.repos.d/almalinux*.repo");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}
