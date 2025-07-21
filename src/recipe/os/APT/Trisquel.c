/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-29>
 * Last Modified : <2025-07-21>
 *
 * Trisquel基于Ubuntu开发，不含任何专有软件及专有固件，内核使用 Linux-libre
 * ------------------------------------------------------------*/

/**
 * @update 2024-11-21
 */
static Source_t os_trisquel_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/trisquel/", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/trisquel/",    DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/trisquel/",     DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/trisquel/",   DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/trisquel/",    DelegateToMirror}
};
def_sources_n(os_trisquel);


void
os_trisquel_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/trisquel/
 */
void
os_trisquel_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_trisquel);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/trisquel/?@", source.url, "@g' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


Feature_t
os_trisquel_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;

  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsf(os_trisquel);
