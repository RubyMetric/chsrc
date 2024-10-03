/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None   <nul@none.org>
 * Created On    : <2024-09-23>
 * Last Modified : <2024-09-23>
 * ------------------------------------------------------------*/

/**
 * chsrc get nvm
 */
void
pl_nodejs_nvm_getsrc (char *option)
{
  char *cmd = "echo NVM_NODEJS_ORG_MIRROR";
  system (cmd);
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
pl_nodejs_nvm_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_nodejs_binary_release);

  char *env = xy_2strjoin ("export NVM_NODEJS_ORG_MIRROR=", source.url);

  char *zshrc  = "~/.zshrc";
  char *bashrc = "~/.bashrc";

  chsrc_append_to_file (env, bashrc);

  if (xy_file_exist (zshrc))
    chsrc_append_to_file (env, zshrc);

  chsrc_conclude (&source, SetsrcType_Auto);
}


/**
 * chsrc reset nvm
 */
void
pl_nodejs_nvm_resetsrc (char *option)
{
  // pl_nodejs_nvm_setsrc (SetsrcType_Reset);
  chsrc_error ("暂不支持对 nvm 重置");
  exit (Exit_Unsupported);
}


/**
 * chsrc ls nvm
 */
FeatInfo
pl_nodejs_nvm_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = false;

  fi.cap_locally = CanNot;
  fi.locally = "";
  fi.can_english = false;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}

// def_target_gsrf(pl_nodejs_nvm);
TargetInfo pl_nodejs_nvm_target = {def_target_inner_gsrf(pl_nodejs_nvm),def_target_sourcesn(pl_nodejs_binary_release)};
