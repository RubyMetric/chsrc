/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(wr_nix, "nix");

/**
 * 运行时检查到底是哪种安装方式
 *
 * https://github.com/RubyMetric/chsrc/issues/337
 */
bool
wr_nix_is_multi_user_installation_mode ()
{
  if (xy_file_exist ("/nix/var/nix/daemon-socket/socket"))
    {
     return true;
    }
  else
    {
      return false;
    }
}

void
wr_nix_prelude ()
{
  chef_prep_this (wr_nix, s);

  chef_set_recipe_created_on   (this, "2023-09-26");
  chef_set_recipe_last_updated (this, "2026-03-25");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_And_Implemented);

  if (wr_nix_is_multi_user_installation_mode ())
    {
      chef_set_default_scope (this, SystemScope);
    }
  else
    {
      chef_set_default_scope (this, UserScope);
    }

  chef_deny_english(this);
  chef_deny_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://channels.nixos.org/", DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/nix-channels/", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/nix-channels/", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/nix-channels/", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/nix-channels/", DelegateToMirror},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/nix-channels/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/nix-channels/", DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/nix-channels/", DelegateToMirror}
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
 *    3. https://github.com/RubyMetric/chsrc/issues/337
 */
void
wr_nix_setsrc (char *option)
{
  wr_nix_check_cmd ();

  chsrc_use_this_source (wr_nix);

  char *user_scope_nix_config = "~/.config/nix/nix.conf";
  char *system_scope_nix_config = "/etc/nix/nix.conf";

  char *config_file = NULL;

  if (wr_nix_is_multi_user_installation_mode ())
    {
      config_file = system_scope_nix_config;
      // https://github.com/RubyMetric/chsrc/issues/337#issuecomment-3979352975
      chsrc_ensure_root ();
    }
  else
    {
      config_file = user_scope_nix_config;
    }

  char *cmd = xy_strcat (3, "nix-channel --add ", source.url, "nixpkgs-unstable nixpkgs");
  chsrc_run (cmd, RunOpt_Default);

  char *w = xy_strcat (3, "substituters = ", source.url, "store https://cache.nixos.org/");
  chsrc_append_to_file (w, config_file);

  chsrc_run ("nix-channel --update", RunOpt_Default);

  chsrc_note2 ("若你使用的是NixOS，请确认你的系统版本<version>（如22.11），并手动运行:");
  cmd = xy_strcat (3, "nix-channel --add ", source.url, "nixpkgs-<version> nixpkgs");
  p(cmd);

  cmd = xy_strcat (3, "nix.settings.substituters = [ \"", source.url, "store\" ];");
  chsrc_note2 ("若你使用的是NixOS，请额外添加下述内容至 configuration.nix 中");
  p(cmd);

  chsrc_determine_chgtype (ChgType_SemiAuto);
  chsrc_conclude (&source);
}
