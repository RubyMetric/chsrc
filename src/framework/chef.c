/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : chef.c
 * File Authors  : @ccmywish
 * Contributors  : @BingChunMoLi
 *               | @Mikachu2333
 * Created On    : <2025-08-09>
 * Last Modified : <2026-10-28>
 *
 * 本文件的函数能够帮助 chefs 为一个 dish 定义 recipe
 * ------------------------------------------------------------*/

#define def_dish(t, aliases) void t##_getsrc(char *option);void t##_setsrc(char *option);void t##_resetsrc(char *option); Dish_t t##_dish={aliases};

/* combo dish 不需要实现任何操作(除了preparefn)即可使用 */
#define def_combo_dish(t, aliases) Dish_t t##_dish={aliases};

/* 仅内部使用的 dish，只用来存储源信息，请参考 pl_nodejs_binary 以及 pl_pypi */
#define def_sources_only_dish(t, name) Dish_t t##_dish={"__internal__sources_only_dish__" name "__"}



#define chef_allow_gsr(t) this->getfn = t##_getsrc; this->setfn = t##_setsrc; this->resetfn = t##_resetsrc;
#define chef_allow_s(t)   this->getfn = NULL;       this->setfn = t##_setsrc; this->resetfn = NULL;
#define chef_allow_sr(t)  this->getfn = NULL;       this->setfn = t##_setsrc; this->resetfn = t##_resetsrc;
#define chef_allow_gs(t)  this->getfn = t##_getsrc; this->setfn = t##_setsrc; this->resetfn = NULL;
#define chef_allow_NOOP(t)
#define chef_prep_this_dish(t,op) Dish_t *this = &t##_dish; this->prepared = true; chef_allow_##op(t);

/* 简化 sources only dish 的编写 */
#define chef_prep_this_sources_only_dish(t) Dish_t *this = &t##_dish; this->prepared = true; chef_allow_NOOP(t);

#define chef_prep_this_combo_dish(t) Dish_t *this = &t##_dish; this->prepared = true; chef_allow_NOOP(t);

/* 内部 sources only dish 的 preparefn 未通过 menu.c 的 add() 注册, 手动挂载 */
#define chef_set_preparefn_for_sources_only_dish(t) t##_dish.preparefn = t##_prepare;


#define chef_use_this(t) Dish_t *this = &t##_dish;
#define chsrc_use_this_source(t) Dish_t *this = &t##_dish; Source_t source = chsrc_yield_source_and_confirm (this, option);

/**
 * 用于定义换源列表
 *
 *   def_sources_begin()
 *   {&UpstreamProvider, "换源URL", "精准测速链接"},
 *   {&某镜像站1,        "换源URL", "精准测速链接"},
 *   {&某镜像站2,        "换源URL",  NULL },  // 若精准测速链接为空，则为模糊测速，默认使用该镜像站的整体测速链接
 *   def_sources_end()
 *
 *   出于代码美观考虑，上述第三列可以写 FeedByPrepare，然后下面使用文档 ./doc/11-如何设置换源链接与测速链接.md 中的函数来填充
 */
#define def_sources_begin()  Source_t sources[] = {
#define def_sources_end()    }; \
  this->sources_n = xy_c_array_len(sources); \
  char *_sources_storage = xy_malloc0 (sizeof(sources)); \
  memcpy (_sources_storage, sources, sizeof(sources)); \
  this->sources = (Source_t *)_sources_storage;


