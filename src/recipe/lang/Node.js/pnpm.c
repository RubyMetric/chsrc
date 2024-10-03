/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2024-04-18>
 * Major Revision :      2
 * Last Modified  : <2024-09-13>
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
  SourceInfo source;
  chsrc_yield_for_the_source (pl_nodejs);
  if (ProgMode_Target_Group!=true)
    chsrc_confirm_source;

  char *cmd = NULL;

  if (CliOpt_Locally)
    cmd = xy_2strjoin ("pnpm config --location project set registry ", source.url);
  else
    cmd = xy_2strjoin ("pnpm config -g set registry ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (ProgMode_Target_Group!=true)
    chsrc_conclude (&source, SetsrcType_Auto);
}


/**
 * chsrc reset pnpm
 */
void
pl_nodejs_pnpm_resetsrc (char *option)
{
  pl_nodejs_pnpm_setsrc (SetsrcType_Reset);
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

  fi.cap_locally = FullyCan;
  fi.locally = NULL;
  fi.can_english = true;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}


// def_target_gsrf(pl_nodejs_pnpm);
TargetInfo pl_nodejs_pnpm_target = {def_target_inner_gsrf(pl_nodejs_pnpm),def_target_sourcesn(pl_nodejs)};
