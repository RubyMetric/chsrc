/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_haskell, "haskell/cabal/stack/hackage");

void
pl_haskell_prelude ()
{
  use_this(pl_haskell);
  chef_allow_s(pl_haskell);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-10");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, NULL, DelegateToUpstream},
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
  use_this_source(pl_haskell);

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
  println (content);

  config = xy_normalize_path ("~/.stack/config.yaml");
  content = xy_str_gsub (RAWSTR_pl_haskell_stackage_yaml, "@url@", source.url);
  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  println (content);

  chsrc_conclude (&source);
}
