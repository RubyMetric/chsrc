/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-15>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-15
 */
static Source_t
pl_ocaml_sources[] = {
  {&UpstreamProvider,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/git/opam-repository.git"}
};
def_sources_n(pl_ocaml);


void
pl_ocaml_check_cmd ()
{
  chsrc_ensure_program ("opam");
}

void
pl_ocaml_getsrc(char *option)
{
  pl_ocaml_check_cmd ();
  chsrc_run ("opam repo get-url default", RunOpt_Default);
}

/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/git/opam-repository.git
 */
void
pl_ocaml_setsrc(char *option)
{
  pl_ocaml_check_cmd ();

  chsrc_yield_source_and_confirm (pl_ocaml);

  char *cmd = xy_strjoin (3, "opam repo set-url default ",
                              source.url,
                             " --all --set-default");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_note2 ("如果是首次使用 opam ，请使用以下命令进行初始化");
  say (xy_2strjoin ("opam init default ", source.url));

  ProgMode_ChgType = ChgType_SemiAuto;
  chsrc_conclude (&source);
}

def_target(pl_ocaml);
