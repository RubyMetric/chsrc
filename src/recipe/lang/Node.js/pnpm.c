/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2024-04-18>
 * Major Revision :      2
 * Last Modified  : <2024-09-10>
 * ------------------------------------------------------------*/

/**
 * @update 2024-04-08
 */
static SourceInfo
pl_nodejs_pnpm_sources[] = {
  {&Upstream,      "https://registry.npmjs.org/"}, // @note 根据 pnpm 官网，有最后的斜线
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs_pnpm);


/**
 * chsrc get pnpm
 */
void
pl_nodejs_pnpm_getsrc (char *option)
{
  chsrc_run ("pnpm config get registry", RunOpt_Default);
}


/**
 * @consult https://pnpm.io/feature-comparison
 * @consult https://pnpm.io/cli/config
 *
 * chsrc set pnpm
 */
void
pl_nodejs_pnpm_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_nodejs_pnpm);

  char *cmd = NULL;

  if (CliOpt_Locally)
    cmd = xy_2strjoin ("pnpm config --location project set registry ", source.url);
  else
    cmd = xy_2strjoin ("pnpm config -g set registry ", source.url);

  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source, ChsrcTypeAuto);
}


/**
 * chsrc reset pnpm
 */
void
pl_nodejs_pnpm_resetsrc (char *option)
{
  pl_nodejs_pnpm_setsrc (ChsrcTypeReset);
}


/**
 * chsrc ls pnpm
 */
FeatInfo
pl_nodejs_pnpm_feat (char *option)
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


def_target_gsrf(pl_nodejs_pnpm);
