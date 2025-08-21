/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : chef.c
 * File Authors  : 曾奥然 <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2025-08-09>
 * Last Modified : <2025-08-20>
 *
 * chef DSL: for chefs (recipe makers) to define a target
 * ------------------------------------------------------------*/

#pragma once

#define chef_allow_gsr(t) this->getfn = t##_getsrc; this->setfn = t##_setsrc; this->resetfn = t##_resetsrc;
#define chef_allow_s(t)   this->getfn = NULL;       this->setfn = t##_setsrc; this->resetfn = NULL;
#define chef_allow_sr(t)  this->getfn = NULL;       this->setfn = t##_setsrc; this->resetfn = t##_resetsrc;
#define chef_allow_gs(t)  this->getfn = t##_getsrc; this->setfn = t##_setsrc; this->resetfn = NULL;

/**
 * @brief 登记所有贡献者
 *
 * @param     id       贡献者 ID，这个ID最好是GitHub用户名，但也可以不是，只需要在 chsrc 内部进行区分即可
 * @param display_name 如果没有提供该参数，则使用 name
 */
void
chef_register_contributor (char *id, char *name, char *email, char *display_name)
{
  if (!id || !name || !email)
    xy_unreached();

  Contributor_t *contributor = xy_malloc0 (sizeof (Contributor_t));
  contributor->id = xy_strdup (id);
  contributor->name = xy_strdup (name);
  contributor->email = xy_strdup (email);

  if (!display_name)
    contributor->display_name = xy_strdup (name);
  else
    contributor->display_name = xy_strdup (display_name);

  xy_map_set (ProgStatus.contributors, id, contributor);
}


/**
 * @brief 修改 Provider 的测速地址
 */
void
chef_set_provider_speed_measure_url (SourceProvider_t *provider, char *url)
{
  provider->psmi.skip = NotSkip;
  provider->psmi.url = xy_strdup (url);
  chsrc_debug ("m", xy_strcat (4, "recipe 重新为 ", provider->code, " 设置测速链接: ", url));
}


/**
 * @brief 修改 Provider 的测速精度
 */
void
chef_set_provider_speed_measure_accuracy (SourceProvider_t *provider, bool accuracy)
{
  provider->psmi.accurate = accuracy;
  chsrc_debug ("m", xy_strcat (4, "recipe 重新为 ", provider->code, " 设置测速精度: ", accuracy ? "精准" : "粗略"));
}


/**
 * @brief 提供一个函数，这个函数基于 "换源 URL" 和用户提供的数据来构造和填充精准测速链接
 */
void
chef_set_sources_speed_measure_url_with_func (
  Target_t *target,
  char *(*func)(const char *url, const char *user_data),
  char *user_data)
{
  xy_cant_be_null (target);

  Source_t *sources = target->sources;
  int n = target->sources_n;

  for (int i=0; i<n; i++)
    {
      Source_t *src = &sources[i];
      ProviderType_t type = src->provider->type;
      if (src->url)
        {
          /* 为空时才修改 或者里面是脏数据 */
          if (NULL==src->speed_measure_url || !chef_is_url (src->speed_measure_url))
            {
              src->speed_measure_url = func (src->url, user_data);
            }
        }
    }
}


/**
 * @brief 给 "换源 URL" 增加一个后缀来构造和填充专用测速链接
 */
void
chef_set_sources_speed_measure_url_with_postfix (Target_t *target, char *postfix)
{
  chef_set_sources_speed_measure_url_with_func (target, xy_2strcat, postfix);
}


void
chef_allow_english (Target_t *target)
{
  xy_cant_be_null (target);
  target->can_english = true;
}

void
chef_forbid_english (Target_t *target)
{
  xy_cant_be_null (target);
  target->can_english = false;
}


void
chef_allow_local_mode (Target_t *target, Capability_t cap, const char *explain_zh, const char *explain_en)
{
  xy_cant_be_null (target);

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
  xy_cant_be_null (target);

  target->can_user_define = true;
  target->can_user_define_explain = NULL;
}

