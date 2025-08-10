/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static SourceProvider_t os_voidlinux_upstream =
{
  def_upstream, "https://repo-default.voidlinux.org",
  {NotSkip, NA, NA, "https://repo-default.voidlinux.org/live/20240314/void-live-x86_64-musl-20240314-xfce.iso", ACCURATE}
};

def_target(os_voidlinux);

void
os_voidlinux_prelude ()
{
  use_this(os_voidlinux);

  chef_set_created_on   (this, "2023-09-24");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-12-18");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Yangmoooo", "yangmoooo@outlook.com");

  chef_has_getfn();
  chef_has_setfn();
  chef_has_resetsrc();
  this.cap_locally = CanNot;
  this.cap_locally_explain = NULL;
  this.can_english = false;
  this.can_user_define = true;
  this.note = NULL;

  def_upstream_provider(os_voidlinux_upstream);
  def_sources_begin()
  {&upstream,        "https://repo-default.voidlinux.org", DelegateToUpstream},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/voidlinux", DelegateToMirror},
  {&Sjtug_Zhiyuan,   "https://mirror.sjtu.edu.cn/voidlinux", DelegateToMirror},
  {&Bfsu,            "https://mirrors.bfsu.edu.cn/voidlinux", DelegateToMirror}
  def_sources_end()
}


void
os_voidlinux_getsrc (char *option)
{
  char* cmd = "xbps-query -L";
  chsrc_run (cmd, RunOpt_No_Last_New_Line);
}

/**
 * @consult https://help.mirrors.cernet.edu.cn/voidlinux/
 */
void
os_voidlinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this(os_voidlinux);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  chsrc_ensure_dir ("/etc/xbps.d");
  char *cmd = "cp /usr/share/xbps.d/*-repository-*.conf /etc/xbps.d/";
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3,
            "sed -i 's|https://repo-default.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3,
            "sed -i 's|https://alpha.de.repo.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );

  chsrc_note2 ("若报错可尝试使用以下命令:");
  p (cmd);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_voidlinux_resetsrc (char *option)
{
  os_voidlinux_setsrc (option);
}
