/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo  <yangmoooo@outlook.com>
 *               | yongxiang  <1926885268@qq.com>
 *               |
 * Created On    : <2023-09-10>
 * Last Modified : <2025-07-14>
 * ------------------------------------------------------------*/

 #include "rawstr4c.h"

/**
 * @update 2025-07-14
 * @note 这些链接将会在setsrc函数中补充完整
 */
static Source_t wr_anaconda_sources[] =
{
  {&UpstreamProvider, "https://repo.anaconda.com",    NULL},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn", NULL},
  {&Bjtu,             "https://mirror.bjtu.edu.cn",    NULL},
  {&Nju,              "https://mirror.nju.edu.cn",     NULL},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn",  NULL},
  {&Zju,              "https://mirrors.zju.edu.cn",    NULL},
  {&Pku,              "https://mirrors.pku.edu.cn",    NULL},
  {&NJTech,           "https://mirrors.njtech.edu.cn", NULL},
  {&Ustc,             "https://mirrors.ustc.edu.cn",   NULL},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn",    NULL},
  {&Lzuoss,           "https://mirror.lzu.edu.cn",     NULL}
};
def_sources_n(wr_anaconda);


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
  chsrc_yield_source_and_confirm (wr_anaconda);

  char *w = xy_str_gsub (RAWSTR_wr_anaconda_condarc, "@1@", source.url);

  /* Windows 也是在这里 */
  char *configfile = xy_2strjoin (xy_os_home, "/.condarc");

  if (xy_on_windows)
    {
      if (xy_file_exist (configfile))
        {
          chsrc_note2 ("配置文件不存在，将使用 conda 命令创建");
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


Feature_t
wr_anaconda_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.can_english = false;
  f.can_user_define = false;

  return f;
}

def_target_gsf (wr_anaconda);
