/** ------------------------------------------------------------
 * Copyright © 2023-2025 Aoran Zeng, Heng Guo
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Project Name    :     chsrc
 * Project Authors :   Aoran Zeng   <ccmywish@qq.com>
 *                 |    Heng Guo    <2085471348@qq.com>
 * Contributors    :   Aaron Ruan   <aaron212cn@outlook.com>
 *                 |    Rui Chen    <rui@chenrui.dev>
 *                 |  Shengwei Chen <414685209@qq.com>
 *                 |    BlockLune   <blocklune@gmail.com>
 *                 |    Mr. Will    <mr.will.com@outlook.com>
 *                 |    Terrasse    <terrasse@qq.com>
 *                 |    Lontten     <lontten@163.com>
 *                 |   happy game   <happygame1024@gmail.com>
 *                 |    Word2VecT   <tangzinan@bupt.edu.cn>
 *                 |     xuan       <wick.dynex@qq.com>
 *                 |    GnixAij     <gaojiaxing0220@gmail.com>
 *                 |    ChatGPT     <https://chatgpt.com>
 *                 |     czyt       <czyt.go@gmail.com>
 *                 |     zouri      <guoshuaisun@outlook.com>
 *                 |   yongxiang    <1926885268@qq.com>
 *                 |     YU-7       <2747046473@qq.com>
 *                 |    juzeon      <skyjuzheng@gmail.com>
 *                 |  Jialin Lyu    <jialinlvcn@aliyun.com>
 *                 | GitHub Copilot <https://github.com/copilot>
 *                 |     ccy        <icuichengyi@gmail.com>
 *                 |  MadDogOwner   <xiaoran@xrgzs.top>
 *                 |  sanchuanhehe  <wyihe5520@gmail.com>
 *                 |  Mikachu2333   <mikachu.23333@zohomail.com>
 *                 |   Rui Yang     <techoc@foxmail.com>
 *                 |  BingChunMoLi  <bingchunmoli@bingchunmoli.com>
 *                 |
 * Created On      : <2023-08-28>
 * Last Modified   : See <Chsrc_Release_Date>
 *
 * chsrc: Change Source —— 全平台通用命令行换源工具
 * ------------------------------------------------------------*/

#define Chsrc_Version        "0.2.2.2"
#define Chsrc_Release_Date   "2025/08/09"
#define Chsrc_Maintain_URL   "https://github.com/RubyMetric/chsrc"
#define Chsrc_Maintain_URL2  "https://gitee.com/RubyMetric/chsrc"

#include "framework/core.c"
#include "framework/chef.c"


#include "recipe/lang/rawstr4c.h"

#include "recipe/lang/Ruby/Ruby.c"
#include "recipe/lang/Python/common.h"
 #include "recipe/lang/Python/pip.c"
  #include "recipe/lang/Python/Poetry.c"
  #include "recipe/lang/Python/PDM.c"
  #include "recipe/lang/Python/Rye.c"
  #include "recipe/lang/Python/uv.c"
#include "recipe/lang/Python/Python.c"

#include "recipe/lang/JavaScript/common.h"
  #include "recipe/lang/JavaScript/npm.c"
  #include "recipe/lang/JavaScript/pnpm.c"
  #include "recipe/lang/JavaScript/Yarn.c"
#include "recipe/lang/JavaScript/JavaScript.c"
#include "recipe/lang/JavaScript/Bun.c"
#include "recipe/lang/JavaScript/nvm.c"

#include "recipe/lang/Perl.c"
#include "recipe/lang/PHP.c"
#include "recipe/lang/Lua.c"
#include "recipe/lang/Go.c"
#include "recipe/lang/Java.c"



#include "rawstr4c.h"



