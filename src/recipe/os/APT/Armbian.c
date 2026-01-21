/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_armbian, "armbian");

void
os_armbian_prelude ()
{
  chef_prep_this (os_armbian, gsr);

  chef_set_created_on   (this, "2024-06-14");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-11-21");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@livelycode36");
  chef_set_sauciers (this, 2, "@ccmywish", "@Yangmoooo");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_deny_english(this);
  chef_deny_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://apt.armbian.com", DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/armbian",        DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/armbian", DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/armbian",            DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/armbian",           DelegateToMirror},
  {&Sustech,          "https://mirrors.sustech.edu.cn/armbian",        DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/armbian",           DelegateToMirror},
  {&Nju,              "https://mirrors.nju.edu.cn/armbian",             DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/armbian",             DelegateToMirror}
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

  chsrc_use_this_source (os_armbian);

  chsrc_backup (OS_Armbian_SourceList);

  char *cmd = xy_strcat (3, "sed -E -i 's@https?[^ ]*armbian/?[^ ]*@", source.url,
                             "@g' " OS_Armbian_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt-get update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_armbian_resetsrc (char *option)
{
  os_armbian_setsrc (option);
}
