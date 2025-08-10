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
#define FeedByPrelude NULL

#define def_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)


typedef enum Capability_t
{
  CanNot,
  FullyCan,
  PartiallyCan
}
Capability_t;



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

  Source_t  *sources;
  size_t    sources_n;


  /* Features */
  bool  can_english;        /* 是否支持英文输出 */

  bool  can_user_define;    /* 是否支持用户自定义URL来换源 */
  char *can_user_define_explain; /* 用户自定义URL的说明 */

  Capability_t cap_local;  /* 是否支持 local mode */
  char *cap_local_explain; /* local mode 的说明 */

  char *note;              /* 备注 */


  /* Recipe maintain info */
  char *created_on;
  char *last_updated;
  char *sources_last_updated;

  Contributor_t *authors;
  size_t         authors_n;

  Contributor_t *contributors;
  size_t         contributors_n;

  Contributor_t *chef;        /* Chef 仅有一个 */
  Contributor_t *sous_chefs;  /* Sous Chef 可以有多个 */
  size_t         sous_chefs_n;
}
Target_t;


typedef struct TargetRegisterInfo_t
{
  Target_t *target;           /* target 本身 */
  void     (*prelude) (void); /* 填充 target 信息等预置操作 */
  char     *aliases;          /* 以空格分隔的 alias 字符串 */
}
TargetRegisterInfo_t;


#define def_target(t) void t##_getsrc(char *option);void t##_setsrc(char *option);void t##_resetsrc(char *option); Target_t t##_target={0};

/* 以下宏仅能放在 prelude() 中使用 */
#define use_this(t) Target_t *this = &t##_target;
#define use_this_source(t) use_this(t); Source_t source = chsrc_yield_source_and_confirm (this, option);

#define def_sources_begin()  SourceProvider_t upstream = UpstreamProvider; Source_t sources[] = {
#define def_sources_end()    }; \
  this->sources_n = xy_arylen(sources); \
  char *_sources_storage = xy_malloc0 (sizeof(sources)); \
  memcpy (_sources_storage, sources, sizeof(sources)); \
  this->sources = (Source_t *)_sources_storage;
