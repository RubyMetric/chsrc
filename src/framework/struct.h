/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     :  struct.h
 * File Authors  :  Aoran Zeng   <ccmywish@qq.com>
 *               |   Heng Guo    <2085471348@qq.com>
 * Contributors  : Shengwei Chen <414685209@qq.com>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2025-08-09>
 *
 * chsrc struct
 * ------------------------------------------------------------*/

typedef struct ProviderSpeedMeasureInfo_t
{
  bool  skip;           /* 是否默认跳过 */
  char *skip_reason_CN; /* 跳过的原因（中文）*/
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
  /* 用于换源的 URL */
  char *url;

  /* 对该 source 的专用测速链接，这就是精准测速 */
  char *speed_measure_url;
}
Source_t;

/* 不用给专用测速链接，因为 Upstream 的整体测速链接已是精准测速 */
#define DelegateToUpstream  NULL
/* 不用给专用测速链接，因为该镜像站是专用镜像站，其整体测速链接已是精准测速 */
#define DelegateToMirror    NULL
/* 看到该注释的贡献者，你可以帮忙寻找专用测速链接 */
#define NeedContribute      NULL
/* 由 _sources_prepare 填充 */
#define FeedBySourcesPrepare NULL
/* 由 prelude() 修改 */
#define ChangedByPrelude NULL

#define def_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)


typedef enum Capability_t
{
  CanNot,
  FullyCan,
  PartiallyCan
}
Capability_t;


/* Target Feature */
typedef struct Feature_t
{
  bool can_get;
  bool can_reset;       /* 有的reset不是暂时没有实现，而是现在的实现根本就无法重置 */

  bool can_english;

  bool can_user_define;

  Capability_t cap_locally;
  char *cap_locally_explain;

  char *note;
}
Feature_t;


typedef struct Contributor_t
{
  char *name;
  char *email;
}
Contributor_t;


typedef struct Target_t
{
  char *name; /* 目标名称，该名称必须是 menu 中的 aliase 之一 */

  void (*getfn)   (char *option);
  void (*setfn)   (char *option);
  void (*resetfn) (char *option);

  Source_t  sources;
  size_t    sources_n;

  bool can_english;        /* 是否支持英文输出 */
  bool can_user_define;    /* 是否支持用户自定义URL来换源 */
  Capability_t cap_local;  /* 是否支持 local mode */
  char *cap_local_explain; /* local mode 的说明 */
  char *note;              /* 备注 */

  Contributor_t *recipe_authors;
  size_t         recipe_authors_n;

  Contributor_t *contributors;
  size_t         contributors_n;

  Contributor_t *current_chef;        // 只有一个主维护者
  Contributor_t *current_sous_chefs;  // 可以有多个副维护者
  size_t         current_sous_chefs_n;

  char *recipe_created_on;
  char *recipe_last_updated;
  char *sources_last_updated;
}
Target_t;


typedef struct TargetRegisterInfo_t
{
  Target_t *target;           /* target 本身 */
  void     (*prelude) (void); /* 填充 target 信息等预置操作 */
  char     *aliases;          /* 以空格分隔的 alias 字符串 */
}
TargetRegisterInfo_t;


// #define def_target_inner_s(t)    NULL,       t##_setsrc, NULL,         NULL
// #define def_target_inner_sr(t)   NULL,       t##_setsrc, t##_resetsrc, NULL
// #define def_target_inner_sf(t)   NULL,       t##_setsrc, NULL,         t##_feat
// #define def_target_inner_srf(t)  NULL,       t##_setsrc, t##_resetsrc, t##_feat
// #define def_target_inner_gs(t)   t##_getsrc, t##_setsrc, NULL,         NULL
// #define def_target_inner_gsr(t)  t##_getsrc, t##_setsrc, t##_resetsrc, NULL
// #define def_target_inner_gsf(t)  t##_getsrc, t##_setsrc, NULL,         t##_feat
// #define def_target_inner_gsrf(t) t##_getsrc, t##_setsrc, t##_resetsrc, t##_feat

#define def_target_sourcesn(t)   t##_sources, t##_sources_n

/* 大部分target还不支持reset，所以暂时先默认设置为NULL来过渡 */
// #define def_target(t)      Target_t t##_target = {def_target_inner_gs(t),def_target_sourcesn(t)}
// #define def_target_s(t)    Target_t t##_target = {def_target_inner_s(t),def_target_sourcesn(t)}
// #define def_target_sr(t)   Target_t t##_target = {def_target_inner_sr(t),def_target_sourcesn(t)}
// #define def_target_sf(t)   Target_t t##_target = {def_target_inner_sf(t),def_target_sourcesn(t)}
// #define def_target_srf(t)  Target_t t##_target = {def_target_inner_srf(t),def_target_sourcesn(t)}
// #define def_target_gs(t)   Target_t t##_target = {def_target_inner_gs(t),def_target_sourcesn(t)}
// #define def_target_gsr(t)  Target_t t##_target = {def_target_inner_gsr(t),def_target_sourcesn(t)}
// #define def_target_gsf(t)  Target_t t##_target = {def_target_inner_gsf(t),def_target_sourcesn(t)}
// #define def_target_gsrf(t) Target_t t##_target = {def_target_inner_gsrf(t),def_target_sourcesn(t)}

#define def_target(t) void t##_getsrc(char *option);void t##_setsrc(char *option);void t##_resetsrc(char *option); Target_t t##_target={0};

/* 以下宏仅能放在 prelude() 中使用 */
#define use_this(t) Target_t *this = &t##_target;

#define def_sources_begin()  Source_t sources[] = {
#define def_sources_end()    }; this->sources = sources; this->sources_n = xy_arylen(sources);

#define def_upstream(url) SourceProvider_t upstream = UpstreamProvider; upstream.site = url;
