/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_freebsd);

void
os_freebsd_prelude ()
{
  use_this(os_freebsd);

  chef_set_created_on   (this, "2023-09-03");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2023-09-27");

  chef_set_authors (this, 2, "Aoran Zeng", "ccmywish@qq.com", "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nil Null", "nil@null.org");

  chef_allow_set();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note ("2023-09-24: 以下三个USTC, NJU, Netease 均维护了 freebsd-pkg freebsd-ports\n2023-09-27: 请务必保持Nju前面有至少一个镜像，原因请查看 freebsd 的换源函数", NULL);

  def_upstream (NULL);

  def_sources_begin()
  {&upstream,         NULL,                  DelegateToUpstream},
  {&Ustc,             "mirrors.ustc.edu.cn", DelegateToMirror},
  {&Nju,              "mirror.nju.edu.cn",   DelegateToMirror},
  {&Netease,          "mirrors.163.com",     DelegateToMirror},
  def_sources_end()
}

/**
 * @consult
 *  1. https://book.bsdcn.org/di-3-zhang-ruan-jian-yuan-ji-bao-guan-li-qi/di-3.2-jie-freebsd-huan-yuan-fang-shi.html
 *  2. https://help.mirrors.cernet.edu.cn/FreeBSD-ports/
 *
 * 据 @ykla,
 *   FreeBSD 有五类源：pkg、ports、port、portsnap、update，其中 portsnap 在 FreeBSD 14 已经被移除了
 */
void
os_freebsd_setsrc (char *option)
{
  // 据 @ykla，FreeBSD不自带sudo，但是我们依然要保证是root权限
  chsrc_ensure_root ();

  int index = use_specific_mirror_or_auto_select (option, os_freebsd);

  Source_t source = os_freebsd_sources[index];
  chsrc_confirm_source();

  chsrc_log2 ("1. 添加 freebsd-pkg 源 (二进制安装包)");
  chsrc_ensure_dir ("/usr/local/etc/pkg/repos");

  char *conf = xy_strjoin (3, "/usr/local/etc/pkg/repos/", source.mirror->code, ".conf");

  char *pkg_content = xy_strjoin (4,
                      source.mirror->code, ": { \n"
                      "  url: \"http://", source.url, "/freebsd-pkg/${ABI}/latest\",\n"
                      "}\n"
                      "FreeBSD: { enabled: no }"
                    );

  chsrc_overwrite_file (pkg_content, conf);
  chsrc_note2 (
    xy_strjoin (3, "若要使用季度分支，请在", conf ,"中将latest改为quarterly"));

  chsrc_alert2 ("若要使用HTTPS源，请先安装securtiy/ca_root_ns，并将'http'改成'https'，最后使用'pkg update -f'刷新缓存即可\n");
  br();

  chsrc_log2 ("2. 修改 freebsd-ports 源");
  // @ccmywish: 2023-09-27 据 @ykla , NJU的freebsd-ports源没有设置 Git，
  //                       但是我认为由于使用Git还是要比非Git方便许多，我们尽可能坚持使用Git
  //                       而 gitup 又要额外修改它自己的配置，比较麻烦
  bool git_exist = query_program_exist (xy_str_to_quietcmd ("git version"), "git", Noisy_When_Exist|Noisy_When_NonExist);
  if (git_exist)
    {
      if (xy_streql("nju",source.mirror->code))
        {
          source = os_freebsd_sources[index-1]; // 使用NJU的前一个源，即USTC源
        }
      char *git_cmd = xy_strjoin (3, "git clone --depth 1 https://", source.url, "/freebsd-ports/ports.git /usr/ports");
      chsrc_run (git_cmd, RunOpt_Default);
      source = os_freebsd_sources[index]; // 恢复至选中的源
      chsrc_alert2 ("下次更新请使用 git -C /usr/ports pull 而非使用 gitup");
    }
  else
    {
      char *fetch  = xy_strjoin (3, "fetch https://", source.url, "/freebsd-ports/ports.tar.gz");  // 70多MB
      char *unzip  = "tar -zxvf ports.tar.gz -C /usr/ports";
      char *delete = "rm ports.tar.gz";
      chsrc_run (fetch, RunOpt_Default);
      chsrc_run (unzip, RunOpt_Default);
      chsrc_run (delete, RunOpt_Default);
      chsrc_log2 ("下次更新请重新下载内容至 /usr/ports");
    }


  chsrc_log2 ("3. 指定 port 源");
  // https://help.mirrors.cernet.edu.cn/FreeBSD-ports/
  chsrc_backup ("/etc/make.conf");

  char *ports = xy_strjoin (3, "MASTER_SITE_OVERRIDE?=http://", source.url, "/freebsd-ports/distfiles/${DIST_SUBDIR}/\n");
  chsrc_append_to_file (ports, "/etc/make.conf");


  /* 不再换 portsnap */
  /*
    chsrc_backup ("/etc/portsnap.conf");

    char *portsnap =xy_strjoin(3,"s@(.*)SERVERNAME=[\\.|a-z|A-Z]*@\\1SERVERNAME=", source.url,
                                "@g < /etc/portsnap.conf.bak");

    chsrc_overwrite_file (portsnap, "/etc/portsnap.conf");

    chsrc_log2 ("portsnap sources changed");
    chsrc_log2 ("获取portsnap更新使用此命令: 'portsnap fetch extract'");
  */


  // HELP: 暂时没有源提供
  chsrc_alert2 ("4. 抱歉，目前境内无 freebsd-update 源，若存在请报告issue，谢谢");
  /*
    chsrc_log2 ("3. 修改 freebsd-update 源");

    char *update_cp = "cp /etc/freebsd-update.conf /etc/freebsd-update.conf.bak";
    chsrc_run (update_cp, RunOpt_Default);

    char *update =xy_strjoin (3,"s@(.*)SERVERNAME [\\.|a-z|A-Z]*@\\1SERVERNAME ",
                                 source.url,
                                "@g < /etc/freebsd-update.conf.bak");

    chsrc_overwrite_file (update, "/etc/freebsd-update.conf");
  */

  chsrc_conclude (&source);
}
