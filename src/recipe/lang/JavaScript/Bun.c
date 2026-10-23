/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_bun, "bun");

void
pl_bun_prepare (void)
{
  chef_prep_this_dish (pl_bun, gsr);

  chef_set_recipe_created_on   (this, "2024-09-29");
  chef_set_recipe_last_updated (this, "2026-10-23");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@lontten");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unknown);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_dish_sources (this, &pl_npm_dish);
}

/**
 * chsrc get bun
 */
void
pl_bun_getsrc (char *option)
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
pl_bun_setsrc (char *option)
{
  chsrc_use_this_source (pl_bun);

  char *content = RAWSTR_pl_bun_config;

  content = xy_str_gsub (content, "@url@", source.url);

  if (chsrc_in_project_scope_mode())
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
pl_bun_resetsrc (char *option)
{
  pl_bun_setsrc (option);
}
