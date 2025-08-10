chef_set_created_on ("2023-09-03");
chef_set_authors ("Heng Guo <2085471348@qq.com>", "Aoran Zeng <ccmywish@qq.com>");
chef_set_contributors ("Nil Null <nil@null.org>");
chef_has_getfn;
chef_has_setfn;
use_this;

/**
 * @update 2025-07-31
 */
static Source_t os_openbsd_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/OpenBSD/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/OpenBSD/",   DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/OpenBSD/", DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/OpenBSD/",    DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/OpenBSD/",  DelegateToMirror},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/OpenBSD/",   DelegateToMirror},
  {&Iscas,            "https://mirror.iscas.ac.cn/OpenBSD/",   DelegateToMirror},
  {&Cqupt,            "https://mirrors.cqupt.edu.cn/openbsd/", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/OpenBSD/",   DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/OpenBSD/",  DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/OpenBSD/", DelegateToMirror},
  {&Netease,          "https://mirrors.163.com/OpenBSD/",      DelegateToMirror},
  {&Sohu,             "https://mirrors.sohu.com/OpenBSD/",     DelegateToMirror}
};
def_sources_n(os_openbsd);


void
os_openbsd_getsrc (char *option)
{
  chsrc_view_file ("/etc/installurl");
}


/**
 * @consult
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/openbsd/
 * 2. https://book.bsdcn.org/di-26-zhang-openbsd/di-26.2-jie-pei-zhi.html
 */
void
os_openbsd_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_openbsd);

  chsrc_backup ("/etc/installurl");
  chsrc_overwrite_file (source.url, "/etc/installurl");

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


def_target(os_openbsd);
