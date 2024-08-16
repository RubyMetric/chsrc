/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-03>
 * Last Modified : <2024-08-17>
 * ------------------------------------------------------------*/

/**
 * 2024-05-24 更新
 *
 * @note 不要添加Zju，浙大的pypi在校外访问会自动转向Tuna
 */
static SourceInfo
pl_python_sources[] = {
  {&Upstream,      "https://pypi.org/simple"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/pypi/web/simple"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/pypi/web/simple"},
  {&Jlu,           "https://mirrors.jlu.edu.cn/pypi/web/simple"},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/pypi/web/simple"},
  {&Tuna,          "https://pypi.tuna.tsinghua.edu.cn/simple"},
  {&Ali,           "https://mirrors.aliyun.com/pypi/simple/"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/pypi/simple"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/pypi/simple"},
  {&Hust,          "https://mirrors.hust.edu.cn/pypi/web/simple"}
  // {&Netease,    "https://mirrors.163.com/.help/pypi.html"} // 不用，24小时更新一次
};

def_sources_n(pl_python);

/**
 * @param[out] prog 返回 Python 的可用名，如果不可用，则返回 NULL
 */
void
pl_python_check_cmd (char **prog, bool *poetry_exist, bool *pdm_exist)
{
  *prog = NULL;
  *pdm_exist = false;
  *poetry_exist = false;

  bool py_exist = false;

  // 由于Python2和Python3的历史，目前（2024-06）许多python命令实际上仍然是python2
  // https://gitee.com/RubyMetric/chsrc/issues/I9VZL2
  // 因此我们首先测试 python3
  py_exist = chsrc_check_program ("python3");

  if (py_exist) *prog = "python3";
  else
    {
      // 不要调用 python 自己，而是使用 python --version，避免Windows弹出Microsoft Store
      py_exist = chsrc_check_program ("python");

      if (py_exist) *prog = "python";
      else
        {
          chsrc_error ("未找到 Python 相关命令，请检查是否存在");
          exit (Exit_UserCause);
        }
    }

  *poetry_exist = chsrc_check_program ("poetry");
  *pdm_exist = chsrc_check_program ("pdm");
}

void
pl_python_getsrc (char *option)
{
  char *prog = NULL;
  bool pdm_exist = false,
       poetry_exist = false;

  pl_python_check_cmd (&prog, &poetry_exist, &pdm_exist);
  char *cmd = xy_2strjoin (prog, " -m pip config get global.index-url");
  chsrc_run (cmd, RunOpt_Default);

  if (pdm_exist)
    {
      cmd = "pdm config --global pypi.url";
      chsrc_run (cmd, RunOpt_Default);
    }

  if (poetry_exist)
    {
      chsrc_note2 ("poetry换源情况: 请查阅本项目 pyproject.toml 中 [[tool.poetry.source]]");
    }
}

/**
 * Python换源，参考：
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/pypi/
 * 2. https://github.com/RubyMetric/chsrc/issues/19
 *
 * 经测试，Windows上调用换源命令，会写入 C:\Users\RubyMetric\AppData\Roaming\pip\pip.ini
 *
 * 3. Poetry: https://python-poetry.org/docs/repositories/#project-configuration
 */
void
pl_python_setsrc (char *option)
{
  char *chsrc_type = xy_streql (option, ChsrcTypeReset) ? ChsrcTypeReset : ChsrcTypeAuto;
  char *prog = NULL;

  bool pdm_exist = false,
       poetry_exist = false,
       uv_exist = false;  // 2024-08-08: uv 似乎暂时没有实现该功能

  pl_python_check_cmd (&prog, &poetry_exist, &pdm_exist);

  SourceInfo source;
  chsrc_yield_source (pl_python);
  chsrc_confirm_source (&source);

  // 这里用的是 config --user，会写入用户目录（而不是项目目录）
  // GitHub#39
  char *cmd = xy_2strjoin (prog, xy_2strjoin (" -m pip config --user set global.index-url ", source.url));
  chsrc_run (cmd, RunOpt_Default);

  if (pdm_exist)
    {
      char *where = " --global ";
      if (CliOpt_Locally==true)
        {
          where = " --local ";
        }
      cmd = xy_strjoin (4, "pdm config", where, "pypi.url ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (poetry_exist)
    {
      cmd = xy_2strjoin ("poetry source add my_mirror ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  chsrc_say_lastly (&source, chsrc_type);
}

void
pl_python_resetsrc (char *option)
{
  pl_python_setsrc (ChsrcTypeReset);
}


FeatInfo
pl_python_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.locally = "pip 不支持; pdm 支持 (From v0.1.6); poetry 默认使用项目级 (From v0.1.7.2)";
  fi.can_english = false;
  fi.can_user_define = true;

  return fi;
}

def_target_gsrf(pl_python);
