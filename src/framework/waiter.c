/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : source.c
 * File Authors  : @ccmywish
 *               | @G_I_Y
 * Contributors  : Nul None <nul@none.org>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2026-09-07>
 *
 * 由 chsrc-main.c 拆分而来
 * ------------------------------------------------------------*/

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
cli_print_dish_available_sources (Dish_t *dish)
{
  size_t size = dish->sources_n;
  for (int i=0; i<size; i++)
    {
      Source_t src = dish->sources[i];
      const MirrorSite_t *mir = src.mirror;
      if (NULL == src.url)
        {
          src.url = "Please help to add the upstream url!";
        }
      printf ("%-12s %-55s %-4s %s\n", mir->code, src.url, " ", mir->name);
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



static char *
indent_level_to_string (unsigned int indent_level)
{
  const int indent_max = 64;
  char *indent_str = xy_malloc0 (indent_max);
  memset (indent_str, ' ', indent_max);

  indent_str[indent_max - 1] = '\0';
  if (indent_level > indent_max - 1)
    {
      chsrc_breakdown ("Indentation too deep!");
    }
  else
    {
      indent_str[indent_level] = '\0';
    }
  return indent_str;
}


/**
 * @brief 详细打印维护信息
 *
 * 用于 chsrc ls <dish>
 */
void
cli_print_dish_maintain_info (Dish_t *dish, const char *input_dish_name, unsigned int indent)
{
  char *indent_str = indent_level_to_string (indent);
  if (dish->created_on)
    {
      char *msg = CHINESE ? "食谱创建: " : "Recipe Created On: ";
      printf ("%s%s%s ", indent_str, bdblue(msg), dish->created_on);
    }

  if (dish->last_updated)
    {
      char *msg = CHINESE ? "食谱更新: " : "Recipe Last Updated: ";
      printf ("%s%s\n", bdblue(msg), dish->last_updated);
    }

  {
    char *msg = CHINESE ? "主厨: " : "Chefs: ";
    if (dish->chefs && xy_seq_len(dish->chefs) > 0)
      {
        printf ("%s%s", indent_str, bdblue(msg));
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
    char *msg = CHINESE ? "调味: " : "Sauciers: ";
    if (dish->sauciers && xy_seq_len(dish->sauciers) > 0)
      {
        printf ("%s%s", indent_str, bdblue(msg));
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

  if (chsrc_in_dish_group_mode() && current_combo_dish()->all_sub_dishes_use_same_source)
    {
      /**
       * 这种情况下，list 已经帮我们展示了所有 sub dish 共享的这个 sources dish，
       * 我们不要再对每个 sub dish 重复显示它了
       */
      xy_noop();
    }
  else
    {
      if (dish_use_other_dish_sources(dish))
        {
          char *alias = dish_get_first_alias(dish->sources_dish);

          indent_str[indent] = ' ';
          indent_str[indent + 4] = '\0';

          printf ("\n%s%s %s%s\n", indent_str, bdpurple(input_dish_name),
            CHINESE ? "使用其它菜品的源: " : "uses other dish's source: ", bdpurple(alias));

          Dish_t *sources_dish = dish->sources_dish;
          cli_print_dish_maintain_info (sources_dish, alias, indent + 4);
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
 * 由 waiter_handle_user_command() 拆分而来
 */
void
waiter_handle_List_Info (Dish_t *dish, const char *input, char *option)
{
  /* group dish 仅展示维护信息 */
  if (dish_has_sub_dishes(dish))
    {
      push_combo_stack (dish);

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

          printf ("%s%s\n",
            indent_level_to_string((current_combo_stack_depth()-1) * 2),
            bdpurple (sub_dish->aliases));

          /* 嵌套的 combo 的处理 */
          char *sub_dish_alias = dish_get_first_alias(sub_dish);
          if (dish_has_sub_dishes(sub_dish))
            {
              waiter_handle_List_Info (sub_dish, sub_dish_alias, option);
            }
          else
            {
              cli_print_dish_maintain_info (sub_dish, sub_dish_alias, (current_combo_stack_depth()-1) * 2);
              br();
            }
        }

      if (dish->all_sub_dishes_use_same_source)
        {
          char *msg = CHINESE ? "上述所有子菜品借用本菜品源"
                              : "All above sub dishes use the same source from this";

          Dish_t *shared_dish = dish->sources_dish;

          if (combo_has_sub_dish(dish, shared_dish))
            {
              /* 如果上述已经打印过了，就不再多余打印，参考 JavaScript combo dish */
              xy_noop();
            }
          else
            {
              char *alias = dish_get_first_alias(shared_dish);
              printf ("%s (%s)\n", bdpurple(alias), msg);
              cli_print_dish_maintain_info (shared_dish, alias, (current_combo_stack_depth()-1) * 2);
            }
        }

      pop_combo_stack();
      return;
    }

  {
  char *msg = CHINESE ? "指定使用某源，请使用 chsrc set " : "To specify a source, use chsrc set ";
  say (bdblue(xy_strcat (3, msg, input, " <code>\n")));
  }

  {
  char *msg = CHINESE ? "可用源: \n" : "Available Sources: \n";
  say (bdgreen(msg));
  }

  {
  char *msg1 = CHINESE ? "换源链接" : "Source URL";
  char *msg2 = CHINESE ? "镜像站"   : "Mirror Name";
  char *format = CHINESE ?
    "  %-13s %-18s %-38s %-10s %s\n" :
    "  %-13s %-18s %-38s %-10s %s\n";
  printf (format,
    "code",  " ",  msg1, " ", msg2);

  say    ("---------    ---------------------------------------------------------    ------------------------");
  cli_print_dish_available_sources (dish);
  }

  cli_print_dish_features (dish, input);

  {
  char *msg = CHINESE ? "维护信息:\n" : "Maintainer Information:\n";
  say (bdgreen(msg));
  cli_print_dish_maintain_info (dish, input, 0);
  }
}


/**
 * 由 waiter_handle_user_command() 拆分而来
 */
void
waiter_handle_Measure_Source (Dish_t *dish, const char *input, char *option)
{
  /* dish group 不测速，让用户自己分别测速 */
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
      dish_select_fastest_source (dish);
      return;
    }
}


/**
 * 由 waiter_handle_user_command() 拆分而来
 */
void
waiter_handle_Get_Source (Dish_t *dish, const char *input, char *option)
{
  if (dish_has_sub_dishes(dish))
    {
      for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple(sub_dish->aliases));
          waiter_handle_Get_Source (sub_dish, dish_get_first_alias(sub_dish), option);
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
 * 由 waiter_handle_user_command() 拆分而来
 */
void
waiter_handle_Set_Source (Dish_t *dish, const char *input, char *option)
{
  char *user_defined_url = NULL;
  char *mirror_code = NULL;

  if (hp_is_url (option))
    {
      user_defined_url = option;
    }
  else if (option)
    {
      mirror_code = option;
    }

  if (dish_has_sub_dishes(dish))
    {
      push_combo_stack (dish);

      if (user_defined_url)
        {
          if (!dish->all_sub_dishes_use_same_source)
            {
              char *default_msg = "该套餐不支持用户自定义源，因为该套餐的多个子菜品所需要的源不同，无法确定你提供的源到底适用于哪个子菜品，因此请尝试单独为每个子菜品指定源";
              chsrc_error (default_msg);
              exit (Exit_Unsupported);
            }
        }
      else if (mirror_code)
        {
          if (!combo_at_least_one_sub_dish_has_source_from_mirror (dish, mirror_code))
            {
              char *default_msg = "该套餐的所有子菜品都不支持你指定的镜像站，请尝试使用其他镜像站或自定义源";
              chsrc_error (default_msg);
              exit (Exit_Unsupported);
            }
        }

      for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple(sub_dish->aliases));
          waiter_handle_Set_Source (sub_dish, dish_get_first_alias(sub_dish), option);
          br();
        }

      pop_combo_stack ();
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
 * 由 waiter_handle_user_command() 拆分而来
 */
void
waiter_handle_Reset_Source (Dish_t *dish, const char *input, char *option)
{
  if (dish_has_sub_dishes(dish))
    {
      for (size_t i=0; i<xy_seq_len(dish->sub_dishes); i++)
        {
          Dish_t *sub_dish = xy_seq_at (dish->sub_dishes, i);
          sub_dish->preparefn();
          println (bdpurple(sub_dish->aliases));
          waiter_handle_Reset_Source (sub_dish, dish_get_first_alias(sub_dish), option);
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
waiter_handle_user_command (Dish_t *dish, TargetCmd code, const char *input, char *option)
{
  if (TargetCmd_Get_Source==code || TargetCmd_Set_Source==code || TargetCmd_Reset_Source==code)
    {
      if (dish_has_sub_dishes(dish))
        {
          int sub_count = xy_seq_len(dish->sub_dishes);
          char *zh_msg = bdyellow(xy_strcat (5, "套餐 ", input, " 由 ", xy_int2str(sub_count), " 个子菜品组成: "));
          char *en_msg = bdyellow(xy_strcat (5, "Combo dish '", input, "' consists of ", xy_int2str(sub_count), " sub dishes\n"));

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
      waiter_handle_Set_Source (dish, input, option);
    }
  else if (TargetCmd_Reset_Source==code)
    {
      waiter_handle_Reset_Source (dish, input, option);
    }
  else if (TargetCmd_Get_Source==code)
    {
      waiter_handle_Get_Source (dish, input, option);
    }
  else if (TargetCmd_List_Info==code)
    {
      waiter_handle_List_Info (dish, input, option);
    }
  else if (TargetCmd_Measure_Source==code)
    {
      waiter_handle_Measure_Source (dish, input, option);
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
              // waiter_handle_user_command (sub_dish, code, input, option);
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
