/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_ocaml, "ocaml/opam");

void
pl_ocaml_prelude ()
{
  use_this(pl_ocaml);
  chef_allow_gs(pl_ocaml);

  chef_set_created_on   (this, "2023-09-15");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&upstream,        NULL, DelegateToUpstream},
  {&Sjtug_Zhiyuan,   "https://mirrors.sjtug.sjtu.edu.cn/git/opam-repository.git", DelegateToMirror}
  def_sources_end()
}


void
pl_ocaml_check_cmd ()
{
  chsrc_ensure_program ("opam");
}


void
pl_ocaml_getsrc (char *option)
{
  pl_ocaml_check_cmd ();
  chsrc_run ("opam repo get-url default", RunOpt_Default);
}


/**
 * @consult https://mirrors.sjtug.sjtu.edu.cn/docs/git/opam-repository.git
 */
void
pl_ocaml_setsrc (char *option)
{
  pl_ocaml_check_cmd ();

  use_this_source(pl_ocaml);

  char *cmd = xy_strjoin (3, "opam repo set-url default ",
                              source.url,
                             " --all --set-default");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_alert2 ("如果是首次使用 opam ，请使用以下命令进行初始化");
  println (xy_2strjoin ("opam init default ", source.url));

  chsrc_conclude (&source);
}
