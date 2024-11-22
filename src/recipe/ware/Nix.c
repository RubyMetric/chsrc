/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-26>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-22
 * @note {
 *   1. 目前只有一个源
 *   2. 这些链接将会在setsrc函数中补充完整
 * }
 */
static Source_t
wr_nix_sources[] = {
  {&UpstreamProvider,       NULL},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/nix-channels/"}
};
def_sources_n(wr_nix);


void
wr_nix_check_cmd ()
{
  chsrc_ensure_program ("nix-channel");
}

/**
 * 参考:
 *  1. https://mirrors.bfsu.edu.cn/help/nix-channels/
 *  2. https://gitee.com/RubyMetric/chsrc/issues/I83894
 */
void
wr_nix_setsrc (char *option)
{
  wr_nix_check_cmd ();

  chsrc_yield_source_and_confirm (wr_nix);

  char *cmd = xy_strjoin (3, "nix-channel --add ", source.url, "nixpkgs-unstable nixpkgs");
  chsrc_run (cmd, RunOpt_Default);

  char *w = xy_strjoin (3, "substituters = ", source.url, "store https://cache.nixos.org/");
  chsrc_append_to_file (w, "~/.config/nix/nix.conf");

  chsrc_run ("nix-channel --update", RunOpt_Default);

  chsrc_note2 ("若您使用的是NixOS，请确认您的系统版本<version>（如22.11），并手动运行:");
  cmd = xy_strjoin (3, "nix-channel --add ", source.url, "nixpkgs-<version> nixpkgs");
  say (cmd);

  cmd = xy_strjoin (3, "nix.settings.substituters = [ \"", source.url, "store\" ];");
  chsrc_note2 ("若您使用的是NixOS，请额外添加下述内容至 configuration.nix 中");
  say (cmd);

  ProgMode_ChgType = ChgType_SemiAuto;
  chsrc_conclude (&source);
}

def_target_s (wr_nix);
