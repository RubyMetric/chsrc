/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Yangmoooo <yangmoooo@outlook.com>
 *               |  Happy Game <happygame1024@gmail.com>
 *               |
 * Created On    : <2023-09-29>
 * Last Modified : <2025-01-03>
 *
 * Kali Linux 基于 Debian Testing branch
 * ------------------------------------------------------------*/

static SourceProvider_t os_kali_upstream =
{
  def_upstream, "http://http.kali.org/kali",
  // https://github.com/RubyMetric/chsrc/issues/121
  {NotSkip, NA, NA, "https://http.kali.org/kali/dists/kali-dev/Contents-amd64.gz"} // 47MB
};

/**
 * @update 2025-01-03
 */
static Source_t os_kali_sources[] =
{
  {&os_kali_upstream, "http://http.kali.org/kali"},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/kali"},
  {&Ali,              "https://mirrors.aliyun.com/kali"},
  {&Volcengine,       "https://mirrors.volces.com/kali"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/kali"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/kali"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/kali"},
  // {&Tencent,          "https://mirrors.tencent.com/kali"}, // 无kali源
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/kali"},
  // {&Huawei,           "https://mirrors.huaweicloud.com/kali"}, // 未与上游同步
  // {&Netease,          "https://mirrors.163.com/kali"}, // 无kali源
  // {&Sohu,             "https://mirrors.sohu.com/kali"} //无kali源
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

def_target(os_kali);
