/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   :   Aoran Zeng   <ccmywish@qq.com>
 *                |    Heng Guo    <2085471348@qq.com>
 * Contributors   :    Yangmoooo   <yangmoooo@outlook.com>
 *                | GitHub Copilot <https://github.com/copilot>
 *                |
 * Created On     : <2023-09-02>
 * Major Revision :      3
 * Last Modified  : <2025-06-16>
 * ------------------------------------------------------------*/

static SourceProvider_t os_debian_upstream =
{
  def_upstream, "https://ftp.debian.org/debian/",
  {NotSkip, NA, NA, "https://ftp.debian.org/debian/dists/bookworm/main/Contents-all.gz"} // 32MB
};


/**
 * @update 2024-11-21
 */
static Source_t os_debian_sources[] =
{
  {&os_debian_upstream, "http://deb.debian.org/debian"},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/debian/"},
  {&Ali,              "https://mirrors.aliyun.com/debian"},
  {&Volcengine,       "https://mirrors.volces.com/debian"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/debian"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/debian"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/debian"},
  {&Tencent,          "https://mirrors.tencent.com/debian"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/debian"},
  // {&Netease,          "https://mirrors.163.com/debian"}, /* 不启用原因：过慢 */
  {&Sohu,             "https://mirrors.sohu.com/debian"}
};
def_sources_n(os_debian);


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

  char *msg = CliOpt_InEnglish ? "Source list file missing! However, you can still run `chsrc set debian` to add and use new sources"
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
  chsrc_yield_source_and_confirm (os_debian);

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

  chsrc_note2 ("将基于旧格式(非DEB822)换源");

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

  chsrc_yield_source_and_confirm (os_debian);

  chsrc_note2 ("如果遇到无法拉取 HTTPS 源的情况，我们会使用 HTTP 源并需要您运行:");
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


Feature_t
os_debian_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(os_debian);
