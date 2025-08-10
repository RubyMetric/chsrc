chef_set_created_on ("2023-09-24");
chef_set_authors ("Aoran Zeng <ccmywish@qq.com>");
chef_set_contributors ("Nil Null <nil@null.org>");
chef_allow_set();
use_this;

/**
 * @update 2024-06-12
 */
static Source_t os_anolis_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&Ali,              "https://mirrors.aliyun.com/anolis",  DelegateToMirror},
  {&Hust,             "https://mirrors.hust.edu.cn/anolis", DelegateToMirror}
};
def_sources_n(os_anolis);


/**
 * @consult: https://mirrors.hust.edu.cn/docs/anolis
 */
void
os_anolis_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_anolis);

  char *cmd = xy_strjoin (3, "sed -i.bak -E 's|https?://(mirrors\\.openanolis\\.cn/anolis)|", source.url, "|g' /etc/yum.repos.d/*.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("dnf makecache", RunOpt_Default);
  chsrc_run ("dnf update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


def_target(os_anolis);
