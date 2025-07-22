/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2025-07-22>
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

/**
 * @update 2023-09-10
 */
static Source_t pl_haskell_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/hackage", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/hackage",          DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/hackage",             DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/hackage",          DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/hackage",           DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/hackage",          DelegateToMirror}
};
def_sources_n(pl_haskell);


/**
 * @consult https://help.mirrors.cernet.edu.cn/hackage/
 */
void
pl_haskell_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_haskell);

  char *content = xy_str_gsub (RAWSTR_pl_haskell_cabal_config, "@url@", source.url);

  char *config = NULL;
  if (xy_on_windows)
    {
      config = xy_normalize_path ("~/AppData/Roaming/cabal/config");
    }
  else
    {
      config = "~/.cabal/config";
    }

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  print (content);

  config = xy_normalize_path ("~/.stack/config.yaml");
  content = xy_str_gsub (RAWSTR_pl_haskell_stackage_yaml, "@url@", source.url);
  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  print (content);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


Feature_t
pl_haskell_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NA;
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_sf (pl_haskell);
