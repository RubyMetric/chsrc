/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_solus, "solus");

void
os_solus_prelude ()
{
  use_this(os_solus);
  chef_allow_s(os_solus);

  chef_set_created_on   (this, "2023-09-29");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-29");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 0);


  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "https://packages.getsol.us/shannon/eopkg-index.xml.xz", DelegateToUpstream},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/solus/
 */
void
os_solus_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_solus);

  char *cmd = xy_2strcat ("eopkg add-repo Solus ", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}
