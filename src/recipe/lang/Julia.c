/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-31>
 * Last Modified : <2024-11-22>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-05 更新
 * @note 缺少商业公司或开源社区软件源
 */
static SourceInfo
pl_julia_sources[] = {
  {&UpstreamProvider,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/julia"},
  {&Pku,           "https://mirrors.pku.edu.cn/julia"},
  {&Nju,           "https://mirror.nju.edu.cn/julia"}
};
def_sources_n(pl_julia);

#define PL_Julia_Config "~/.julia/config/startup.jl"

/**
 * Julia的换源可以通过两种方式
 * 1. 写入 startup.jl
 * 2. 使用环境变量
 *
 * 我们采用第一种
 */
void
pl_julia_getsrc (char *option)
{
  chsrc_view_file (PL_Julia_Config);
}

/**
 * @consult
 *  1. https://help.mirrors.cernet.edu.cn/julia/
 *  2. https://docs.julialang.org/en/v1/manual/command-line-interface/#Startup-file
 */
void
pl_julia_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_julia);

  char *w = xy_strjoin (3, "ENV[\"JULIA_PKG_SERVER\"] = \"", source.url, "\"");

  chsrc_append_to_file (w, PL_Julia_Config);

  chsrc_conclude (&source, SetsrcType_Untested);
}


FeatInfo
pl_julia_feat (char *option)
{
  FeatInfo f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NA;
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsf(pl_julia);
