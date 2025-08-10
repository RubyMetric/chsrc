/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(wr_cocoapods);

void
wr_cocoapods_prelude ()
{
  use_this(wr_cocoapods);

  chef_set_created_on   (this, "2024-06-08");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nil Null", "nil@null.org");

  // chef_allow_get();
  chef_allow_set();
  // chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_upstream("https://github.com/CocoaPods/Specs.git");
  def_sources_begin()
  {&upstream, "https://github.com/CocoaPods/Specs.git", DelegateToUpstream},
  {&Tuna,     "https://mirrors.tuna.tsinghua.edu.cn/git/CocoaPods/Specs.git", DelegateToMirror},
  {&Bfsu,     "https://mirrors.bfsu.edu.cn/git/CocoaPods/Specs.git", DelegateToMirror},
  {&Nju,      "https://mirror.nju.edu.cn/git/CocoaPods/Specs.git", DelegateToMirror},
  {&Nyist,    "https://mirror.nyist.edu.cn/git/CocoaPods/Specs.git", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/CocoaPods/
 */
void
wr_cocoapods_setsrc (char *option)
{
  use_this(wr_cocoapods);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  chsrc_note2 ("请手动执行以下命令:");
  p("cd ~/.cocoapods/repos");
  p("pod repo remove master");
  char *git_cmd = xy_strjoin (3, "git clone ", source.url, " master");
  p(git_cmd);
  br();

  chsrc_note2 ("最后进入项目工程目录，在Podfile中第一行加入:");
  char *source_str = xy_strjoin (3, "source '", source.url, "'");
  p(source_str);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}
