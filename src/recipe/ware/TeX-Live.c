/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-10 更新
 */
static SourceInfo
wr_tex_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/ctan/systems/texlive/tlnet"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Jlu,           "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Sustech,       "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet"}
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
      exit(1);
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
 * 参考 https://help.mirrors.cernet.edu.cn/CTAN/
 */
void
wr_tex_setsrc (char *option)
{
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd (&tlmgr_exist, &mpm_exist);

  SourceInfo source;
  chsrc_yield_source (wr_tex);
  chsrc_confirm_source (&source);

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

  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

def_target(wr_tex);
