/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_combo_dish(pl_uv, "uv");

void pl_uv_getsrc (char *option);
void pl_uv_setsrc (char *option);
void pl_uv_resetsrc (char *option);

static Source_t
pl_uv_yield_source_for_dish (Dish_t *dish, char *option)
{
  if (!dish->inited)
    dish->preparefn ();

  if (hp_is_url (option))
    {
      Source_t user = { &UserDefinedProvider, option };
      return user;
    }

  int index = use_specific_mirror_or_auto_select (option, dish);
  return dish->sources[index];
}

static bool
pl_uv_write_config (const char *uv_config, const char *pypi_url, const char *py_dl_url)
{
  char *content = xy_file_read (xy_normalize_path (uv_config));
  if (!content)
    {
      chsrc_error2 ("无法读取 uv 配置文件");
      return false;
    }

  bool pyproject = xy_str_end_with (uv_config, PL_uv_PyprojectConfigFile);
  const char *index_header = pyproject ? "[[tool.uv.index]]" : "[[index]]";
  const char *parent_table = pyproject ? "[tool.uv]" : NULL;

  char *updated = pl_uv_toml_replace_index_url (content, pypi_url, index_header, parent_table);
  char *final = pl_uv_toml_replace_key_value (updated, "python-install-mirror", py_dl_url, parent_table);

  chsrc_backup (uv_config);
  chsrc_overwrite_file (final, uv_config);
  return true;
}

void
pl_uv_prepare (void)
{
  chef_prep_this_dish (pl_uv, NOOP);

  chef_set_recipe_created_on   (this, "2026-08-12");
  chef_set_recipe_last_updated (this, "2026-08-13");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_But_Not_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english (this);
  chef_allow_user_define (this);

  this->getfn = pl_uv_getsrc;
  this->setfn = pl_uv_setsrc;
  this->resetfn = pl_uv_resetsrc;

  chef_set_sub_dishes (this, 2, &pl_uv_pypi_index_dish, &pl_uv_python_build_dish);

  chef_use_other_dish_sources (this, &pl_pypi_dish);
}



void
pl_uv_getsrc (char *option)
{
  char *uv_config = pl_uv_find_uv_config (false);

  if (!uv_config || !chsrc_check_file (uv_config))
    {
      if (!uv_config)
        chsrc_error2 ("无法获取 uv 配置文件路径");
      else
        chsrc_error2 ("未找到 uv 配置文件");
      return;
    }

  char *content = xy_file_read (xy_normalize_path (uv_config));
  if (!content)
    {
      chsrc_error2 ("无法读取 uv 配置文件");
      return;
    }

  bool pyproject = xy_str_end_with (uv_config, PL_uv_PyprojectConfigFile);
  const char *index_header = pyproject ? "[[tool.uv.index]]" : "[[index]]";
  const char *parent_table = pyproject ? "[tool.uv]" : NULL;

  char *url = pl_uv_toml_get_index_url (content, index_header);
  if (url)
    {
      println (url);
    }
  else
    {
      if (ENGLISH)
        chsrc_note2 ("No source configured in uv, showing default upstream source:");
      else
        chsrc_note2 ("uv 中未配置源，显示默认上游源：");
      Source_t default_source = chsrc_yield_source (&pl_pypi_dish, "upstream");
      println (default_source.url);
    }

  char *mirror = pl_uv_toml_get_value_in_table (content, "python-install-mirror", parent_table);
  if (mirror)
    {
      println (mirror);
    }
  else
    {
      if (ENGLISH)
        chsrc_note2 ("No python-install-mirror configured in uv, showing default upstream source:");
      else
        chsrc_note2 ("uv 中未配置 python-install-mirror，显示默认上游源：");
      Source_t default_source = pl_uv_yield_source_for_dish (&pl_uv_python_build_dish, "upstream");
      println (default_source.url);
    }
}



void
pl_uv_setsrc (char *option)
{
  char *uv_config = pl_uv_find_uv_config (true);
  if (!uv_config)
    {
      chsrc_error2 ("无法获取 uv 配置文件路径");
      return;
    }

  if (chsrc_in_reset_mode ())
    {
      if (!chsrc_check_file (uv_config))
        {
          chsrc_info ("没有 uv 配置文件，无需重置");
          return;
        }

      Source_t default_pypi = chsrc_yield_source (&pl_pypi_dish, "upstream");
      Source_t default_build = pl_uv_yield_source_for_dish (&pl_uv_python_build_dish, "upstream");

      if (!pl_uv_write_config (uv_config, default_pypi.url, default_build.url))
        return;

      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&default_pypi);
      return;
    }

  char *pypi_opt = option;
  char *build_opt = NULL;

  if (option && !hp_is_url (option))
    {
      if (!pl_uv_python_build_dish.inited)
        pl_uv_python_build_dish.preparefn ();

      bool build_found = false;
      for (int i=0; i < pl_uv_python_build_dish.sources_n; i++)
        {
          if (xy_streql (pl_uv_python_build_dish.sources[i].mirror->code, option))
            {
              build_found = true;
              break;
            }
        }

      if (!pl_pypi_dish.inited)
        pl_pypi_dish.preparefn ();

      bool pypi_found = false;
      for (int i=0; i < pl_pypi_dish.sources_n; i++)
        {
          if (xy_streql (pl_pypi_dish.sources[i].mirror->code, option))
            {
              pypi_found = true;
              break;
            }
        }

      if (build_found && !pypi_found)
        {
          pypi_opt = NULL;
          build_opt = option;
        }
      else if (build_found)
        {
          build_opt = option;
        }
    }

  Source_t source = chsrc_yield_source (&pl_pypi_dish, pypi_opt);
  chsrc_confirm_source (&source);

  Source_t build_source = pl_uv_yield_source_for_dish (&pl_uv_python_build_dish, build_opt);

  if (!pl_uv_write_config (uv_config, source.url, build_source.url))
    return;

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}



void
pl_uv_resetsrc (char *option)
{
  pl_uv_setsrc (option);
}