void
cli_print_available_mirrors ()
{
  {
  char *msg = ENGLISH ? "To specify a source, use chsrc set " : "指定使用某源，请使用 chsrc set ";
  say (bdblue(xy_strjoin (3, msg, "<target>", " <code>\n")));
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

  for (int i = 0; i < xy_arylen (chsrc_available_mirrors); i++)
    {
      MirrorSite_t *mir = chsrc_available_mirrors[i];
      printf ("%-14s%-18s%-41s ", mir->code, mir->abbr, mir->site); say (mir->name);
    }
}


/**
 * 遍历以空格分隔的别名字符串，对每个别名调用回调函数
 *
 * @param aliases   空格分隔的 alias 字符串
 * @param callback  对每个 alias 调用的回调函数
 * @param user_data 传递给回调函数的用户数据
 * @return          如果回调函数返回true则停止遍历并返回true，否则返回false
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
      while (*cursor != ' ' && *cursor != '\0') cursor++;

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
 * 用于 cli_print_supported_targets_ 的回调函数，打印每个别名
 */
bool
print_alias_callback (const char *alias, void *user_data)
{
  printf ("%s  ", alias);
  return false; // 继续遍历，不停止
}

void
cli_print_supported_targets_ (TargetRegisterInfo_t registry[], size_t size)
{
  for (int i = 0; i < size; i++)
    {
      TargetRegisterInfo_t *entry = &registry[i];

      // 使用通用的别名遍历函数打印所有别名
      iterate_aliases (entry->aliases, print_alias_callback, NULL);

      br(); // 每个target换行
    }
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
  cli_print_supported_targets_ (chsrc_pl_menu, xy_arylen(chsrc_pl_menu));
  }

  {
  char *msg = ENGLISH ? "Operating Systems" : "操作系统";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_supported_targets_ (chsrc_os_menu, xy_arylen(chsrc_os_menu));
  }

  {
  char *msg = ENGLISH ? "Softwares" : "软件";
  say (bdgreen(msg));
  say ("-------------------------");
  cli_print_supported_targets_ (chsrc_wr_menu, xy_arylen(chsrc_wr_menu));
  }
}

void
cli_print_supported_pl ()
{
  char *msg = ENGLISH ? "Support following Programming Languages (same line indicates these targets are compatible)\n"
                      : "支持对以下编程语言生态换源 (同一行表示这几个目标兼容)\n";
  say (bdgreen(msg));

  cli_print_supported_targets_ (chsrc_pl_menu,   xy_arylen(chsrc_pl_menu));
}

void
cli_print_supported_os ()
{
  char *msg = ENGLISH ? "Support following Operating Systems (same line indicates these targets are compatible)\n"
                      : "支持对以下操作系统换源 (同一行表示这几个目标兼容)\n";
  say (bdgreen(msg));
  cli_print_supported_targets_ (chsrc_os_menu, xy_arylen(chsrc_os_menu));
}

void
cli_print_supported_wr ()
{
  char *msg = ENGLISH ? "Support following Softwares (same line indicates these targets are compatible)\n"
                      : "支持对以下软件换源 (同一行表示这几个目标兼容)\n";
  say (bdgreen(msg));
  cli_print_supported_targets_ (chsrc_wr_menu, xy_arylen(chsrc_wr_menu));
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
  char *get_msg = xy_strjoin (3, msg, "| chsrc get ", input_target_name);
  if (target->getfn != NULL) printf (" %s%s\n", bdgreen(YesMark), purple(get_msg));
  else printf (" %s%s\n", bdred(NoMark), get_msg);br();
  }

  {
  char *msg = ENGLISH ? " Reset: Reset to the default source " : " Reset: 重置回默认源 ";
  char *reset_msg = xy_strjoin (3, msg, "| chsrc reset ", input_target_name);
  if (target->resetfn != NULL) printf (" %s%s\n", bdgreen(YesMark), purple(reset_msg));
  else printf (" %s%s\n", bdred(NoMark), reset_msg);br();
  }

  {
  char *msg = ENGLISH ? " UserDefine: using user-defined source URL " : " UserDefine: 用户自定义换源URL ";
  char *user_define_msg = xy_strjoin (5, msg, "| chsrc set ", input_target_name, " https://user-define-url.org/", input_target_name);
  if (target->can_user_define) printf (" %s%s\n", bdgreen(YesMark), purple(user_define_msg));
  else printf (" %s%s\n", bdred(NoMark), user_define_msg);br();
  }

  {
  char *msg = ENGLISH ? " Locally: Change source only for this project " : " Locally: 仅对本项目换源 ";
  char *locally_msg = xy_strjoin (3, msg, "| chsrc set -local ", input_target_name);

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
  char *english_msg = xy_strjoin (3, msg, "| chsrc set -en ", input_target_name);
  if (target->can_english) printf (" %s%s\n", bdgreen(YesMark), purple(english_msg));
  else printf (" %s%s\n", bdred(NoMark), english_msg);br();
  }

  if (target->note)
    {
      char *msg = ENGLISH ? "NOTE: " : "备注: ";
      printf ("%s%s\n", bdyellow (msg), bdyellow (target->note));
    }
}


