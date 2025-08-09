/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : chef.h
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None  <nul@none.org>
 * Created On    : <2025-07-14>
 * Last Modified : <2025-08-09>
 *
 * For chefs (recipe makers) and sometimes framewoker
 * to do some work not releated to OS operations
 * ------------------------------------------------------------*/

#pragma once

bool
chef_is_url (const char *str)
{
  return (xy_str_start_with (str, "http://") || xy_str_start_with (str, "https://"));
}

/**
 * @return 一律返回新字符串
 */
char *
chef_remove_trailing_slash (char *str)
{
  char *newstr = xy_strdup (str);
  size_t len = strlen (newstr);
  if (len > 0 && newstr[len - 1] == '/')
    newstr[len - 1] = '\0';
  return newstr;
}

/**
 * @return 一律返回新字符串
 */
char *
chef_ensure_trailing_slash (char *str)
{
  size_t len = strlen (str);
  if (len == 0 || str[len - 1] == '/')
    return xy_strdup (str);

  return xy_2strjoin (str, "/");
}


void
chef_set_contributors (Target_t *target, uint32_t count, ...)
{
  if (!target || count==0)
    return;

  target->contributors_n = count;
  target->contributors = xy_malloc0 (count * sizeof(Contributor_t));

  va_list args;
  va_start(args, count);

  for (uint32_t i = 0; i < count; i++)
    {
      char *name  = va_arg(args, char*);
      char *email = va_arg(args, char*);

      target->contributors[i].name  = xy_strdup (name);
      target->contributors[i].email = xy_strdup (email);
    }
}


void
chef_set_recipe_authors (Target_t *target, size_t count, ...)
{
  if (!target || count == 0)
    return;

  va_list args;
  va_start(args, count);

  target->recipe_authors = xy_malloc0 (count * sizeof(Contributor_t));
  target->recipe_authors_n = count;

  for (size_t i = 0; i < count; i++)
    {
      char *name = va_arg(args, char*);
      char *email = va_arg(args, char*);

      target->recipe_authors[i].name = xy_strdup(name);
      target->recipe_authors[i].email = xy_strdup(email);
    }

  va_end(args);
}


void
chef_set_current_chef (Target_t *target, char *name, char *email)
{
  if (!target || !name || !email)
    return;

  target->current_chef = xy_malloc0 (sizeof(Contributor_t));
  target->current_chef->name  = xy_strdup (name);
  target->current_chef->email = xy_strdup (email);
}


void
chef_set_current_sous_chefs (Target_t *target, size_t count, ...)
{
  if (!target)
    return;

  if (count == 0)
    {
      target->current_sous_chefs = NULL;
      target->current_sous_chefs_n = 0;
      return;
    }

  va_list args;
  va_start(args, count);

  target->current_sous_chefs = xy_malloc0 (count * sizeof(Contributor_t));
  target->current_sous_chefs_n = count;

  for (size_t i = 0; i < count; i++)
    {
      char *name = va_arg(args, char*);
      char *email = va_arg(args, char*);

      target->current_sous_chefs[i].name = xy_strdup(name);
      target->current_sous_chefs[i].email = xy_strdup(email);
    }

  va_end(args);
}


void
chef_set_recipe_created_on_date (Target_t *target, char *date)
{
  if (!target)
    return;

  target->recipe_created_on = xy_strdup (date);
}


void
chef_set_recipe_last_updated_date (Target_t *target, char *date)
{
  if (!target)
    return;

  target->recipe_last_updated = xy_strdup (date);
}


void
chef_set_sources_last_updated_date (Target_t *target, char *date)
{
  if (!target)
    return;

  target->sources_last_updated = xy_strdup (date);
}


void
chef_debug_target (Target_t *target)
{
  if (!target)
    return;

  say ("Debug Target Information:");
  printf ("  Get Function: %p\n", target->getfn);
  printf ("  Set Function: %p\n", target->setfn);
  printf ("  Reset Function: %p\n", target->resetfn);
  printf ("  Feature Function: %p\n", target->featfn);
  printf ("  Sources: %p\n", target->sources);
  printf ("  Sources Count: %lld\n", target->sources_n);
  printf ("  Contributors: %p\n", target->contributors);
  printf ("  Contributors Count: %lld\n", target->contributors_n);
}
