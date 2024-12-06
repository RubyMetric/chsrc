/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None  <nul@none.org>
 * Created On    : <2024-06-05>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

/**
 * chsrc get pdm
 */
void
pl_python_pdm_getsrc (char *option)
{
  // @HELP 不加 --global 时，会检测到本项目的配置吗？
  // 如果不行的话，就要分别改成 --global 和 --local
  char *cmd = "pdm config pypi.url";
  chsrc_run (cmd, RunOpt_Default);
}


/**
 * @consult https://github.com/RubyMetric/chsrc/issues/19
 *
 * chsrc set pdm
 */
void
pl_python_pdm_setsrc (char *option)
{
  Source_t source;
  chsrc_yield_for_the_source (pl_python);
  if (ProgMode_Target_Group!=true)
    chsrc_confirm_source;

  char *cmd = NULL;

  if (CliOpt_Locally==true)
    cmd = xy_2strjoin ("pdm config --local pypi.url ", source.url);
  else
    cmd = xy_2strjoin ("pdm config --global pypi.url ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (ProgMode_Target_Group!=true)
    {
      ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
      chsrc_conclude (&source);
    }
}


/**
 * chsrc reset pdm
 */
void
pl_python_pdm_resetsrc (char *option)
{
  pl_python_pdm_setsrc (option);
}


/**
 * chsrc ls pdm
 */
Feature_t
pl_python_pdm_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  // PDM 完全支持项目级换源
  f.cap_locally = FullyCan;
  f.cap_locally_explain = NULL;

  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

// def_target_gsrf(pl_python_pdm);
Target_t pl_python_pdm_target = {def_target_inner_gsrf(pl_python_pdm),def_target_sourcesn(pl_python)};
