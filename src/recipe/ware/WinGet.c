/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-06-07>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-07 更新
 * @note 目前仅有一个源
 */
static SourceInfo
wr_winget_sources[] = {
  {&Upstream,       "https://cdn.winget.microsoft.com/cache"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/winget-source"},
};
def_sources_n(wr_winget);


void
wr_winget_getsrc (char *option)
{
  chsrc_run ("winget source list", RunOpt_Default);
}

/* 参考：https://mirrors.ustc.edu.cn/help/winget-source.html */
void
wr_winget_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_winget);
  chsrc_confirm_source (&source);

  chsrc_run ("winget source remove winget", RunOpt_Default);
  chsrc_run (xy_2strjoin ("winget source add winget ", source.url), RunOpt_Default);

  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

void
wr_winget_resetsrc (char *option)
{
  chsrc_run ("winget source reset winget", RunOpt_Default);
  chsrc_say_lastly (NULL, ChsrcTypeAuto);
}

def_target_gsr(wr_winget);
