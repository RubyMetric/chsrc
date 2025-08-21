/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_clojure, "clojure/clojars/cloj/lein");

void
pl_clojure_prelude ()
{
  use_this(pl_clojure);
  chef_allow_s(pl_clojure);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,"https://repo.clojars.org/", DelegateToUpstream},
  {&MirrorZ,         "https://mirrors.cernet.edu.cn/clojars/",        DelegateToMirror},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/clojars/", DelegateToMirror},
  {&Nju,             "https://mirror.nju.edu.cn/clojars/",             DelegateToMirror},
  {&Nyist,           "https://mirror.nyist.edu.cn/clojars/",          DelegateToMirror},
  {&Ustc,            "https://mirrors.ustc.edu.cn/clojars/",          DelegateToMirror},
  {&Iscas,           "https://mirror.iscas.ac.cn/clojars/",           DelegateToMirror}
  def_sources_end()
}


void
pl_clojure_setsrc (char *option)
{
  use_this_source(pl_clojure);

  if (chsrc_in_local_mode())
    {
      chsrc_note2 ("请在项目根目录中的 project.clj 中手动添加 :mirrors 关键字:");
      char *config = xy_str_gsub (RAWSTR_pl_clojure_project_clj, "@url@", source.url);
      println (config);
    }
  else
    {
      chsrc_note2 ("请在 ~/.lein/projfiles.clj 中手动添加 :mirrors 关键字:");
      char *config = xy_str_gsub (RAWSTR_pl_clojure_projfiles_clj, "@url@", source.url);
      println (config);
    }

  chsrc_conclude (&source);
}
