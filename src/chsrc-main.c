/** ------------------------------------------------------------
 * Copyright © 2023-2025 曾奥然, 郭恒
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

#define Chsrc_Version        "0.2.2.3-dev6"
#define Chsrc_Release_Date   "2025/08/22"
#define Chsrc_Maintain_URL   "https://github.com/RubyMetric/chsrc"
#define Chsrc_Maintain_URL2  "https://gitee.com/RubyMetric/chsrc"

#include "framework/core.c"
#include "framework/chef.c"

void
chsrc_register_contributors ()
{
  /* 项目创建者 */
  chef_register_contributor ("@ccmywish",       "曾奥然",         "ccmywish@qq.com",                NULL);
  // 该 ID 为 Gitee ID
  chef_register_contributor ("@G_I_Y",          "郭恒",           "2085471348@qq.com",              NULL);

  /* 所有贡献者 (按参与贡献时间排序) */
  chef_register_contributor ("@Aaron-212",      "Aaron Ruan",     "aaron212cn@outlook.com",         NULL);
  chef_register_contributor ("@chenrui333",     "Rui Chen",       "rui@chenrui.dev",                NULL);
  chef_register_contributor ("@livelycode36",   "Shengwei Chen",  "414685209@qq.com",               NULL);
  chef_register_contributor ("@Gn3po4g",        "Peng Gao",       "gn3po4g@outlook.com",            NULL);
  chef_register_contributor ("@BlockLune",      "BlockLune",      "blocklune@gmail.com",            NULL);
  chef_register_contributor ("@MrWillCom",      "Mr. Will",       "mr.will.com@outlook.com",        NULL);
  chef_register_contributor ("@Jerry-Terrasse", "Terrasse",       "terrasse@qq.com",                NULL);
  chef_register_contributor ("@lontten",        "Lontten",        "lontten@163.com",                NULL);
  chef_register_contributor ("@happy-game",     "Happy Game",     "happygame1024@gmail.com",        NULL);
  chef_register_contributor ("@Word2VecT",      "Word2VecT",      "tangzinan@bupt.edu.cn",          NULL);
  chef_register_contributor ("@wickdynex",      "Xuan",           "wick.dynex@qq.com",              NULL);
  chef_register_contributor ("@Efterklang",     "GnixAij",        "gaojiaxing0220@gmail.com",       NULL);
  chef_register_contributor ("@czyt",           "czyt",           "czyt.go@gmail.com",              NULL);
  chef_register_contributor ("@XUANJI233",      "XUANJI233",      "xuanji233@outlook.com",          NULL);
  chef_register_contributor ("@Yangmoooo",      "Yangmoooo",      "yangmoooo@outlook.com",          NULL);
  chef_register_contributor ("@zouri",          "Zouri",          "guoshuaisun@outlook.com",        NULL);
  chef_register_contributor ("@xyx1926885268",  "Yongxiang",      "1926885268@qq.com",              NULL);
  chef_register_contributor ("@YU-7",           "YU-7",           "2747046473@qq.com",              NULL);
  chef_register_contributor ("@juzeon",         "juzeon",         "skyjuzheng@gmail.com",           NULL);
  chef_register_contributor ("@jialinlvcn",     "Jialin Lyu",     "jialinlvcn@aliyun.com",          NULL);
  chef_register_contributor ("@Kattos",         "ccy",            "icuichengyi@gmail.com",          NULL);
  chef_register_contributor ("@xrgzs",          "MadDogOwner",    "xiaoran@xrgzs.top",              NULL);
  chef_register_contributor ("@sanchuanhehe",   "sanchuanhehe",   "wyihe5520@gmail.com",            NULL);
  chef_register_contributor ("@Mikachu2333",    "Mikachu2333",    "mikachu.23333@zohomail.com",     NULL);
  chef_register_contributor ("@techoc",         "Rui Yang",       "techoc@foxmail.com",             NULL);
  chef_register_contributor ("@BingChunMoLi",   "BingChunMoLi",   "bingchunmoli@bingchunmoli.com",  NULL);
  // 该 ID 为 Gitee ID
  chef_register_contributor ("@hezonglun",      "HeZongLun",      "hezonglun123456@outlook.com",    NULL);

  /**
   * AI贡献者：
   *
   *    ChatGPT、GitHub Copilot、DeepSeek
   *
   * 这几位是贡献者显式说明的，也许还有隐式使用的一些AI并没有被记录下来，可以添加在这里
   */
}

