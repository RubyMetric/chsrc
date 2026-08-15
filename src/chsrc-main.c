/** ------------------------------------------------------------
 * Copyright © 2023-2026 曾奥然, 郭恒
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 *
 *                  chsrc: Change Source
 *
 *                 全平台通用换源工具与框架
 *
 *         Change Source everywhere for every software
 *
 * 项目创建者: 曾奥然, 郭恒
 * -------------------------------------------------------------
 *
 * 这是一个高度依靠贡献者的项目，我们直接把每一位贡献者写进了代码
 * 里，这是比 Git commit message 更加稳定的方式。但我们不满足于仅
 * 把大家写在注释里，因为C语言在编译时会无情地把这些信息抹去。
 *
 *           所以，我们让每一位贡献者直接参与运行时！
 *
 * 想象一下，每一位贡献者都化作一串比特，穿梭于全国各地的桌面PC中，
 * 流淌在数据中心的机房，分身在手机、路由器、小型嵌入式设备中...
 * 每当 chsrc 执行一次 recipe 相关的操作，这些比特便会复苏，贡献者
 * 的信息随之浮现。它提醒着使用者：在你与开源世界的每一次交互背后，
 * 都是许多人的协作，跨越省份与疆域，以代码相连。
 *
 * 我们希望营造像 Richard Stallman 在创建 GNU 项目之前，在 MIT 时
 * 那样的氛围 —— 写自己用得上的软件，与大家一起开发、维护，简单纯
 * 粹，无关商业利益。就像小区、校园自发组建的足球篮球队，从一次偶
 * 然的加入，逐渐成长为互相支持的伙伴。
 * ------------------------------------------------------------*/

#define Chsrc_Maintain_URL   "https://github.com/RubyMetric/chsrc"
#define Chsrc_Maintain_URL2  "https://gitee.com/RubyMetric/chsrc"

#include "framework/version.h"
#include "framework/core.c"
#include "framework/chef.c"
#include "framework/dish.c"

void
chsrc_register_chefs_and_sauciers ()
{
  /* 项目创建者 */
  chef_register_contributor ("@ccmywish",       "曾奥然",         "ccmywish@qq.com",                NULL);
  // 该注释下一行的用户 ID 为 Gitee ID
  chef_register_contributor ("@G_I_Y",          "郭恒",           "2085471348@qq.com",              NULL);

  /* 项目协作者 */
  chef_register_contributor ("@happy-game",     "Happy Game",     "happygame1024@gmail.com",        NULL);
  chef_register_contributor ("@Mikachu2333",    "Mikachu2333",    "linkchou@yandex.com",            NULL);

  /* 所有贡献者 (按参与贡献时间排序) */
  chef_register_contributor ("@Aaron-212",      "Aaron Ruan",     "aaron212cn@outlook.com",         NULL);
  chef_register_contributor ("@chenrui333",     "Rui Chen",       "rui@chenrui.dev",                NULL);
  chef_register_contributor ("@livelycode36",   "Shengwei Chen",  "414685209@qq.com",               NULL);
  chef_register_contributor ("@Gn3po4g",        "Peng Gao",       "gn3po4g@outlook.com",            NULL);
  chef_register_contributor ("@BlockLune",      "BlockLune",      "blocklune@gmail.com",            NULL);
  chef_register_contributor ("@MrWillCom",      "Mr. Will",       "mr.will.com@outlook.com",        NULL);
  chef_register_contributor ("@Jerry-Terrasse", "Terrasse",       "terrasse@qq.com",                NULL);
  chef_register_contributor ("@lontten",        "Lontten",        "lontten@163.com",                NULL);
  chef_register_contributor ("@Word2VecT",      "Word2VecT",      "tangzinan@bupt.edu.cn",          NULL);
  chef_register_contributor ("@wickdynex",      "Xuan",           "wick.dynex@qq.com",              NULL);
  chef_register_contributor ("@Efterklang",     "GnixAij",        "gaojiaxing0220@gmail.com",       NULL);
  chef_register_contributor ("@czyt",           "czyt",           "czyt.go@gmail.com",              NULL);
  chef_register_contributor ("@XUANJI233",      "XUANJI233",      "xuanji233@outlook.com",          NULL);
  chef_register_contributor ("@Yangmoooo",      "Yangmoooo",      "yangmoooo@outlook.com",          NULL);
  chef_register_contributor ("@zouri",          "Zouri",          "guoshuaisun@outlook.com",        NULL);
  chef_register_contributor ("@ReachForStar",  "徐永祥",          "1926885268@qq.com",              NULL);
  chef_register_contributor ("@YU-7",           "YU-7",           "2747046473@qq.com",              NULL);
  chef_register_contributor ("@juzeon",         "juzeon",         "skyjuzheng@gmail.com",           NULL);
  chef_register_contributor ("@jialinlvcn",     "Jialin Lyu",     "jialinlvcn@aliyun.com",          NULL);
  chef_register_contributor ("@Kattos",         "ccy",            "icuichengyi@gmail.com",          NULL);
  chef_register_contributor ("@xrgzs",          "MadDogOwner",    "xiaoran@xrgzs.top",              NULL);
  chef_register_contributor ("@sanchuanhehe",   "sanchuanhehe",   "wyihe5520@gmail.com",            NULL);
  chef_register_contributor ("@techoc",         "Rui Yang",       "techoc@foxmail.com",             NULL);
  chef_register_contributor ("@BingChunMoLi",   "BingChunMoLi",   "bingchunmoli@bingchunmoli.com",  NULL);
  chef_register_contributor ("@wcbing",         "wcbing",         "i@wcbing.top",                   NULL);
  // 该注释下一行的用户 ID 为 Gitee ID
  chef_register_contributor ("@hezonglun",      "HeZongLun",      "hezonglun123456@outlook.com",    NULL);
  chef_register_contributor ("@Young-Lord",     "LY",             "ly-niko@qq.com",                 NULL);
  chef_register_contributor ("@MingriLingran",  "MingriLingran",  "i@linran.moe",                   NULL);
  chef_register_contributor ("@usernameisnull", "MaBing",         "cumt_ttr@163.com",               NULL);
  chef_register_contributor ("@AnonTokio",      "Anon Tokio",     "anontokio@163.com",              NULL);
  chef_register_contributor ("@NewbieXvwu",     "Mingzhe Jiang",   "newbiexvwu@gmail.com",          NULL);
  chef_register_contributor ("@daocoder",       "daocoder",       "daocoder@foxmail.com",           NULL);

  /**
   * AI贡献者：
   *
   *    ChatGPT、GitHub Copilot、DeepSeek、Claude
   *
   * 这几位是贡献者显式说明的，也许还有隐式使用的一些AI并没有被记录下来，可以添加在这里
   */
}

