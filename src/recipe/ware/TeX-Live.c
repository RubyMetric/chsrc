/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * MiKTeX 和 TeX Live 都是流行的 LaTeX 发行版，但它们是不同的实现
 * ------------------------------------------------------------*/

def_target(wr_tex, "latex/ctan/tex/texlive/miktex/tlmgr/mpm");

void
wr_tex_prelude ()
{
  chef_prep_this (wr_tex, gs);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-28");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 1, "@Mikachu2333");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://ctan.org/tex-archive/systems/texlive/tlnet", DelegateToUpstream},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/CTAN/systems/texlive/tlnet", DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/ctan/systems/texlive/tlnet",
                      "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet",
                      "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet",
                      "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},
  {&Bjtu,             "https://mirror.bjtu.edu.cn/ctan/systems/texlive/tlnet",
                      "https://mirror.bjtu.edu.cn/ctan/systems/texlive/tlnet/archive/fandol.tar.xz"},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet",
                      "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},
  {&Jlu,              "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet",
                      "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"},
  {&Sustech,          "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet",
                      "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet/archive/fandol.tar.xz"}
  def_sources_end()
}


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

  use_this_source(wr_tex);

  char *cmd = NULL;

  if (tlmgr_exist)
    {
      cmd = xy_2strcat ("tlmgr option repository ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (mpm_exist)
    {
      char *miktex_url = xy_2strcat (xy_str_delete_suffix (source.url, "texlive/tlnet"), "win32/miktex/tm/packages/");
      cmd = xy_2strcat ("mpm --set-repository=", miktex_url);
      chsrc_run (cmd, RunOpt_Default);
    }

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
