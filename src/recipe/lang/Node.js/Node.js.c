/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 *                |
 * Created On     : <2023-08-30>
 * Major Revision :      2
 * Last Modified  : <2024-11-08>
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

  hr();

  if (npm_exist)
    {
      pl_nodejs_npm_getsrc (option);
      br();
    }

  if (yarn_exist)
    {
      pl_nodejs_yarn_getsrc (option);
      br();
    }

  if (pnpm_exist)
    {
      pl_nodejs_pnpm_getsrc (option);
      br();
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
      br();
    }

  if (yarn_exist)
    {
      pl_nodejs_yarn_setsrc (option);
      br();
    }

  if (pnpm_exist)
    {
      pl_nodejs_pnpm_setsrc (option);
    }

  ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
  chsrc_conclude (&source);
}


/**
 * chsrc reset nodejs
 */
void
pl_nodejs_resetsrc (char *option)
{
  pl_nodejs_setsrc (option);
}


/**
 * chsrc ls nodejs
 */
Feature_t
pl_nodejs_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = FullyCan;
  f.cap_locally_explain = "Support `npm` & `yarn v2` & `pnpm`. No support for `yarn v1`";
  f.can_english = true;
  f.can_user_define = true;

  return f;
}

def_target_gsrf (pl_nodejs);
