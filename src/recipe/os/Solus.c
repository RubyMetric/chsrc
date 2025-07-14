/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-29>
 * Last Modified : <2025-07-14>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-29
 */
static Source_t os_solus_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/solus/packages/shannon/eopkg-index.xml.xz", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/solus/packages/shannon/eopkg-index.xml.xz",          DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/solus/packages/shannon/eopkg-index.xml.xz",            DelegateToMirror}
};
def_sources_n(os_solus);


/**
 * @consult https://help.mirrors.cernet.edu.cn/solus/
 */
void
os_solus_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_solus);

  char *cmd = xy_2strjoin ("eopkg add-repo Solus ", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


Feature_t
os_solus_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_sf(os_solus);
