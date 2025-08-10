/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : chef.c
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None  <nul@none.org>
 * Created On    : <2025-08-09>
 * Last Modified : <2025-08-10>
 *
 * chef DSL: for chefs (recipe makers) to define a target
 * ------------------------------------------------------------*/

#pragma once

#define chef_allow_get() this->getfn = t##_getsrc;
#define chef_allow_set() this->setfn = t##_setsrc;
#define chef_allow_reset() this->resetfn = t##_resetsrc;


void
chef_allow_english (Target_t *target)
{
  if (!target)
    return;

  target->can_english = true;
}

void
chef_forbid_english (Target_t *target)
{
  if (!target)
    return;

  target->can_english = false;
}


void
chef_allow_local_mode (Target_t *target, Capability_t cap, const char *explain_zh, const char *explain_en)
{
  if (!target)
    return;

  target->cap_local = cap;
  target->cap_local_explain = xy_strdup (CHINESE ? explain_zh : explain_en);
}


void
chef_allow_user_define (Target_t *target)
{
  if (!target)
    return;

  target->can_user_define = true;
  target->can_user_define_explain = NULL;
}

void
chef_forbid_user_define (Target_t *target)
{
  if (!target)
    return;

  target->can_user_define = false;

  char *reason = CHINESE ? "URL将会根据内部实现重写,因此不能自定义"
                         : "The URL will be rewritten based on internal implementation, so it cannot be customized";
  target->can_user_define_explain = reason;
}


void
chef_set_note (Target_t *target, const char *note_zh, const char *note_en)
{
  if (!target)
    return;

  char *msg = CHINESE ? xy_strdup(note_zh) : xy_strdup(note_en);
  target->note = msg;
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
chef_set_authors (Target_t *target, size_t count, ...)
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
chef_set_chef (Target_t *target, char *name, char *email)
{
  if (!target || !name || !email)
    return;

  target->current_chef = xy_malloc0 (sizeof(Contributor_t));
  target->current_chef->name  = xy_strdup (name);
  target->current_chef->email = xy_strdup (email);
}


void
chef_set_sous_chefs (Target_t *target, size_t count, ...)
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
chef_set_created_on (Target_t *target, char *date)
{
  if (!target)
    return;

  target->recipe_created_on = xy_strdup (date);
}


void
chef_set_last_updated (Target_t *target, char *date)
{
  if (!target)
    return;

  target->recipe_last_updated = xy_strdup (date);
}


void
chef_set_sources_last_updated (Target_t *target, char *date)
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
