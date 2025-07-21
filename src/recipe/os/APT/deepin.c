/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Yangmoooo <yangmoooo@outlook.com>
 * Created On    : <2023-09-26>
 * Last Modified : <2025-07-21>
 *
 * 名称为小写deepin，而非Deepin
 * ------------------------------------------------------------*/

/**
 * @update 2024-09-14
 */
static Source_t os_deepin_sources[] =
{
  {&UpstreamProvider, "https://community-packages.deepin.com/deepin", DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/deepin",            DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/deepin",          DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/deepin",          DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/deepin",  DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/deepin",          DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/deepin",       DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/deepin",           DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/deepin",          DelegateToMirror}
};
def_sources_n(os_deepin);


void
os_deepin_getsrc(char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

/**
 * HELP: 未经测试
 */
void
os_deepin_setsrc (char *option)
{
  chsrc_ensure_root();

  chsrc_yield_source_and_confirm (os_deepin);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/deepin/?@",
                              source.url,
                              "@g\' " OS_Apt_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_deepin_resetsrc (char *option)
{
  os_deepin_setsrc (option);
}


Feature_t
os_deepin_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;

  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(os_deepin);