#include "recipe/menu.c"

#include "rawstr4c.h"



void
cli_print_all_mirror_sites ()
{
  {
  char *msg = ENGLISH ? "To specify a source, use chsrc set " : "指定使用某源，请使用 chsrc set ";
  println (bdblue(xy_strcat (3, msg, "<dish>", " <code>\n")));
  }

  {
  char *msg = ENGLISH ? "Available Mirror Sites: \n" : "可用镜像站: \n";
  println (bdgreen(msg));
  }

  {
  char *msg1 = ENGLISH ? "Mirror abbr" : "镜像站简写";
  char *msg2 = ENGLISH ? "Mirror URL"  : "镜像站URL";
  char *msg3 = ENGLISH ? "Mirror Name" : "镜像站";
  char *format = ENGLISH ? "  %-13s%-28s%-35s%s\n" : "  %-13s%-33s%-42s%s\n";
  printf (format, "code", msg1, msg2, msg3);
  println ("---------    --------------    -------------------------------------     ---------------------");
  }

  for (size_t i=0; i<xy_seq_len(ProgStore.mirror_sites); i++)
    {
      MirrorSite_t *mir = xy_seq_at(ProgStore.mirror_sites, i);
      printf ("  %-13s%-28s%-35s%s\n", mir->code, mir->abbr, mir->site, mir->name);
    }
}



/**
 * @brief 遍历以 / 为分隔符的别名字符串，对每个别名调用回调函数
 *
 * @param  aliases    空格分隔的 alias 字符串
 * @param  callback   对每个 alias 调用的回调函数
 * @param  user_data  传递给回调函数的用户数据
 *
 * @return 如果回调函数返回true则停止遍历并返回true，否则返回false
 */
bool
iterate_aliases (const char *aliases, bool (*callback)(const char *alias, void *user_data), void *user_data)
{
  char *aliases_copy = xy_strdup (aliases);
  char *tok_start = aliases_copy;
  char *cursor;
  bool result = false;

  while (*tok_start != '\0')
    {
      cursor = tok_start;
      while (*cursor != '/' && *cursor != '\0') cursor++;

      // 结束当前token
      char space_or_eos = *cursor;
      *cursor = '\0';

      // 调用回调函数
      if (callback(tok_start, user_data))
        {
          result = true;
          break;
        }

      *cursor = space_or_eos;
      if (space_or_eos == '\0') { break; }
      tok_start = cursor+1;
    }

  return result;
}


/**
 * 用于 cli_print_dishes_for_menu() 的回调函数，打印每个别名
 */
bool
callback_print_alias (const char *alias, void *DUMMY)
{
  printf ("%s  ", alias);
  return false; // 继续遍历，不停止
}

void
cli_print_dishes_for_menu (XySeq_t *menu)
{
  for (size_t i=0; i<xy_seq_len(menu); i++)
    {
      Dish_t *dish = xy_seq_at (menu, i);
      // 使用通用的别名遍历函数打印所有别名
      iterate_aliases (dish->aliases, callback_print_alias, NULL);
      br(); // 每个 dish 换行
    }
  br(); // 最后额外换行
}

void
cli_print_supported_dishes ()
{
  {
  char *msg = CHINESE ? "支持对以下菜品换源 (同一行表示这几个菜品兼容)"
                      : "Support following dishes (same line indicates these dishes are compatible)";
  say (bdblue(msg)); br();
  }

  {
  char *msg = CHINESE ? "编程语言" : "Programming Languages";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_dishes_for_menu (ProgStore.pl);
  }

  {
  char *msg = CHINESE ? "操作系统" : "Operating Systems";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_dishes_for_menu (ProgStore.os);
  }

  {
  char *msg = CHINESE ? "软件" : "Softwares";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_dishes_for_menu (ProgStore.wr);
  }
}

void
cli_print_menu (char *menu)
{
  if (xy_streql (menu, "pl"))
    {
      char *msg =
        CHINESE ? "支持对以下编程语言生态换源 (同一行表示这几个菜品兼容)\n"
                : "Support following Programming Languages (same line indicates these dishes are compatible)\n";
      say (bdgreen(msg));
      cli_print_dishes_for_menu (ProgStore.pl);
    }
  else if (xy_streql (menu, "os"))
    {
      char *msg =
        CHINESE ? "支持对以下操作系统换源 (同一行表示这几个菜品兼容)\n"
                : "Support following Operating Systems (same line indicates these dishes are compatible)\n";
      say (bdgreen(msg));
      cli_print_dishes_for_menu (ProgStore.os);
    }
  else if (xy_streql (menu, "wr"))
    {
      char *msg =
        CHINESE ? "支持对以下软件换源 (同一行表示这几个菜品兼容)\n"
                : "Support following Softwares (same line indicates these dishes are compatible)\n";
      say (bdgreen(msg));
      cli_print_dishes_for_menu (ProgStore.wr);
    }
}



