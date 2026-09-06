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
#include "framework/base.c"
#include "framework/OS.c"
#include "framework/dish.c"
#include "framework/chef.c"
#include "framework/source.c"

/* 某些特殊 recipe 需要依赖 toml 解析库 */
#include "mikachu-toml.h"


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
  chef_register_contributor ("@rocascent",       "Peng Gao",       "gn3po4g@outlook.com",            NULL);
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
  chef_register_contributor ("@JIAFALSEDREAM",  "假发",           "jiafa@hello-happy.world",         NULL);
  chef_register_contributor ("@yayoinoyume",    "YAYOI27",        "yayoi27@yayoi.uk",               NULL);

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

#include "framework/waiter.c"

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
              waiter_handle_user_command (the_found_dish, TargetCmd_List_Info, dish_name, NULL);
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
          waiter_handle_user_command (the_found_dish, TargetCmd_Measure_Source, dish_name, NULL);
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
          waiter_handle_user_command (the_found_dish, TargetCmd_Get_Source, dish_name, NULL);
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
          waiter_handle_user_command (the_found_dish, TargetCmd_Set_Source, dish_name, mirrorCode_or_url);
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
          waiter_handle_user_command (the_found_dish, TargetCmd_Reset_Source, dish_name, NULL);
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
