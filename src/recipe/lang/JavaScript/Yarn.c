/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_yarn, "yarn");

void
pl_yarn_prepare (void)
{
  chef_prep_this_dish (pl_yarn, gsr);

  chef_set_recipe_created_on   (this, "2023-09-09");
  chef_set_recipe_last_updated (this, "2025-07-11");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@MrWillCom");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_dish_sources (this, &pl_npm_dish);
}

static double
pl_yarn_get_yarn_version ()
{
  char *ver = xy_run ("yarn --version", 0);
  double version = xy_str2float (ver);
  return version;
}


void
pl_yarn_getsrc (char *option)
{
  // 最后一个版本应该是 v1.22.22
  if (pl_yarn_get_yarn_version () >= 2)
    // https://github.com/RubyMetric/chsrc/issues/53
    // 从 Yarn V2 开始，使用新的配置名
    chsrc_run ("yarn config get npmRegistryServer", RunOpt_No_Last_New_Line);
  else
    chsrc_run ("yarn config get registry", RunOpt_No_Last_New_Line);
}


/**
 * @consult https://github.com/RubyMetric/chsrc/issues/53
 * @consult https://yarnpkg.com/cli/config/set
 */
void
pl_yarn_setsrc (char *option)
{
  Source_t source = chsrc_yield_source (&pl_npm_dish, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *cmd = NULL;

  // 从 Yarn V2 开始，使用新的配置名
  if (pl_yarn_get_yarn_version () >= 2)
    {
      if (chsrc_in_project_scope_mode()) // Yarn 默认情况下就是基于本项目换源
        cmd = xy_2strcat ("yarn config set npmRegistryServer ", source.url);
      else
        cmd = xy_2strcat ("yarn config set npmRegistryServer --home ", source.url);

      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      if (chsrc_in_project_scope_mode())
        {
          char *msg = ENGLISH ? "Yarn v1 doesn't support `--scope=project`. SKIP changing source!"
                              : "Yarn v1 不支持项目级换源，跳过换源";
          chsrc_error (msg);
          // 不能直接退出，因为 combo dish 不能就此结束
          return;
        }
      // 不再阻止换源命令输出到终端，即不再调用 xy_quiet_cmd()
      cmd = xy_2strcat ("yarn config set registry ", source.url);
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_yarn_resetsrc (char *option)
{
  pl_yarn_setsrc (option);
}
