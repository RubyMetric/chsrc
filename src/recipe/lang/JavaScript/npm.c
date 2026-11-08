/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_npm, "npm");

void
pl_npm_prepare (void)
{
  chef_prep_this_dish (pl_npm, gsr);

  chef_set_recipe_created_on   (this, "2023-08-30");
  chef_set_recipe_last_updated (this, "2026-08-12");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@MrWillCom");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://registry.npmjs.org/",      FeedByPrepare}, /* @note 根据 pnpm 官网，有最后的斜线 */
  {&NpmMirror,        "https://registry.npmmirror.com/",  FeedByPrepare},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/npm/", FeedByPrepare},
  {&Tencent,          "https://mirrors.tencent.com/npm/", FeedByPrepare}
  def_sources_end()

  // 65MB 大小
  chef_set_rest_smURL_with_postfix (this, "@kenotron/100mb/-/100mb-100.0.0.tgz");
}


void
pl_npm_getsrc (char *option)
{
  chsrc_run ("npm config get registry", RunOpt_No_Last_New_Line);
}


/**
 * @consult https://npmmirror.com/
 */
void
pl_npm_setsrc (char *option)
{
  Source_t source = chsrc_yield_source (&pl_npm_dish, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *cmd = NULL;

  if (chsrc_in_project_scope_mode())
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
pl_npm_resetsrc (char *option)
{
  pl_npm_setsrc (option);
}
