/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Heng Guo <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 * Created On    : <2023-09-05>
 * Last Modified : <2025-06-20>
 * ------------------------------------------------------------*/

/**
 * @update 2025-06-20
 */
static Source_t os_gentoo_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Ali,              "mirrors.aliyun.com"},
  {&Bfsu,             "mirrors.bfsu.edu.cn"},
  {&Ustc,             "mirrors.ustc.edu.cn"},
  {&Tuna,             "mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,          "mirrors.tencent.com"},
  // {&Tencent_Intra, "mirrors.tencentyun.com"},
  // {&Netease,          "mirrors.163.com"}, /* 不启用原因：过慢 */
  // {&Sohu,             "mirrors.sohu.com"} /* 不启用原因：过慢 */
};
def_sources_n(os_gentoo);


/**
 * HELP: 未经测试
 */
void
os_gentoo_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_gentoo);

  chsrc_backup ("/etc/portage/repos.conf/gentoo.conf");

  char *cmd = xy_strjoin (3, "sed -i \"s#rsync://.*/gentoo-portage#rsync://",
                             source.url,
                            "gentoo-portage#g");
  chsrc_run (cmd, RunOpt_Default);

  char *w = xy_strjoin (3, "GENTOO_MIRRORS=\"https://", source.url, "gentoo\"\n");

  chsrc_append_to_file (w, "/etc/portage/make.conf");

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}

def_target_s(os_gentoo);
