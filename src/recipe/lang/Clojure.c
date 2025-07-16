/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2025-07-16>
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

/**
 * @update 2025-07-14
 */
static Source_t pl_clojure_sources[] =
{
  {&UpstreamProvider, NULL, NULL},
  {&MirrorZ,         "https://mirrors.cernet.edu.cn/clojars/",        DelegateToMirror},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/clojars/", DelegateToMirror},
  {&Nju,             "https://mirror.nju.edu.cn/clojars/",             DelegateToMirror},
  {&Nyist,           "https://mirror.nyist.edu.cn/clojars/",          DelegateToMirror},
  {&Ustc,            "https://mirrors.ustc.edu.cn/clojars/",          DelegateToMirror},
  {&Iscas,           "https://mirror.iscas.ac.cn/clojars/",           DelegateToMirror}
};
def_sources_n(pl_clojure);


void
pl_clojure_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_clojure);

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

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


/**
 * chsrc ls clojure
 */
Feature_t
pl_clojure_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = false;

  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_sf(pl_clojure);
