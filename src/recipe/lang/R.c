/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_r, "r/cran");

void
pl_r_prelude ()
{
  use_this(pl_r);
  chef_allow_gs(pl_r);

  chef_set_created_on   (this, "2023-09-21");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-08-21");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 1, "@hezonglun");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  // 以下注释的，是不含有bioconductor的镜像站，我们在换cran的同时，也直接帮助用户换bioconductor
  def_sources_begin()
  {&UpstreamProvider, "https://cran.r-project.org/", DelegateToUpstream},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/cran/",    DelegateToMirror},
  // {&Ali,           "https://mirrors.aliyun.com/CRAN/",           DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/CRAN/", DelegateToMirror},
  // {&Sustech,       "https://mirrors.sustech.edu.cn/CRAN",        DelegateToMirror},
  // {&Bfsu,          "https://mirrors.bfsu.edu.cn/CRAN/",          DelegateToMirror},
  // {&Bjtu,          "https://mirror.bjtu.edu.cn/cran/",           DelegateToMirror},
  def_sources_end()
}


#define PL_R_Config_Windows "~/Documents/.Rprofile"
#define PL_R_Config_POSIX   "~/.Rprofile"

void
pl_r_getsrc (char *option)
{
  /**
   * 或参考：https://zhuanlan.zhihu.com/p/585036231
   *
   * options()$repos
   * options()$BioC_mirror
   */
  if (xy_on_windows)
    {
      chsrc_view_file (PL_R_Config_Windows);
    }
  else
    {
      chsrc_view_file (PL_R_Config_POSIX);
    }
}

/**
 * @consult https://help.mirrors.cernet.edu.cn/CRAN/
 */
void
pl_r_setsrc (char *option)
{
  use_this_source(pl_r);

  char *bioconductor_url = xy_str_delete_suffix (xy_str_delete_suffix (source.url, "cran/"), "CRAN/");
  bioconductor_url = xy_2strcat(bioconductor_url, "bioconductor");

  const char *w1 = xy_strcat (3, "options(\"repos\" = c(CRAN=\"", source.url, "\"))\n" );
  const char *w2 = xy_strcat (3, "options(BioC_mirror=\"", bioconductor_url, "\")\n" );

  char *w = xy_2strcat (w1, w2);

  // 或者我们调用 r.exe --slave -e 上面的内容

  char *config = xy_on_windows ? PL_R_Config_Windows : PL_R_Config_POSIX;

  chsrc_append_to_file (w, config);

  chsrc_conclude (&source);
}
