/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

def_target(wr_anaconda);

void
wr_anaconda_prelude ()
{
  use_this(wr_anaconda);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 2,
    "Yangmoooo", "yangmoooo@outlook.com",
    "yongxiang", "1926885268@qq.com");

  chef_has_getfn();
  chef_has_setfn();
  // chef_has_resetsrc();
  this.cap_locally = CanNot;
  this.cap_locally_explain = NULL;
  this.can_english = false;
  this.can_user_define = false;
  this.note = "无法自定义源, 因为这些链接将会在setsrc函数中补充完整";

  def_upstream("https://repo.anaconda.com");
  def_sources_begin()
  {&upstream,        "https://repo.anaconda.com", DelegateToUpstream},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn", DelegateToMirror},
  {&Bjtu,            "https://mirror.bjtu.edu.cn", DelegateToMirror},
  {&Nju,             "https://mirror.nju.edu.cn", DelegateToMirror},
  {&Bfsu,            "https://mirrors.bfsu.edu.cn", DelegateToMirror},
  {&Zju,             "https://mirrors.zju.edu.cn", DelegateToMirror},
  {&Pku,             "https://mirrors.pku.edu.cn", DelegateToMirror},
  {&NJTech,          "https://mirrors.njtech.edu.cn", DelegateToMirror},
  {&Ustc,            "https://mirrors.ustc.edu.cn", DelegateToMirror},
  {&Sjtug_Siyuan,    "https://mirror.sjtu.edu.cn", DelegateToMirror},
  {&Lzuoss,          "https://mirror.lzu.edu.cn", DelegateToMirror}
  def_sources_end()
}


void
wr_anaconda_getsrc (char *option)
{
  chsrc_view_file ("~/.condarc");
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/anaconda/
 */
void
wr_anaconda_setsrc (char *option)
{
  use_this(wr_anaconda);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  char *w = xy_str_gsub (RAWSTR_wr_anaconda_condarc, "@1@", source.url);

  /* Windows 也是在这里 */
  char *configfile = xy_2strjoin (xy_os_home, "/.condarc");

  if (xy_on_windows)
    {
      if (xy_file_exist (configfile))
        {
          chsrc_alert2 ("配置文件不存在，将使用 conda 命令创建");
          bool conda_exist = chsrc_check_program ("conda");
          if (!conda_exist)
            {
              chsrc_error ("未找到 conda 命令，请检查是否存在");
              exit (Exit_UserCause);
            }
          chsrc_run ("conda config --set show_channel_urls yes", RunOpt_Default);
        }
    }

  chsrc_note2 (xy_strjoin (3, "请向 ", configfile, " 中手动添加:"));
  println (w);

  chsrc_note2 ("然后运行 conda clean -i 清除索引缓存，保证用的是镜像站提供的索引");

  chsrc_note2 ("若还需要添加其他第三方源, 可参考: https://help.mirrors.cernet.edu.cn/anaconda/");

  chsrc_determine_chgtype (ChgType_SemiAuto);
  chsrc_conclude (&source);
}
