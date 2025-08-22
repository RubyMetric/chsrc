/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_manjaro, "manjaro");

void
os_manjaro_prelude ()
{
  chef_prep_this (os_manjaro, s);

  chef_set_created_on   (this, "2023-09-06");
  chef_set_last_updated (this, "2025-08-10");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, NULL, DelegateToUpstream}
  /* Manjaro uses GUI tool, no manual sources needed */
  def_sources_end()
}

/**
 * 似乎会弹出GUI，待确定
 */
void
os_manjaro_setsrc (char *option)
{
  chsrc_ensure_root ();
  char *cmd = "pacman-mirrors -i -c China -m rank";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  chsrc_conclude (NULL);
}
