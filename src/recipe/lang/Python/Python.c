/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-03>
 * Last Modified : <2024-09-13>
 *
 * 2024-08-08: uv 似乎暂时没有实现换源
 * ------------------------------------------------------------*/

/**
 * @update 2024-05-24
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


void
pl_python_getsrc (char *option)
{
  bool pdm_exist    = false,
       poetry_exist = false;

  pl_python_check_unofficial_pkger (&poetry_exist, &pdm_exist);

  // 交给后面检查命令的存在性
  pl_python_pip_getsrc (option);
  say ("");

  if (poetry_exist)
    {
      pl_python_poetry_getsrc (option);
      say ("");
    }

  if (pdm_exist)
    {
      pl_python_pdm_getsrc (option);
    }
}


void
pl_python_setsrc (char *option)
{
  char *chsrc_type = xy_streql (option, ChsrcTypeReset) ? ChsrcTypeReset : ChsrcTypeAuto;

  bool pdm_exist    = false,
       poetry_exist = false;

  pl_python_check_unofficial_pkger (&poetry_exist, &pdm_exist);

  // chsrc_yield_source_and_confirm (pl_python);

  // 交给后面检查命令的存在性
  pl_python_pip_setsrc (option);
  say ("");

  if (poetry_exist)
    {
      pl_python_poetry_setsrc (option);
      say ("");
    }

  if (pdm_exist)
    {
      pl_python_pdm_setsrc (option);
    }

  // chsrc_conclude (&source, chsrc_type);
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

  fi.stcan_locally = CanSemi;
  fi.locally = "pip 不支持，其他支持";
  fi.can_english = false;
  fi.can_user_define = true;

  return fi;
}

def_target_gsrf(pl_python);
