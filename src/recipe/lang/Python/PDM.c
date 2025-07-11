/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None  <nul@none.org>
 * Created On    : <2024-06-05>
 * Last Modified : <2025-07-11>
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
  chsrc_yield_source (pl_python);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source;

  char *cmd = NULL;

  if (chsrc_in_local_mode())
    cmd = xy_2strjoin ("pdm config --local pypi.url ", source.url);
  else
    cmd = xy_2strjoin ("pdm config --global pypi.url ", source.url);

  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
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
