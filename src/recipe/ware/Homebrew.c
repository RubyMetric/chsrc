/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nil Null  <nil@null.org>
 * Created On     : <2023-09-10>
 * Major Revision :      3
 * Last Modified  : <2024-08-28>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-10
 * @note {
 *   1. 这些链接将会在setsrc函数中补充完整
 *   2. 不确定 Sustech 能否工作
 * }
 */
static SourceInfo
wr_homebrew_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/"},
  {&Zju,            "https://mirrors.zju.edu.cn/"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/"}
};
def_sources_n(wr_homebrew);


void
wr_homebrew_getsrc (char *option)
{
  char *cmd = "echo HOMEBREW_API_DOMAIN=$HOMEBREW_API_DOMAIN;"
              "echo HOMEBREW_BOTTLE_DOMAIN=$HOMEBREW_BOTTLE_DOMAIN;"
              "echo HOMEBREW_BREW_GIT_REMOTE=$HOMEBREW_BREW_GIT_REMOTE;"
              "echo HOMEBREW_CORE_GIT_REMOTE=$HOMEBREW_CORE_GIT_REMOTE;";
  system (cmd);
}

/**
 * 参考自: https://mirrors.tuna.tsinghua.edu.cn/help/homebrew/
 *
 * 自brew 4.0.0 (2023 年 2 月 16日) 起，
 * HOMEBREW_INSTALL_FROM_API 会成为默认行为，无需设置。大部分用户无需再克隆 homebrew-core 仓库，故无需设置 HOMEBREW_CORE_GIT_REMOTE 环境变量；
 * 但是为了以防万一，我们还是为用户设置该环境变量
 */
void
wr_homebrew_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_homebrew);

  char *splitter = "\n# --- chsrc BLOCK BEGIN for Homebrew ---\n";
  char *w1 = xy_strjoin (4, "export HOMEBREW_API_DOMAIN=\"",      source.url, "homebrew-bottles/api",  "\"\n");
  char *w2 = xy_strjoin (4, "export HOMEBREW_BOTTLE_DOMAIN=\"",   source.url, "homebrew-bottles",      "\"\n");
  char *w3 = xy_strjoin (4, "export HOMEBREW_BREW_GIT_REMOTE=\"", source.url, "git/homebrew/brew.git", "\"\n");
  char *w4 = xy_strjoin (4, "export HOMEBREW_CORE_GIT_REMOTE=\"", source.url, "git/homebrew/homebrew-core.git", "\"\n");
  char *end = "# --- chsrc BLOCK ENDIN for Homebrew ---\n";

  char *w = xy_strjoin (6, splitter, w1, w2, w3, w4, end);

  char *zshrc = "~/.zshrc";
  chsrc_backup (zshrc);
  chsrc_append_to_file (w, zshrc);

  char *bashrc = "~/.bashrc";
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *fishrc = "~/.config/fish/config.fish";
  if (xy_file_exist (fishrc))
    {
      char *w1 = xy_strjoin(4, "set -x HOMEBREW_API_DOMAIN \"",     source.url, "homebrew-bottles/api",  "\"\n");
      char *w2 = xy_strjoin(4, "set -x HOMEBREW_BOTTLE_DOMAIN \"",  source.url, "homebrew-bottles",      "\"\n");
      char *w3 = xy_strjoin(4, "set -x HOMEBREW_BREW_GIT_REMOTE \"",source.url, "git/homebrew/brew.git", "\"\n");
      char *w4 = xy_strjoin(4, "set -x HOMEBREW_CORE_GIT_REMOTE \"",source.url, "git/homebrew/homebrew-core.git", "\"\n");

      char *w = xy_strjoin (6, splitter, w1, w2, w3, w4, end);
      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }

  chsrc_conclude(&source, SetsrcType_Auto);
  chsrc_note2 ("请您重启终端使Homebrew环境变量生效");
}


FeatInfo
wr_homebrew_feat (char *option)
{
  FeatInfo f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;

  /* 该换源方案中，URL存在拼凑，因此不能让用户手动使用某URL来换源 */
  f.can_user_define = false;

  f.note = "该换源通过写入环境变量实现，若多次换源，请手动清理profile文件";
  return f;
}


def_target_gsf(wr_homebrew);
