/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 * Created On     : <2023-08-30>
 * Major Revision :      2
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

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
  SourceInfo source;
  chsrc_yield_for_the_source (pl_nodejs);
  if (ProgMode_Target_Group!=true)
    chsrc_confirm_source;

  char *cmd = NULL;

  if (CliOpt_Locally==true)
    cmd = xy_2strjoin ("npm config --location project set registry ", source.url);
  else
    cmd = xy_2strjoin ("npm config set registry ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (ProgMode_Target_Group!=true)
    chsrc_conclude (&source, SetsrcType_Auto);
}


/**
 * chsrc reset npm
 */
void
pl_nodejs_npm_resetsrc (char *option)
{
  pl_nodejs_npm_setsrc (SetsrcType_Reset);
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

// def_target_gsrf(pl_nodejs_npm);
TargetInfo pl_nodejs_npm_target = {def_target_inner_gsrf(pl_nodejs_npm),def_target_sourcesn(pl_nodejs)};
