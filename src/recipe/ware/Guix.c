/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-11>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-11
 * @note 目前只有一个源
 */
static Source_t wr_guix_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/git/guix.git"}
};
def_sources_n(wr_guix);


/**
 * @consult https://mirrors.sjtug.sjtu.edu.cn/docs/guix
 */
void
wr_guix_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_guix);

  char *file =  xy_strjoin (3, "(list (channel\n"
                               "       (inherit (car %default-channels))\n"
                               "       (url \"", source.url, "\")))");

  chsrc_note2 ("为防止扰乱配置文件，请您手动写入以下内容到 ~/.config/guix/channels.scm 文件中");
  say (file);

  ProgMode_ChgType = ChgType_Manual;
  chsrc_conclude (&source);
}

def_target_s (wr_guix);
