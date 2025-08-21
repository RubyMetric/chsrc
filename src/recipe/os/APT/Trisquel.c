/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Trisquel基于Ubuntu开发，不含任何专有软件及专有固件，内核使用 Linux-libre
 * ------------------------------------------------------------*/

def_target(os_trisquel, "trisquel");

void
os_trisquel_prelude ()
{
  use_this(os_trisquel);
  chef_allow_gsr(os_trisquel);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-11-21");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://archive.trisquel.info/trisquel/", DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/trisquel/", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/trisquel/",    DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/trisquel/",     DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/trisquel/",   DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/trisquel/",    DelegateToMirror}
  def_sources_end()
}


void
os_trisquel_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/trisquel/
 */
void
os_trisquel_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_trisquel);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strcat (3, "sed -E -i 's@https?://.*/trisquel/?@", source.url, "@g' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_trisquel_resetsrc (char *option)
{
  os_trisquel_setsrc (option);
}
