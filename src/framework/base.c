/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : base.c
 * File Authors  : @ccmywish
 *               | @G_I_Y
 * Contributors  : @happy-game
 *               | @BingChunMoLi
 *               | @Mikachu2333
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2026-09-05>
 *
 * framework foundation
 * ------------------------------------------------------------*/

#if defined(__STDC__) && __STDC_VERSION__ < 201112L
#   error "chsrc requires C11 or later, please use a new compiler which at least supports C11"
#endif

#if defined(__STDC__) && __STDC_VERSION__ < 201710L
#   warning "chsrc recommends a C17 or later compiler"
#endif

#include "xy.h"
#include "struct.h"
#include "mirror.c"
#include "helper.c"

#define App_Name "chsrc"

/* Global Program Mode */
struct
{
  // 用户命令
  bool MeasureMode;
  bool ResetMode;

  // 用户命令选项
  bool Ipv6Mode;
  Scope_t Scope;
  bool EnglishMode;
  bool DryRunMode;
  bool NoColorMode;
}
ProgMode =
{
  .MeasureMode = false,
  .ResetMode   = false,
  .Ipv6Mode = false,
  .Scope = ImplementationDefinedScope,
  .EnglishMode = false,
  .DryRunMode = false,
  .NoColorMode = false
};

/* recipe 相关 mode */
bool chsrc_in_reset_mode(){return ProgMode.ResetMode;}
/* 默认换源作用域就是 ImplementationDefinedScope */
bool chsrc_in_default_scope_mode(){return ProgMode.Scope == ImplementationDefinedScope;}
bool chsrc_in_user_scope_mode(){return ProgMode.Scope == UserScope;}
bool chsrc_in_project_scope_mode(){return ProgMode.Scope == ProjectScope;}
bool chsrc_in_system_scope_mode(){return ProgMode.Scope == SystemScope;}

bool chsrc_in_english_mode(){return ProgMode.EnglishMode;}
bool chsrc_in_no_color_mode(){return ProgMode.NoColorMode;}

/* 仅 framework 相关 mode */
static bool in_measure_mode(){return ProgMode.MeasureMode;}
static bool in_ipv6_mode(){return ProgMode.Ipv6Mode;}
static bool in_dry_run_mode(){return ProgMode.DryRunMode;}



typedef enum ChgType_t
{
  ChgType_Auto,
  ChgType_Reset,
  ChgType_SemiAuto,
  ChgType_Manual,
  ChgType_Untested
} ChgType_t;


#define MaxComboStackDepth 4

/* Global Program Status */
struct
{
  ChgType_t chgtype; /* 换源实现的类型 */

  /* 此时 chsrc_run() 不再是recipe中指定要运行的一个外部命令，而是作为一个功能实现的支撑 */
  bool chsrc_run_faas;
  char *user_agent;

  /**
   * combo dish 嵌套解析栈，该栈记录当前正在处理的 combo dish
   *
   * 用户只给一个 code/URL，但不同 sub dish 可能使用不同的源列表
   */
  Dish_t *ComboStack[MaxComboStackDepth];
  XySeq_t *ComboBackedUpPaths[MaxComboStackDepth];
  int ComboStackDepth;

  /* 多个 sub dish 使用同一个源 */
  Source_t SharedSource[MaxComboStackDepth];
}
ProgStatus =
{
  .chgtype = ChgType_Auto,
  .chsrc_run_faas = false,
  .user_agent = "chsrc/" Chsrc_Version,

  .ComboStackDepth = 0
};


/* Global Program Store */
struct
{
  XySeq_t *mirror_sites;
  XySeq_t *pl;
  XySeq_t *os;
  XySeq_t *wr;
  XyMap_t *contributors; /* 所有贡献者 */
}
ProgStore =
{
  .mirror_sites = NULL,
  .pl = NULL,
  .os = NULL,
  .wr = NULL,
  .contributors = NULL,
};


/**
 * Dish Group mode (相反则称为 standalone mode)
 *
 *   1. 一个 dish group 包含了多个 dish
 *   2. 触发该运行模式的 dish 被称为 combo dish，其只是一个 virtual dish，
 *      类似 APT 中的 virtual package，而这个 combo dish 包含了多个 sub dish
 *
 * 目前使用该模式的有3个:
 *   - Python, JavaScript，因为二者的包管理器存在多个
 *   - uv，因为其需要换不止一个源
 */