/**
 * 用于 chsrc list <dish>
 */
void
cli_print_dish_available_sources (Source_t sources[], size_t size)
{
  for (int i=0; i<size; i++)
    {
      Source_t src = sources[i];
      const MirrorSite_t *mir = src.mirror;
      if (NULL == src.url)
        {
          src.url = "Please help to add the upstream url!";
        }
      printf ("%-14s%-18s%-50s ", mir->code, mir->abbr, src.url);
      say (mir->name);
    }
}


void
cli_print_dish_features (Dish_t *dish, const char *input_dish_name)
{
  {
  char *msg = CHINESE ? "\n可用功能:\n" : "\nAvailable Features:\n";
  say (bdgreen(msg));
  }

  {
  char *msg = CHINESE ? " Get: 查看当前源状态 " : " Get: View the current source state ";
  char *get_msg = xy_strcat (3, msg, "| chsrc get ", input_dish_name);
  if (dish->getfn != NULL) printf (" %s%s\n", bdgreen(YesMark), purple(get_msg));
  else printf (" %s%s\n", bdred(NoMark), get_msg);br();
  }

  {
  char *msg = CHINESE ? " Reset: 重置回默认源 " : " Reset: Reset to the default source ";
  char *reset_msg = xy_strcat (3, msg, "| chsrc reset ", input_dish_name);
  if (dish->resetfn != NULL) printf (" %s%s\n", bdgreen(YesMark), purple(reset_msg));
  else printf (" %s%s\n", bdred(NoMark), reset_msg);br();
  }

  {
  char *msg = CHINESE ? " UserDefine: 用户自定义换源链接 " : " UserDefine: using user-defined source link ";
  char *user_define_msg = xy_strcat (5, msg, "| chsrc set ", input_dish_name, " https://user-define-url.org/", input_dish_name);
  if (dish->can_user_define) printf (" %s%s\n", bdgreen(YesMark), purple(user_define_msg));
  else printf (" %s%s\n", bdred(NoMark), user_define_msg);br();
  }

  {
  char *scope_msg = NULL;

  for (int i=0; i<NumberOfScopeType; i++)
    {
      ScopeCapability_t cap = dish->scope_caps[i];
      char *scope_name;
      if (i == 0)
        {
          scope_name = CHINESE ? " 项目级换源" : " project scope";
          scope_msg = xy_strcat (3, scope_name, " | chsrc set -scope=project ", input_dish_name);
        }
      else if (i == 1)
        {
          scope_name = CHINESE ? " 用户级换源" : " user scope";
          scope_msg = xy_strcat (3, scope_name, " | chsrc set -scope=user    ", input_dish_name);
        }
      else if (i == 2)
        {
          scope_name = CHINESE ? " 系统级换源" : " system scope";
          scope_msg = xy_strcat (3, scope_name, " | chsrc set -scope=system  ", input_dish_name);
        }
      else
        {
          xy_unreached();
        }

      char *msg = NULL;
      switch (cap)
        {
        case ScopeCap_Unknown:
          msg = xy_strcat (6, " ", bdred(NoMark), scope_msg, " (", "是否支持该作用域尚不了解，欢迎贡献", ")");
          puts (msg);
          break;
        case ScopeCap_Unable:
          msg = xy_strcat (6, " ", bdred(NoMark), scope_msg, " (", "不支持", ")");
          puts (msg);
          break;
        case ScopeCap_Able_But_Not_Implemented:
          msg = xy_strcat (6, " ", bdyellow(HalfYesMark), scope_msg, " (", "支持但未实现", ")");
          puts (msg);
          break;
        case ScopeCap_Able_And_Implemented:
          msg = xy_strcat (3, " ", bdgreen(YesMark),
            purple (xy_strcat (4, scope_msg, " (", "支持且已实现", ")")));
          puts (msg);
          break;
        default:
          xy_unreached();
        }
    }
    Scope_t default_scope = dish->default_scope;
    char *default_scope_name = NULL;
    switch (default_scope)
      {
      case ProjectScope:
        default_scope_name = CHINESE ? "项目级" : "Project Scope";
        break;
      case UserScope:
        default_scope_name = CHINESE ? "用户级" : "User Scope";
        break;
      case SystemScope:
        default_scope_name = CHINESE ? "系统级" : "System Scope";
        break;
      case ImplementationDefinedScope:
        default_scope_name = CHINESE ? "由实现定义" : "Implementation Defined Scope";
        break;
      default:
        xy_unreached();
      }
    char *msg = xy_strcat (2, bdblue (" = "),
      purple (xy_strcat (5, "默认作用域 | chsrc set -scope=default ", input_dish_name, " (= ", default_scope_name ,")")));
    puts (msg);
    br();
  }

  {
  char *msg = ENGLISH ? " English: Output in English " : " English: 英文输出 ";
  char *english_msg = xy_strcat (3, msg, "| chsrc set -en ", input_dish_name);
  if (dish->can_english) printf (" %s%s\n", bdgreen(YesMark), purple(english_msg));
  else printf (" %s%s\n", bdred(NoMark), english_msg);br();
  }

  if (dish->note)
    {
      char *msg = ENGLISH ? "NOTE: " : "备注: ";
      printf ("%s%s\n\n", bdyellow (msg), bdyellow (dish->note));
    }
}


