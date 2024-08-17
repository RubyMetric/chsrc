/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Heng Guo <2085471348@qq.com>
 * Contributors  : Nil Null <nil@null.org>
 * Created On    : <2023-09-05>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-05 更新
 * @note 源并不完整，且未经测试是否有效
 */
static SourceInfo
os_gentoo_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "mirrors.aliyun.com"},
  {&Bfsu,          "mirrors.bfsu.edu.cn"},
  {&Ustc,          "mirrors.ustc.edu.cn"},
  {&Tuna,          "mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,       "mirrors.tencent.com"},
  {&Netease,       "mirrors.163.com"},
  {&Sohu,          "mirrors.sohu.com"}
};
def_sources_n(os_gentoo);


/**
 * HELP: 未经测试
 */
void
os_gentoo_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_gentoo);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/portage/repos.conf/gentoo.conf");

  char *cmd = xy_strjoin (3, "sed -i \"s#rsync://.*/gentoo-portage#rsync://",
                             source.url,
                            "gentoo-portage#g");
  chsrc_run (cmd, RunOpt_Default);

  char *towrite = xy_strjoin (3, "GENTOO_MIRRORS=\"https://", source.url, "gentoo\"");

  chsrc_append_to_file (towrite, "/etc/portage/make.conf");
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

def_target_s(os_gentoo);
