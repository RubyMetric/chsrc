/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Yangmoooo <yangmoooo@outlook.com>
 *               |  Happy Game <happygame1024@gmail.com>
 *               |
 * Created On    : <2023-09-29>
 * Last Modified : <2025-07-21>
 *
 * Kali Linux 基于 Debian Testing branch
 * ------------------------------------------------------------*/

static SourceProvider_t os_kali_upstream =
{
  def_upstream, "http://http.kali.org/kali",
  // https://github.com/RubyMetric/chsrc/issues/121
  {NotSkip, NA, NA, "https://http.kali.org/kali/dists/kali-dev/Contents-amd64.gz", ACCURATE} // 47MB
};

/**
 * @update 2025-06-20
 */
static Source_t os_kali_sources[] =
{
  {&os_kali_upstream, "http://http.kali.org/kali",          DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/kali", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/kali",    DelegateToMirror},
  {&Volcengine,       "https://mirrors.volces.com/kali",    DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/kali",   DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/kali",   DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/kali", DelegateToMirror},

  /* 不启用原因: 未与上游同步 */
  // {&Huawei,           "https://mirrors.huaweicloud.com/kali",   DelegateToMirror},
};
def_sources_n(os_kali);

void
os_kali_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

void
os_kali_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_kali);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/kali/?@",
                              source.url,
                             "@g\' " OS_Apt_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


void
os_kali_resetsrc (char *option)
{
  os_kali_setsrc (option);
}


Feature_t
os_kali_feat (char *option)
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

def_target_gsrf(os_kali);
