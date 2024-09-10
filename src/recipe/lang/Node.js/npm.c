/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 * Created On     : <2023-08-30>
 * Major Revision :      2
 * Last Modified  : <2024-09-10>
 * ------------------------------------------------------------*/

/**
 * @update 2024-04-08
 */
static SourceInfo
pl_nodejs_npm_sources[] = {
  {&Upstream,      "https://registry.npmjs.org/"}, // @note 根据 pnpm 官网，有最后的斜线
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs_npm);


/**
 * chsrc get npm
 */
void
pl_nodejs_npm_getsrc (char *option)
{
  chsrc_run ("npm config get registry", RunOpt_No_Last_New_Line);
}


/**
 * @consult https://npmmirror.com/
 *
 * chsrc set npm
 */
void
pl_nodejs_npm_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_nodejs_npm);

  char *cmd = NULL;

  if (CliOpt_Locally==true)
    cmd = xy_2strjoin ("npm config --location project set registry ", source.url);
  else
    cmd = xy_2strjoin ("npm config set registry ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  chsrc_conclude (&source, ChsrcTypeAuto);
}


/**
 * chsrc reset npm
 */
void
pl_nodejs_npm_resetsrc (char *option)
{
  pl_nodejs_npm_setsrc (ChsrcTypeReset);
}


/**
 * chsrc ls npm
 */
FeatInfo
pl_nodejs_npm_feat (char *option)
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

def_target_gsrf(pl_nodejs_npm);
