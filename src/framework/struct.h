/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : struct.h
 * File Authors  : @ccmywish
 *               | @G_I_Y
 * Contributors  : @livelycode36
 *               | @Mikachu2333
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2026-10-28>
 *
 * chsrc struct
 * ------------------------------------------------------------*/

typedef struct ProviderSpeedMeasureInfo_t
{
  bool  skip;           /* 是否默认跳过 */
  char *skip_reason_ZH; /* 跳过的原因（中文）*/
  char *skip_reason_EN; /* 跳过的原因（英文）*/
  char *url;            /* 测速链接 */
  bool  accurate;       /* 是否为精准测速，上游源和专用镜像站为 ACCURATE，通用镜像站为 ROUGH */
}
ProviderSpeedMeasureInfo_t;

#define SKIP    true
#define NotSkip false
#define ToFill  NULL
#define NA		  NULL

#define ACCURATE true
#define ROUGH    false

typedef enum ProviderType_t
{
  IS_GeneralMirrorSite,   /* 通用镜像站 */
  IS_DedicatedMirrorSite, /* 专用镜像站 */
  IS_UpstreamProvider,    /* 上游默认源 */
  IS_UserDefinedProvider, /* 用户提供   */
}
ProviderType_t;

typedef struct SourceProvider_t
{
  const ProviderType_t type; /* 类型 */
  const char *code; /* 用于用户指定某一 Provider */
  const char *abbr; /* 需要使用 Provider 的英文名时，用这个代替，因为大部分 Provider 没有提供正式的英文名 */
  const char *name; /* Provider 中文名 */
  const char *site; /* Provider 首页   */
  ProviderSpeedMeasureInfo_t psmi;
}
SourceProvider_t;

typedef SourceProvider_t MirrorSite_t;

SourceProvider_t UpstreamProvider =
{
  IS_UpstreamProvider,
  /* 引入新的上游默认源时，请使下面第一行的前三个字段保持不变，只添加第四个字段 */
  "upstream", "Upstream", "上游默认源", NULL,
  /* 引入新的上游默认源时，请完全修改下面这个结构体，可使用 def_need_measure_info 宏 */
  {SKIP, "URL未知，邀您参与贡献!", "URL unknown, welcome to contribute!", NULL, ACCURATE}
};

#define def_need_measure_info   {SKIP, "缺乏较大的测速对象，邀您参与贡献!", "Lack of large object URL, welcome to contribute!", NULL, ACCURATE}

SourceProvider_t UserDefinedProvider =
{
  IS_UserDefinedProvider,
  "user", "用户自定义", "用户自定义", NULL,
  {SKIP, "用户自定义源不测速", "SKIP for user-defined source", NULL, ACCURATE}
};


typedef struct Source_t
{
  union {
    SourceProvider_t *provider;
    MirrorSite_t     *mirror;
  };
  /* 用于换源的 URL，也称 repoURL */
  char *url;

  /* 对该 source 的专用测速链接，这就是精准测速，也称 smURL */
  char *speed_measure_url;
}
Source_t;

/* 不用给专用测速链接，因为 Upstream 的整体测速链接已是精准测速 */
#define DelegateToUpstream  NULL
/* 不用给专用测速链接，因为该镜像站是专用镜像站，其整体测速链接已是精准测速 */
#define DelegateToMirror    NULL
/* 看到该注释的贡献者，你可以帮忙寻找专用测速链接 */
#define NeedContribute      NULL
/* 由 _prepare() 填充 */
#define FeedByPrepare NULL


/**
 * 换源的作用域
 *
 * 在 chsrc v0.2.4.2 以前，我们一直使用的是 `-local` 这个选项，其含义是启用 *项目级* 换源
 *
 * 1. 默认不使用该选项时，含义是 *全局* 换源，
 *
 *    全局分为 (1)系统级 (2)用户级
 *
 *    大多数第三方配置软件往往默认进行的是 *用户级* 的配置。所以 chsrc 首先将尝试使用 *用户级* 配置
 *
 * 2. 若不存在 *用户级* 的配置，chsrc 将采用 *系统级* 的配置
 *
 * 3. 最终效果本质由第三方软件决定，如 poetry 默认实现的就是项目级的换源
 *
 * 但是后来，我们认为非 -local 时的行为（即默认时）比较模糊，所以我们现在清晰地把作用域指明出来，总共有3种类型的作用：
 * 分别是 ProjectScope、UserScope 和 SystemScope，分别对应项目级、用户级和系统级的换源配置
 *
 * 还有一个叫 ImplementationDefinedScope 的作用域，它不是一种新类型，而是表示根据实际情况决定的作用域。
 * chsrc 将根据该 dish 的实际情况来选择最合适的作用域来进行换源配置。最好的情况下，ImplementationDefinedScope 是三者之一，
 * 这也是这里设计的初衷。然而现在有些 recipe 的换源行为，会在某种 Scope 不能够成功时退而求其次地使用另一个 Scope
 * 来进行换源配置，这时就只能用 ImplementationDefinedScope 来表示。
 */
