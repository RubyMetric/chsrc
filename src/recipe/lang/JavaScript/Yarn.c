/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_js_yarn);

void
pl_js_yarn_prelude (void)
{
  use_this(pl_js_yarn);

  chef_set_created_on   (this, "2023-09-09");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Mr. Will", "mr.will.com@outlook.com");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  // 使用 pl_js_group 的源
  this->sources = pl_js_group_target.sources;
  this->sources_n = pl_js_group_target.sources_n;
}

static double
pl_js_yarn_get_yarn_version ()
{
  char *ver = xy_run ("yarn --version", 0);
  double version = atof (ver);
  return version;
}


void
pl_js_yarn_getsrc (char *option)
{
  // 最后一个版本应该是 v1.22.22
  if (pl_js_yarn_get_yarn_version () >= 2)
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
pl_js_yarn_setsrc (char *option)
{
  chsrc_yield_source (pl_js_group);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source();

  char *cmd = NULL;

  // 从 Yarn V2 开始，使用新的配置名
  if (pl_js_yarn_get_yarn_version () >= 2)
    {
      if (chsrc_in_local_mode()) // Yarn 默认情况下就是基于本项目换源
        cmd = xy_2strjoin ("yarn config set npmRegistryServer ", source.url);
      else
        cmd = xy_2strjoin ("yarn config set npmRegistryServer --home ", source.url);

      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      if (chsrc_in_local_mode())
        {
          char *msg = ENGLISH ? "Yarn v1 doesn't support `-local`. SKIP changing source!" : "Yarn v1 不支持 -local，跳过换源";
          chsrc_error (msg);
          // 不能直接退出，因为 Leader target 不能就此结束
          return;
        }
      // 不再阻止换源命令输出到终端，即不再调用 xy_str_to_quietcmd()
      cmd = xy_2strjoin ("yarn config set registry ", source.url);
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_js_yarn_resetsrc (char *option)
{
  pl_js_yarn_setsrc (option);
}
