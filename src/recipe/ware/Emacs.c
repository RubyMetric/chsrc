/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-10-10>
 * Last Modified : <2025-07-13>
 * ------------------------------------------------------------*/

static MirrorSite_t EmacsChina =
{
  IS_DedicatedMirrorSite,
  "emacschina", "EmacsChina", "Emacs China 社区", "https://elpamirror.emacs-china.org/",
  {SKIP, ToFill, ToFill, NULL, ROUGH}
};

/**
 * @update 2025-07-13
 * @note Emacs用户往往只需要一次性换源，只会极少次调用 chsrc，我们只给用户提供文档
 */
Source_t wr_emacs_sources[] =
{
  {&UpstreamProvider,    NULL, NULL},
  {&MirrorZ,            "https://help.mirrors.cernet.edu.cn/elpa/"},
  {&Sjtug_Zhiyuan,      "https://mirrors.sjtug.sjtu.edu.cn/docs/emacs-elpa"},
  {&Tuna,               "https://mirrors.tuna.tsinghua.edu.cn/help/elpa/"},
  {&Bfsu,               "https://mirrors.bfsu.edu.cn/help/elpa/"},
  {&Ustc,               "https://mirrors.ustc.edu.cn/help/elpa.html"},
  {&Zju,                "https://mirrors.zju.edu.cn/docs/elpa/"},
  {&EmacsChina,         "https://elpamirror.emacs-china.org/"}
};
def_sources_n(wr_emacs);


void
wr_emacs_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_emacs);

  chsrc_note2 ("Emacs换源涉及Elisp，需要手动查阅并换源:");
  p(source.url);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


Feature_t
wr_emacs_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.can_english = false;
  f.can_user_define = false;

  return f;
}

def_target_sf(wr_emacs);
