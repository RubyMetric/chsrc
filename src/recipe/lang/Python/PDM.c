/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2024-06-05>
 * Last Modified : <2024-09-13>
 * ------------------------------------------------------------*/

/**
 * @update 2024-05-24
 * @note 不要添加Zju，浙大的pypi在校外访问会自动转向Tuna
 */
static SourceInfo
pl_python_pdm_sources[] = {
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
def_sources_n(pl_python_pdm);


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
  chsrc_yield_source_and_confirm (pl_python_pdm);

  char *cmd = NULL;

  if (CliOpt_Locally==true)
    cmd = xy_2strjoin ("pdm config --local pypi.url ", source.url);
  else
    cmd = xy_2strjoin ("pdm config --global pypi.url ", source.url);

  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source, ChsrcTypeAuto);
}


/**
 * chsrc reset pdm
 */
void
pl_python_pdm_resetsrc (char *option)
{
  pl_python_pdm_setsrc (ChsrcTypeReset);
}


/**
 * chsrc ls pdm
 */
FeatInfo
pl_python_pdm_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  // PDM 完全支持项目级换源
  fi.stcan_locally = CanFully;
  fi.locally = NULL;

  fi.can_english = true;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}

def_target_gsrf(pl_python_pdm);
