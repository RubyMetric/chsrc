/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_haskell, "haskell/cabal/stack/hackage");

void
pl_haskell_prepare ()
{
  chef_prep_this_dish (pl_haskell, s);

  chef_set_recipe_created_on   (this, "2023-09-10");
  chef_set_recipe_last_updated (this, "2026-11-08");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@hezonglun", "@Mikachu2333");


  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unknown);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unknown);
  chef_set_default_scope (this, UserScope);

  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://hackage.haskell.org", DelegateToUpstream},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/hackage", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/hackage",          DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/hackage",             DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/hackage",          DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/hackage",           DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/hackage",          DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/hackage/
 */
void
pl_haskell_setsrc (char *option)
{
  chsrc_use_this_source (pl_haskell);

  char *content = xy_str_gsub (RAWSTR_pl_haskell_cabal_config, "@url@", source.url);

  char *config = NULL;
  if (xy.on_windows)
    {
      config = xy_normalize_path ("~/AppData/Roaming/cabal/config");
    }
  else
    {
      config = "~/.cabal/config";
    }

  chsrc_note2 (xy_strcat (3, "请向 ", config, " 中手动添加:"));
  println (content);

  // 2026.11.08
  // 根据 https://docs.haskellstack.org/en/stable/topics/stack_root/#__tabbed_1_2 的说明，win的配置文件与linux不同
  if (xy.on_windows)
    config = xy_normalize_path ("~/AppData/Roaming/stack/config.yaml");
  else
    config = "~/.stack/config.yaml";

  content = xy_str_gsub (RAWSTR_pl_haskell_stackage_yaml, "@url@", source.url);
  chsrc_note2 (xy_strcat (3, "请向 ", config, " 中手动添加:"));
  println (content);

  chsrc_conclude (&source);
}
