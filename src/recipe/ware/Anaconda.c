/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 *               | yongxiang <1926885268@qq.com>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-12-07>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-10
 * @note 这些链接将会在setsrc函数中补充完整
 */
static Source_t wr_anaconda_sources[] =
{
  {&UpstreamProvider, "https://repo.anaconda.com/"},
  {&Nju,              "https://mirror.nju.edu.cn/anaconda/"},
  {&Bjtu,              "https://mirror.bjtu.edu.cn/anaconda/"},
  {&Pku,               "https://mirrors.pku.edu.cn/anaconda/"},
  {&Njtech,            "https://mirrors.njtech.edu.cn/anaconda/"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/anaconda/"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/anaconda/"},
  {&Zju,              "https://mirrors.zju.edu.cn/anaconda/"},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/anaconda"}
};
def_sources_n(wr_anaconda);


void
wr_anaconda_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_anaconda);

  char *main  = xy_2strjoin (source.url, "pkgs/main");
  char *r     = xy_2strjoin (source.url, "pkgs/r");
  char *msys2 = xy_2strjoin (source.url, "pkgs/msys2");
  char *cloud = xy_2strjoin (source.url, "cloud");

  char *file = xy_strjoin (22,
               "channels:\n  - defaults\n"
               "show_channel_urls: true\ndefault_channels:"
             "\n  - ", main,
             "\n  - ", r,
             "\n  - ", msys2,
             "\ncustom_channels:\n"
               "  conda-forge: ", cloud,
             "\n  msys2: ",        cloud,
             "\n  bioconda: ",     cloud,
             "\n  menpo: ",        cloud,
             "\n  pytorch: ",      cloud,
             "\n  pytorch-lts: ",  cloud,
             "\n  simpleitk: ",    cloud,
             "\n  deepmodeling: ", cloud);


  // TODO: 待确认 windows 上也是这里吗？
  char *config = xy_2strjoin (xy_os_home, "/.condarc");

  if (xy_on_windows)
    {
      bool exist = chsrc_check_program ("conda");
      if (!exist)
        {
          chsrc_error ("未找到 conda 命令，请检查是否存在");
          exit (Exit_UserCause);
        }
      chsrc_run ("conda config --set show_channel_urls yes", RunOpt_Default);
    }

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  puts (file);

  chsrc_note2 ("然后运行 conda clean -i 清除索引缓存，保证用的是镜像站提供的索引");

  ProgMode_ChgType = ChgType_SemiAuto;
  chsrc_conclude (&source);
}

def_target_s (wr_anaconda);
