/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Shengwei Chen <414685209@qq.com>
 * Contributors  :  Aoran Zeng   <ccmywish@qq.com>
 * Created On    : <2024-06-14>
 * Last Modified : <2024-08-27>
 * ------------------------------------------------------------*/

/**
 * @time 2024-06-20 更新
 */
static SourceInfo
os_armbian_sources[] = {
    {&Upstream,      "http://apt.armbian.com"},
    {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/armbian"},
    {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/armbian"},
    {&Bfsu,          "https://mirrors.bfsu.edu.cn/armbian"},
    {&Sustech,       "https://mirrors.sustech.edu.cn/armbian"},
    {&Ustc,          "https://mirrors.ustc.edu.cn/armbian"},
    {&Nju,           "https://mirrors.nju.edu.cn/armbian"},
    {&Ali,           "https://mirrors.aliyun.com/armbian"},
};
def_sources_n(os_armbian);

void
os_armbian_getsrc (char *option)
{
  if (chsrc_check_file (OS_Armbian_SourceList))
    {
      chsrc_view_file (OS_Armbian_SourceList);
      return;
    }

  char *msg = CliOpt_InEnglish ? "Source list config file missing! Path: " OS_Armbian_SourceList
                               : "缺少源配置文件！路径：" OS_Armbian_SourceList;
  chsrc_error2 (msg);
}

/**
 * 参考: https://mirrors.tuna.tsinghua.edu.cn/help/armbian
 */
void
os_armbian_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_armbian);

  chsrc_backup (OS_Armbian_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?[^ ]*armbian/?[^ ]*@", source.url,
                             "@g' " OS_Armbian_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_conclude (&source, SetsrcType_Auto);
}


FeatInfo
os_armbian_feat (char *option)
{
  FeatInfo f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.can_english = true;
  f.can_user_define = true;

  return f;
}

def_target_gsf(os_armbian);
