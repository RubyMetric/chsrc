/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_nvm, "nvm");

static void
pl_nvm_check_platform (void)
{
  if (xy.on_windows)
    {
      char *msg = ENGLISH
        ? "Official nvm (nvm-sh) is only supported on POSIX systems; on Windows (nvm-windows), use `nvm node_mirror <url>` instead"
        : "官方 nvm (nvm-sh) 仅支持 POSIX 环境；Windows 下的 nvm-windows 机制不同，请使用 `nvm node_mirror <url>` 命令";
      chsrc_error (msg);
      exit (Exit_Unsupported);
    }
}

void
pl_nvm_prepare (void)
{
  chef_prep_this_dish (pl_nvm, gsr);

  chef_set_recipe_created_on   (this, "2024-09-23");
  chef_set_recipe_last_updated (this, "2026-10-28");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_And_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english (this);
  chef_allow_user_define (this);

  chef_set_note (this,
    "官方 nvm (nvm-sh) 原生不支持 Fish，Fish 下需配合 bass 或 nvm.fish 等插件使用；Windows 下的 nvm-windows 机制不同，暂不支持",
    "Official nvm (nvm-sh) does not natively support Fish (use with bass or nvm.fish); nvm-windows on Windows uses a different mechanism and is unsupported");

  chef_use_other_dish_sources (this, &pl_nodejs_binary_dish);
}


void
pl_nvm_getsrc (char *option)
{
  pl_nvm_check_platform ();
  chsrc_get_env ("NVM_NODEJS_ORG_MIRROR", NULL);
}


/**
 * @consult https://github.com/nvm-sh/nvm?tab=readme-ov-file#use-a-mirror-of-node-binaries
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/nodejs-release/
 * @issue   https://github.com/RubyMetric/chsrc/issues/81
 */
void
pl_nvm_setsrc (char *option)
{
  pl_nvm_check_platform ();

  Source_t source = chsrc_yield_source_and_confirm (&pl_nodejs_binary_dish, option);

  char *url = hp_remove_trailing_slash (source.url);
  chsrc_set_env ("NVM_NODEJS_ORG_MIRROR", url);
  free (url);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_nvm_resetsrc (char *option)
{
  pl_nvm_check_platform ();

  chsrc_set_env ("NVM_NODEJS_ORG_MIRROR", NULL);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
