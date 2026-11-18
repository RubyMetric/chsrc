/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * CLI 解析与 recipe 分发回归测试，不执行真实换源操作。
 * 带参数运行时，通过测试 recipe 输出收到的参数字节，用于进程边界测试。
 * ------------------------------------------------------------*/

#define main chsrc_program_main
#include "../src/chsrc-main.c"
#undef main

static void
reset_modes (void)
{
  ProgMode.MeasureMode = ProgMode.ResetMode = false;
  ProgMode.Ipv6Mode = ProgMode.EnglishMode = ProgMode.DryRunMode = false;
  ProgMode.NoColorMode = true;
  ProgMode.Scope = ImplementationDefinedScope;
  xy.enable_color = false;
}

#define parse(a) cli_parse_args ((int) xy_c_array_len(a) - 1, a, &parsed)

static void
test_parser (void)
{
  CliArgs_t parsed;
  const char *base[] = {"chsrc", "set", "cargo", "first", NULL};
  const char *flags[] = {"--dry", "--ipv6", "--english", "--English", "--no-color"};
  for (size_t f=0; f<xy_c_array_len(flags); f++)
    for (int position=1; position<=4; position++)
      {
        reset_modes ();
        ProgMode.NoColorMode = false;
        xy.enable_color = true;
        const char *av[6] = {0};
        int src = 0;
        for (int i=0; i<5; i++)
          av[i] = i == position ? flags[f] : base[src++];
        assert (parse(av) == CliAction_Run);
        assert_str (parsed.command, "set");
        assert_str (parsed.dish_name, "cargo");
        assert_str (parsed.mirror, "first");
        assert (f != 0 || ProgMode.DryRunMode);
        assert (f != 1 || ProgMode.Ipv6Mode);
        assert ((f != 2 && f != 3) || ProgMode.EnglishMode);
        assert (f != 4 || !xy.enable_color);
      }

  for (int position=1; position<=4; position++)
    for (int equals=0; equals<2; equals++)
      {
        reset_modes ();
        const char *av[7] = {0};
        int out = 0;
        for (int i=0; i<5; i++)
          {
            if (i == position)
              {
                av[out++] = equals ? "--scope=PrOjEcT" : "--scope";
                if (!equals) av[out++] = "PrOjEcT";
              }
            if (i < 4) av[out++] = base[i];
          }
        assert (cli_parse_args (out, av, &parsed) == CliAction_Run);
        assert (ProgMode.Scope == ProjectScope);
        assert_str (parsed.dish_name, "cargo");
        assert_str (parsed.mirror, "first");
      }

  const struct { const char *av[9]; CliAction_t action; } cases[] = {
    {{"chsrc", NULL}, CliAction_Help},
    {{"chsrc", "--version", "--unknown", NULL}, CliAction_Version},
    {{"chsrc", "--unknown", "--version", NULL}, CliAction_Error},
    {{"chsrc", "set", "missing", "--help", "--scope", NULL}, CliAction_Help},
    {{"chsrc", "help", "--unknown", NULL}, CliAction_Help},
    {{"chsrc", "--version", "--help", NULL}, CliAction_Version},
    {{"chsrc", "-h", "--version", NULL}, CliAction_Help},
    {{"chsrc", "bad-command", "one", "two", "three", "--version", NULL}, CliAction_Version},
    {{"chsrc", "set", "x", "first", "extra", NULL}, CliAction_Error},
    {{"chsrc", "get", "x", "extra", NULL}, CliAction_Error},
    {{"chsrc", "issue", "extra", NULL}, CliAction_Error},
    {{"chsrc", "--scope", NULL}, CliAction_Error},
    {{"chsrc", "--scope", "--", "project", NULL}, CliAction_Error},
    {{"chsrc", "--scope=", NULL}, CliAction_Error},
    {{"chsrc", "--scope=invalid", NULL}, CliAction_Error},
    {{"chsrc", "--scope=invalid", "--help", NULL}, CliAction_Error},
    {{"chsrc", "--help", "--scope=invalid", NULL}, CliAction_Help},
    {{"chsrc", "set", "help", NULL}, CliAction_Run},
    {{"chsrc", "--", "--version", NULL}, CliAction_Help},
    {{"chsrc", "set", "x", "--", NULL}, CliAction_Run}
  };
  for (size_t i=0; i<xy_c_array_len(cases); i++)
    {
      reset_modes ();
      int n = 0;
      while (cases[i].av[n]) n++;
      assert (cli_parse_args (n, cases[i].av, &parsed) == cases[i].action);
    }

  const char *obsolete[] = {"-dry", "--dry-run", "-scope", "-local", "-ipv6",
    "-en", "-english", "-English", "-no-color", "-no-colour", "--no-colour", "-v", "-version", "-help"};
  for (size_t i=0; i<xy_c_array_len(obsolete); i++)
    {
      const char *av[] = {"chsrc", obsolete[i], NULL};
      assert (parse(av) == CliAction_Error);
    }

  reset_modes ();
  const char *tail[] = {"chsrc", "set", "abc", "--", "--help", "--dry", "--scope", "--",
    "--skfw sq", "", "he said \"hi\"", "C:\\a b\\", "中文 日本語 한국어 😀", NULL};
  assert (parse(tail) == CliAction_Run);
  assert (parsed.recipe_args.argc == 9);
  assert (parsed.recipe_args.argv == tail + 4);
  for (int i=0; i<parsed.recipe_args.argc; i++)
    assert (parsed.recipe_args.argv[i] == tail[i+4]);
  assert (!ProgMode.DryRunMode);
  assert (ProgMode.Scope == ImplementationDefinedScope);

  const char *later_english[] = {"chsrc", "--version", "--English", NULL};
  assert (parse(later_english) == CliAction_Version);
  assert (!ProgMode.EnglishMode);
  const char *earlier_english[] = {"chsrc", "--English", "--version", NULL};
  assert (parse(earlier_english) == CliAction_Version);
  assert (ProgMode.EnglishMode);

  const char *scopes[] = {"default", "project", "user", "system"};
  const Scope_t values[] = {ImplementationDefinedScope, ProjectScope, UserScope, SystemScope};
  for (size_t i=0; i<xy_c_array_len(scopes); i++)
    {
      const char *av[] = {"chsrc", "--scope=project", "set", "cargo", "--scope", scopes[i], NULL};
      assert (parse(av) == CliAction_Run);
      assert (ProgMode.Scope == values[i]);
    }
}

