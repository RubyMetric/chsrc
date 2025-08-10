/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_ubuntu);

void
os_ubuntu_prelude ()
{
  use_this(os_ubuntu);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 2,
    "Aoran Zeng", "ccmywish@qq.com",
    "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Zhao", "1792582687@qq.com");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

#define OS_Ubuntu_Speed_URL_Postfix "/dists/noble/Contents-amd64.gz"

static SourceProvider_t os_ubuntu_upstream =
{
  def_upstream, "http://archive.ubuntu.com/",
  // https://github.com/RubyMetric/chsrc/issues/121
  {NotSkip, NA, NA, "http://archive.ubuntu.com/ubuntu/dists/noble/Contents-amd64.gz", /* 48.9 MB*/  ACCURATE}
};

/**
 * @update 2025-07-11
 */
static Source_t os_ubuntu_sources[] =
{
  {&os_ubuntu_upstream, "http://archive.ubuntu.com/ubuntu/", /* 不支持https */
                         DelegateToUpstream},

  {&MirrorZ,          "https://mirrors.cernet.edu.cn/ubuntu/",
                      "https://mirrors.cernet.edu.cn/ubuntu/" OS_Ubuntu_Speed_URL_Postfix },

  {&Ali,              "https://mirrors.aliyun.com/ubuntu",
                      "https://mirrors.aliyun.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  {&Volcengine,       "https://mirrors.volces.com/ubuntu",
                      "https://mirrors.volces.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  {&Bfsu,             "https://mirrors.bfsu.edu.cn/ubuntu",
                      "https://mirrors.bfsu.edu.cn/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  {&Ustc,             "https://mirrors.ustc.edu.cn/ubuntu",
                      "https://mirrors.ustc.edu.cn/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/ubuntu",
                      "https://mirrors.tuna.tsinghua.edu.cn/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  {&Tencent,          "https://mirrors.tencent.com/ubuntu",
                      "https://mirrors.tencent.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  // {&Tencent_Intra, "https://mirrors.tencentyun.com/ubuntu",
  //                  "https://mirrors.tencentyun.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  {&Huawei,           "https://mirrors.huaweicloud.com/ubuntu",
                      "https://mirrors.huaweicloud.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/ubuntu",
  //                     "https://mirrors.163.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix},

  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/ubuntu",
  //                     "https://mirrors.sohu.com/ubuntu" OS_Ubuntu_Speed_URL_Postfix}
};
def_sources_n(os_ubuntu);


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
  chsrc_yield_source_and_confirm (os_ubuntu);

  chsrc_backup (OS_Ubuntu_SourceList_DEB822);

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (strncmp (arch, "x86_64", 6)==0)
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Ubuntu_SourceList_DEB822);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Ubuntu_SourceList_DEB822);
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

  chsrc_yield_source_and_confirm (os_ubuntu);

  /* 不存在的时候，用的是我们生成的无效文件，不要备份 */
  if (sourcelist_exist)
    {
      chsrc_backup (OS_Ubuntu_old_SourceList);
    }

  char *arch = chsrc_get_cpuarch();
  char *cmd  = NULL;
  if (0==strncmp (arch, "x86_64", 6))
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Ubuntu_old_SourceList);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Ubuntu_old_SourceList);
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
