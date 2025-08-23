/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_poetry, "poetry");

void
pl_python_poetry_prelude (void)
{
  chef_prep_this (pl_python_poetry, gsr);

  chef_set_created_on   (this, "2024-08-08");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_local_mode (this, FullyCan, "Poetry 默认使用项目级换源", "Poetry uses project-level source changing by default");
  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_target_sources (this, &pl_python_group_target);
}

void
pl_python_poetry_getsrc (char *option)
{
  chsrc_note2 ("poetry换源情况: 请查看本项目 pyproject.toml 中 [[tool.poetry.source]]");
}


/**
 * @consult https://python-poetry.org/docs/repositories/#project-configuration
 */
void
pl_python_poetry_setsrc (char *option)
{
  Source_t source = chsrc_yield_source (&pl_python_group_target, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *cmd = NULL;

  if (!chsrc_in_local_mode())
    chsrc_alert2 ("Poetry 仅支持项目级换源");

  cmd = xy_2strcat ("poetry source add my_mirror ", source.url);
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_python_poetry_resetsrc (char *option)
{
  pl_python_poetry_setsrc (option);
}
