/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static MirrorSite_t EmacsChina =
{
  IS_DedicatedMirrorSite,
  "emacschina", "EmacsChina", "Emacs China 社区", "https://elpamirror.emacs-china.org/",
  {SKIP, ToFill, ToFill, NULL, ROUGH}
};

def_target(wr_emacs, "emacs/elpa");

void
wr_emacs_prelude ()
{
  use_this(wr_emacs);
  chef_allow_s(wr_emacs);

  chef_set_created_on   (this, "2023-10-10");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-13");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note (this, "Emacs用户往往只需要一次性换源，只会极少次调用 chsrc，我们只给用户提供文档",
                       "Emacs users typically only need to switch sources once and rarely call chsrc, so we only provide documentation to users");

  def_sources_begin()
  {&UpstreamProvider, "https://elpa.gnu.org/", DelegateToUpstream},
  {&MirrorZ,          "https://help.mirrors.cernet.edu.cn/elpa/", DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/docs/emacs-elpa", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/help/elpa/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/help/elpa/", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/help/elpa.html", DelegateToMirror},
  {&Zju,              "https://mirrors.zju.edu.cn/docs/elpa/", DelegateToMirror},
  {&EmacsChina,       "https://elpamirror.emacs-china.org/", DelegateToMirror}
  def_sources_end()
}


void
wr_emacs_setsrc (char *option)
{
  use_this_source(wr_emacs);

  chsrc_note2 ("Emacs换源涉及Elisp, 需要手动查阅并换源:");
  p (source.url);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}
