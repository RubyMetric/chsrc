/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2023-09-03>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

/**
 * chsrc get pip
 */
void
pl_python_pip_getsrc (char *option)
{
  char *py_prog_name = NULL;
  pl_python_get_py_program_name (&py_prog_name);

  char *cmd = xy_2strjoin (py_prog_name, " -m pip config get global.index-url");
  chsrc_run (cmd, RunOpt_Default);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/pypi/
 *
 * chsrc set pip
 */
void
pl_python_pip_setsrc (char *option)
{
  // 对于不支持的情况，尽早结束
  if (CliOpt_Locally)
    {
      char *msg = CliOpt_InEnglish ? "pip doesn't support `-local`. SKIP changing source!" : "pip 不支持 -local，跳过换源";
      chsrc_error (msg);
      // 不能直接退出，因为 Leader target 不能就此结束
      return;
    }

  SourceInfo source;
  chsrc_yield_for_the_source (pl_python);
  if (ProgMode_Target_Group!=true)
    chsrc_confirm_source;

  char *py_prog_name = NULL;
  pl_python_get_py_program_name (&py_prog_name);

  // 这里用的是 config --user，会写入用户目录（而不是项目目录）
  // https://github.com/RubyMetric/chsrc/issues/39
  // 经测试，Windows上调用换源命令，会写入 C:\Users\RubyMetric\AppData\Roaming\pip\pip.ini
  char *cmd = xy_2strjoin (py_prog_name, xy_2strjoin (" -m pip config --user set global.index-url ", source.url));
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (ProgMode_Target_Group!=true)
    chsrc_conclude (&source, SetsrcType_Auto);
}


/**
 * chsrc reset pip
 */
void
pl_python_pip_resetsrc (char *option)
{
  pl_python_pip_setsrc (SetsrcType_Reset);
}


/**
 * chsrc ls pip
 */
FeatInfo
pl_python_pip_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  // pip 不支持项目级换源
  fi.stcan_locally = CanNot;
  fi.locally = NULL;
  fi.can_english = true;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}

// def_target_gsrf(pl_python_pip);
TargetInfo pl_python_pip_target = {def_target_inner_gsrf(pl_python_pip),def_target_sourcesn(pl_python)};
