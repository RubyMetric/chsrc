/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_debian, "debian");

void
os_debian_prelude ()
{
  use_this(os_debian);
  chef_allow_gsr(os_debian);

  chef_set_created_on   (this, "2023-09-02");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 2,
    "Aoran Zeng", "ccmywish@qq.com",
    "Heng Guo",   "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 1,
    "Yangmoooo", "yangmoooo@outlook.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&UpstreamProvider, "http://deb.debian.org/debian",          FeedByPrelude},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/debian/", FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/debian", FeedByPrelude},
  {&Volcengine,       "https://mirrors.volces.com/debian", FeedByPrelude},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/debian", FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/debian", FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/debian", FeedByPrelude},
  {&Tencent,          "https://mirrors.tencent.com/debian", FeedByPrelude}

  // {&Tencent_Intra, "https://mirrors.tencentyun.com/debian", FeedByPrelude},

  /* 不启用原因：过慢 */
  // {&Netease,     "https://mirrors.163.com/debian", FeedByPrelude},

  /* 不启用原因：过慢 */
  // {&Sohu,        "https://mirrors.sohu.com/debian", FeedByPrelude}
  def_sources_end()

  chef_set_sources_speed_measure_url_with_postfix (this, "/dists/bookworm/main/Contents-all.gz");
}


void
os_debian_getsrc (char *option)
{
  if (chsrc_check_file (OS_Debian_SourceList_DEB822))
    {
      chsrc_view_file (OS_Debian_SourceList_DEB822);
      return;
    }

  if (chsrc_check_file (OS_Debian_old_SourceList))
    {
      chsrc_view_file (OS_Debian_old_SourceList);
      return;
    }

  char *msg = ENGLISH ? "Source list file missing! However, you can still run `chsrc set debian` to add and use new sources"
                      : "缺少源配置文件！但仍可直接通过 chsrc set debian 来添加使用新的源";
  chsrc_error2 (msg);
  return;
}


static bool
os_debian_does_old_sourcelist_use_cdrom (void)
{
  /* 我们只检查旧版sourcelist，因为 common.h 中的填充只支持旧版 */
  char *cmd = xy_2strjoin ("grep -q '^deb cdrom:' ", OS_Debian_old_SourceList);
  int ret = system (cmd);
  bool use_cdrom = ret == 0;

  return use_cdrom;
}


void
os_debian_setsrc_for_deb822 (char *option)
{
  use_this_source(os_debian);

  chsrc_backup (OS_Debian_SourceList_DEB822);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/debian/?@", source.url, "@g' " OS_Debian_SourceList_DEB822);
  chsrc_run (cmd, RunOpt_Default);

  /* debian-security 源和其他源不一样 */
  cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/debian-security/?@", source.url, "-security@g' " OS_Debian_SourceList_DEB822);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * 处理旧版(非DEB822) sourcelist 的换源
 *
 * Debian 10 Buster 以上版本默认支持 HTTPS 源。如果遇到无法拉取 HTTPS 源的情况，请先使用 HTTP 源并安装
 * apt install apt-transport-https ca-certificates
 */
void
os_debian_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (OS_Debian_SourceList_DEB822))
    {
      chsrc_note2 ("将基于新格式(DEB822)换源");
      os_debian_setsrc_for_deb822 (option);
      return;
    }

  chsrc_alert2 ("将基于旧格式(非DEB822)换源");

  /* Docker环境下，Debian镜像可能不存在该文件 */
  bool sourcelist_exist = ensure_debian_or_ubuntu_old_sourcelist (OS_Is_Debian_Literally);

  /**
   * 处理带有CDROM源的sourcelist
   *
   * https://github.com/RubyMetric/chsrc/issues/185#issuecomment-2746072917
   */
  if (sourcelist_exist)
    {
      bool use_cdrom = os_debian_does_old_sourcelist_use_cdrom();
      if (use_cdrom)
        {
          chsrc_backup (OS_Debian_old_SourceList);
          int rm_status = system ("rm " OS_Debian_old_SourceList);
          if (rm_status != 0) { /* Placate -Wunused-result */ }

          chsrc_warn2 ("旧版源配置文件中使用了 CDROM 源，已删除(但备份)该配置文件，重新配置");
          /* 现在的情况是：系统中已经没有配置文件了 */
          sourcelist_exist = ensure_debian_or_ubuntu_old_sourcelist (OS_Is_Debian_Literally);
        }
    }

  use_this_source(os_debian);

  chsrc_alert2 ("如果遇到无法拉取 HTTPS 源的情况，请手动运行:");
  say ("apt install apt-transport-https ca-certificates");

  /* 不存在的时候，用的是我们生成的用来填充占位的无效文件，不要备份 */
  if (sourcelist_exist)
    {
      chsrc_backup (OS_Debian_old_SourceList);
    }

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/debian/?@", source.url, "@g\' " OS_Debian_old_SourceList);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
os_debian_resetsrc (char* option)
{
  os_debian_setsrc (option);
}
