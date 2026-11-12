/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : dish.c
 * File Authors  : @ccmywish
 * Contributors  : @Mikachu2333
 *               |
 * Created On    : <2026-08-13>
 * Last Modified : <2026-09-05>
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
dish_use_other_dish_sources (Dish_t *dish)
{
  if (dish->sources_dish)
    {
      return true;
    }
  else
    {
      return false;
    }
}



void
push_combo_stack (Dish_t *combo_dish)
{
  if (ProgStatus.ComboStackDepth >= MaxComboStackDepth)
    chsrc_breakdown ("combo dish 嵌套层级过深");

  int depth = ProgStatus.ComboStackDepth;
  ProgStatus.ComboStack[depth] = combo_dish;
  ProgStatus.ComboBackedUpPaths[depth] = xy_seq_new ();
  ProgStatus.ComboStackDepth++;
}

void
pop_combo_stack (void)
{
  if (ProgStatus.ComboStackDepth <= 0)
    chsrc_breakdown ("combo dish 栈已为空");

  ProgStatus.ComboStackDepth--;
}


int
current_combo_stack_depth ()
{
  return ProgStatus.ComboStackDepth;
}


Dish_t *
current_combo_dish ()
{
  int depth = current_combo_stack_depth ();

  if (depth <= 0)
    return NULL;

  return ProgStatus.ComboStack[depth - 1];
}



/**
 * @brief 判断 dish 是否拥有来自指定镜像站的源
 *
 * @note 支持两个 "伪 mirror code": `upstream` 与 `first`
 */
bool
dish_has_source_from_mirror (Dish_t *dish, const char *mirror_code)
{
  if (!dish || !dish->sources || !mirror_code)
    return false;

  if (xy_streql_ic (mirror_code, "upstream"))
    return dish->sources_n > 0;

  if (xy_streql_ic (mirror_code, "first"))
    return dish->sources_n > 1;

  for (int i=0; i < dish->sources_n; i++)
    {
      if (xy_streql (dish->sources[i].mirror->code, mirror_code))
        return true;
    }

  return false;
}


/**
 * @brief 判断 dish 的兄弟 sub dish 是否拥有来自指定镜像站的源
 *
 * @example
 *
 *          / `dish`   current dish as a sub dish
 *   combo -  sibling 1 sub dish
 *          \ sibling 2 sub dish
 */
static bool
subdish_sibling_has_source_from_mirror (Dish_t *dish, const char *mirror_code)
{
  Dish_t *current = dish;

  if (ProgStatus.ComboStackDepth <= 0)
    return false;

  Dish_t *combo = ProgStatus.ComboStack[ProgStatus.ComboStackDepth - 1];
  if (!combo || !combo->sub_dishes)
    return false;

  for (size_t i=0; i < xy_seq_len(combo->sub_dishes); i++)
    {
      Dish_t *sub_dish = xy_seq_at (combo->sub_dishes, i);
      if (sub_dish == current)
        continue;

      if (!sub_dish->prepared)
        sub_dish->preparefn ();

      if (dish_has_source_from_mirror (sub_dish, mirror_code))
        return true;
    }

  return false;
}

/**
 * @brief 判断 combo dish 的所有 sub dish 是否至少有一个拥有来自指定镜像站的源
 *
 * 用于 waiter_handle_Set_Source() 中对 combo dish 的总处理
 */
bool
combo_at_least_one_sub_dish_has_source_from_mirror (Dish_t *combo_dish, const char *mirror_code)
{
  if (!combo_dish || !combo_dish->sub_dishes)
    {
      chsrc_breakdown ("该套餐没有子菜品");
    }

  for (size_t i=0; i < xy_seq_len(combo_dish->sub_dishes); i++)
    {
      Dish_t *sub_dish = xy_seq_at (combo_dish->sub_dishes, i);
      if (!sub_dish->prepared)
        sub_dish->preparefn ();

      if (dish_has_source_from_mirror (sub_dish, mirror_code))
        return true;
    }

  return false;
}



