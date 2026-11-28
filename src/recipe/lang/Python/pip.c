/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_pip, "pip");

void
pl_pip_prepare (void)
{
  chef_prep_this_dish (pl_pip, gsr);

  chef_set_recipe_created_on   (this, "2023-09-03");
  chef_set_recipe_last_updated (this, "2026-08-12");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@happy-game");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_dish_sources (this, &pl_pypi_dish);
}



/**
 * @param[out] prog_name 返回 Python 的可用名，如果不可用，则返回 NULL
 */
void
pl_py_get_python_program_name (char **prog_name)
{
  *prog_name = NULL;

  bool py_exist = false;

  /**
   * @issue https://gitee.com/RubyMetric/chsrc/issues/I9VZL2
   *
   * 由于Python2和Python3的历史，目前（2024-06）许多python命令实际上仍然是python2
   * 因此我们首先测试 python3
   */
  py_exist = chsrc_check_program ("python3");

  bool python3_is_winstore_placeholder = false;

  const int winstore_python_status_code = 9009;

  const char *winstore_placeholder_cn = "是微软商店的占位符，并非真正可用的 Python";
  const char *winstore_placeholder_en = "a placeholder of Microsoft Store, not the real Python";

  if (py_exist)
    {
      *prog_name = "python3";

      // https://github.com/RubyMetric/chsrc/issues/327
      if (xy.on_windows)
        {
          int status = xy_run_get_status ("python3 --version");
          if (status == winstore_python_status_code)
            {
              python3_is_winstore_placeholder = true;
              *prog_name = NULL;
              // https://github.com/RubyMetric/chsrc/issues/351
              // 仅警告，不要直接退出，因为用户环境中还可能存在真正的 `python` 命令
              chsrc_warn2 (CHINESE ? xy_2strcat ("用户环境中的 `python3` 命令，", winstore_placeholder_cn)
                                   : xy_2strcat ("`python3` in your environment is ", winstore_placeholder_en));
            }
        }
    }

  if (!py_exist || python3_is_winstore_placeholder)
    {
      /**
       * 不要直接:
       *
       *   $ python
       *
       * 这样调用 `python` 自己，而是使用 `python --version`，或者其他方式
       * 因为直接执行 `python` 会使 Windows 弹出Microsoft Store
       */
      py_exist = chsrc_check_program ("python");

      if (py_exist)
        {
          if (xy.on_windows)
            {
              int status = xy_run_get_status ("python --version");
              if (status == winstore_python_status_code)
                {
                  chsrc_warn2 (CHINESE ? xy_2strcat ("用户环境中的 `python` 命令，也", winstore_placeholder_cn)
                                       : xy_2strcat ("`python` in your environment is also ", winstore_placeholder_en));
                  chsrc_error (CHINESE ? "请安装真正的 Python 后重试！"
                                       : "Please install the real Python and try again!");
                  exit (Exit_UserCause);
                }
            }
          *prog_name = "python";
        }
      else
        {
          chsrc_error ("未找到 Python 相关命令，请检查是否存在");
          exit (Exit_UserCause);
        }
    }
}



void
pl_pip_getsrc (char *option)
{
  char *py_prog_name = NULL;
  pl_py_get_python_program_name (&py_prog_name);

  char *cmd = xy_2strcat (py_prog_name, " -m pip config get global.index-url");

  int status = chsrc_run_directly (cmd);
  if (0 == status)
    {
      // 执行成功时显示当前源
      xy_noop ();
    }
  else
    {
      // 执行失败时显示默认源
      if (ENGLISH)
        chsrc_note2 ("No source configured in pip, showing default upstream source:");
      else
        chsrc_note2 ("pip 中未配置源，显示默认上游源：");

      Source_t default_source = chsrc_yield_source (&pl_pypi_dish, "upstream");
      say (default_source.url);
    }
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/pypi/
 */
void
pl_pip_setsrc (char *option)
{
  // 对于不支持的情况，尽早结束
  if (chsrc_in_project_scope_mode())
    {
      char *msg = ENGLISH ? "pip doesn't support `--scope=project`. SKIP changing source!"
                          : "pip 不支持项目级换源，跳过换源";
      chsrc_error (msg);
      // 不能直接退出，因为 combo dish 不能就此结束
      return;
    }

  Source_t source = chsrc_yield_source_and_confirm (&pl_pypi_dish, option);

  char *py_prog_name = NULL;
  pl_py_get_python_program_name (&py_prog_name);

  // 这里用的是 config --user，会写入用户目录（而不是项目目录）
  // https://github.com/RubyMetric/chsrc/issues/39
  // 经测试，Windows上调用换源命令，会写入 C:\Users\RubyMetric\AppData\Roaming\pip\pip.ini
  char *cmd = xy_2strcat (py_prog_name, xy_2strcat (" -m pip config --user set global.index-url ", source.url));
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_pip_resetsrc (char *option)
{
  pl_pip_setsrc (option);
}
