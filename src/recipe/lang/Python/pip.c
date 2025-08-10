/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_pip, "pip");

void
pl_python_pip_prelude (void)
{
  use_this(pl_python_pip);
  chef_allow_gsr(pl_python_pip);

  chef_set_created_on   (this, "2023-09-03");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 0);

  chef_forbid_local_mode (this);
  chef_allow_english(this);
  chef_allow_user_define(this);

  // 使用 pl_python_group 的源
  this->sources = pl_python_group_target.sources;
  this->sources_n = pl_python_group_target.sources_n;
}


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
 */
void
pl_python_pip_setsrc (char *option)
{
  // 对于不支持的情况，尽早结束
  if (chsrc_in_local_mode())
    {
      char *msg = ENGLISH ? "pip doesn't support `-local`. SKIP changing source!" : "pip 不支持 -local，跳过换源";
      chsrc_error (msg);
      // 不能直接退出，因为 Leader target 不能就此结束
      return;
    }

  Source_t source = chsrc_yield_source (&pl_python_group_target, option);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source(&source);

  char *py_prog_name = NULL;
  pl_python_get_py_program_name (&py_prog_name);

  // 这里用的是 config --user，会写入用户目录（而不是项目目录）
  // https://github.com/RubyMetric/chsrc/issues/39
  // 经测试，Windows上调用换源命令，会写入 C:\Users\RubyMetric\AppData\Roaming\pip\pip.ini
  char *cmd = xy_2strjoin (py_prog_name, xy_2strjoin (" -m pip config --user set global.index-url ", source.url));
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_python_pip_resetsrc (char *option)
{
  pl_python_pip_setsrc (option);
}