void
chef_forbid_user_define (Target_t *target)
{
  xy_cant_be_null (target);

  target->can_user_define = false;

  char *reason = CHINESE ? "URL将会根据内部实现重写,因此不能自定义"
                         : "The URL will be rewritten based on internal implementation, so it cannot be customized";
  target->can_user_define_explain = reason;
}


void
chef_set_note (Target_t *target, const char *note_zh, const char *note_en)
{
  xy_cant_be_null (target);

  const char *msg = CHINESE ? note_zh : note_en;

  if (msg)
    target->note = xy_strdup (msg);
}



/**
 * @brief 验证该 `id` 所指的贡献者确有其人
 */
Contributor_t *
chef_verify_contributor (const char *id)
{
  xy_cant_be_null (id);

  Contributor_t *c = xy_map_get (ProgStatus.contributors, id);
  if (!c)
    {
      char error[256];
      snprintf (error, sizeof (error), "贡献者不存在: %s", id);
      chsrc_panic (error);
    }
  return c;
}


void
chef_set_chef (Target_t *target, const char *id)
{
  xy_cant_be_null (target);

  Contributor_t *c = chef_verify_contributor (id);

  target->chef = c;
}


void
chef_set_cooks (Target_t *target, size_t count, ...)
{
  xy_cant_be_null (target);

  if (count == 0)
    {
      target->cooks = NULL;
      target->cooks_n = 0;
      return;
    }

  va_list args;
  va_start (args, count);

  target->cooks = xy_malloc0 (count * sizeof (Contributor_t*));
  target->cooks_n = count;

  for (size_t i = 0; i < count; i++)
    {
      char *id = va_arg (args, char*);
      target->cooks[i] = chef_verify_contributor (id);
    }

  va_end (args);
}

void
chef_set_contributors (Target_t *target, uint32_t count, ...)
{
  xy_cant_be_null (target);

  if (count == 0)
    {
      target->contributors = NULL;
      target->contributors_n = 0;
      return;
    }

  va_list args;
  va_start (args, count);

  target->contributors = xy_malloc0 (count * sizeof (Contributor_t*));
  target->contributors_n = count;

  for (uint32_t i = 0; i < count; i++)
    {
      char *id = va_arg (args, char*);
      target->contributors[i] = chef_verify_contributor (id);
    }
}



void
chef_set_created_on (Target_t *target, char *date)
{
  xy_cant_be_null (target);
  xy_cant_be_null (date);

  target->created_on = xy_strdup (date);
}


void
chef_set_last_updated (Target_t *target, char *date)
{
  xy_cant_be_null (target);
  xy_cant_be_null (date);

  target->last_updated = xy_strdup (date);
}


void
chef_set_sources_last_updated (Target_t *target, char *date)
{
  xy_cant_be_null (target);
  xy_cant_be_null (date);

  target->sources_last_updated = xy_strdup (date);
}


void
chef_debug_target (Target_t *target)
{
#ifdef XY_DEBUG
  if (!target)
    {
      chsrc_debug2 ("t", "Target is NULL");
      return;
    }

  say ("Debug Target Information:");
  printf ("  Get Function: %p\n", target->getfn);
  printf ("  Set Function: %p\n", target->setfn);
  printf ("  Reset Function: %p\n", target->resetfn);
  printf ("  Sources: %p\n", target->sources);
  printf ("  Sources Count: %lld\n", target->sources_n);

  printf ("  Authors: %p\n", target->authors);
  printf ("  Authors Count: %lld\n", target->authors_n);
  printf ("  Chef: %p\n", target->chef);
  printf ("  Cooks: %p\n", target->cooks);
  printf ("  Cooks Count: %lld\n", target->cooks_n);
  printf ("  Contributors: %p\n", target->contributors);
  printf ("  Contributors Count: %lld\n", target->contributors_n);
#endif
}
