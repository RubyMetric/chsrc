/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-24>
 * Last Modified : <2024-08-16>
 *
 * Anolis OS 为这个操作系统的名字，OpenAnolis(龙蜥社区) 只是社区名
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-12 更新
 */
static Source_t
os_anolis_sources[] = {
  {&UpstreamProvider,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/anolis"},
  {&Hust,           "https://mirrors.hust.edu.cn/anolis"}
};
def_sources_n(os_anolis);


/**
 * 参考: https://mirrors.hust.edu.cn/docs/anolis
 */
void
os_anolis_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_anolis);

  char *cmd = xy_strjoin (3, "sed -i.bak -E 's|https?://(mirrors\\.openanolis\\.cn/anolis)|", source.url, "|g' /etc/yum.repos.d/*.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("dnf makecache", RunOpt_Default);
  chsrc_run ("dnf update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}

def_target_s(os_anolis);
