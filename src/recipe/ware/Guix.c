/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-11>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-11 更新
 * @note 目前只有一个源
 */
static SourceInfo
wr_guix_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/git/guix.git"}
};
def_sources_n(wr_guix);


/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/guix
 */
void
wr_guix_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_guix);
  chsrc_confirm_source (&source);

  char *file =  xy_strjoin (3, "(list (channel\n"
                               "       (inherit (car %default-channels))\n"
                               "       (url \"", source.url, "\")))");

  chsrc_note2 ("为防止扰乱配置文件，请您手动写入以下内容到 ~/.config/guix/channels.scm 文件中");
  puts (file);
  chsrc_say_lastly (&source, ChsrcTypeManual);
}

def_target_s (wr_guix);