#define NumberOfScopeType    3
 typedef enum Scope_t
{
  ProjectScope,
  UserScope,
  SystemScope,

  /**
   * 这是 dish 默认的作用域，一种特殊的作用域，即根据 dish 的实际情况来决定的。
   * 它不是一种真正的类型，因为最终换源后，用户看到的作用域依然是 ProjectScope、UserScope 或 SystemScope 中的一个
   */
  ImplementationDefinedScope,
}
Scope_t;

#define ScopeCap_Slot_Project 0
#define ScopeCap_Slot_User    1
#define ScopeCap_Slot_System  2

typedef enum ScopeCapability_t
{
  ScopeCap_Unknown,                  /* 未知，缺乏对该 dish 的细致了解 */
  ScopeCap_Unable,                   /* 不支持该作用域 */
  ScopeCap_Able_But_Not_Implemented, /* 支持但chsrc尚未实现 */
  ScopeCap_Able_But_Just_Docs,       /* 支持但chsrc仅提供文档指引 */
  ScopeCap_Able_And_Implemented      /* 支持且chsrc已经实现 */
}
ScopeCapability_t;

static inline bool
scope_cap_is_runnable (ScopeCapability_t cap)
{
  return (   cap == ScopeCap_Able_And_Implemented
          || cap == ScopeCap_Able_But_Just_Docs);
}

typedef enum Capability_t
{
  CanNot,
  FullyCan,
  PartiallyCan
}
Capability_t;



typedef struct Contributor_t
{
  char *id;     /* 全局唯一贡献者标识符，防止反复写信息，以 @ 开头 */
  char *name;   /* 贡献者姓名; 鉴于该项目完全依赖于贡献者，建议留下真实姓名或者昵称 */
  char *email;
  char *display_name; /* recipe 结束时会显示贡献者信息，如果你不愿显示真实姓名或者昵称，可以另外提供一个名字 */
}
Contributor_t;


typedef struct Dish_t
{
  /* 以 / 为分隔符的多个菜品别名 */
  char *aliases;

  void (*getfn)   (char *option);
  void (*setfn)   (char *option);
  void (*resetfn) (char *option);

  /**
   * 初始化函数，用于填充该 struct 的各种信息
   *
   * 值得注意的是，preparefn() 将初始化该结构体内三个最重要的函数:
   * 即 getfn setfn resetfn 的函数地址，但是 preparefn 这个函数自
   * 身的函数地址不可能由自己初始化，所以需要额外的位置初始化:
   *
   *   1. menu.c 中通过 add() 注册
   *   2. recipe 内部手动调用，如 "sources only dish"
   */
  void (*preparefn) (void);
  bool   prepared; /* 是否执行过了 preparefn() */

  XySeq_t *sub_dishes; /* 某些 dish 为 combo dish，完全由 sub dishes 定义 */
  /**
   * 该字段有2个作用:
   *   1. 确认该 combo dish 是否可以由用户提供源
   *   2. 避免重复测速
   *
   * @ref https://github.com/RubyMetric/chsrc/issues/358#issuecomment-5350147424
   */
  bool all_sub_dishes_use_same_source;


  Source_t *sources;
       int  sources_n;

  struct Dish_t *sources_dish; /* 有些 dish 用的是其它 dish 的 sources */


  /* Features */
  bool  can_english;        /* 是否支持英文输出 */

  bool  can_user_define;         /* 是否支持用户自定义URL来换源 */
  char *user_define_cap_explain; /* 用户不能自定义URL的原因解释 */

  /**
   * 各作用域的支持情况
   * 参考 ScopeCap_Slot_Xxx 的值
   */
  ScopeCapability_t scope_caps[NumberOfScopeType];
  Scope_t default_scope;   /* 默认作用域 */

  char *note;              /* 备注 */


  /* recipe 维护信息 */
  char *created_on;
  char *last_updated;

  XySeq_t *chefs;    /* 该 recipe 的主要作者   */
  XySeq_t *sauciers; /* 该 recipe 的次要贡献者 */
}
Dish_t;
