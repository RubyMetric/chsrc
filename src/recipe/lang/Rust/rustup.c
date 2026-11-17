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
  chef_set_recipe_last_updated (this, "2026-10-28");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@Yangmoooo", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_And_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english (this);
  chef_allow_user_define (this);

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
  chsrc_get_env ("RUSTUP_UPDATE_ROOT", "RUSTUP_DIST_SERVER", NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/rustup/
 */
void
pl_rust_rustup_setsrc (char *option)
{
  chsrc_use_this_source (pl_rust_rustup);

  char *dist_server = hp_remove_trailing_slash (source.url);
  char *update_root = xy_2strcat (dist_server, "/rustup");

  chsrc_set_env_block (
    "RUSTUP_DIST_SERVER", dist_server,
    "RUSTUP_UPDATE_ROOT", update_root,
    NULL);

  free (update_root);
  free (dist_server);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_rust_rustup_resetsrc (char *option)
{
  chsrc_set_env_block (
    "RUSTUP_DIST_SERVER", NULL,
    "RUSTUP_UPDATE_ROOT", NULL,
    NULL);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
