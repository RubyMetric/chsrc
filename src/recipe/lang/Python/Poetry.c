/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-08-08>
 * Last Modified : <2024-09-13>
 * ------------------------------------------------------------*/

// @note Poetry 默认使用项目级换源


/**
 * @update 2024-05-24
 * @note 不要添加Zju，浙大的pypi在校外访问会自动转向Tuna
 */
static SourceInfo
pl_python_poetry_sources[] = {
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
def_sources_n(pl_python_poetry);


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
  chsrc_yield_source_and_confirm (pl_python_poetry);

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

def_target_gsrf(pl_python_poetry);
