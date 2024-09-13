/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 * Created On     : <2023-09-09>
 * Major Reviison :      3
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

/**
 * @update 2024-04-08
 */
static SourceInfo
pl_nodejs_yarn_sources[] = {
  {&Upstream,      "https://registry.npmjs.org/"}, // @note 根据 pnpm 官网，有最后的斜线
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs_yarn);


static double
pl_nodejs_yarn_get_yarn_version ()
{
  char *ver = xy_run ("yarn --version", 0, NULL);
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
  chsrc_yield_source_and_confirm (pl_nodejs_yarn);

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

  chsrc_conclude (&source, ChsrcTypeAuto);
}


/**
 * chsrc reset yarn
 */
void
pl_nodejs_yarn_resetsrc (char *option)
{
  pl_nodejs_yarn_setsrc (ChsrcTypeReset);
}


/**
 * chsrc ls yarn
 */
FeatInfo
pl_nodejs_yarn_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.stcan_locally = CanFully;
  fi.locally = NULL;
  fi.can_english = true;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}

def_target_gsrf(pl_nodejs_yarn);
