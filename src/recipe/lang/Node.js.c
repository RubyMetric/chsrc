/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Mr. Will  <mr.will.com@outlook.com>
 * Created On    : <2023-08-30>
 * Last Modified : <2024-08-28>
 * ------------------------------------------------------------*/

static MirrorSite
NpmMirror = {"npmmirror",    "npmmirror",    "npmmirror (阿里云赞助)",    "https://npmmirror.com/",
              // 注意，这个是跳转后的地址，不确定未来会不会改变
              "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.10.0/tfjs-4.10.0.tgz"}; // 29MB


/**
 * @time 2024-04-18 更新
 * @note {
 *   Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *   腾讯软件源的npm源一直未证实是否可用
 * }
 */
static SourceInfo
pl_nodejs_sources[] = {
  {&Upstream,       NULL},
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs);


void
pl_nodejs_check_cmd (bool *npm_exist, bool *yarn_exist, bool *pnpm_exist)
{
  *npm_exist  = chsrc_check_program ("npm");
  *yarn_exist = chsrc_check_program ("yarn");
  *pnpm_exist = chsrc_check_program ("yarn");

  if (!*npm_exist && !*yarn_exist && !*pnpm_exist)
    {
      chsrc_error ("未找到 npm 或 yarn 或 pnpm 命令，请检查是否存在其一");
      exit (Exit_UserCause);
    }
}


double
get_yarn_version ()
{
  char *ver = xy_run ("yarn --version", 0, NULL);
  double version = atof (ver);
  return version;
}


void
pl_nodejs_getsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  if (npm_exist)
    {
      chsrc_run ("npm config get registry", RunOpt_Default);
    }
  if (yarn_exist)
    {
      // 最后一个版本应该是 v1.22.22
      if (get_yarn_version () >= 2)
        // https://github.com/RubyMetric/chsrc/issues/53
        // 从 Yarn V2 开始，使用新的配置名
        chsrc_run ("yarn config get npmRegistryServer", RunOpt_Default);
      else
        chsrc_run ("yarn config get registry", RunOpt_Default);
    }
  if (pnpm_exist)
    {
      chsrc_run ("pnpm config get registry", RunOpt_Default);
    }
}

/**
 * NodeJS换源，参考：https://npmmirror.com/
 */
void
pl_nodejs_setsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  chsrc_yield_source_and_confirm (pl_nodejs);

  char *cmd = NULL;

  char *where = " ";
  if (CliOpt_Locally==true)
    {
      where = " --location project ";
    }

  if (npm_exist)
    {
      cmd = xy_strjoin (4, "npm config", where, "set registry ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (yarn_exist)
    {
      // 从 Yarn V2 开始，使用新的配置名
      if (get_yarn_version () >= 2)
        {
          if (CliOpt_Locally==true) // 默认基于本项目换源
            cmd = xy_2strjoin ("yarn config set npmRegistryServer ", source.url);
          else
            cmd = xy_2strjoin ("yarn config set npmRegistryServer --home ", source.url);

          chsrc_run (cmd, RunOpt_Default);
        }
      else
        {
          // 不再阻止换源命令输出到终端，即不再调用 xy_str_to_quietcmd()
          cmd = xy_2strjoin ("yarn config set registry ", source.url);
          chsrc_run (cmd, RunOpt_Default);
        }
    }

  if (pnpm_exist)
    {
      cmd = xy_2strjoin ("pnpm config set registry ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  chsrc_conclude (&source, ChsrcTypeAuto);
}


FeatInfo
pl_nodejs_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = false;

  fi.stcan_locally = CanSemi;
  fi.locally = "支持 npm (From v0.1.7); 支持 yarn v2 (From v0.1.8.1); pnpm 未知";
  fi.can_english = false;
  fi.can_user_define = true;

  return fi;
}

def_target_gsf (pl_nodejs);
