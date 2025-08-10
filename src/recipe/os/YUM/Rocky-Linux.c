chef_set_created_on ("2023-09-24");
chef_set_authors ("Aoran Zeng <ccmywish@qq.com>");
chef_set_contributors ("happy game <happygame1024@gmail.com>");
chef_has_setfn;
chef_has_resetsrc;
use_this;

/**
 * @update 2025-06-20
 */
static Source_t os_rockylinux_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/rocky",  DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/rockylinux", DelegateToMirror},
  {&Volcengine,       "https://mirrors.volces.com/rockylinux", DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/rocky",      DelegateToMirror},
  {&Sustech,          "https://mirrors.sustech.edu.cn/rocky-linux", DelegateToMirror},
  {&Zju,              "https://mirrors.zju.edu.cn/rocky",      DelegateToMirror},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/rocky",       DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/rocky",      DelegateToMirror},
   /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/Rocky",     DelegateToMirror}
};
def_sources_n(os_rockylinux);


/**
 * @consult https://help.mirrors.cernet.edu.cn/rocky/
 */
void
os_rockylinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_rockylinux);

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

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


def_target(os_rockylinux);