/**
 * @brief 简略打印维护信息
 *
 * 用于 chsrc get/set/reset <dish>
 */
void
cli_print_dish_maintain_info_briefly (Dish_t *dish, const char *input_dish_name)
{
  if (dish->last_updated)
    {
      char *msg = ENGLISH ? "Recipe Last Updated: " : "食谱更新: ";
      printf ("%s%s  ", msg, purple(dish->last_updated));
    }

  char num[32]; sprintf(num, "%d", xy_seq_len(dish->chefs) + xy_seq_len(dish->sauciers));
  char *msg = ENGLISH ? "Contributors: " : "后厨人数: ";
  printf ("%s%s  ", msg, purple(num));

  msg = ENGLISH ? xy_strcat (3, "(See chsrc ls ",  input_dish_name, ")")
                : xy_strcat (3, "(详查 chsrc ls ", input_dish_name, ")");
  printf ("%s\n", msg);
}


/**
 * @brief 详细打印维护信息
 *
 * 用于 chsrc ls <dish>
 */
void
cli_print_dish_maintain_info (Dish_t *dish, const char *input_dish_name)
{
  if (dish->created_on)
    {
      char *msg = ENGLISH ? "Recipe Created On: " : "食谱创建: ";
      printf ("%s%s ", bdblue(msg), dish->created_on);
    }

  if (dish->last_updated)
    {
      char *msg = ENGLISH ? "Recipe Last Updated: " : "食谱更新: ";
      printf ("%s%s\n", bdblue(msg), dish->last_updated);
    }

  {
    char *msg = ENGLISH ? "Chefs: " : "主厨: ";
    if (dish->chefs && xy_seq_len(dish->chefs) > 0)
      {
        printf ("%s", bdblue(msg));
        for (size_t i=0; i<xy_seq_len(dish->chefs); i++)
          {
            if (i > 0) printf (", ");
            Contributor_t *chef = xy_seq_at (dish->chefs, i);
            printf ("%s <%s>",
                    chef->name  ? chef->name : "Unknown",
                    chef->email ? chef->email : "unknown@example.com");
          }
        printf ("\n");
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎参与贡献" : "Vacant, Welcome to contribute!";
        printf ("%s%s\n", bdblue(msg), bdgreen(msg1));
      }
  }

  {
    char *msg = ENGLISH ? "Sauciers: " : "调味: ";
    if (dish->sauciers && xy_seq_len(dish->sauciers) > 0)
      {
        printf ("%s", bdblue(msg));
        for (size_t i=0; i<xy_seq_len(dish->sauciers); i++)
          {
            if (i > 0) printf (", ");
            Contributor_t *saucier = xy_seq_at (dish->sauciers, i);
            printf ("%s <%s>", saucier->name, saucier->email );
          }
        br();
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎参与贡献" : "Vacant, Welcome to contribute!";
        printf ("%s%s\n", bdblue(msg), bdgreen(msg1));
      }
  }
}


void
cli_print_version ()
{
  char *version_string = "v" Chsrc_Version " (" Chsrc_Release_Date ")";

  char *str = xy_str_gsub (CHINESE ? RAWSTR_chsrc_for_v_CHINESE : RAWSTR_chsrc_for_v_ENGLISH,
                           "@ver@", version_string);
  println (str);
}


void
cli_print_help ()
{
  char *version_string = "v" Chsrc_Version " (" Chsrc_Release_Date ")";

  const char *raw = CHINESE ? RAWSTR_chsrc_USAGE_CHINESE : RAWSTR_chsrc_USAGE_ENGLISH;

  char *str = xy_str_gsub (raw, "@ver@", version_string);
  println (str);
}


void
cli_print_issues ()
{
  println (RAWSTR_chsrc_for_issue);

  /*
  if (chsrc_check_program ("gh"))
    {
      char *cmd = xy_quiet_cmd ("gh browse --repo RubyMetric/chsrc");
      system (cmd);
    }
  */
}


/**
 * @brief 用于 callback_is_one_of_dish_aliases() 的回调函数，检查别名是否匹配用户输入
 */
bool
callback_match_alias (const char *alias, void *user_data)
{
  const char *input = (const char *)user_data;
  return xy_streql_ic (input, alias);
}

/**
 * @brief 用于 iterate_menu() 的回调函数
 */
bool
callback_is_one_of_dish_aliases (void *data, void *input)
{
  Dish_t *dish = (Dish_t *) data;
  if (iterate_aliases (dish->aliases, callback_match_alias, input))
    {
      return true;
    }
  else
    return false;
}

/**
 * 查询用户输入 `input` 是否与该 `menu` 中的某个 dish 匹配
 *
 * @param[in]   menu    menu
 * @param[in]   input  用户输入的菜品名
 * @param[out]  dish   返回匹配到的 Dish_t 指针
 *
 * @return 匹配到则返回true，未匹配到则返回false
 */
bool
iterate_menu (XySeq_t *menu, const char *input, Dish_t **dish)
{
  Dish_t *d = xy_seq_find (menu, callback_is_one_of_dish_aliases, (void *) input);

  if (d)
    {
      *dish = d;
      return true;
    }
  else
    {
      *dish = NULL;
      return false;
    }
}