#include "recipe/menu.c"

#include "rawstr4c.h"



void
cli_print_available_mirrors ()
{
  {
  char *msg = ENGLISH ? "To specify a source, use chsrc set " : "指定使用某源，请使用 chsrc set ";
  say (bdblue(xy_strcat (3, msg, "<target>", " <code>\n")));
  }

  {
  char *msg = ENGLISH ? "Available Mirror Sites: \n" : "可用镜像站: \n";
  say (bdgreen(msg));
  }

  {
  char *msg1 = ENGLISH ? "Mirror abbr" : "镜像站简写";
  char *msg2 = ENGLISH ? "Mirror URL"  : "镜像站URL";
  char *msg3 = ENGLISH ? "Mirror Name" : "镜像站";
  char *format = ENGLISH ? "  %-13s%-28s%-35s%s\n" : "  %-13s%-33s%-42s%s\n";
  printf (format, "code", msg1, msg2, msg3);
  say    ("---------    --------------    -------------------------------------     ---------------------");
  }

  for (int i = 0; i < xy_c_array_len (chsrc_available_mirrors); i++)
    {
      MirrorSite_t *mir = chsrc_available_mirrors[i];
      printf ("%-14s%-18s%-41s ", mir->code, mir->abbr, mir->site); say (mir->name);
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
 * 用于 cli_print_targets_for_menu 的回调函数，打印每个别名
 */
bool
callback_print_alias (const char *alias, void *user_data)
{
  printf ("%s  ", alias);
  return false; // 继续遍历，不停止
}

void
callback_print_targets (void *data, void *DUMMY)
{
  Target_t *target = (Target_t *) data;
  // 使用通用的别名遍历函数打印所有别名
  iterate_aliases (target->aliases, callback_print_alias, NULL);
  br(); // 每个target换行
}

void
cli_print_targets_for_menu (XySeq_t *menu)
{
  xy_seq_each (menu, callback_print_targets, NULL);
  br(); // 最后额外换行
}

void
cli_print_supported_targets ()
{
  {
  char *msg = ENGLISH ? "Support following targets (same line indicates these targets are compatible)"
                      : "支持对以下目标换源 (同一行表示这几个命令兼容)" ;
  say (bdblue(msg)); br();
  }

  {
  char *msg = ENGLISH ? "Programming Languages" : "编程语言";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_targets_for_menu (ProgStore.pl);
  }

  {
  char *msg = ENGLISH ? "Operating Systems" : "操作系统";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_targets_for_menu (ProgStore.os);
  }

  {
  char *msg = ENGLISH ? "Softwares" : "软件";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_targets_for_menu (ProgStore.wr);
  }
}

void
cli_print_menu (char *menu)
{
  if (xy_streql (menu, "pl"))
    {
      char *msg =
        ENGLISH ? "Support following Programming Languages (same line indicates these targets are compatible)\n"
                : "支持对以下编程语言生态换源 (同一行表示这几个目标兼容)\n";
      say (bdgreen(msg));
      cli_print_targets_for_menu (ProgStore.pl);
    }
  else if (xy_streql (menu, "os"))
    {
      char *msg =
        ENGLISH ? "Support following Operating Systems (same line indicates these targets are compatible)\n"
                : "支持对以下操作系统换源 (同一行表示这几个目标兼容)\n";
      say (bdgreen(msg));
      cli_print_targets_for_menu (ProgStore.os);
    }
  else if (xy_streql (menu, "wr"))
    {
      char *msg =
        ENGLISH ? "Support following Softwares (same line indicates these targets are compatible)\n"
                : "支持对以下软件换源 (同一行表示这几个目标兼容)\n";
      say (bdgreen(msg));
      cli_print_targets_for_menu (ProgStore.wr);
    }
}



/**
 * 用于 chsrc list <target>
 */
void
cli_print_target_available_sources (Source_t sources[], size_t size)
{
  for (int i=0;i<size;i++)
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
cli_print_target_features (Target_t *target, const char *input_target_name)
{
  {
  char *msg = ENGLISH ? "\nAvailable Features:\n" : "\n可用功能:\n";
  say (bdgreen(msg));
  }

  {
  char *msg = ENGLISH ? " Get: View the current source state " : " Get: 查看当前源状态 ";
  char *get_msg = xy_strcat (3, msg, "| chsrc get ", input_target_name);
  if (target->getfn != NULL) printf (" %s%s\n", bdgreen(YesMark), purple(get_msg));
  else printf (" %s%s\n", bdred(NoMark), get_msg);br();
  }

  {
  char *msg = ENGLISH ? " Reset: Reset to the default source " : " Reset: 重置回默认源 ";
  char *reset_msg = xy_strcat (3, msg, "| chsrc reset ", input_target_name);
  if (target->resetfn != NULL) printf (" %s%s\n", bdgreen(YesMark), purple(reset_msg));
  else printf (" %s%s\n", bdred(NoMark), reset_msg);br();
  }

  {
  char *msg = ENGLISH ? " UserDefine: using user-defined source link " : " UserDefine: 用户自定义换源链接 ";
  char *user_define_msg = xy_strcat (5, msg, "| chsrc set ", input_target_name, " https://user-define-url.org/", input_target_name);
  if (target->can_user_define) printf (" %s%s\n", bdgreen(YesMark), purple(user_define_msg));
  else printf (" %s%s\n", bdred(NoMark), user_define_msg);br();
  }

  {
  char *msg = ENGLISH ? " Locally: Change source only for this project " : " Locally: 仅对本项目换源 ";
  char *locally_msg = xy_strcat (3, msg, "| chsrc set -local ", input_target_name);

  switch (target->cap_local)
    {
    case CanNot:
      printf (" %s%s\n", bdred(NoMark), locally_msg);br();
      break;
    case FullyCan:
      printf (" %s%s\n", bdgreen(YesMark), purple(locally_msg));br();
      break;
    case PartiallyCan:
      printf (" %s%s\n\n   %s\n", bdgreen(HalfYesMark), purple(locally_msg),
              target->cap_local_explain ? target->cap_local_explain : "");br();
      break;
    default:
      xy_unreached();
    }
  }

  {
  char *msg = ENGLISH ? " English: Output in English " : " English: 英文输出 ";
  char *english_msg = xy_strcat (3, msg, "| chsrc set -en ", input_target_name);
  if (target->can_english) printf (" %s%s\n", bdgreen(YesMark), purple(english_msg));
  else printf (" %s%s\n", bdred(NoMark), english_msg);br();
  }

  if (target->note)
    {
      char *msg = ENGLISH ? "NOTE: " : "备注: ";
      printf ("%s%s\n\n", bdyellow (msg), bdyellow (target->note));
    }
}


void
cli_print_target_maintain_info (Target_t *target, const char *input_target_name)
{
  if (target->created_on)
    {
      char *msg = ENGLISH ? "Recipe Created On: " : "食谱创建时间: ";
      printf ("%s%s\n", bdblue(msg), target->created_on);
    }

  {
    char *msg = ENGLISH ? "Current Chef: " : "品控: ";
    if (target->chef)
      {
        printf ("%s%s <%s>\n", bdblue(msg),
                target->chef->name  ? target->chef->name  : "Unknown",
                target->chef->email ? target->chef->email : "unknown@example.com");
      }
    else
      {
        char *msg1 = CHINESE ? "该 recipe 的负责人暂空缺, 欢迎担任" : "Vacant, Welcome to hold the position";
        printf ("%s%s\n", bdblue(msg), bdgreen(msg1));
      }
  }


  {
    char *msg = ENGLISH ? "Cooks: " : "掌勺: ";
    if (target->cooks && target->cooks_n > 0)
      {
        printf ("%s", bdblue(msg));
        for (size_t i = 0; i < target->cooks_n; i++)
          {
            if (i > 0) printf (", ");
            printf ("%s <%s>",
                    target->cooks[i]->name  ? target->cooks[i]->name : "Unknown",
                    target->cooks[i]->email ? target->cooks[i]->email : "unknown@example.com");
          }
        printf ("\n");
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎担任" : "Vacant, Welcome to hold the position";
        printf ("%s%s\n", bdblue(msg), bdgreen(msg1));
      }
  }

  {
    char *msg = ENGLISH ? "Sauciers: " : "调味: ";
    if (target->sauciers && target->sauciers_n > 0)
      {
        printf ("%s", bdblue(msg));
        for (size_t i = 0; i < target->sauciers_n; i++)
          {
            if (i > 0) printf (", ");
            printf ("%s <%s>", target->sauciers[i]->name, target->sauciers[i]->email );
          }
        br();
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎参与贡献" : "Vacant, Welcome to contribute!";
        printf ("%s%s\n", bdblue(msg), bdgreen(msg1));
      }
  }

  if (target->sources_last_updated)
    {
      char *msg = ENGLISH ? "Ingredient(Sources) Last Updated: " : "食源检查时间: ";
      printf ("%s%s\n", bdblue(msg), target->sources_last_updated);
    }

  if (target->last_updated)
    {
      char *msg = ENGLISH ? "Recipe Last Updated: " : "食谱更新时间: ";
      printf ("%s%s\n", bdblue(msg), target->last_updated);
    }
}


void
cli_print_version ()
{
  char *str = xy_str_gsub (RAWSTR_chsrc_for_v, "@ver@", Chsrc_Version);
  println (str);
}


void
cli_print_help ()
{
  char *version_string = "v" Chsrc_Version "-" Chsrc_Release_Date;

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
      char *cmd = xy_str_to_quietcmd ("gh browse --repo RubyMetric/chsrc");
      system (cmd);
    }
  */
}


/**
 * @brief 用于 callback_find_target() 的回调函数，检查别名是否匹配用户输入
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
callback_is_one_of_target_aliases (void *data, void *input)
{
  Target_t *target = (Target_t *) data;
  if (iterate_aliases (target->aliases, callback_match_alias, input))
    {
      target->preludefn();
      return true;
    }
  else
    return false;
}

/**
 * 查询用户输入 `input` 是否与该 `menu` 中的某个 target 匹配
 * 若匹配将直接调用 prelude
 *
 * @param[in]   menu    menu
 * @param[in]   input   用户输入的目标名
 * @param[out]  target  返回匹配到的 Target_t 指针
 *
 * @return 匹配到则返回true，未匹配到则返回false
 */
bool
iterate_menu (XySeq_t *menu, const char *input, Target_t **target)
{
  Target_t *t = xy_seq_find (menu, callback_is_one_of_target_aliases, (void *) input);

  if (t)
    {
      *target = t;
      t->preludefn();
      return true;
    }
  else
    {
      *target = NULL;
      return false;
    }
}


void
callback_perform_all_prelude_for_menu (void *data, void *DUMMY)
{
  Target_t *target = (Target_t *) data;

  if (!target->preludefn)
    {
      chef_debug_target (target);
      chsrc_panic ("未定义 _prelude() !");
    }

  target->preludefn();
}

/**
 * @brief 用于检查所有 _prelude() 是否能正常工作
 *
 * 为了防止 DEBUG 模式下运行流程和普通模式下运行流程不一样，我们只在 Get, Set, Reset
 * 之后才运行该函数
 */
void
chsrc_perform_all_prelude ()
{
  chsrc_debug ("prelude", "DEBUG模式下, 额外检查所有 _prelude() 是否能正常工作");
  xy_seq_each (ProgStore.pl, callback_perform_all_prelude_for_menu, NULL);
  xy_seq_each (ProgStore.os, callback_perform_all_prelude_for_menu, NULL);
  xy_seq_each (ProgStore.wr, callback_perform_all_prelude_for_menu, NULL);
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



typedef enum {
  TargetOp_Get_Source = 1,
  TargetOp_Set_Source,
  TargetOp_Reset_Source,
  TargetOp_Measure_Source,
  TargetOp_List_Config
} TargetOp;

/**
 * 寻找target，并根据 `code` 执行相应的操作
 *
 * @param  input   用户输入的目标
 * @param  code    对target要执行的操作
 * @param  option  额外的指示，可为NULL
 *
 * @return 找到目标返回true，未找到返回false
 */
bool
get_target (const char *input, TargetOp code, char *option)
{
  chsrc_register_contributors ();

  Target_t *target = NULL;

           bool matched = iterate_menu (ProgStore.pl, input, &target);
  if (!matched) matched = iterate_menu (ProgStore.os, input, &target);
  if (!matched) matched = iterate_menu (ProgStore.wr, input, &target);

  if (!matched) return false;

  if (TargetOp_Set_Source==code)
    {
      if (target->setfn)
        {
          target->setfn(option);
        }
      else chsrc_error (xy_strcat (3, "暂未对 ", input, " 实现 set 功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_Reset_Source==code)
    {
      if (target->resetfn)
        {
          target->resetfn(option);
        }
      else chsrc_error (xy_strcat (3, "暂未对 ", input, " 实现 reset 功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_Get_Source==code)
    {
      if (target->getfn)
        {
          target->getfn("");
        }
      else chsrc_error (xy_strcat (3, "暂未对 ", input, " 实现 get 功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_List_Config==code)
    {
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

      cli_print_target_available_sources (target->sources, target->sources_n);
      cli_print_target_features (target, input);

      {
      char *msg = ENGLISH ? "Maintainer Information:\n" : "维护信息:\n";
      say (bdgreen(msg));
      cli_print_target_maintain_info (target, input);
      }
    }
  else if (TargetOp_Measure_Source==code)
    {
      auto_select_mirror (target->sources, target->sources_n, input);
      return true;
    }

  if (TargetOp_Get_Source==code || TargetOp_Set_Source==code || TargetOp_Reset_Source==code)
    {
      br();
      cli_print_target_maintain_info (target, input);
    }

  if (TargetOp_Set_Source==code || TargetOp_Measure_Source==code)
    {
      chsrc_op_epilogue ();
    }

#ifdef XY_DEBUG
  chef_debug_target (target);
  chsrc_perform_all_prelude ();
#endif

  return true;
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

  // chsrc set target mirror
  //        1    2      3
  int cli_arg_Target_pos = 2;
  int cli_arg_Mirror_pos = cli_arg_Target_pos + 1;
  const char *target = NULL;

  /**
   * (1)
   * chsrc set -local -en target mirror
   *        1    2     3     4      5
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
              ProgMode.LocalMode = true;
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
              xy_enable_color = false;
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
          cli_arg_Target_pos++;
          cli_arg_Mirror_pos++;
        }
    }


  bool matched = false;

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
      if (argc < cli_arg_Target_pos)
        {
          cli_print_available_mirrors ();
          br();
          cli_print_supported_targets ();
        }
      else
        {
          target = argv[cli_arg_Target_pos];
          if (   xy_streql (target, "mirrors")
              || xy_streql (target, "mirror"))
            {
              cli_print_available_mirrors ();
              return Exit_OK;
            }

          else if (   xy_streql (target, "targets")
                   || xy_streql (target, "target"))
            {
              cli_print_supported_targets ();
              return Exit_OK;
            }

          else if (xy_streql (target, "os"))
            {
              cli_print_menu ("os");
              return Exit_OK;
            }

          else if (   xy_streql (target, "lang")
                   || xy_streql (target, "pl")
                   || xy_streql (target, "language"))
            {
              cli_print_menu ("pl");
              return Exit_OK;
            }

          else if (   xy_streql (target, "ware")
                   || xy_streql (target, "software"))
            {
              cli_print_menu ("wr");
              return Exit_OK;
            }

          matched = get_target (target, TargetOp_List_Config, NULL);
          if (!matched) goto not_matched;
        }
      return Exit_OK;
  }

#define MSG_EN_USE_LIST_TARGETS "Use `chsrc list targets` to see all supported targets"
#define MSG_CN_USE_LIST_TARGETS "使用 chsrc list targets 查看所有支持的目标"

  /* chsrc measure */
  else if (   xy_streql (command, "measure")
           || xy_streql (command, "mea")
           || xy_streql (command, "m")
           || xy_streql (command, "cesu")
           || xy_streql (command, "ce")
           || xy_streql (command, "c"))
    {
      if (argc < cli_arg_Target_pos)
        {
          char *msg = ENGLISH ? "Please provide the target name you want to measure. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要测速源的目标名。" MSG_CN_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }
      ProgMode.MeasureMode = true;
      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Measure_Source, NULL);
      if (!matched) goto not_matched;
      return Exit_OK;
    }


  /* chsrc get */
  else if (   xy_streql (command, "get")
           || xy_streql (command, "g"))
    {
      if (argc < cli_arg_Target_pos)
        {
          char *msg = ENGLISH ? "Please provide the target name you want to view the source. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要查看源的目标名。" MSG_CN_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }
      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Get_Source, NULL);
      if (!matched) goto not_matched;
      return Exit_OK;
    }

  /* chsrc set */
  else if (   xy_streql (command, "set")
           || xy_streql (command, "s"))
    {
      if (argc < cli_arg_Target_pos)
        {
          char *msg = ENGLISH ? "Please provide the target name you want to set the source. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要设置源的目标名。" MSG_CN_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }

      target = argv[cli_arg_Target_pos];
      char *mirrorCode_or_url = NULL;
      if (argc >= cli_arg_Mirror_pos)
        {
          mirrorCode_or_url = xy_strdup (argv[cli_arg_Mirror_pos]);
        }

      matched = get_target (target, TargetOp_Set_Source, mirrorCode_or_url);
      if (!matched) goto not_matched;
      return Exit_OK;
    }

  /* chsrc reset */
  else if (   xy_streql (command, "reset")
           || xy_streql (command, "rest")
           || xy_streql (command, "r"))
    {
      if (argc < cli_arg_Target_pos)
        {
          char *msg = ENGLISH ? "Please provide the target name you want to reset the source. " MSG_EN_USE_LIST_TARGETS
                              : "请提供想要重置源的目标名。" MSG_CN_USE_LIST_TARGETS;
          chsrc_error (msg);
          return Exit_Unknown;
        }

      ProgMode.ResetMode = true;
      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Reset_Source, NULL);
      if (!matched) goto not_matched;
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

not_matched:
  if (!matched)
    {
      char *msg = ENGLISH ? "Unknown target. "  MSG_EN_USE_LIST_TARGETS
                          : "暂不支持的换源目标。" MSG_CN_USE_LIST_TARGETS;
      chsrc_error (msg);
      return Exit_Unknown;
    }
}
