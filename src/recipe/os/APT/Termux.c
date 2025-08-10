/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_termux);

void
os_termux_prelude ()
{
  use_this(os_termux);

  chef_set_created_on   (this, "2025-03-04");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-03-04");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nul None", "nul@none.org");

  chef_has_get();
  chef_has_set();
  chef_has_reset();
  this.note = "该 recipe 存在对应的 bootstrapper";

  def_upstream("https://packages.termux.org/apt/termux-main/");
  def_sources_begin(os_termux)
  {&upstream,       "https://packages.termux.org/apt/termux-main/", DelegateToUpstream},
  {&Ustc,           "https://mirrors.ustc.edu.cn/termux/", DelegateToMirror},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/termux/",   DelegateToMirror},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/termux/", DelegateToMirror},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/termux/",  DelegateToMirror},
  {&Pku,            "https://mirrors.pku.edu.cn/termux/",   DelegateToMirror},
  {&Nyist,          "https://mirror.nyist.edu.cn/termux/",  DelegateToMirror},
  {&Nju,            "https://mirror.nju.edu.cn/termux/",    DelegateToMirror},
  {&Sustech,        "https://mirrors.sustech.edu.cn/termux/", DelegateToMirror},
  {&Iscas,          "https://mirror.iscas.ac.cn/termux/",   DelegateToMirror},
  {&Zju,            "https://mirrors.zju.edu.cn/termux/",   DelegateToMirror},
  {&Sdu,            "https://mirrors.sdu.edu.cn/termux/",   DelegateToMirror},
  {&Cqupt,          "https://mirrors.cqupt.edu.cn/termux/", DelegateToMirror}
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

  chsrc_yield_source_and_confirm (os_termux);

  char *cmd = xy_strjoin (3, "sed -i 's@^\\(deb.*stable main\\)$@#\\1\\ndeb ",
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