static const RecipeArgs_t *expected_args;
static TargetCmd expected_command;
static int parser_calls, operation_calls, stop_at;
static RecipeArgsResult_t stop_result;

static void mock_prepare (void) {}
static void mock_operation (char *option) { operation_calls++; }

static RecipeArgsResult_t
mock_parser (TargetCmd command, int argc, const char *const argv[])
{
  assert (command == expected_command);
  assert (argc == expected_args->argc);
  assert (argv == expected_args->argv);
  parser_calls++;
  return parser_calls == stop_at ? stop_result : RecipeArgs_Continue;
}

static void
test_dispatch (void)
{
  reset_modes ();
  const char *tail[] = {"--help", "nested \"quote\" 中文 😀", "", NULL};
  RecipeArgs_t args = {3, tail};
  expected_args = &args;
  Dish_t leaf = {.aliases = "leaf", .getfn = mock_operation, .setfn = mock_operation,
    .resetfn = mock_operation, .preparefn = mock_prepare};
  Dish_t ignored = leaf;
  chef_set_args_parser (&leaf, mock_parser);
  Dish_t inner = {.aliases = "inner", .preparefn = mock_prepare, .parse_args = mock_parser};
  Dish_t outer = {.aliases = "outer", .preparefn = mock_prepare, .parse_args = mock_parser};
  chef_set_sub_dishes (&inner, 2, &leaf, &ignored);
  chef_set_sub_dishes (&outer, 2, &inner, &leaf);

  typedef RecipeArgsResult_t (*Handler_t) (Dish_t *, const char *, char *, const RecipeArgs_t *);
  Handler_t handlers[] = {waiter_handle_Get_Source, waiter_handle_Set_Source, waiter_handle_Reset_Source};
  TargetCmd commands[] = {TargetCmd_Get_Source, TargetCmd_Set_Source, TargetCmd_Reset_Source};
  const struct {
    int stop_at;
    RecipeArgsResult_t callback_result, result;
    int operations, parsers;
  } scenarios[] = {
    {0, RecipeArgs_Continue, RecipeArgs_Continue, 3, 4},
    {3, RecipeArgs_Handled,  RecipeArgs_Continue, 2, 4},
    {3, RecipeArgs_Error,    RecipeArgs_Error,    0, 3},
    {1, RecipeArgs_Handled,  RecipeArgs_Handled,  0, 1}
  };
  for (size_t i=0; i<xy_c_array_len(handlers); i++)
    for (size_t scenario=0; scenario<xy_c_array_len(scenarios); scenario++)
      {
        expected_command = commands[i];
        parser_calls = operation_calls = 0;
        stop_at = scenarios[scenario].stop_at;
        stop_result = scenarios[scenario].callback_result;
        RecipeArgsResult_t result = handlers[i] (&outer, "outer", NULL, &args);
        assert (result == scenarios[scenario].result);
        assert (operation_calls == scenarios[scenario].operations);
        assert (parser_calls == scenarios[scenario].parsers);
        assert (current_combo_stack_depth () == 0);
      }

  /* 未注册解析器的组合仍向子 recipe 传递原始参数。 */
  outer.parse_args = inner.parse_args = NULL;
  expected_command = TargetCmd_Get_Source;
  parser_calls = operation_calls = stop_at = 0;
  assert (waiter_handle_Get_Source (&outer, "outer", NULL, &args) == RecipeArgs_Continue);
  assert (parser_calls == 2 && operation_calls == 3);

  RecipeArgs_t empty = {0};
  expected_args = &empty;
  parser_calls = operation_calls = 0;
  assert (waiter_handle_Get_Source (&leaf, "leaf", NULL, &empty) == RecipeArgs_Continue);
  assert (parser_calls == 1 && operation_calls == 1);
  expected_args = &args;

  /* 所有操作都在执行前调用解析器；结束/失败不再输出正常维护信息。 */
  for (int cmd=TargetCmd_Get_Source; cmd<=TargetCmd_List_Info; cmd++)
    for (int error=0; error<2; error++)
      {
        expected_command = cmd;
        parser_calls = operation_calls = 0;
        stop_at = 1;
        stop_result = error ? RecipeArgs_Error : RecipeArgs_Handled;
        int status = waiter_handle_user_command (&leaf, (TargetCmd) cmd, "leaf", NULL, &args);
        assert (status == (error ? Exit_Unknown : Exit_OK));
        assert (parser_calls == 1 && operation_calls == 0);
      }
  expected_args = NULL;
}

static RecipeArgsResult_t
echo_args (TargetCmd command, int argc, const char *const argv[])
{
  printf ("%d\n", argc);
  for (int i=0; i<argc; i++)
    {
      for (const unsigned char *p=(const unsigned char *) argv[i]; *p; p++)
        printf ("%02x", *p);
      putchar ('\n');
    }
  return RecipeArgs_Handled;
}

int
main (int argc, const char *argv[])
{
  chsrc_init_framework ();
  assert (cli_get_utf8_args (&argc, &argv));
  if (argc > 1)
    {
      CliArgs_t parsed;
      if (cli_parse_args (argc, argv, &parsed) != CliAction_Run)
        return Exit_Unknown;
      Dish_t probe = {.aliases = "probe", .parse_args = echo_args};
      return waiter_handle_user_command (&probe, TargetCmd_Set_Source, "probe", NULL, &parsed.recipe_args);
    }
  test_parser ();
  test_dispatch ();
  puts ("CLI parser and recipe dispatch tests passed");
  return 0;
}
