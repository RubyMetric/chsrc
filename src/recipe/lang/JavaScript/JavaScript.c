/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 *                |
 * Created On     : <2023-08-30>
 * Major Revision :      2
 * Last Modified  : <2025-07-11>
 * ------------------------------------------------------------*/

void
pl_js_check_cmd (bool *npm_exist, bool *yarn_exist, bool *pnpm_exist)
{
  *npm_exist  = chsrc_check_program ("npm");
  *yarn_exist = chsrc_check_program ("yarn");
  *pnpm_exist = chsrc_check_program ("pnpm");

  if (!*npm_exist && !*yarn_exist && !*pnpm_exist)
    {
      char *msg = ENGLISH ? "No npm, yarn or pnpm command found, check if at least one is present"
                          : "未找到 npm 或 yarn 或 pnpm 命令，请检查是否存在其一";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }
}


void
pl_js_group_getsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_js_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  hr();

  if (npm_exist)
    {
      pl_js_npm_getsrc (option);
      br();
    }

  if (yarn_exist)
    {
      pl_js_yarn_getsrc (option);
      br();
    }

  if (pnpm_exist)
    {
      pl_js_pnpm_getsrc (option);
      br();
    }
}


void
pl_js_group_setsrc (char *option)
{
  {
    char *msg = ENGLISH ? "Three package managers will be replaced for you at the same time: "
                          "npm, pnpm, yarn. If you need to change the source independently, "
                          "please run independently `chsrc set <pkg-manager>`"
                        : "将同时更换3个包管理器 npm, pnpm, Yarn 的源，若需要独立换源，请独立运行 chsrc set <pkg-manager>";
    chsrc_alert2 (msg);
  }

  bool npm_exist, yarn_exist, pnpm_exist;
  pl_js_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  chsrc_set_target_group_mode ();

  use_this(pl_js_group);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  if (npm_exist)
    {
      pl_js_npm_setsrc (option);
      br();
    }

  if (yarn_exist)
    {
      pl_js_yarn_setsrc (option);
      br();
    }

  if (pnpm_exist)
    {
      pl_js_pnpm_setsrc (option);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_js_group_resetsrc (char *option)
{
  pl_js_group_setsrc (option);
}
