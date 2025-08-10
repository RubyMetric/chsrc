/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(wr_guix);

void
wr_guix_prelude ()
{
  use_this(wr_guix);
  chef_allow_s(wr_guix);

  chef_set_created_on   (this, "2023-09-11");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note (this, "目前只有一个源, guixcn 的源不知道是否可用",
                       "Currently only one source available, guixcn source availability unknown");

  def_sources_begin()
  {&upstream,        "https://git.savannah.gnu.org/git/guix.git", DelegateToUpstream},
  {&Sjtug_Zhiyuan,   "https://mirror.sjtu.edu.cn/git/guix.git", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://mirrors.sjtug.sjtu.edu.cn/docs/guix
 */
void
wr_guix_setsrc (char *option)
{
  use_this_source(wr_guix);

  char *file =  xy_strjoin (3, "(list (channel\n"
                               "       (inherit (car %default-channels))\n"
                               "       (url \"", source.url, "\")))");

  chsrc_note2 ("为防止扰乱配置文件，请手动写入以下内容到 ~/.config/guix/channels.scm 文件中");
  p(file);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}