/**
 * @brief 判断 `combo_dish` 是否包含指定的 `sub_dish`
 */
bool
combo_has_sub_dish (Dish_t *combo_dish, Dish_t *sub_dish)
{
  if (!combo_dish || !dish_has_sub_dishes(combo_dish))
    chsrc_breakdown ("该套餐定义有问题");

  for (size_t i=0; i < xy_seq_len(combo_dish->sub_dishes); i++)
    {
      Dish_t *d = xy_seq_at (combo_dish->sub_dishes, i);
      if (d == sub_dish)
        return true;
    }

  return false;
}



Source_t
dish_select_source_by_mirror_code (Dish_t *dish, char *mirror_code)
{
  char *dish_name = dish->aliases;
  Source_t *sources = dish->sources;
  size_t size = dish->sources_n;

  if (0==size)
    {
      char *msg1 = CHINESE ? "当前 " : "Currently ";
      char *msg2 = CHINESE ? " 无任何可用源，请联系维护者" : " doesn't have any source available. Please contact the maintainers";
      chsrc_error (xy_strcat (3, msg1, dish_name, msg2));
      exit (Exit_MaintainerCause);
    }

  if (1==size)
    {
      char *msg1 = CHINESE ? "当前 " : "Currently ";
      char *msg2 = CHINESE ? " 仅存在上游默认源，请联系维护者" : " only the upstream source exists. Please contact the maintainers";
      chsrc_error (xy_strcat (3, msg1, dish_name, msg2));
      exit (Exit_MaintainerCause);
    }

  if (chsrc_in_reset_mode())
    {
      char *msg = CHINESE ? "将重置为上游默认源"
                          : "Will reset to the upstream's default source";
      say (msg);
      return sources[0]; /* 返回第1个，因为第1个是上游默认源 */
    }

  if (2==size)
    {
      char *msg1 = CHINESE ? " 是 " : " is ";
      char *msg2 = CHINESE ? " 目前唯一可用镜像站，感谢他们的慷慨支持"
                           : "'s ONLY mirror available currently, thanks for their generous support";
      const char *name = CHINESE ? sources[1].mirror->name
                                 : sources[1].mirror->abbr;
      chsrc_succ (xy_strcat (4, name, msg1, dish_name, msg2));
    }

  if (xy_streql_ic ("first", mirror_code))
    {
      char *msg = ENGLISH ? "Will use the first speedy source measured by maintainers" : "将使用维护团队测速第一的源";
      say (msg);
      return sources[1]; /* 返回第2个，因为第1个是上游默认源 */
    }



  Source_t src = sources[0];

  bool exist = false;

  for (int i=0; i<size; i++)
    {
      src = sources[i];
      if (xy_streql (src.mirror->code, mirror_code))
        {
          exist = true;
          break;
        }
    }

  if (!exist)
    {
      bool mirror_site_exist = false;
      for (int i=0; i<xy_seq_len(ProgStore.mirror_sites); i++)
        {
          MirrorSite_t *mir = xy_seq_at (ProgStore.mirror_sites, i);
          if (xy_streql_ic (mir->code, mirror_code))
            {
              mirror_site_exist = true;
              break;
            }
        }

      if (mirror_site_exist)
        {
          char *msg1 = CHINESE ? "镜像站 "   : "Mirror site ";
          char *msg2 = CHINESE ? " 存在，但未提供该软件源" : " exists, but is not available for this software";
          chsrc_error (xy_strcat (3, msg1, mirror_code, msg2));
          exit (Exit_UserCause);
        }
      else
        {
          char *msg1 = CHINESE ? "镜像站 "   : "Mirror site ";
          char *msg2 = CHINESE ? " 不存在" : " doesn't exist";
          chsrc_error (xy_strcat (3, msg1, mirror_code, msg2));
        }

      char *msg = CHINESE ? "查看可使用源，请使用 chsrc list "
                          : "To see available sources, use chsrc list ";
      chsrc_error (xy_2strcat (msg, dish_name));
      exit (Exit_UserCause);
    }
  return src;
}
