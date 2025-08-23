/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_js_npm, "npm");

void
pl_js_npm_prelude (void)
{
  chef_prep_this (pl_js_npm, gsr);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@MrWillCom");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_target_sources (this, &pl_js_group_target);
}


void
pl_js_npm_getsrc (char *option)
{
  chsrc_run ("npm config get registry", RunOpt_No_Last_New_Line);
}


/**
 * @consult https://npmmirror.com/
 */
void
pl_js_npm_setsrc (char *option)
{
  Source_t source = chsrc_yield_source (&pl_js_group_target, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *cmd = NULL;

  if (chsrc_in_local_mode())
    cmd = xy_2strcat ("npm config --location project set registry ", source.url);
  else
    cmd = xy_2strcat ("npm config set registry ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_js_npm_resetsrc (char *option)
{
  pl_js_npm_setsrc (option);
}