void
callback_perform_all_prepare_for_menu (void *data, void *DUMMY)
{
  Dish_t *dish = (Dish_t *) data;

  if (!dish->preparefn)
    {
      chef_debug_dish (dish);
      chsrc_breakdown ("未定义 _prepare() !");
    }

  dish->preparefn();
}

/**
 * @brief 用于检查所有 _prepare() 是否能正常工作
 *
 * 为了防止 DEBUG 模式下运行流程和普通模式下运行流程不一样，我们只在 Get, Set, Reset
 * 之后才运行该函数
 */
void
chsrc_perform_all_prepare ()
{
  chsrc_debug ("prepare", "DEBUG模式下, 额外检查所有 _prepare() 是否能正常工作");
  xy_seq_each (ProgStore.pl, callback_perform_all_prepare_for_menu, NULL);
  xy_seq_each (ProgStore.os, callback_perform_all_prepare_for_menu, NULL);
  xy_seq_each (ProgStore.wr, callback_perform_all_prepare_for_menu, NULL);
}


/**
 * @brief 在必要的时期，最后告诉用户一些信息
 */
void
chsrc_op_epilogue ()
{
  br();
  chsrc_note2 (RAWSTR_chsrc_op_epilogue);
}



/**
 * 寻找 dish，并填充其 recipe 信息
 *
 * @param  input   用户输入的菜品名
 *
 * @return 找到时返回 dish 指针，否则返回 NULL
 */
Dish_t *
find_dish (const char *input)
{
  /**
   * 由于具体需要某一个 dish 时，才会表达贡献者信息，所以像
   *
   *   $ chsrc -v
   *   $ chsrc issue
   *
   * 等命令不需要 dish 时，就不需要注册贡献者信息，加快执行速度
   */
  chsrc_register_chefs_and_sauciers ();

  Dish_t *dish = NULL;

           bool matched = iterate_menu (ProgStore.pl, input, &dish);
  if (!matched) matched = iterate_menu (ProgStore.os, input, &dish);
  if (!matched) matched = iterate_menu (ProgStore.wr, input, &dish);

  if (matched)
    {
      /* 按需加载 recipe 信息: 填充好该 recipe 所有信息，其他 recipe 信息不填充 */
      dish->preparefn();
      return dish;
    }
  else
    {
      return NULL;
    }
}

typedef enum {
  TargetCmd_Get_Source = 1,
  TargetCmd_Set_Source,
  TargetCmd_Reset_Source,
  TargetCmd_Measure_Source,
  TargetCmd_List_Info
} TargetCmd;


/**
 * 由 chefs_handle_user_command() 拆分而来
 */
void
chefs_handle_List_Info (Dish_t *dish, const char *input, char *option)
{
  /* group dish 仅展示维护信息 */
  if (dish_has_sub_dishes(dish))
    {
      int sub_count = xy_seq_len (dish->sub_dishes);

      char *zh_msg = xy_strcat (3,
        bdyellow(xy_strcat (4, input, " 由以下", xy_int2str(sub_count), "个子菜品组成，可使用 chsrc ls <")),
        bdpurple("sub-dish"),
        bdyellow("> 分别查看\n"));

      char *en_msg = xy_strcat (3,
        bdyellow(xy_strcat (4, input, " consists of the following ", xy_int2str(sub_count), " sub dishes, you can use `chsrc ls <")),
        bdpurple("sub-dish"),
        bdyellow("> to view each\n"));

      chsrc_log (CHINESE ? zh_msg : en_msg);

      for (size_t i=0; i<sub_count; i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple (sub_dish->aliases));
          /* 嵌套的 group dish 的处理 */
          if (dish_has_sub_dishes(sub_dish))
            {
              chefs_handle_List_Info (sub_dish, dish_get_first_alias(sub_dish), option);
            }
          else
            {
              cli_print_dish_maintain_info (sub_dish, input);
              br();
            }
        }
      return;
    }

  {
  char *msg = ENGLISH ? "To specify a source, use chsrc set " : "指定使用某源，请使用 chsrc set ";
  say (bdblue(xy_strcat (3, msg, input, " <code>\n")));
  }

  {
  char *msg = ENGLISH ? "Available Sources: \n" : "可用源: \n";
  say (bdgreen(msg));
  }

  {
  char *msg1 = ENGLISH ? "Mirror abbr" : "镜像站简写";
  char *msg2 = ENGLISH ? "Source URL"  : "换源链接";
  char *msg3 = ENGLISH ? "Mirror Name" : "镜像站";
  char *format = ENGLISH ? "  %-13s%-33s%-38s%s\n" : "  %-13s%-36s%-46s%s\n";
  printf (format, "code", msg1, msg2, msg3);
  say    ("---------    --------------    -----------------------------------------------    ---------------------");
  }

  cli_print_dish_available_sources (dish->sources, dish->sources_n);
  cli_print_dish_features (dish, input);

  {
  char *msg = ENGLISH ? "Maintainer Information:\n" : "维护信息:\n";
  say (bdgreen(msg));
  cli_print_dish_maintain_info (dish, input);
  }
}


/**
 * 由 chefs_handle_user_command() 拆分而来
 */
void
chefs_handle_Measure_Source (Dish_t *dish, const char *input, char *option)
{
  /* group dish 不测速，让用户自己分别测速 */
  if (dish_has_sub_dishes(dish))
    {
      int sub_count = xy_seq_len (dish->sub_dishes);

      char *zh_msg = xy_strcat (3,
        bdyellow(xy_strcat (4, input, " 由以下", xy_int2str(sub_count), "个子菜品组成，可使用 chsrc measure <")),
        bdpurple("sub-dish"),
        bdyellow("> 分别测速"));

      char *en_msg = xy_strcat (3,
        bdyellow(xy_strcat (4, input, " consists of the following ", xy_int2str(sub_count), " sub dishes, you can use `chsrc measure <")),
        bdpurple("sub-dish"),
        bdyellow("> to measure each"));

      chsrc_log (CHINESE ? zh_msg : en_msg);

      for (size_t i=0; i<sub_count; i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple (sub_dish->aliases));
        }
    }
  else
    {
      auto_select_mirror (dish->sources, dish->sources_n, input);
      return;
    }
}


