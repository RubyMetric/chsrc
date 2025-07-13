/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-26>
 * Last Modified : <2025-07-13>
 * ------------------------------------------------------------*/

/**
 * @update 2025-07-13
 *
 * @note 链接将会在setsrc函数中补充完整
 */
static Source_t wr_nix_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/nix-channels/", NULL},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/nix-channels/", NULL},
  {&Nju,              "https://mirror.nju.edu.cn/nix-channels/",   NULL},
  {&Ustc,             "https://mirrors.ustc.edu.cn/nix-channels/", NULL},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/nix-channels/",   NULL},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/nix-channels/", NULL},
  {&Iscas,            "https://mirror.iscas.ac.cn/nix-channels/",  NULL}
};
def_sources_n(wr_nix);


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

  chsrc_yield_source_and_confirm (wr_nix);

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

def_target_s (wr_nix);
