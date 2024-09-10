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
 * @time 2024-04-18 更新
 * @note {
 *   Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *   腾讯软件源的npm源一直未证实是否可用
 * }
 */
static SourceInfo
pl_nodejs_sources[] = {
  {&Upstream,      "https://registry.npmjs.org/"},
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
  *pnpm_exist = chsrc_check_program ("pnpm");

  if (!*npm_exist && !*yarn_exist && !*pnpm_exist)
    {
      char *msg = CliOpt_InEnglish ? "No npm, yarn or pnpm command found, check if at least one is present"
                                   : "未找到 npm 或 yarn 或 pnpm 命令，请检查是否存在其一";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }
}


/**
 * chsrc get nodejs
 */
void
pl_nodejs_getsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  split_between_source_changing_process;

  if (npm_exist)
    {
      pl_nodejs_npm_getsrc (option);
      say ("");
    }

  if (yarn_exist)
    {
      pl_nodejs_yarn_getsrc (option);
      say ("");
    }

  if (pnpm_exist)
    {
      pl_nodejs_pnpm_getsrc (option);
      say ("");
    }
}


/**
 * chsrc set nodejs
 */
void
pl_nodejs_setsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  // chsrc_yield_source_and_confirm (pl_nodejs);
  split_between_source_changing_process;

  if (npm_exist)
    {
      pl_nodejs_npm_setsrc (option);
      say ("");
    }

  if (yarn_exist)
    {
      pl_nodejs_yarn_setsrc (option);
      say ("");
    }

  if (pnpm_exist)
    {
      pl_nodejs_pnpm_setsrc (option);
      say ("");
    }

  // chsrc_conclude (&source, ChsrcTypeAuto);
}


/**
 * chsrc reset nodejs
 */
void
pl_nodejs_resetsrc (char *option)
{
  pl_nodejs_setsrc (ChsrcTypeReset);
}


/**
 * chsrc ls nodejs
 */
FeatInfo
pl_nodejs_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.stcan_locally = CanFully;
  fi.locally = "Support npm (chsrc v0.1.7)\nSupport yarn v2 (chsrc v0.1.8.1)\nSupport pnpm (chsrc v0.1.8.2)";
  fi.can_english = true;
  fi.can_user_define = true;

  return fi;
}

def_target_gsrf (pl_nodejs);
