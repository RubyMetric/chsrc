/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None   <nul@none.org>
 *               |
 * Created On    : <2024-09-23>
 * Last Modified : <2025-06-19>
 * ------------------------------------------------------------*/

/**
 * chsrc get nvm
 */
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
 * chsrc set nvm
 *
 * @note nvm does not support Fish
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


/**
 * chsrc reset nvm
 */
void
pl_js_nvm_resetsrc (char *option)
{
  // pl_js_nvm_setsrc (ChgType_Reset);
  chsrc_error ("暂不支持对 nvm 重置");
  exit (Exit_Unsupported);
}


/**
 * chsrc ls nvm
 */
Feature_t
pl_js_nvm_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = "";
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

// def_target_gsrf(pl_js_nvm);
Target_t pl_js_nvm_target = {def_target_inner_gsrf(pl_js_nvm),def_target_sourcesn(pl_js_binary_release)};
