/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_poetry);

void
pl_python_poetry_prelude (void)
{
  use_this(pl_python_poetry);

  chef_set_created_on   (this, "2024-08-08");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nil Null", "nil@null.org");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_allow_local_mode (this, FullyCan, "Poetry 默认使用项目级换源", "Poetry uses project-level source changing by default");
  chef_allow_english(this);
  chef_allow_user_define(this);

  // 使用 pl_python_group 的源
  this->sources = pl_python_group_target.sources;
  this->sources_n = pl_python_group_target.sources_n;
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
  chsrc_yield_source (pl_python_group);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source();

  char *cmd = NULL;

  if (!chsrc_in_local_mode())
    chsrc_alert2 ("Poetry 仅支持项目级换源");

  cmd = xy_2strjoin ("poetry source add my_mirror ", source.url);
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
