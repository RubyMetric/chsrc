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

#define chef_allow_gsr(t) this->getfn = t##_getsrc; this->setfn = t##_setsrc; this->resetfn = t##_resetsrc;
#define chef_allow_s(t)   this->getfn = NULL;       this->setfn = t##_setsrc; this->resetfn = NULL;
#define chef_allow_sr(t)  this->getfn = NULL;       this->setfn = t##_setsrc; this->resetfn = t##_resetsrc;
#define chef_allow_gs(t)  this->getfn = t##_getsrc; this->setfn = t##_setsrc; this->resetfn = NULL;


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

  if (cap == FullyCan)
    {
      target->cap_local_explain = xy_strdup (CHINESE ? "完全支持项目级换源" : "Supports project-level source switching");
      return;
    }

  if (cap == CanNot)
    {
      target->cap_local_explain = xy_strdup (CHINESE ? "无法进行项目级换源" : "Unable to perform project-level source switching");
      return;
    }

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

  const char *msg = CHINESE ? note_zh : note_en;

  if (msg)
    target->note = xy_strdup(msg);
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

  target->authors = xy_malloc0 (count * sizeof(Contributor_t));
  target->authors_n = count;

  for (size_t i = 0; i < count; i++)
    {
      char *name = va_arg(args, char*);
      char *email = va_arg(args, char*);

      target->authors[i].name = xy_strdup(name);
      target->authors[i].email = xy_strdup(email);
    }

  va_end(args);
}


void
chef_set_chef (Target_t *target, char *name, char *email)
{
  if (!target || !name || !email)
    return;

  target->chef = xy_malloc0 (sizeof(Contributor_t));
  target->chef->name  = xy_strdup (name);
  target->chef->email = xy_strdup (email);
}


void
chef_set_cooks (Target_t *target, size_t count, ...)
{
  if (!target)
    return;

  if (count == 0)
    {
      target->cooks = NULL;
      target->cooks_n = 0;
      return;
    }

  va_list args;
  va_start(args, count);

  target->cooks = xy_malloc0 (count * sizeof(Contributor_t));
  target->cooks_n = count;

  for (size_t i = 0; i < count; i++)
    {
      char *name = va_arg(args, char*);
      char *email = va_arg(args, char*);

      target->cooks[i].name = xy_strdup(name);
      target->cooks[i].email = xy_strdup(email);
    }

  va_end(args);
}


void
chef_set_created_on (Target_t *target, char *date)
{
  if (!target)
    return;

  target->created_on = xy_strdup (date);
}


void
chef_set_last_updated (Target_t *target, char *date)
{
  if (!target)
    return;

  target->last_updated = xy_strdup (date);
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
  printf ("  Sources: %p\n", target->sources);
  printf ("  Sources Count: %lld\n", target->sources_n);
  printf ("  Contributors: %p\n", target->contributors);
  printf ("  Contributors Count: %lld\n", target->contributors_n);
}
