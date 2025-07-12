/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2025-03-04>
 * Last Modified : <2025-03-04>
 *
 * 该 recipe 存在对应的 bootstrapper
 * ------------------------------------------------------------*/

/**
 * @update 2025-03-04
 */
static Source_t
os_termux_sources[] = {
  {&UpstreamProvider, NULL},
  {&Ustc,          "https://mirrors.ustc.edu.cn/termux/", NULL},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/termux/",   NULL},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/termux/", NULL},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/termux/",  NULL},
  {&Pku,           "https://mirrors.pku.edu.cn/termux/",   NULL},
  {&Nyist,         "https://mirror.nyist.edu.cn/termux/",  NULL},
  {&Nju,           "https://mirror.nju.edu.cn/termux/",    NULL},
  {&Sustech,       "https://mirrors.sustech.edu.cn/termux/", NULL},
  {&Iscas,         "https://mirror.iscas.ac.cn/termux/",   NULL},
  {&Zju,           "https://mirrors.zju.edu.cn/termux/",   NULL},
  {&Sdu,           "https://mirrors.sdu.edu.cn/termux/",   NULL},
  {&Cqupt,         "https://mirrors.cqupt.edu.cn/termux/", NULL}

};
def_sources_n(os_termux);


/**
 * chsrc get termux
 */
void
os_termux_getsrc (char *option)
{
  chsrc_view_file (OS_Termux_SourceList);
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/termux/
 *
 * chsrc set termux
 */
void
os_termux_setsrc (char *option)
{

  // chsrc_ensure_root (); Termux下禁止使用root

  chsrc_yield_source_and_confirm (os_termux);

  char *cmd = xy_strjoin (3, "sed -i 's@^\\(deb.*stable main\\)$@#\\1\\ndeb ",
                              source.url, "apt/termux-main stable main@' " OS_Termux_SourceList);

  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update",  RunOpt_Default);
  chsrc_run ("apt upgrade", RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * chsrc reset termux
 *
 * 暂时缺乏原地址
 */
void
os_termux_resetsrc (char *option)
{
  os_termux_setsrc (option);
}


/**
 * chsrc ls termux
 */
Feature_t
os_termux_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;

  f.can_english = false;
  f.can_user_define = false;

  f.note = NULL;
  return f;
}


def_target_gsf(os_termux);
