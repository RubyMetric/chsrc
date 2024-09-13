/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-08-08>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

// @note Poetry 默认使用项目级换源

/**
 * chsrc get poetry
 */
void
pl_python_poetry_getsrc (char *option)
{
  chsrc_note2 ("poetry换源情况: 请查阅本项目 pyproject.toml 中 [[tool.poetry.source]]");
}


/**
 * @consult https://python-poetry.org/docs/repositories/#project-configuration
 *
 * chsrc set poetry
 */
void
pl_python_poetry_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_the_source (pl_python);
  if (ProgMode_Target_Group!=true)
    chsrc_confirm_source;

  char *cmd = NULL;

  if (CliOpt_Locally==false)
    chsrc_note2 ("Poertry 仅支持项目级换源");

  cmd = xy_2strjoin ("poetry source add my_mirror ", source.url);
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  chsrc_conclude (&source, ChsrcTypeAuto);
}


/**
 * chsrc reset poetry
 */
void
pl_python_poetry_resetsrc (char *option)
{
  pl_python_poetry_setsrc (ChsrcTypeReset);
}


/**
 * chsrc ls poetry
 */
FeatInfo
pl_python_poetry_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.stcan_locally = CanFully;
  fi.locally = NULL;
  fi.can_english = false;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}

// def_target_gsrf(pl_python_poetry);
TargetInfo pl_python_poetry_target = {def_target_inner_gsrf(pl_python_poetry),def_target_sourcesn(pl_python)};
