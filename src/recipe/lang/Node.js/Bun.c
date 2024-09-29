/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2024-09-29>
 * Last Modified : <2024-09-29>
 * ------------------------------------------------------------*/

/**
 * chsrc get bun
 */
void
pl_nodejs_bun_getsrc (char *option)
{
  chsrc_view_file ("~/.bun/bunfig.toml");
}


/**
 * @consult https://bun.sh/guides/install/custom-registry
 * @consult https://github.com/RubyMetric/chsrc/issues/83
 *
 * chsrc set bun
 */
void
pl_nodejs_bun_setsrc (char *option)
{
  char *chsrc_type = xy_streql (option, SetsrcType_Reset) ? SetsrcType_Reset : SetsrcType_Manual;
  chsrc_yield_source (pl_nodejs);

  char *file = xy_strjoin(3, "[install]\n"
                             "registry = \"", source.url, "\"");

  chsrc_note2 (xy_strjoin (3, "请您手动写入以下内容到 ", xy_uniform_path ("~/.bun/bunfig.toml"), " 文件中:"));
  puts (file);

  chsrc_conclude (&source, chsrc_type);
}


/**
 * chsrc reset bun
 */
void
pl_nodejs_bun_resetsrc (char *option)
{
  pl_nodejs_bun_setsrc (SetsrcType_Reset);
}


/**
 * chsrc ls bun
 */
FeatInfo
pl_nodejs_bun_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.stcan_locally = CanFully;
  fi.locally = NULL;
  fi.can_english = true;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}

// def_target_gsrf(pl_nodejs_bun);
TargetInfo pl_nodejs_bun_target = {def_target_inner_gsrf(pl_nodejs_bun),def_target_sourcesn(pl_nodejs)};
