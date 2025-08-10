/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_gentoo);

void
os_gentoo_prelude ()
{
  use_this(os_gentoo);
  chef_allow_s(os_gentoo);

  chef_set_created_on   (this, "2023-09-05");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_authors (this, 1, "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Aoran Zeng", "ccmywish@qq.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&upstream,  "rsync://rsync.gentoo.org/gentoo-portage", DelegateToUpstream},
  {&Ali,       "mirrors.aliyun.com", DelegateToMirror},
  {&Bfsu,      "mirrors.bfsu.edu.cn", DelegateToMirror},
  {&Ustc,      "mirrors.ustc.edu.cn", DelegateToMirror},
  {&Tuna,      "mirrors.tuna.tsinghua.edu.cn", DelegateToMirror},
  {&Tencent,   "mirrors.tencent.com", DelegateToMirror}
  def_sources_end()
}


/**
 * HELP: 未经测试
 */
void
os_gentoo_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this(os_gentoo);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  chsrc_backup ("/etc/portage/repos.conf/gentoo.conf");

  char *cmd = xy_strjoin (3, "sed -i \"s#rsync://.*/gentoo-portage#rsync://",
                             source.url,
                            "gentoo-portage#g");
  chsrc_run (cmd, RunOpt_Default);

  char *w = xy_strjoin (3, "GENTOO_MIRRORS=\"https://", source.url, "gentoo\"\n");

  chsrc_append_to_file (w, "/etc/portage/make.conf");

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
