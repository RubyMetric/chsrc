/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Lontten  <lontten@163.com>
 * Created On    : <2024-09-29>
 * Last Modified : <2024-10-02>
 * ------------------------------------------------------------*/

/**
 * chsrc get bun
 */
void
pl_nodejs_bun_getsrc (char *option)
{
  chsrc_view_file ("~/.bunfig.toml");
}


/**
 * @consult https://bun.sh/docs/runtime/bunfig#global-vs-local
 * @consult https://github.com/RubyMetric/chsrc/issues/83
 * @consult https://github.com/RubyMetric/chsrc/pull/90
 *
 * chsrc set bun
 */
void
pl_nodejs_bun_setsrc (char *option)
{
  char *setsrc_type = xy_streql (option, SetsrcType_Reset) ? SetsrcType_Reset : SetsrcType_Manual;

  // 用的是 npm Registry 的源
  chsrc_yield_source (pl_nodejs);

  char *file = xy_strjoin(3, "[install]\n"
                             "registry = \"", source.url, "\"");

  if (CliOpt_Locally==true)
    {
      chsrc_note2 ("请您手动写入以下内容到本项目根目录的 bunfig.toml 文件中");
    }
  else
    {
      chsrc_note2 (xy_strjoin (3, "请您手动写入以下内容到 ", xy_uniform_path ("~/.bunfig.toml"), " 文件中"));
    }

  puts (file);
  chsrc_conclude (&source, setsrc_type);
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