void
chef_debug_dish (Dish_t *dish)
{
#ifdef XY_DEBUG
  if (!dish)
    {
      chsrc_debug2 ("dish", "Dish is NULL");
      return;
    }

  say ("DEBUG Dish Information:");
  printf ("  Aliases: %s\n", dish->aliases);
  printf ("  Prepare Function: %p\n", dish->preparefn);
  printf ("  Prepared?: %d\n", dish->prepared);

  if (dish_has_sub_dishes(dish))
    {
      printf ("  Sub Dishes Count: %d\n", xy_seq_len(dish->sub_dishes));
    }
  else
    {
      printf ("  Get Function: %p\n", dish->getfn);
      printf ("  Set Function: %p\n", dish->setfn);
      printf ("  Reset Function: %p\n", dish->resetfn);

      printf ("  Sources: %p\n", dish->sources);
      printf ("  Sources Count: %d\n", dish->sources_n);

      printf ("  Chefs Count: %d\n", xy_seq_len(dish->chefs));
      printf ("  Sauciers Count: %d\n", xy_seq_len(dish->sauciers));
    }
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
 *
 * @note 这个修改的是全局 Provider 里的信息。往往用来设置 UpstreamProvider
 */
void
chef_set_provider_smURL (SourceProvider_t *provider, char *url)
{
  provider->psmi.skip = NotSkip;
  provider->psmi.url = xy_strdup (url);
  chsrc_debug ("chef", xy_strcat (4, "recipe 重新为 ", provider->code, "(镜像站信息本身) 设置测速链接: ", url));
}


/**
 * @brief 修改 Provider 的测速精度
 *
 * @note 这个修改的是全局 Provider 里的信息。往往用来设置 UpstreamProvider
 */
void
chef_set_provider_sm_accuracy (SourceProvider_t *provider, bool accuracy)
{
  provider->psmi.accurate = accuracy;
  chsrc_debug ("chef", xy_strcat (4, "recipe 重新为 ", provider->code, "(镜像站信息本身) 设置测速精度: ", accuracy ? "精准" : "粗略"));
}


/**
 * @brief 修改 或 补充 某个镜像站的换源链接，即修改 Source_t.url
 *
 * @example 见 os_ubuntu_resetsrc() 中对非 x86_64 架构源地址的修改
 */
void
chef_set_repoURL (Dish_t *dish, SourceProvider_t *provider, char *url)
{
  xy_cant_be_null (dish);
  xy_cant_be_null (provider);
  xy_cant_be_null (url);

  for (int i=0; i < dish->sources_n; i++)
    {
      Source_t *src = &dish->sources[i];
      SourceProvider_t *p = src->provider;
      if (p == provider)
        {
          src->url = xy_strdup (url);
          return;
        }
    }

  xy_unreached();
}


/**
 * @brief 提供一个函数，这个函数基于 "换源链接" 和用户提供的数据来构造和填充精准测速链接
 */
void
chef_set_smURL_with_func (
  Dish_t *dish,
  SourceProvider_t *provider,
  char *(*func)(const char *url, const char *user_data),
  char *user_data)
{
  xy_cant_be_null (dish);
  xy_cant_be_null (provider);
  xy_cant_be_null (func);

  for (int i=0; i < dish->sources_n; i++)
    {
      Source_t *src = &dish->sources[i];
      SourceProvider_t *p = src->provider;
      if (p == provider)
        {
          if (src->url)
            {
              src->speed_measure_url = func (src->url, user_data);
              return;
            }
          else
            {
              chsrc_breakdown ("该函数基于已有的换源链接来生成测速链接，但该源的换源链接为空");
            }
        }
    }

  xy_unreached();
}


/**
 * @brief 给 "换源链接" 增加一个后缀来构造和填充专用测速链接
 */
void
chef_set_smURL_with_postfix (Dish_t *dish, SourceProvider_t *provider, char *postfix)
{
  chef_set_smURL_with_func (dish, provider, xy_2strcat, postfix);
}


/**
 * @internal 该函数仅用于实现 chef_set_smURL()
 */
char *
_chef_strdup_2nd_argument (const char *DUMMY, const char *str)
{
  return xy_strdup (str);
}

/**
 * @breif 设置 或 修改 某个镜像站的 *精准*测速链接，即修改 Source_t.speed_measure_url
 */
void
chef_set_smURL (Dish_t *dish, SourceProvider_t *provider, char *url)
{
  chef_set_smURL_with_func (dish, provider, _chef_strdup_2nd_argument, url);
}


/**
 * @brief 针对每一个剩下的还未设置专用测速链接的源，对其 "换源链接" 使用函数 `func` 来生成专用测速链接
 */
void
chef_set_rest_smURL_with_func (
  Dish_t *dish,
  char *(*func)(const char *url, const char *user_data),
  char *user_data)
{
  xy_cant_be_null (dish);

  Source_t *sources = dish->sources;
  int n = dish->sources_n;

  for (int i=0; i<n; i++)
    {
      Source_t *src = &sources[i];
      ProviderType_t type = src->provider->type;
      if (src->url)
        {
          /* 为空时才修改 或者里面是脏数据 */
          if (NULL==src->speed_measure_url || !hp_is_url (src->speed_measure_url))
            {
              src->speed_measure_url = func (src->url, user_data);
            }
        }
    }
}


/**
 * @brief 针对每一个剩下的还未设置专用测速链接的源，对其 "换源链接" 增加一个后缀来生成专用测速链接
 */
void
chef_set_rest_smURL_with_postfix (Dish_t *dish, char *postfix)
{
  chef_set_rest_smURL_with_func (dish, xy_2strcat, postfix);
}


void
chef_use_other_dish_sources (Dish_t *this, Dish_t *other)
{
  if (!other->prepared)
    {
      if (other->preparefn)
        other->preparefn();
      else
        {
          chef_debug_dish (other);
          chsrc_breakdown ("`other` 未定义 _prepare() !");
        }
    }

  this->sources_dish = other;
  this->sources = other->sources;
  this->sources_n = other->sources_n;
}


void
chef_allow_english (Dish_t *dish)
{
  xy_cant_be_null (dish);
  dish->can_english = true;
}

void
chef_deny_english (Dish_t *dish)
{
  xy_cant_be_null (dish);
  dish->can_english = false;
}


/**
 * @brief 设置该 dish 的作用域能力
 */
void
chef_set_scope_cap (Dish_t *dish, Scope_t scope, ScopeCapability_t cap)
{
  xy_cant_be_null (dish);

  /* 我们在这里固定好索引的位置，而不是直接用 enum 的值，防止以后顺序或者新增枚举值 */
  if (scope == ProjectScope)
    {
      dish->scope_caps[ScopeCap_Slot_Project] = cap;
    }
  else if (scope == UserScope)
    {
      dish->scope_caps[ScopeCap_Slot_User] = cap;
    }
  else if (scope == SystemScope)
    {
      dish->scope_caps[ScopeCap_Slot_System] = cap;
    }
  else
    {
      chsrc_breakdown ("无效的 scope 参数");
    }
}


/**
 * @brief 设置该 dish 的默认作用域
 *
 * @note 该函数必须在 chef_set_scope_cap() 之后调用，以确保默认作用域的能力已经被明确了
 */
void
chef_set_default_scope (Dish_t *dish, Scope_t scope)
{
  xy_cant_be_null (dish);
  dish->default_scope = scope;

  ScopeCapability_t cap = ScopeCap_Unknown;

  if (scope == ProjectScope)
    cap = dish->scope_caps[ScopeCap_Slot_Project];
  else if (scope == UserScope)
    cap = dish->scope_caps[ScopeCap_Slot_User];
  else if (scope == SystemScope)
    cap = dish->scope_caps[ScopeCap_Slot_System];
  else if (scope == ImplementationDefinedScope)
    {
      /* ImplementationDefinedScope 即由 chsrc 根据实际情况来决定，因此我们不对它检查 */
      return;
    }
  else
    {
      chsrc_breakdown ("无效的 scope 参数");
    }

  /* 防止 chef 们写错 */
  if (!scope_cap_is_runnable (cap))
    {
      chsrc_breakdown ("该作用域未被明确支持，无法设置为默认作用域");
    }
}


/**
 * @brief 由于操作系统相关的 dish 换源都是系统级，所以 scope 都是固定的，我们提供此快捷函数来设置
 */
void
chef_set_os_scope (Dish_t *dish)
{
  xy_cant_be_null (dish);

  chef_set_scope_cap (dish, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (dish, UserScope,    ScopeCap_Unable);
  chef_set_scope_cap (dish, SystemScope,  ScopeCap_Able_And_Implemented);

  chef_set_default_scope (dish, SystemScope);
}



void
chef_allow_user_define (Dish_t *dish)
{
  xy_cant_be_null (dish);

  dish->can_user_define = true;
  dish->user_define_cap_explain = NULL;
}

void
chef_deny_user_define (Dish_t *dish)
{
  xy_cant_be_null (dish);

  dish->can_user_define = false;

  dish->user_define_cap_explain = NULL;
}


void
chef_set_note (Dish_t *dish, const char *note_zh, const char *note_en)
{
  xy_cant_be_null (dish);

  const char *msg = CHINESE ? note_zh : note_en;

  if (msg)
    dish->note = xy_strdup (msg);
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
      chsrc_breakdown (error);
    }
  return c;
}



/**
 * @brief 设置 Chefs (recipe 主要作者)
 */
void
chef_set_chefs (Dish_t *dish, size_t count, ...)
{
  xy_cant_be_null (dish);

  if (count == 0)
    {
      chsrc_breakdown ("recipe 一定至少有1位主要作者(chefs)");
      return;
    }

  va_list args;
  va_start (args, count);

  dish->chefs = xy_seq_new ();

  for (size_t i = 0; i < count; i++)
    {
      char *id = va_arg (args, char*);
      xy_seq_push (dish->chefs, chef_verify_contributor (id));
    }

  va_end (args);
}

void
chef_set_sauciers (Dish_t *dish, uint32_t count, ...)
{
  xy_cant_be_null (dish);

  dish->sauciers = xy_seq_new ();

  if (count == 0)
    {
      return;
    }

  va_list args;
  va_start (args, count);

  for (uint32_t i = 0; i < count; i++)
    {
      char *id = va_arg (args, char*);
      xy_seq_push (dish->sauciers, chef_verify_contributor (id));
    }
  va_end (args);
}



void
chef_set_sub_dishes (Dish_t *dish, uint32_t count, ...)
{
  xy_cant_be_null (dish);

  dish->sub_dishes = xy_seq_new ();

  if (count < 1)
    {
      chsrc_breakdown ("套餐必须至少有一个子菜品");
    }

  va_list args;
  va_start (args, count);

  for (uint32_t i = 0; i < count; i++)
    {
      Dish_t *sub_dish = va_arg (args, Dish_t*);
      xy_seq_push (dish->sub_dishes, sub_dish);
    }
  va_end (args);
}



void
chef_set_recipe_created_on (Dish_t *dish, char *date)
{
  xy_cant_be_null (dish);
  xy_cant_be_null (date);

  dish->created_on = xy_strdup (date);
}


void
chef_set_recipe_last_updated (Dish_t *dish, char *date)
{
  xy_cant_be_null (dish);
  xy_cant_be_null (date);

  dish->last_updated = xy_strdup (date);
}



/**
 * @note 某些 dish 需要修改 User-Agent
 * 由于单独测速 (chsrc measure) 的时候也需要进行此项修改，
 * 所以该函数不能仅仅放在 _setsrc() 里，而是应当放在 _prepare() 里
 */
void
chef_set_user_agent (char *user_agent)
{
  ProgStatus.user_agent = user_agent;
}


void
chef_all_sub_dishes_use_same_source_from (Dish_t *combo_dish, Dish_t *sources_dish)
{
  xy_cant_be_null (combo_dish);

  combo_dish->all_sub_dishes_use_same_source = true;
  combo_dish->sources_dish = sources_dish;
}
