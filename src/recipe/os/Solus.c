/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_solus);

void
os_solus_prelude ()
{
  use_this(os_solus);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-29");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nil Null", "nil@null.org");

  // chef_allow_get();
  chef_allow_set();
  // chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&upstream,  "https://packages.getsol.us/shannon/eopkg-index.xml.xz", DelegateToUpstream},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror},
  {&Nju,       "https://mirror.nju.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/solus/
 */
void
os_solus_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this(os_solus);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  char *cmd = xy_2strjoin ("eopkg add-repo Solus ", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}
