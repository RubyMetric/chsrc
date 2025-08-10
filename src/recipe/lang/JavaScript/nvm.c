/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_js_nvm);

void
pl_js_nvm_prelude (void)
{
  use_this(pl_js_nvm);

  chef_set_created_on   (this, "2024-09-23");
  chef_set_last_updated (this, "2025-06-19");
  chef_set_sources_last_updated (this, "2025-06-19");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nul None", "nul@none.org");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_forbid_local_mode (this);
  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_set_note ("nvm does not support Fish", "nvm 不支持 Fish shell");

  // 使用 pl_js_nodejs_binary 的源
  this->sources = pl_js_nodejs_binary_target.sources;
  this->sources_n = pl_js_nodejs_binary_target.sources_n;
}


void
pl_js_nvm_getsrc (char *option)
{
  chsrc_view_env ("NVM_NODEJS_ORG_MIRROR", NULL);
}


/**
 * @consult https://github.com/nvm-sh/nvm?tab=readme-ov-file#use-a-mirror-of-node-binaries
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/nodejs-release/
 * @issue   https://github.com/RubyMetric/chsrc/issues/81
 *
 * @note nvm 不支持 Fish
 */
void
pl_js_nvm_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_js_binary_release);

  char *w = xy_strjoin (3, "export NVM_NODEJS_ORG_MIRROR=", source.url, "\n");

  char *zshrc  = xy_zshrc;
  char *bashrc = xy_bashrc;

  chsrc_append_to_file (w, bashrc);

  if (xy_file_exist (zshrc))
    chsrc_append_to_file (w, zshrc);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_js_nvm_resetsrc (char *option)
{
  // pl_js_nvm_setsrc (ChgType_Reset);
  chsrc_error ("暂不支持对 nvm 重置");
  exit (Exit_Unsupported);
}