void
cli_print_target_maintain_info (Target_t *target, const char *input_target_name)
{
  {
  char *msg = ENGLISH ? "\nMaintainer Information:\n" : "\n维护信息:\n";
  say (bdblue(msg));
  }

  if (target->authors && target->authors_n > 0)
    {
      char *msg = ENGLISH ? "Recipe Authors: " : "配方作者: ";
      printf ("%s", bdgreen(msg));
      for (size_t i = 0; i < target->authors_n; i++)
        {
          if (i > 0) printf (", ");
          printf ("%s <%s>",
                  target->authors[i].name  ? target->authors[i].name : "Unknown",
                  target->authors[i].email ? target->authors[i].email : "unknown@example.com");
        }
      printf ("\n");
    }


  {
    char *msg = ENGLISH ? "Chef: " : "主厨 Chef: ";
    if (target->chef)
      {
        printf ("%s%s <%s>\n", bdgreen(msg),
                target->chef->name  ? target->chef->name  : "Unknown",
                target->chef->email ? target->chef->email : "unknown@example.com");
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎担任!" : "Vacant, Welcome to hold the position";
        printf ("%s%s\n", bdgreen(msg), bdgreen(msg1));
      }
  }


  {
    char *msg = ENGLISH ? "Sous Chefs: " : "副厨 Sous Chefs: ";
    if (target->sous_chefs && target->sous_chefs_n > 0)
      {
        printf ("%s", bdgreen(msg));
        for (size_t i = 0; i < target->sous_chefs_n; i++)
          {
            if (i > 0) printf (", ");
            printf ("%s <%s>",
                    target->sous_chefs[i].name  ? target->sous_chefs[i].name : "Unknown",
                    target->sous_chefs[i].email ? target->sous_chefs[i].email : "unknown@example.com");
          }
        printf ("\n");
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎担任!" : "Vacant, Welcome to hold the position!";
        printf ("%s%s\n", bdgreen(msg), bdgreen(msg1));
      }
  }

  {
    char *msg = ENGLISH ? "Contributors: " : "贡献者: ";
    if (target->contributors && target->contributors_n > 0)
      {
        printf ("%s", bdgreen(msg));
        for (size_t i = 0; i < target->contributors_n; i++)
          {
            if (i > 0) printf (", ");
            printf ("%s <%s>",
                    target->contributors[i].name ? target->contributors[i].name : "Unknown",
                    target->contributors[i].email ? target->contributors[i].email : "unknown@example.com");
          }
        printf ("\n");
      }
    else
      {
        char *msg1 = CHINESE ? "暂空缺, 欢迎参与贡献!" : "Vacant, Welcome to contribute!";
        printf ("%s%s\n", bdgreen(msg), bdgreen(msg1));
      }
  }


  if (target->created_on)
    {
      char *msg = ENGLISH ? "Recipe Created On: " : "配方创建时间: ";
      printf ("%s%s\n", bdgreen(msg), target->created_on);
    }

  if (target->last_updated)
    {
      char *msg = ENGLISH ? "Recipe Last Updated: " : "配方最后更新: ";
      printf ("%s%s\n", bdgreen(msg), target->last_updated);
    }

  if (target->sources_last_updated)
    {
      char *msg = ENGLISH ? "Sources Last Updated: " : "源列表最后更新: ";
      printf ("%s%s\n", bdgreen(msg), target->sources_last_updated);
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
  char *version_string = purple("v" Chsrc_Version "-" Chsrc_Release_Date);

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
 * 用于 iterate_menu_ 的回调函数，检查别名是否匹配用户输入
 */
bool
match_alias_callback (const char *alias, void *user_data)
{
  const char *input = (const char *)user_data;
  return xy_streql_ic (input, alias);
}


/**
 * 查询用户输入 @param:input 是否与该 @param:registry 中的某个 target 匹配
 * 若匹配将直接调用 prelude
 *
 * @param[in]  registry  registry
 * @param[in]  size      registry 大小
 * @param[in]  input     用户输入的目标名
 * @param[out] target    返回匹配到的 Target_t 指针
 *
 * @return 匹配到则返回true，未匹配到则返回false
 */
#define iterate_menu(ary, input, target) iterate_menu_(ary, xy_arylen(ary), input, target)
bool
iterate_menu_ (TargetRegisterInfo_t registry[], size_t size, const char *input, Target_t **target)
{
  for (int i = 0; i < size; i++)
    {
      TargetRegisterInfo_t *entry = &registry[i];

      if (iterate_aliases (entry->aliases, match_alias_callback, (void *)input))
        {
          if (entry->prelude)
            {
              chsrc_log ("该target已定义 prelude()");
              entry->prelude();
            }
          else
            {
              chsrc_warn ("该target未定义 prelude()");
            }

          *target = entry->target;
          return true;
        }
    }

  *target = NULL;
  return false;
}


/**
 * 我们总是最后告诉用户一些维护信息
 */
void
cli_notify_lastly_for_users ()
{
  br();
  chsrc_note2 (RAWSTR_chsrc_last_message);
}



typedef enum {
  TargetOp_Get_Source = 1,
  TargetOp_Set_Source,
  TargetOp_Reset_Source,
  TargetOp_Measure_Source,
  TargetOp_List_Config
} TargetOp;

/**
 * 寻找target，并根据 @param:code 执行相应的操作
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
  Target_t *target = NULL;

           bool matched = iterate_menu (chsrc_pl_menu, input, &target);
  if (!matched) matched = iterate_menu (chsrc_os_menu, input, &target);
  if (!matched) matched = iterate_menu (chsrc_wr_menu, input, &target);

  if (!matched) return false;

  if (TargetOp_Set_Source==code)
    {
      if (target->setfn)
        {
          target->setfn(option);
          cli_print_target_maintain_info (target, input);
        }
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现 set 功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_Reset_Source==code)
    {
      if (target->resetfn)
        {
          target->resetfn(option);
          cli_print_target_maintain_info (target, input);
        }
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现 reset 功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_Get_Source==code)
    {
      if (target->getfn)
        {
          target->getfn("");
          cli_print_target_maintain_info (target, input);
        }
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现 get 功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_List_Config==code)
    {
      {
      char *msg = ENGLISH ? "To specify a source, use chsrc set " : "指定使用某源，请使用 chsrc set ";
      say (bdblue(xy_strjoin (3, msg, input, " <code>\n")));
      }

      {
      char *msg = ENGLISH ? "Available Sources: \n" : "可用源: \n";
      say (bdgreen(msg));
      }

      {
      char *msg1 = ENGLISH ? "Mirror abbr" : "镜像站简写";
      char *msg2 = ENGLISH ? "Source URL"  : "换源URL";
      char *msg3 = ENGLISH ? "Mirror Name" : "镜像站";
      char *format = ENGLISH ? "  %-13s%-33s%-38s%s\n" : "  %-13s%-36s%-46s%s\n";
      printf (format, "code", msg1, msg2, msg3);
      say    ("---------    --------------    -----------------------------------------------    ---------------------");
      }

      cli_print_target_available_sources (target->sources, target->sources_n);
      cli_print_target_features (target, input);
      cli_print_target_maintain_info (target, input);
    }
  else if (TargetOp_Measure_Source==code)
    {
      auto_select_mirror (target->sources, target->sources_n, input);
      return true;
    }

  if (TargetOp_Set_Source==code || TargetOp_Measure_Source==code)
    {
      cli_notify_lastly_for_users();
    }

  return true;
}


int
main (int argc, char const *argv[])
{
  xy_useutf8 (); argc -= 1;

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
              chsrc_error (xy_2strjoin (msg, argv[i])); return Exit_Unknown;
            }
          cli_arg_Target_pos++;
          cli_arg_Mirror_pos++;
        }
    }


  bool matched = false;

  if (in_dry_run_mode())
    {
      char *dry_msg = ENGLISH ? "Enable [Dry Run] mode. " \
                                         "Simulate the source changing process (skipping speed measurement). " \
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
          if (xy_streql (target, "mirrors") || xy_streql (target, "mirror"))
            {
              cli_print_available_mirrors (); return Exit_OK;
            }
          else if (xy_streql (target, "targets") || xy_streql (target, "target"))
            {
              cli_print_supported_targets (); return Exit_OK;
            }
          else if (xy_streql (target, "os"))
            {
              cli_print_supported_os (); return Exit_OK;
            }
          else if (xy_streql (target, "lang") || xy_streql (target, "pl") || xy_streql (target, "language"))
            {
              cli_print_supported_pl(); return Exit_OK;
            }
          else if (xy_streql (target, "ware") || xy_streql (target, "software"))
            {
              cli_print_supported_wr (); return Exit_OK;
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
      chsrc_error (xy_strjoin (3, msg1, command, msg2));
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