/**
 * 由 chefs_handle_user_command() 拆分而来
 */
void
chefs_handle_Get_Source (Dish_t *dish, const char *input, char *option)
{
  if (dish_has_sub_dishes(dish))
    {
      for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple(sub_dish->aliases));
          chsrc_set_dish_group_mode();
          chefs_handle_Get_Source (sub_dish, dish_get_first_alias(sub_dish), option);
          br();
        }
      return;
    }

  if (dish->getfn)
    {
      dish->getfn("");
    }
  else
    {
      chsrc_error (xy_strcat (3, "暂未对 ", input, " 实现 get 功能，邀您帮助: chsrc issue"));
    }
}


/**
 * 由 chefs_handle_user_command() 拆分而来
 */
void
chefs_handle_Set_Source (Dish_t *dish, const char *input, char *option)
{
  if (dish_has_sub_dishes(dish))
    {
      chsrc_begin_combo_source_resolution (dish, option);

      for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple(sub_dish->aliases));
          chefs_handle_Set_Source (sub_dish, dish_get_first_alias(sub_dish), option);
          br();
        }

      chsrc_end_combo_source_resolution ();
      return;
    }

  if (dish->setfn)
    {
      /**
       * Hook时机: 开始运行前可以在这里进行一些拦截操作，当前仅有的拦截为:
       *
       *    1. 检查用户要求设置的作用域，是否真的可以执行
       *
       * 而检查用户自己提供换源URL 与 dish->can_user_define 的冲突，则是
       * 在 `chsrc_yield_source()` 里完成的
       */
      if (dish_has_sub_dishes(dish))
        {
          /* group dish 不要检查，留给后续 sub dishes 检查 */
          xy_noop();
        }
      else
        {
          chsrc_check_scope_capability (dish);
        }

      dish->setfn(option);
    }
  else
    {
      chsrc_error (xy_strcat (3, "暂未对 ", input, " 实现 set 功能，邀您帮助: chsrc issue"));
    }
}


/**
 * 由 chefs_handle_user_command() 拆分而来
 */
void
chefs_handle_Reset_Source (Dish_t *dish, const char *input, char *option)
{
  if (dish_has_sub_dishes(dish))
    {
      for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple(sub_dish->aliases));
          chefs_handle_Reset_Source (sub_dish, dish_get_first_alias(sub_dish), option);
          br();
        }
      return;
    }

  if (dish->resetfn)
    {
      dish->resetfn(option);
    }
  else
    {
      chsrc_error (xy_strcat (3, "暂未对 ", input, " 实现 reset 功能，邀您帮助: chsrc issue"));
    }
}


/**
 * 某一个 dish 的 chefs 们 开始处理用户的某个请求
 *
 * @param  code    对 dish 要执行的操作
 * @param  input   用户输入的原菜品字符串(为了在提示中还原用户的输入)
 * @param  option  额外的选项，可为NULL
 */
void
chefs_handle_user_command (Dish_t *dish, TargetCmd code, const char *input, char *option)
{
  if (TargetCmd_Get_Source==code || TargetCmd_Set_Source==code || TargetCmd_Reset_Source==code)
    {
      if (dish_has_sub_dishes(dish))
        {
          int sub_count = xy_seq_len(dish->sub_dishes);
          char *zh_msg = bdyellow(xy_strcat (4, input, " 由", xy_int2str(sub_count), "个子菜品组成: "));
          char *en_msg = bdyellow(xy_strcat (4, input, " consists of ", xy_int2str(sub_count), " sub dishes\n"));

          for (size_t i=0; i<sub_count; i++)
            {
              Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
              zh_msg = xy_strcat (3, zh_msg, bdpurple(dish_get_first_alias(sub_dish)),
                (i < sub_count-1) ? ", " : "");
            }

          chsrc_log (CHINESE ? zh_msg : en_msg);
          br();
        }
    }

  if (TargetCmd_Set_Source==code)
    {
      chefs_handle_Set_Source (dish, input, option);
    }
  else if (TargetCmd_Reset_Source==code)
    {
      chefs_handle_Reset_Source (dish, input, option);
    }
  else if (TargetCmd_Get_Source==code)
    {
      chefs_handle_Get_Source (dish, input, option);
    }
  else if (TargetCmd_List_Info==code)
    {
      chefs_handle_List_Info (dish, input, option);
    }
  else if (TargetCmd_Measure_Source==code)
    {
      chefs_handle_Measure_Source (dish, input, option);
    }

  /* 核心命令 get, set, reset 完成后需要简短显示维护信息 */
  if (TargetCmd_Get_Source==code || TargetCmd_Set_Source==code || TargetCmd_Reset_Source==code)
    {
      if (dish_has_sub_dishes(dish))
        {
          for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
            {
              Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
              /* 递归显示 sub dishes 信息 */
              sub_dish->preparefn();
              cli_print_dish_maintain_info_briefly (sub_dish, sub_dish->aliases);
              // chefs_handle_user_command (sub_dish, code, input, option);
            }
        }
      else
        {
          cli_print_dish_maintain_info_briefly (dish, input);
        }
    }

  if (TargetCmd_Set_Source==code || TargetCmd_Measure_Source==code)
    {
      // 2025-09-19: 已告知该消息给用户一个多月，我们不再告知
      // chsrc_op_epilogue ();
    }

#ifdef XY_DEBUG
  chef_debug_dish (dish);
  chsrc_perform_all_prepare ();
#endif

  return;
}



