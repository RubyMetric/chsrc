/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng  <ccmywish@qq.com>
 * Contributors  : Mikachu2333 <mikachu.23333@zohomail.com>
 * Created On    : <2023-09-10>
 * Last Modified : <2025-07-28>
 *
 * MiKTeX 和 TeX Live 都是流行的 LaTeX 发行版，但它们是不同的实现
 * ------------------------------------------------------------*/

/**
 * @update 2025-07-28
 */
static Source_t wr_tex_sources[] =
{
  {&UpstreamProvider, NULL, NULL},
  {&MirrorZ,       "https://mirrors.cernet.edu.cn/CTAN/systems/texlive/tlnet", NULL},

  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/ctan/systems/texlive/tlnet",
                   "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},

  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet",
                   "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},

  {&Bfsu,           "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet",
                    "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},

  {&Bjtu,           "https://mirror.bjtu.edu.cn/ctan/systems/texlive/tlnet",
                    "https://mirror.bjtu.edu.cn/ctan/systems/texlive/tlnet/archive/fandol.tar.xz"},

  {&Lzuoss,         "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet",
                    "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},

  {&Jlu,            "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet",
                    "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},

  {&Sustech,        "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet",
                    "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"}
};
def_sources_n(wr_tex);


void
wr_tex_check_cmd (bool *tlmgr_exist, bool *mpm_exist)
{
  *tlmgr_exist = chsrc_check_program ("tlmgr");
  *mpm_exist = chsrc_check_program ("mpm");

  if (!*tlmgr_exist && !*mpm_exist)
    {
      chsrc_error ("未找到 tlmgr 或 mpm 命令，请检查是否存在（其一）");
      exit (Exit_UserCause);
    }
}

void
wr_tex_getsrc (char *option)
{
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd (&tlmgr_exist, &mpm_exist);

  if (tlmgr_exist)
    {
      chsrc_run("tlmgr option repository", RunOpt_Default);
    }
  if (mpm_exist)
    {
      chsrc_run("mpm --get-repository", RunOpt_Default);
    }
}

/**
 * @consult https://help.mirrors.cernet.edu.cn/CTAN/
 */
void
wr_tex_setsrc (char *option)
{
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd (&tlmgr_exist, &mpm_exist);

  chsrc_yield_source_and_confirm (wr_tex);

  char *cmd = NULL;

  if (tlmgr_exist)
    {
      cmd = xy_2strjoin ("tlmgr option repository ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (mpm_exist)
    {
      char *miktex_url = xy_2strjoin (xy_str_delete_suffix (source.url, "texlive/tlnet"), "win32/miktex/tm/packages/");
      cmd = xy_2strjoin ("mpm --set-repository=", miktex_url);
      chsrc_run (cmd, RunOpt_Default);
    }

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


Feature_t
wr_tex_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.can_english = false;
  f.can_user_define = true;

  return f;
}

def_target_gsf(wr_tex);
