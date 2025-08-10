/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(wr_nix);

void
wr_nix_prelude ()
{
  use_this(wr_nix);

  chef_set_created_on   (this, "2023-09-26");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  // chef_allow_get();
  chef_allow_set();
  // chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&upstream,        "https://channels.nixos.org/", DelegateToUpstream},
  {&MirrorZ,         "https://mirrors.cernet.edu.cn/nix-channels/", DelegateToMirror},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/nix-channels/", DelegateToMirror},
  {&Nju,             "https://mirror.nju.edu.cn/nix-channels/", DelegateToMirror},
  {&Ustc,            "https://mirrors.ustc.edu.cn/nix-channels/", DelegateToMirror},
  {&Sjtug_Siyuan,    "https://mirror.sjtu.edu.cn/nix-channels/", DelegateToMirror},
  {&Bfsu,            "https://mirrors.bfsu.edu.cn/nix-channels/", DelegateToMirror},
  {&Iscas,           "https://mirror.iscas.ac.cn/nix-channels/", DelegateToMirror}
  def_sources_end()
}


void
wr_nix_check_cmd ()
{
  chsrc_ensure_program ("nix-channel");
}

/**
 * @consult
 *    1. https://help.mirrors.cernet.edu.cn/nix-channels/
 *    2. https://gitee.com/RubyMetric/chsrc/issues/I83894
 */
void
wr_nix_setsrc (char *option)
{
  wr_nix_check_cmd ();

  use_this(wr_nix);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  char *cmd = xy_strjoin (3, "nix-channel --add ", source.url, "nixpkgs-unstable nixpkgs");
  chsrc_run (cmd, RunOpt_Default);

  char *w = xy_strjoin (3, "substituters = ", source.url, "store https://cache.nixos.org/");
  chsrc_append_to_file (w, "~/.config/nix/nix.conf");

  chsrc_run ("nix-channel --update", RunOpt_Default);

  chsrc_note2 ("若你使用的是NixOS，请确认你的系统版本<version>（如22.11），并手动运行:");
  cmd = xy_strjoin (3, "nix-channel --add ", source.url, "nixpkgs-<version> nixpkgs");
  p(cmd);

  cmd = xy_strjoin (3, "nix.settings.substituters = [ \"", source.url, "store\" ];");
  chsrc_note2 ("若你使用的是NixOS，请额外添加下述内容至 configuration.nix 中");
  p(cmd);

  chsrc_determine_chgtype (ChgType_SemiAuto);
  chsrc_conclude (&source);
}
