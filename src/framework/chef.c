/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : chef.c
 * File Authors  : 曾奥然 <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2025-08-09>
 * Last Modified : <2025-08-22>
 *
 * chef DSL: for chefs (recipe makers) to define a target
 * ------------------------------------------------------------*/

#pragma once

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

  xy_map_set (ProgStore.contributors, id, contributor);
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


/**
 * @note 用于: 组中的 item target 在 standalone 模式时正确填充源信息
 */
void
chef_use_other_target_sources (Target_t *this, Target_t *other)
{
  if (!other->inited) other->preludefn();
  this->sources = other->sources;
  this->sources_n = other->sources_n;
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

  Contributor_t *c = xy_map_get (ProgStore.contributors, id);
  if (!c)
    {
      char error[256];
      snprintf (error, sizeof (error), "贡献者 %s 不存在, 是否写错？或请在 chsrc-main.c 中登记该贡献者", id);
      chsrc_panic (error);
    }
  return c;
}


/**
 * @brief 设置 Chef (recipe 负责人)
 */
void
chef_set_chef (Target_t *target, const char *id)
{
  xy_cant_be_null (target);

  /* Chef 可为空 */
  if (!id)
    {
      target->chef = NULL;
      return;
    }

  Contributor_t *c = chef_verify_contributor (id);
  target->chef = c;
}


/**
 * @brief 设置 Cooks (recipe 核心作者)
 */
void
chef_set_cooks (Target_t *target, size_t count, ...)
{
  xy_cant_be_null (target);

  if (count == 0)
    {
      chsrc_panic ("recipe 一定至少有1位作者(cooks)");
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
      target->sauciers = NULL;
      target->sauciers_n = 0;
      return;
    }

  va_list args;
  va_start (args, count);

  target->sauciers = xy_malloc0 (count * sizeof (Contributor_t*));
  target->sauciers_n = count;

  for (uint32_t i = 0; i < count; i++)
    {
      char *id = va_arg (args, char*);
      target->sauciers[i] = chef_verify_contributor (id);
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
      chsrc_debug2 ("target", "Target is NULL");
      return;
    }

  say ("DEBUG Target Information:");
  printf ("  Get Function: %p\n", target->getfn);
  printf ("  Set Function: %p\n", target->setfn);
  printf ("  Reset Function: %p\n", target->resetfn);
  printf ("  Sources: %p\n", target->sources);
  printf ("  Sources Count: %lld\n", target->sources_n);

  printf ("  Chef: %p\n", target->chef);
  printf ("  Cooks: %p\n", target->cooks);
  printf ("  Cooks Count: %lld\n", target->cooks_n);
  printf ("  Contributors: %p\n", target->contributors);
  printf ("  Contributors Count: %lld\n", target->contributors_n);
#endif
}


void
chef_debug_contributor (Contributor_t *contributor)
{
#ifdef XY_DEBUG
  if (!contributor)
    {
      chsrc_debug2 ("contrib", "Contributor is NULL");
      return;
    }

  say ("DEBUG Contributor Information:");
  printf ("  ID:    %s\n", contributor->id);
  printf ("  Name:  %s\n", contributor->name);
  printf ("  Email: %s\n", contributor->email);
  printf ("  DisplayName: %s\n", contributor->display_name);
#endif
}
