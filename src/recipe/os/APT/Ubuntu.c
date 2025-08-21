/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

/* Zorin OS 完全使用 Ubuntu 的换源方法，二者兼容 */
def_target(os_ubuntu, "ubuntu/zorinos");

void
os_ubuntu_prelude ()
{
  use_this(os_ubuntu);
  chef_allow_gsr(os_ubuntu);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@ccmywish", "@G_I_Y");
  chef_set_contributors (this, 1, "@Zhao");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://archive.ubuntu.com/ubuntu/",  FeedByPrelude}, /* 不支持https */
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/ubuntu/",FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/ubuntu",FeedByPrelude},
  {&Volcengine,       "https://mirrors.volces.com/ubuntu",FeedByPrelude},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/ubuntu",FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/ubuntu",FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/ubuntu",FeedByPrelude},
  {&Tencent,          "https://mirrors.tencent.com/ubuntu", FeedByPrelude},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/ubuntu",FeedByPrelude},
  {&Huawei,           "https://mirrors.huaweicloud.com/ubuntu",FeedByPrelude}

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/ubuntu", FeedByPrelude},

  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/ubuntu", FeedByPrelude}
  def_sources_end()

  chef_set_sources_speed_measure_url_with_postfix (this, "/dists/noble/Contents-amd64.gz");
}


void
os_ubuntu_getsrc (char *option)
{
  if (chsrc_check_file (OS_Ubuntu_SourceList_DEB822))
    {
      chsrc_view_file (OS_Ubuntu_SourceList_DEB822);
      return;
    }

  if (chsrc_check_file (OS_Ubuntu_old_SourceList))
    {
      chsrc_view_file (OS_Ubuntu_old_SourceList);
      return;
    }

  char *msg = ENGLISH ? "Source list file missing! However, you can still run `chsrc set ubuntu` to add and use new sources"
                      : "缺少源配置文件！但仍可直接通过 chsrc set ubuntu 来添加使用新的源";
  chsrc_error2 (msg);
  return;
}


/**
 * 此函数基本和 os_ubuntu_setsrc() 一致
 */
void
os_ubuntu_setsrc_for_deb822 (char *option)
{
  use_this_source(os_ubuntu);

  chsrc_backup (OS_Ubuntu_SourceList_DEB822);

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (strncmp (arch, "x86_64", 6)==0)
    {
      cmd = xy_strcat (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Ubuntu_SourceList_DEB822);
    }
  else
    {
      cmd = xy_strcat (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Ubuntu_SourceList_DEB822);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

/**
 * @note 不同架构下换源不一样
 */
void
os_ubuntu_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (OS_Ubuntu_SourceList_DEB822))
    {
      char *msg = ENGLISH ? "Will change source based on new format"
                          : "将基于新格式(DEB822)换源";
      chsrc_note2 (msg);
      os_ubuntu_setsrc_for_deb822 (option);
      return;
    }

  chsrc_alert2 ("将基于旧格式(非DEB822)换源");

  bool sourcelist_exist = ensure_debian_or_ubuntu_old_sourcelist (OS_Is_Ubuntu);

  use_this_source(os_ubuntu);

  /* 不存在的时候，用的是我们生成的无效文件，不要备份 */
  if (sourcelist_exist)
    {
      chsrc_backup (OS_Ubuntu_old_SourceList);
    }

  char *arch = chsrc_get_cpuarch();
  char *cmd  = NULL;
  if (0==strncmp (arch, "x86_64", 6))
    {
      cmd = xy_strcat (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Ubuntu_old_SourceList);
    }
  else
    {
      cmd = xy_strcat (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Ubuntu_old_SourceList);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_ubuntu_resetsrc (char *option)
{
  os_ubuntu_setsrc (option);
}