bool
chsrc_in_dish_group_mode (void)
{
  return ProgStatus.ComboStackDepth > 0;
}

bool
chsrc_in_standalone_mode()
{
  return !chsrc_in_dish_group_mode();
}



#define Exit_OK               0
#define Exit_Fatal            1
#define Exit_Unknown          2
#define Exit_Unsupported      3
#define Exit_UserCause        4
#define Exit_MaintainerCause  5
#define Exit_ExternalError    6

/* Convenience */
#define ENGLISH chsrc_in_english_mode()
#define CHINESE !chsrc_in_english_mode()

#define faint(str)    xy_str2faint(str)
#define red(str)      xy_str2red(str)
#define blue(str)     xy_str2blue(str)
#define green(str)    xy_str2green(str)
#define yellow(str)   xy_str2yellow(str)
#define purple(str)   xy_str2purple(str)
#define bold(str)     xy_str2bold(str)
#define bdred(str)    xy_str2bold(xy_str2red(str))
#define bdblue(str)   xy_str2bold(xy_str2blue(str))
#define bdgreen(str)  xy_str2bold(xy_str2green(str))
#define bdyellow(str) xy_str2bold(xy_str2yellow(str))
#define bdpurple(str) xy_str2bold(xy_str2purple(str))

#define chsrc_log(str)     xy_log(App_Name,str)
#define chsrc_succ(str)    xy_succ(App_Name,str)
#define chsrc_info(str)    xy_info(App_Name,str)
#define chsrc_warn(str)    xy_warn(App_Name,str)
#define chsrc_error(str)   xy_error(App_Name,str)
#ifdef XY_DEBUG
  #define chsrc_debug(dom,str) xy_warn(App_Name "(DEBUG " dom ")",str)
#else
  #define chsrc_debug(dom,str)
#endif
#define chsrc_verbose(str) xy_info(App_Name "(VERBOSE)",str)
/* 多语句必须括起来，否则在不带 { } 的 if else 等语句中会出错 */
#define chsrc_breakdown(reason) { \
  char *file = " (" __FILE__ ":"; \
  char *line = xy_2strcat(xy_int2str(__LINE__), ")"); \
  char *loc = xy_2strcat(file, line); \
  xy_error(App_Name "(BREAKDOWN)", xy_2strcat(reason, loc)); \
  exit(Exit_MaintainerCause); }


/* 2系列都是带有括号的 */
#define chsrc_succ2(str)    xy_succ_brkt(App_Name,ENGLISH?"SUCCEED":"成功",str)
#define chsrc_log2(str)     xy_info_brkt(App_Name,"LOG",str)
#define chsrc_warn2(str)    xy_warn_brkt(App_Name,ENGLISH?"WARN":"警告",str)
#define chsrc_error2(str)   xy_error_brkt(App_Name,ENGLISH?"ERROR":"错误",str)
#ifdef XY_DEBUG
  #define chsrc_debug2(dom,str) xy_warn_brkt(App_Name,"DEBUG " dom,str)
#else
  #define chsrc_debug2(dom,str)
#endif
#define chsrc_verbose2(str) xy_info_brkt(App_Name,"VERBOSE",str)

/**
 * @note 输出在 stdout 中
 */
void
chsrc_note2 (const char *str)
{
  char *msg = ENGLISH ? "NOTE" : "提示";
  xy_log_brkt (blue(App_Name), bdblue(msg), blue(str));
}

/**
 * @note 输出在 stdout 中
 */
void
chsrc_alert2 (const char *str)
{
  char *msg = ENGLISH ? "ALERT" : "提醒";
  xy_log_brkt (yellow(App_Name), bdyellow(msg), yellow(str));
}



void
chsrc_init_framework ()
{
  xy_init ();

  ProgStore.contributors = xy_map_new ();

  ProgStore.mirror_sites = xy_seq_new ();
  ProgStore.pl = xy_seq_new ();
  ProgStore.os = xy_seq_new ();
  ProgStore.wr = xy_seq_new ();
}
