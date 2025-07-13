/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-06-07>
 * Last Modified : <2025-07-13>
 * ------------------------------------------------------------*/

/**
 * @update 2025-07-13
 */
static Source_t wr_winget_sources[] =
{
  {&UpstreamProvider,   "https://cdn.winget.microsoft.com/cache",    NULL},
  {&MirrorZ,            "https://mirrors.cernet.edu.cn/winget-source", NULL},
  {&Ustc,               "https://mirrors.ustc.edu.cn/winget-source", NULL},
  {&Nju,                "https://mirror.nju.edu.cn/winget-source",   NULL},
  {&Nyist,              "https://mirror.nyist.edu.cn/winget-source", NULL},
  {&Xjtu,               "https://mirrors.xjtu.edu.cn/winget-source", NULL}
};
def_sources_n(wr_winget);


void
wr_winget_getsrc (char *option)
{
  chsrc_run ("winget source list", RunOpt_Default);
}


/**
 * @consult https://mirrors.ustc.edu.cn/help/winget-source.html
 */
void
wr_winget_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_winget);

  chsrc_run ("winget source remove winget", RunOpt_Default);
  chsrc_run (xy_2strjoin ("winget source add winget ", source.url), RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
wr_winget_resetsrc (char *option)
{
  chsrc_run ("winget source reset winget", RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}


Feature_t
wr_winget_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.can_english = false;
  f.can_user_define = true;

  return f;
}

def_target_gsrf(wr_winget);
