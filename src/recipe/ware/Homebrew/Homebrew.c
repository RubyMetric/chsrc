/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

def_target(wr_homebrew, "brew/homebrew");

void
wr_homebrew_prelude ()
{
  chef_prep_this (wr_homebrew, gs);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-09-12");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Word2VecT");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_forbid_user_define(this);

  chef_set_note (this, "该换源通过写入环境变量实现，若多次换源，请手动清理profile文件",
                       "This source switching is implemented by writing environment variables. If switching sources multiple times, please manually clean the profile file");

  def_sources_begin()
  {&UpstreamProvider, "https://github.com/Homebrew/brew.git", DelegateToUpstream},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/", DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/", DelegateToMirror}
  def_sources_end()
}


void
wr_homebrew_getsrc (char *option)
{
  int status = xy_run_get_status (RAWSTR_wr_homebrew_read_config_cmd);
  if (status != 0) { xy_noop(); }
}

/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/homebrew/
 *
 * 自brew 4.0.0 (2023 年 2 月 16日) 起，
 * HOMEBREW_INSTALL_FROM_API 会成为默认行为，无需设置。大部分用户无需再克隆 homebrew-core 仓库，故无需设置 HOMEBREW_CORE_GIT_REMOTE 环境变量；
 * 但是为了以防万一，我们还是为用户设置该环境变量
 */
void
wr_homebrew_setsrc (char *option)
{
  chsrc_use_this_source (wr_homebrew);

  char *w = xy_str_gsub (RAWSTR_wr_homebrew_config_in_bash, "@1@", source.url);

  char *zshrc = xy_zshrc;
  chsrc_backup (zshrc);
  chsrc_append_to_file (w, zshrc);

  char *bashrc = xy_bashrc;
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *fishrc = xy_fishrc;
  if (xy_file_exist (fishrc))
    {
      char *w = xy_str_gsub (RAWSTR_wr_homebrew_config_in_fish, "@1@", source.url);
      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_alert2 ("请重启终端使Homebrew环境变量生效");
}
