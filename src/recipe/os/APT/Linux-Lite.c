/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_linuxlite, "linuxlite");

void
os_linuxlite_prelude ()
{
  use_this(os_linuxlite);
  chef_allow_gsr(os_linuxlite);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-11-21");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Yangmoooo", "yangmoooo@outlook.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&upstream,       "http://repo.linuxliteos.com/linuxlite/",    DelegateToUpstream},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/linuxliteos/",    DelegateToMirror},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/linuxliteos/", DelegateToMirror},
  {&Nju,            "https://mirror.nju.edu.cn/linuxliteos/",         DelegateToMirror}
  def_sources_end()
}


void
os_linuxlite_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

/**
 * @consult https://help.mirrors.cernet.edu.cn/linuxliteos/
 */
void
os_linuxlite_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_linuxlite);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url, "@g' " OS_Apt_SourceList);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_linuxlite_resetsrc (char *option)
{
  os_linuxlite_setsrc (option);
}
