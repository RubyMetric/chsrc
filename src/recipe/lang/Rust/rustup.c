/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

// Size: 20MB
#define PL_Rustup_Speed_URL_Suffix

def_dish(pl_rust_rustup, "rustup");

void
pl_rust_rustup_prepare (void)
{
  chef_prep_this_dish (pl_rust_rustup, gsr);

  chef_set_recipe_created_on   (this, "2024-10-02");
  chef_set_recipe_last_updated (this, "2026-09-06");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@Yangmoooo", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_deny_english (this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://static.rust-lang.org",                 FeedByPrepare},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/rustup",         FeedByPrepare},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/rustup",  FeedByPrepare},
  {&Ustc,             "https://mirrors.ustc.edu.cn/rust-static",      FeedByPrepare},
  {&Nju,              "https://mirror.nju.edu.cn/rustup",             FeedByPrepare},
  {&Zju,              "https://mirrors.zju.edu.cn/rustup",            FeedByPrepare},
  {&Ha,               "https://mirrors.ha.edu.cn/rustup",             FeedByPrepare},
  {&RsProxyCN,        "https://rsproxy.cn",                           FeedByPrepare}
  // 2026-09-06 以下镜像源因长期不更新而弃用
  //{&Iscas,            "https://mirror.iscas.ac.cn/rustup",            FeedByPrepare},
  //{&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/rust-static",       FeedByPrepare},
  //{&Ali,              "https://mirrors.aliyun.com/rustup",            FeedByPrepare},

  def_sources_end()

  // 40MB大小
  chef_set_rest_smURL_with_postfix (this, "/rustup/dist/x86_64-unknown-linux-gnu/rustup-init");
}



void
pl_rust_rustup_getsrc (char *option)
{
  chsrc_view_env ("RUSTUP_UPDATE_ROOT", "RUSTUP_DIST_SERVER", NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/rustup/
 */
void
pl_rust_rustup_setsrc (char *option)
{
  chsrc_use_this_source (pl_rust_rustup);

#ifdef XY_Build_On_Windows

  char *cmd1 = xy_strcat (3, "setx RUSTUP_DIST_SERVER \"", source.url, "\"");
  char *cmd2 = xy_strcat (3, "setx RUSTUP_UPDATE_ROOT \"", source.url, "/rustup\"");

  char *cmd = xy_strcat (3, cmd1, " & ", cmd2);
  chsrc_run (cmd, RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line);

#else

  char *w1 = xy_strcat (3, "export RUSTUP_DIST_SERVER=\"", source.url, "\"\n");
  char *w2 = xy_strcat (3, "export RUSTUP_UPDATE_ROOT=\"", source.url, "/rustup\"\n");

  char *w = xy_2strcat (w1, w2);

  char *bashrc = xy_bashrc;
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *zshrc = xy_zshrc;
  if (xy_file_exist (zshrc))
    {
      chsrc_backup (zshrc);
      chsrc_append_to_file (w, zshrc);
    }

  char *fishrc = xy_fishrc;
  if (xy_file_exist (fishrc))
    {
      char *w1 = xy_strcat (3, "set -x RUSTUP_DIST_SERVER ", source.url, "\n");
      char *w2 = xy_strcat (3, "set -x RUSTUP_UPDATE_ROOT ", source.url, "/rustup\n");

      char *w = xy_2strcat (w1, w2);

      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }
#endif

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_alert2 ("请重启终端使rustup环境变量生效");
}



void
pl_rust_rustup_resetsrc (char *option)
{
  pl_rust_rustup_setsrc (option);
}