int
main (int argc, char const *argv[])
{
  chsrc_init_framework ();
  chsrc_init_menu ();

  argc -= 1;

  if (0==argc)
    {
      cli_print_help ();
      return Exit_OK;
    }

  const char *command = argv[1];

  // chsrc set <dish-name> <mirror-code>
  //        1        2             3
  int cli_arg_Dish_pos = 2;
  int cli_arg_Mirror_pos = cli_arg_Dish_pos + 1;
  const char *dish_name = NULL;

  Dish_t *the_found_dish = NULL;

  /**
   * (1)
   * chsrc set -scope=project -en dish mirror
   *        1        2         3    4    5
   * argc = 5
   *
   * (2) 考虑到这种情况，i必须还是从1开始
   * chsrc -en -h
   *        1  2
   *
   * argc = 2
   */
  /* 从第一个参数遍历到最后一个参数 */
  for (int i=1; i<=argc; i++)
    {
      if (xy_str_start_with (argv[i], "-"))
        {
          if (xy_streql (argv[i], "-ipv6"))
            {
              ProgMode.Ipv6Mode = true;
            }
          else if (xy_streql (argv[i], "-local"))
            {
              ProgMode.Scope = ProjectScope;
              char *msg = CHINESE ? " -local 选项已弃用，请使用 -scope=project"
                                  : " -local is deprecated, please use -scope=project";
              chsrc_warn (msg);
            }
          else if (xy_str_start_with (argv[i], "-scope"))
            {
              const char *scope = NULL;
              if (xy_streql (argv[i], "-scope"))
                {
                  scope = argv[i+1];
                  cli_arg_Dish_pos++;
                  cli_arg_Mirror_pos++;
                }
              else if (xy_str_start_with (argv[i], "-scope="))
                {
                  scope = argv[i] + 7;
                }
              if (xy_streql_ic (scope, "project"))
                {
                  ProgMode.Scope = ProjectScope;
                }
              else if (xy_streql_ic (scope, "user"))
                {
                  ProgMode.Scope = UserScope;
                }
              else if (xy_streql_ic (scope, "system"))
                {
                  ProgMode.Scope = SystemScope;
                }
              else if (xy_streql_ic (scope, "default"))
                {
                  ProgMode.Scope = ImplementationDefinedScope;
                }
              else
                {
                  if (ENGLISH)
                    {
                      char *msg = "Invalid scope: ";
                      chsrc_error (xy_strcat (3, msg, scope, ". Valid scopes are: default, project, user, system"));
                    }
                  else
                    {
                      char *msg = "无效的换源作用域: ";
                      chsrc_error (xy_strcat (4, msg, scope, "。", "有效的换源作用域为: default, project, user, system"));
                    }
                  return Exit_Unknown;
                }
            }
          else if (xy_streql (argv[i], "-en") || xy_streql (argv[i], "-english"))
            {
              ProgMode.EnglishMode = true;
            }
          else if (xy_streql (argv[i], "-dry"))
            {
              ProgMode.DryRunMode = true;
            }
          else if (xy_streql (argv[i], "-no-color") || xy_streql (argv[i], "-no-colour"))
            {
              ProgMode.NoColorMode = true;
              xy.enable_color = false;
            }
          else if (    xy_streql (argv[i], "-h")
                    || xy_streql (argv[i], "-help")
                    || xy_streql (argv[i], "--help"))
            {
              command = "help"; /* 交到下方处理 */
            }
          else if (    xy_streql (argv[i], "-v")
                    || xy_streql (command, "-version")
                    || xy_streql (command, "--version"))
            {
              command = "version"; /* 交到下方处理 */
            }
          else
            {
              char *msg = ENGLISH ? "Unknown option: " : "未识别的命令行选项 ";
              chsrc_error (xy_2strcat (msg, argv[i])); return Exit_Unknown;
            }
          cli_arg_Dish_pos++;
          cli_arg_Mirror_pos++;
        }
    }

  if (in_dry_run_mode())
    {
      char *dry_msg = ENGLISH ? "Enable [Dry Run] mode. "
                                "Simulate the source changing process (skipping speed measurement). "
                                "Commands only print but don't run\n"
                              : "开启Dry Run模式，模拟换源过程(跳过测速)，命令仅打印并不运行\n";
      chsrc_log (bdyellow(dry_msg));
    }

  /* chsrc help */
  if (   xy_streql (command, "h")
      || xy_streql (command, "-h")
      || xy_streql (command, "help")
      || xy_streql (command, "-help")
      || xy_streql (command, "--help"))
    {
      cli_print_help ();
      return Exit_OK;
    }

  /* chsrc -v */
  else if (   xy_streql (command, "-v")
           || xy_streql (command, "-version")
           || xy_streql (command, "--version")
           || xy_streql (command, "ver")
           || xy_streql (command, "version"))
    {
      cli_print_version ();
      return Exit_OK;
    }

  /* chsrc list */
  else if (   xy_streql (command, "list")
           || xy_streql (command, "l")
           || xy_streql (command, "ls"))
    {
      if (argc < cli_arg_Dish_pos)
        {
          cli_print_all_mirror_sites ();
          br();
          cli_print_supported_dishes ();
        }
      else
        {
          dish_name = argv[cli_arg_Dish_pos];
          if (   xy_streql (dish_name, "mirrors")
              || xy_streql (dish_name, "mirror"))
            {
              cli_print_all_mirror_sites ();
              return Exit_OK;
            }

          else if (   xy_streql (dish_name, "dishes")
                   || xy_streql (dish_name, "dish")
                   || xy_streql (dish_name, "dishs")) /* 防止有人错误拼写 */
            {
              cli_print_supported_dishes ();
              return Exit_OK;
            }

          else if (xy_streql (dish_name, "os"))
            {
              cli_print_menu ("os");
              return Exit_OK;
            }

          else if (   xy_streql (dish_name, "lang")
                   || xy_streql (dish_name, "pl")
                   || xy_streql (dish_name, "language"))
            {
              cli_print_menu ("pl");
              return Exit_OK;
            }

          else if (   xy_streql (dish_name, "ware")
                   || xy_streql (dish_name, "software"))
            {
              cli_print_menu ("wr");
              return Exit_OK;
            }

          the_found_dish = find_dish (dish_name);
          if (!the_found_dish)
            {
              goto not_found;
            }
          else
            {
              chefs_handle_user_command (the_found_dish, TargetCmd_List_Info, dish_name, NULL);
            }
        }
      return Exit_OK;
  }

#define MSG_EN_USE_LIST_TARGETS "Use `chsrc list dishes` to see all supported dishes"
#define MSG_ZH_USE_LIST_TARGETS "使用 chsrc list dishes 查看所有支持的菜品"

  /* chsrc measure */
  else if (   xy_streql (command, "measure")
           || xy_streql (command, "mea")
           || xy_streql (command, "m")
           || xy_streql (command, "cesu")
           || xy_streql (command, "ce")
           || xy_streql (command, "c"))
    {
      if (argc < cli_arg_Dish_pos)
        {
          char *msg = ENGLISH ? "Please provide the dish name you want to measure. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要测速的菜品名。" MSG_ZH_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }
      ProgMode.MeasureMode = true;
      dish_name = argv[cli_arg_Dish_pos];

      the_found_dish = find_dish (dish_name);
      if (!the_found_dish)
        {
          goto not_found;
        }
      else
        {
          chefs_handle_user_command (the_found_dish, TargetCmd_Measure_Source, dish_name, NULL);
        }
      return Exit_OK;
    }


  /* chsrc get */
  else if (   xy_streql (command, "get")
           || xy_streql (command, "g"))
    {
      if (argc < cli_arg_Dish_pos)
        {
          char *msg = ENGLISH ? "Please provide the dish name you want to view the source. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要查看源的菜品名。" MSG_ZH_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }
      dish_name = argv[cli_arg_Dish_pos];
      the_found_dish = find_dish (dish_name);
      if (!the_found_dish)
        {
          goto not_found;
        }
      else
        {
          chefs_handle_user_command (the_found_dish, TargetCmd_Get_Source, dish_name, NULL);
        }
      return Exit_OK;
    }

  /* chsrc set */
  else if (   xy_streql (command, "set")
           || xy_streql (command, "s"))
    {
      if (argc < cli_arg_Dish_pos)
        {
          char *msg = ENGLISH ? "Please provide the dish name you want to set the source. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要设置源的菜品名。" MSG_ZH_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }

      dish_name = argv[cli_arg_Dish_pos];
      char *mirrorCode_or_url = NULL;
      if (argc >= cli_arg_Mirror_pos)
        {
          mirrorCode_or_url = xy_strdup (argv[cli_arg_Mirror_pos]);
        }

      the_found_dish = find_dish (dish_name);
      if (!the_found_dish)
        {
          goto not_found;
        }
      else
        {
          chefs_handle_user_command (the_found_dish, TargetCmd_Set_Source, dish_name, mirrorCode_or_url);
        }
      return Exit_OK;
    }

  /* chsrc reset */
  else if (   xy_streql (command, "reset")
           || xy_streql (command, "rest")
           || xy_streql (command, "r"))
    {
      if (argc < cli_arg_Dish_pos)
        {
          char *msg = ENGLISH ? "Please provide the dish name you want to reset the source. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要重置源的菜品名。" MSG_ZH_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }

      ProgMode.ResetMode = true;
      dish_name = argv[cli_arg_Dish_pos];
      the_found_dish = find_dish (dish_name);
      if (!the_found_dish)
        {
          goto not_found;
        }
      else
        {
          chefs_handle_user_command (the_found_dish, TargetCmd_Reset_Source, dish_name, NULL);
        }
      return Exit_OK;
    }

  /* chsrc issue */
  else if (   xy_streql (command, "issue")
           || xy_streql (command, "issues")
           || xy_streql (command, "isue")
           || xy_streql (command, "i"))
    {
      cli_print_issues ();
      return Exit_OK;
    }

  else
    {
      char *msg1 = ENGLISH ? "Unknown command `" : "不支持的命令 ";
      char *msg2 = ENGLISH ? "`. Use `chsrc help` to view usage" : ". 请使用 chsrc help 查看使用方式";
      chsrc_error (xy_strcat (3, msg1, command, msg2));
      return Exit_Unknown;
    }

not_found:
  if (!the_found_dish)
    {
      char *msg = ENGLISH ? "Unknown dish. "  MSG_EN_USE_LIST_TARGETS
                          : "暂不支持的换源菜品。" MSG_ZH_USE_LIST_TARGETS;
      chsrc_error (msg);
      return Exit_Unknown;
    }
}
