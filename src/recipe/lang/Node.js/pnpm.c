/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2024-04-18>
 * Major Revision :      2
 * Last Modified  : <2025-07-11>
 * ------------------------------------------------------------*/

/**
 * chsrc get pnpm
 */
void
pl_nodejs_pnpm_getsrc (char *option)
{
  chsrc_run ("pnpm config get registry", RunOpt_No_Last_New_Line);
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
  chsrc_yield_source (pl_nodejs_group);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source;

  char *cmd = NULL;

  if (chsrc_in_local_mode())
    cmd = xy_2strjoin ("pnpm config --location project set registry ", source.url);
  else
    cmd = xy_2strjoin ("pnpm config -g set registry ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


/**
 * chsrc reset pnpm
 */
void
pl_nodejs_pnpm_resetsrc (char *option)
{
  pl_nodejs_pnpm_setsrc (option);
}


/**
 * chsrc ls pnpm
 */
Feature_t
pl_nodejs_pnpm_feat (char *option)
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


// def_target_gsrf(pl_nodejs_pnpm);
Target_t pl_nodejs_pnpm_target = {def_target_inner_gsrf(pl_nodejs_pnpm),def_target_sourcesn(pl_nodejs_group)};
