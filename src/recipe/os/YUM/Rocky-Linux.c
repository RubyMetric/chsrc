/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_rockylinux);

void
os_rockylinux_prelude ()
{
  use_this(os_rockylinux);
  chef_allow_sr(os_rockylinux);

  chef_set_created_on   (this, "2023-09-24");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "happy game", "happygame1024@gmail.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&upstream,         "https://dl.rockylinux.org",                 DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/rocky",       DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/rockylinux",      DelegateToMirror},
  {&Volcengine,       "https://mirrors.volces.com/rockylinux",      DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/rocky",          DelegateToMirror},
  {&Sustech,          "https://mirrors.sustech.edu.cn/rocky-linux", DelegateToMirror},
  {&Zju,              "https://mirrors.zju.edu.cn/rocky",          DelegateToMirror},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/rocky",           DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/rocky",      DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/Rocky",     DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/rocky/
 */
void
os_rockylinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_rockylinux);

  char *version_str = xy_run ("sed -nr 's/ROCKY_SUPPORT_PRODUCT_VERSION=\"(.*)\"/\\1/p' " ETC_OS_RELEASE, 0);
  double version = atof (version_str);

  char *cmd = NULL;

  if (version < 9)
    {
      cmd = xy_strjoin (3,
                      "sed -e 's|^mirrorlist=|#mirrorlist=|g' "
                      "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
                      "-i.bak /etc/yum.repos.d/Rocky-*.repo"
                      );
                      // Rocky-AppStream.repo
                      // Rocky-BaseOS.repo
                      // Rocky-Extras
                      // Rocky-PowerTools
    }
  else
    {
      cmd = xy_strjoin (3,
                      "sed -e 's|^mirrorlist=|#mirrorlist=|g' "
                      "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
                      "-i.bak /etc/yum.repos.d/rocky-extras.repo /etc/yum.repos.d/rocky.repo"
                      );
    }


  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  chsrc_conclude (&source);
}


void
os_rockylinux_resetsrc (char *option)
{
  os_rockylinux_setsrc (option);
}
