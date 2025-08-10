/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_js_pnpm);

void
pl_js_pnpm_prelude (void)
{
  use_this(pl_js_pnpm);
  chef_allow_gsr(pl_js_pnpm);

  chef_set_created_on   (this, "2024-04-18");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  // 使用 pl_js_group 的源
  this->sources = pl_js_group_target.sources;
  this->sources_n = pl_js_group_target.sources_n;
}


void
pl_js_pnpm_getsrc (char *option)
{
  chsrc_run ("pnpm config get registry", RunOpt_No_Last_New_Line);
}


/**
 * @consult https://pnpm.io/feature-comparison
 * @consult https://pnpm.io/cli/config
 */
void
pl_js_pnpm_setsrc (char *option)
{
  Source_t source = chsrc_yield_source (&pl_js_group_target, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *cmd = NULL;

  if (chsrc_in_local_mode())
    cmd = xy_2strjoin ("pnpm config --location project set registry ", source.url);
  else
    cmd = xy_2strjoin ("pnpm config -g set registry ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_js_pnpm_resetsrc (char *option)
{
  pl_js_pnpm_setsrc (option);
}
