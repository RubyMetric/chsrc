/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_armbian);

void
os_armbian_prelude ()
{
  use_this(os_armbian);

  chef_set_created_on   (this, "2024-06-14");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-11-21");

  chef_set_authors (this, 1, "Shengwei Chen", "414685209@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 2,
    "Aoran Zeng", "ccmywish@qq.com",
    "Yangmoooo",  "yangmoooo@outlook.com");

  chef_has_getfn();
  chef_has_setfn();
  chef_has_resetsrc();

  def_upstream("http://apt.armbian.com");
  def_sources_begin(os_armbian)
  {&upstream,       "http://apt.armbian.com", DelegateToUpstream},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/armbian",        DelegateToMirror},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/armbian", DelegateToMirror},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/armbian",            DelegateToMirror},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/armbian",           DelegateToMirror},
  {&Sustech,        "https://mirrors.sustech.edu.cn/armbian",        DelegateToMirror},
  {&Ustc,           "https://mirrors.ustc.edu.cn/armbian",           DelegateToMirror},
  {&Nju,            "https://mirrors.nju.edu.cn/armbian",             DelegateToMirror},
  {&Ali,            "https://mirrors.aliyun.com/armbian",             DelegateToMirror}
  def_sources_end()
}


void
os_armbian_getsrc (char *option)
{
  if (chsrc_check_file (OS_Armbian_SourceList))
    {
      chsrc_view_file (OS_Armbian_SourceList);
      return;
    }

  char *msg = ENGLISH ? "Source list config file missing! Path: " OS_Armbian_SourceList
                      : "缺少源配置文件！路径：" OS_Armbian_SourceList;
  chsrc_error2 (msg);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/armbian
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

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_armbian_resetsrc (char *option)
{
  os_armbian_setsrc (option);
}
