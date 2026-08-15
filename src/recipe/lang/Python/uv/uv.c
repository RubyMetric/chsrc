/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_combo_dish(pl_uv, "uv");

void
pl_uv_prepare (void)
{
  chef_prep_this_dish (pl_uv, NOOP);

  chef_set_recipe_created_on   (this, "2026-08-12");
  chef_set_recipe_last_updated (this, "2026-08-13");

  chef_set_sub_dishes (this, 2, &pl_uv_pypi_index_dish, &pl_uv_python_build_dish);

  chef_use_other_dish_sources (this, &pl_pypi_dish);
}
