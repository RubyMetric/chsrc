/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_ocaml, "ocaml/opam");

void
pl_ocaml_prelude ()
{
  chef_prep_this (pl_ocaml, gs);

  chef_set_created_on   (this, "2023-09-15");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@hezonglun");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,  "https://opam.ocaml.org/", DelegateToUpstream},
  {&Sjtug_Zhiyuan,     "https://mirrors.sjtug.sjtu.edu.cn/git/opam-repository.git", DelegateToMirror}
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

  chsrc_use_this_source (pl_ocaml);

  char *cmd = xy_strcat (3, "opam repo set-url default ",
                              source.url,
                             " --all --set-default");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_alert2 ("如果是首次使用 opam ，请使用以下命令进行初始化");
  println (xy_2strcat ("opam init default ", source.url));

  chsrc_conclude (&source);
}
