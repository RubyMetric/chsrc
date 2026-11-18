/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * 命令行解析：本体选项、业务位置参数与 recipe 原始参数分别处理。
 * ------------------------------------------------------------*/

#ifdef XY_Build_On_Windows
#include <shellapi.h>
#endif

typedef enum CliAction_t
{
  CliAction_Run,
  CliAction_Help,
  CliAction_Version,
  CliAction_Error
}
CliAction_t;

typedef struct CliArgs_t
{
  const char *command;
  const char *dish_name;
  char *mirror;
  RecipeArgs_t recipe_args;
}
CliArgs_t;

/**
 * Windows 的窄字符 main(argv) 可能已丢失 Unicode，改从宽字符命令行读取。
 * 由系统解析引号，转码复用 xy；其他平台保留原有 argv。
 * 转换后的参数贯穿本次 CLI 调用，随进程退出回收。
 */
static bool
cli_get_utf8_args (int *argc, const char ***argv)
{
#ifdef XY_Build_On_Windows
  int count = 0;
  wchar_t **wide = CommandLineToArgvW (GetCommandLineW (), &count);
  if (!wide)
    return false;

  const char **utf8 = xy_malloc0 (((size_t) count + 1) * sizeof (*utf8));
  bool ok = true;
  for (int i=0; i<count; i++)
    {
      utf8[i] = xy_wstr_to_utf8 (wide[i]);
      if (!utf8[i])
        {
          ok = false;
          break;
        }
    }
  LocalFree (wide);
  if (!ok)
    return false;

  *argc = count;
  *argv = utf8;
#endif
  return true;
}

static bool
cli_parse_scope (const char *value)
{
  static const struct { const char *name; Scope_t scope; } scopes[] = {
    {"default", ImplementationDefinedScope},
    {"project", ProjectScope},
    {"user",    UserScope},
    {"system",  SystemScope}
  };
  for (size_t i=0; i<xy_c_array_len(scopes); i++)
    if (xy_streql_ic (value, scopes[i].name))
      {
        ProgMode.Scope = scopes[i].scope;
        return true;
      }

  chsrc_error (xy_strcat (3,
    ENGLISH ? "Invalid scope: " : "无效的换源作用域: ", value,
    ENGLISH ? ". Valid scopes are: default, project, user, system"
            : "。有效的换源作用域为: default, project, user, system"));
  return false;
}

static bool
cli_parse_flag (const char *arg)
{
  const struct { const char *name; bool *flag; } flags[] = {
    {"--dry",      &ProgMode.DryRunMode},
    {"--ipv6",     &ProgMode.Ipv6Mode},
    {"--english",  &ProgMode.EnglishMode},
    {"--English",  &ProgMode.EnglishMode},
    {"--no-color", &ProgMode.NoColorMode}
  };
  for (size_t i=0; i<xy_c_array_len(flags); i++)
    if (xy_streql (arg, flags[i].name))
      {
        *flags[i].flag = true;
        if (ProgMode.NoColorMode)
          xy.enable_color = false;
        return true;
      }
  return false;
}

static CliAction_t
cli_parse_args (int argc, const char *const argv[], CliArgs_t *parsed)
{
  *parsed = (CliArgs_t) {0};
  const char *extra = NULL;

  for (int i=1; i<argc; i++)
    {
      const char *arg = argv[i];
      if (xy_streql (arg, "--"))
        {
          parsed->recipe_args.argc = argc - i - 1;
          parsed->recipe_args.argv = argv + i + 1;
          break;
        }

      /* 立即返回，既不读取后续选项，也不校验已收集的业务参数。 */
      if (xy_streql (arg, "--version"))
        return CliAction_Version;
      if (   xy_streql (arg, "--help") || xy_streql (arg, "-h")
          || (!parsed->command && xy_streql (arg, "help")))
        return CliAction_Help;

      if (xy_streql (arg, "--scope") || xy_str_start_with (arg, "--scope="))
        {
          const char *value;
          if (xy_streql (arg, "--scope"))
            {
              if (i + 1 >= argc || xy_str_start_with (argv[i+1], "-"))
                {
                  chsrc_error (ENGLISH ? "Option --scope requires an argument"
                                      : "--scope 选项缺少参数");
                  return CliAction_Error;
                }
              value = argv[++i];
            }
          else
            value = arg + strlen ("--scope=");
          if (!cli_parse_scope (value))
            return CliAction_Error;
          continue;
        }

      if (cli_parse_flag (arg))
        continue;
      if (xy_str_start_with (arg, "-"))
        {
          chsrc_error (xy_2strcat (ENGLISH ? "Unknown option: " : "未识别的命令行选项 ", arg));
          return CliAction_Error;
        }

      if (!parsed->command)
        parsed->command = arg;
      else if (!parsed->dish_name)
        parsed->dish_name = arg;
      else if (!parsed->mirror)
        parsed->mirror = xy_strdup (arg);
      else if (!extra)
        extra = arg;
    }

  if (!parsed->command)
    return CliAction_Help;

  /* 业务参数校验延后到扫描结束，使后置的帮助/版本可以直接返回。 */
  const char *command = parsed->command;
  bool set = xy_streql (command, "set") || xy_streql (command, "s");
  bool issue = xy_streql (command, "issue") || xy_streql (command, "issues")
            || xy_streql (command, "isue")  || xy_streql (command, "i");
  if (issue && parsed->dish_name)
    extra = parsed->dish_name;
  else if (!set && parsed->mirror)
    extra = parsed->mirror;
  if (extra)
    {
      chsrc_error (xy_2strcat (ENGLISH ? "Unexpected positional argument: " : "多余的位置参数: ", extra));
      return CliAction_Error;
    }
  return CliAction_Run;
}
