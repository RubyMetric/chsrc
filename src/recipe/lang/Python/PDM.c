/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_pdm, "pdm");

void
pl_python_pdm_prelude (void)
{
  chef_prep_this (pl_python_pdm, gsr);

  chef_set_created_on   (this, "2024-06-05");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_local_mode (this, FullyCan, "支持项目级配置", "Supports project-level configuration");
  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_target_sources (this, &pl_python_group_target);
}


void
pl_python_pdm_getsrc (char *option)
{
  // @HELP 不加 --global 时，会检测到本项目的配置吗？
  // 如果不行的话，就要分别改成 --global 和 --local
  char *cmd = "pdm config pypi.url";
  chsrc_run (cmd, RunOpt_Default);
}


/**
 * @consult https://github.com/RubyMetric/chsrc/issues/19
 */
void
pl_python_pdm_setsrc (char *option)
{
  Source_t source = chsrc_yield_source (&pl_python_group_target, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *cmd = NULL;

  if (chsrc_in_local_mode())
    cmd = xy_2strcat ("pdm config --local pypi.url ", source.url);
  else
    cmd = xy_2strcat ("pdm config --global pypi.url ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}

void
pl_python_pdm_resetsrc (char *option)
{
  pl_python_pdm_setsrc (option);
}
