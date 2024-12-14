/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 * Created On     : <2023-09-09>
 * Major Reviison :      3
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

static double
pl_nodejs_yarn_get_yarn_version ()
{
  char *ver = xy_run ("yarn --version", 0);
  double version = atof (ver);
  return version;
}


/**
 * chsrc get yarn
 */
void
pl_nodejs_yarn_getsrc (char *option)
{
  // 最后一个版本应该是 v1.22.22
  if (pl_nodejs_yarn_get_yarn_version () >= 2)
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
pl_nodejs_yarn_setsrc (char *option)
{
  Source_t source;
  chsrc_yield_for_the_source (pl_nodejs);
  if (ProgMode_Target_Group!=true)
    chsrc_confirm_source;

  char *cmd = NULL;

  // 从 Yarn V2 开始，使用新的配置名
  if (pl_nodejs_yarn_get_yarn_version () >= 2)
    {
      if (CliOpt_Locally==true) // Yarn 默认情况下就是基于本项目换源
        cmd = xy_2strjoin ("yarn config set npmRegistryServer ", source.url);
      else
        cmd = xy_2strjoin ("yarn config set npmRegistryServer --home ", source.url);

      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      if (CliOpt_Locally)
        {
          char *msg = CliOpt_InEnglish ? "Yarn v1 doesn't support `-local`. SKIP changing source!" : "Yarn v1 不支持 -local，跳过换源";
          chsrc_error (msg);
          // 不能直接退出，因为 Leader target 不能就此结束
          return;
        }
      // 不再阻止换源命令输出到终端，即不再调用 xy_str_to_quietcmd()
      cmd = xy_2strjoin ("yarn config set registry ", source.url);
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }

  if (ProgMode_Target_Group!=true)
    {
      ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
      chsrc_conclude (&source);
    }
}


/**
 * chsrc reset yarn
 */
void
pl_nodejs_yarn_resetsrc (char *option)
{
  pl_nodejs_yarn_setsrc (option);
}


/**
 * chsrc ls yarn
 */
Feature_t
pl_nodejs_yarn_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = FullyCan;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

// def_target_gsrf(pl_nodejs_yarn);
Target_t pl_nodejs_yarn_target = {def_target_inner_gsrf(pl_nodejs_yarn),def_target_sourcesn(pl_nodejs)};
