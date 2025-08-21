/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_js_bun, "bun");

void
pl_js_bun_prelude (void)
{
  use_this(pl_js_bun);
  chef_allow_gsr(pl_js_bun);

  chef_set_created_on   (this, "2024-09-29");
  chef_set_last_updated (this, "2025-07-22");
  chef_set_sources_last_updated (this, "2025-07-22");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 1, "@lontten");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  // 用的是 npm Registry 的源，所以使用 pl_js_group 的源
  this->sources = pl_js_group_target.sources;
  this->sources_n = pl_js_group_target.sources_n;
}

/**
 * chsrc get bun
 */
void
pl_js_bun_getsrc (char *option)
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
pl_js_bun_setsrc (char *option)
{
  // 用的是 npm Registry 的源
  Source_t source = chsrc_yield_source_and_confirm (&pl_js_group_target, option);

  char *content = RAWSTR_pl_js_bun_config;

  content = xy_str_gsub (content, "@url@", source.url);

  if (chsrc_in_local_mode())
    {
      chsrc_note2 ("请手动写入以下内容到本项目根目录的 bunfig.toml 文件中");
    }
  else
    {
      chsrc_note2 (xy_strcat (3, "请手动写入以下内容到 ", xy_normalize_path ("~/.bunfig.toml"), " 文件中"));
    }

  println (content);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * chsrc reset bun
 */
void
pl_js_bun_resetsrc (char *option)
{
  pl_js_bun_setsrc (option);
}
