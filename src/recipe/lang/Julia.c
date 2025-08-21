/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_julia, "julia");

void
pl_julia_prelude ()
{
  use_this(pl_julia);
  chef_allow_gs(pl_julia);

  chef_set_created_on   (this, "2023-08-31");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,  NULL, DelegateToUpstream},
  {&Pku,              "https://mirrors.pku.edu.cn/julia", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/julia",   DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/julia",  DelegateToMirror}
  def_sources_end()
}

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
  use_this_source(pl_julia);

  char *w = xy_strcat (3, "ENV[\"JULIA_PKG_SERVER\"] = \"", source.url, "\"");

  chsrc_append_to_file (w, PL_Julia_Config);

  chsrc_conclude (&source);
}
