/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 *               |
 * Created On    : <2024-06-12>
 * Last Modified : <2025-07-21>
 * ------------------------------------------------------------*/

static SourceProvider_t os_almalinux_upstream =
{
  def_upstream, "http://repo.almalinux.org/almalinux",
  {NotSkip, NA, NA, "https://raw.repo.almalinux.org/almalinux/9.5/isos/x86_64/AlmaLinux-9-latest-x86_64-minimal.iso", ACCURATE}
};

/**
 * @update 2024-12-18
 */
static Source_t os_almalinux_sources[] =
{
  {&os_almalinux_upstream , "http://repo.almalinux.org/almalinux", DelegateToUpstream},
  {&Ali,                    "https://mirrors.aliyun.com/almalinux", DelegateToMirror},
  {&Volcengine,             "https://mirrors.volces.com/almalinux", DelegateToMirror},
  {&Sjtug_Zhiyuan,          "https://mirrors.sjtug.sjtu.edu.cn/almalinux", DelegateToMirror},
  {&Zju,                    "https://mirrors.zju.edu.cn/almalinux",        DelegateToMirror},
  {&Nju,                    "https://mirror.nju.edu.cn/almalinux",         DelegateToMirror},
};
def_sources_n(os_almalinux);

/**
 * @consult: https://developer.aliyun.com/mirror/almalinux
 */
void
os_almalinux_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_almalinux);

  char *cmd = xy_strjoin (3,
    "sed -e 's|^mirrorlist=|#mirrorlist=|g' -e 's|^#\\s*baseurl=https://repo.almalinux.org/almalinux|baseurl=", source.url, "|g'  -i.bak  /etc/yum.repos.d/almalinux*.repo");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


Feature_t
os_almalinux_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = false;

  f.note = NULL;
  return f;
}

def_target_sf(os_almalinux);
