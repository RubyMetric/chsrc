/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 * Created On     : <2023-08-30>
 * Major Revision :      2
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

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
  {
    char *msg = CliOpt_InEnglish ? "Three package managers will be replaced for you at the same time: " \
                                         "npm, pnpm, yarn. If you need to change the source independently, " \
                                         "please run independently `chsrc set <pkg-manager>`"
                                       : "将同时为您更换3个包管理器 npm, pnpm, Yarn 的源，若需要独立换源，请独立运行 chsrc set <pkg-manager>";
    chsrc_note2 (msg);
  }

  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  ProgMode_Target_Group = true;
  chsrc_yield_source_and_confirm (pl_nodejs);

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
    }

  chsrc_conclude (&source, ChsrcTypeAuto);
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
