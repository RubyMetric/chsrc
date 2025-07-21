/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Word2VecT <tangzinan@bupt.edu.cn>
 * Created On     : <2023-09-10>
 * Major Revision :      3
 * Last Modified  : <2025-07-14>
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

/**
 * @update 2025-07-13
 *
 * @note 这些链接将会在setsrc函数中补充完整
 */
static Source_t wr_homebrew_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/",          DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/",             DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/",           DelegateToMirror},

  /* 注释原因: 这两者和其他镜像站URL补全结果不一样(2025-07-13)  */
  // {&Zju,           "https://mirrors.zju.edu.cn/",            DelegateToMirror},
  // {&Cqu,           "https://mirrors.cqu.edu.cn/",            DelegateToMirror},

  /* 注释原因: 该源已不存在(2025-07-13) */
  // {&Sustech,          "https://mirrors.sustech.edu.cn/", DelegateToMirror}
};
def_sources_n(wr_homebrew);


void
wr_homebrew_getsrc (char *option)
{
  int status = system (RAWSTR_wr_homebrew_read_config_cmd);
  if (status != 0) { /* NOOP */ }
}

/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/homebrew/
 *
 * 自brew 4.0.0 (2023 年 2 月 16日) 起，
 * HOMEBREW_INSTALL_FROM_API 会成为默认行为，无需设置。大部分用户无需再克隆 homebrew-core 仓库，故无需设置 HOMEBREW_CORE_GIT_REMOTE 环境变量；
 * 但是为了以防万一，我们还是为用户设置该环境变量
 */
void
wr_homebrew_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_homebrew);

  char *w = xy_str_gsub (RAWSTR_wr_homebrew_config_in_bash, "@1@", source.url);

  char *zshrc = xy_zshrc;
  chsrc_backup (zshrc);
  chsrc_append_to_file (w, zshrc);

  char *bashrc = xy_bashrc;
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *fishrc = xy_fishrc;
  if (xy_file_exist (fishrc))
    {
      char *w = xy_str_gsub (RAWSTR_wr_homebrew_config_in_fish, "@1@", source.url);
      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_alert2 ("请重启终端使Homebrew环境变量生效");
}


Feature_t
wr_homebrew_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;

  /* 该换源方案中，URL存在拼凑，因此不能让用户手动使用某URL来换源 */
  f.can_user_define = false;

  f.note = "该换源通过写入环境变量实现，若多次换源，请手动清理profile文件";
  return f;
}


def_target_gsf(wr_homebrew);
