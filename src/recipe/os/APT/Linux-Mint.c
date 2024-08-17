/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-29>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-29 更新
 * @note 实际上镜像站里的内容和Ubuntu的不太一样
 */
static SourceInfo
os_linuxmint_sources[] = {
  {&Upstream,       NULL},
  {&MirrorZ,       "https://mirrors.cernet.edu.cn/linuxmint/"},
  {&Ali,           "http://mirrors.aliyun.com/linuxmint-packages/"},
  {&Netease,       "https://mirrors.163.com/linuxmint/packages/"},
};
def_sources_n(os_linuxmint);


void
os_linuxmint_getsrc (char *option)
{
  chsrc_view_file (OS_LinuxMint_SourceList);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/linuxmint/
 */
void
os_linuxmint_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_linuxmint);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_LinuxMint_SourceList);

  char* cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url,
                            "@g' " OS_LinuxMint_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
  chsrc_warn2 ("完成后请不要再使用 mintsources（自带的图形化软件源设置工具）进行任何操作，因为在操作后，无论是否有按“确定”，mintsources 均会覆写我们刚才换源的内容");
}

def_target(os_linuxmint);
