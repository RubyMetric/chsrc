/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : dish.c
 * File Authors  : @ccmywish
 * Contributors  : Nil Null <nil@null.org>
 * Created On    : <2026-08-13>
 * Last Modified : <2026-08-14>
 *
 * dish 为主体的一些函数
 * ------------------------------------------------------------*/

/**
 * @brief 获取 dish 的第一个别名
 *
 * 由 chefs_handle_XXX() 里的 group dish 递归时调用
 */
char *
dish_get_first_alias (Dish_t *dish)
{
  if (!dish || !dish->aliases || *dish->aliases == '\0')
    return NULL;

  char *alias = xy_strdup (dish->aliases);
  char *separator = strchr (alias, '/');
  if (separator)
    *separator = '\0';
  return alias;
}



bool
dish_has_sub_dishes (Dish_t *dish)
{
  return (dish->sub_dishes && xy_seq_len(dish->sub_dishes) > 1);
}



bool
dish_has_source_from_mirror (Dish_t *dish, const char *code)
{
  if (!dish || !dish->sources || !code)
    return false;

  if (xy_streql (code, "upstream"))
    return dish->sources_n > 0;

  if (xy_streql (code, "first"))
    return dish->sources_n > 1;

  for (int i=0; i < dish->sources_n; i++)
    {
      if (xy_streql (dish->sources[i].mirror->code, code))
        return true;
    }

  return false;
}
