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
  // 用的是 npm Registry 的源
  chsrc_yield_source (pl_nodejs);

  char *file = xy_strjoin(3, "[install]\n"
                             "registry = \"", source.url, "\"");

  if (chsrc_in_local_mode())
    {
      chsrc_note2 ("请您手动写入以下内容到本项目根目录的 bunfig.toml 文件中");
    }
  else
    {
      chsrc_note2 (xy_strjoin (3, "请您手动写入以下内容到 ", xy_normalize_path ("~/.bunfig.toml"), " 文件中"));
    }

  p(file);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * chsrc reset bun
 */
void
pl_nodejs_bun_resetsrc (char *option)
{
  pl_nodejs_bun_setsrc (option);
}


/**
 * chsrc ls bun
 */
Feature_t
pl_nodejs_bun_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = FullyCan;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

// def_target_gsrf(pl_nodejs_bun);
Target_t pl_nodejs_bun_target = {def_target_inner_gsrf(pl_nodejs_bun),def_target_sourcesn(pl_nodejs)};
