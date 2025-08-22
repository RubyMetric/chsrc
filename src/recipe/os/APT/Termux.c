/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_termux, "termux");

void
os_termux_prelude ()
{
  chef_prep_this (os_termux, gsr);

  chef_set_created_on   (this, "2025-03-04");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-03-04");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, "该 recipe 存在对应的 bootstrapper", "This recipe has a corresponding bootstrapper");

  def_sources_begin()
  {&UpstreamProvider, "https://packages.termux.org/apt/termux-main/", DelegateToUpstream},
  {&Ustc,             "https://mirrors.ustc.edu.cn/termux/", DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/termux/",   DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/termux/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/termux/",  DelegateToMirror},
  {&Pku,              "https://mirrors.pku.edu.cn/termux/",   DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/termux/",  DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/termux/",    DelegateToMirror},
  {&Sustech,          "https://mirrors.sustech.edu.cn/termux/", DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/termux/",   DelegateToMirror},
  {&Zju,              "https://mirrors.zju.edu.cn/termux/",   DelegateToMirror},
  {&Sdu,              "https://mirrors.sdu.edu.cn/termux/",   DelegateToMirror},
  {&Cqupt,            "https://mirrors.cqupt.edu.cn/termux/", DelegateToMirror}
  def_sources_end()
}


void
os_termux_getsrc (char *option)
{
  chsrc_view_file (OS_Termux_SourceList);
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/termux/
 *
 * chsrc set termux
 */
void
os_termux_setsrc (char *option)
{

  // chsrc_ensure_root (); Termux下禁止使用root

  use_this_source(os_termux);

  char *cmd = xy_strcat (3, "sed -i 's@^\\(deb.*stable main\\)$@#\\1\\ndeb ",
                              source.url, "apt/termux-main stable main@' " OS_Termux_SourceList);

  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update",  RunOpt_Default);
  chsrc_run ("apt upgrade", RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

void
os_termux_resetsrc (char *option)
{
  os_termux_setsrc (option);
}
