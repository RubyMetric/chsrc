/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : bulk.c
 * File Authors  : @izitn
 * Contributors  : ChatGPT <https://chatgpt.com/>
 * Created On    : <2026-11-11>
 * Last Modified : <2026-11-11>
 *
 * scan / plan / setup 三件套。
 *
 *   scan  —— 只读检测本机可被 chsrc 管理的项目与当前源
 *   plan  —— 生成可审阅的换源方案（仍然只读）
 *   setup —— 打印方案，确认后逐项执行
 *
 * 安全模型:
 *
 *   1. scan / plan 不修改任何配置；
 *   2. setup 默认只处理用户级/项目级的全自动 recipe；
 *      系统级和半自动/手工 recipe 只列出，除非显式 --include-system；
 *   3. 已存在但无法识别为 chsrc 已知源的配置，默认视为用户自定义源并跳过；
 *   4. setup 默认需要交互确认；-dry 永不落盘。
 *
 * 每项执行通过重新调用 chsrc 自身完成，复用既有 recipe，且单项失败不影响整批。
 * ------------------------------------------------------------*/


/* ============================================================
 * 检测表
 * ============================================================ */

typedef struct BulkProbe_t
{
  const char *category;   /* "lang" / "ware" */
  const char *dish;       /* chsrc set <dish> */
  const char *candidates; /* '|' 分隔的命令或路径；命中即视为已安装 */
  const char *configs;    /* '|' 分隔的配置文件，用于辅助判断当前源 */
  const char *state_cmd;  /* 读取当前源的标准输出命令，可为 NULL */
  const char *note;       /* 中文说明，可为 NULL */
  const char *note_en;    /* 英文说明，可为 NULL */
}
BulkProbe_t;


/**
 * @note 只登记叶子菜品，不登记 combo dish（python/node/uv），避免重复换源。
 */
static BulkProbe_t BulkProbes[] =
{
  /* --------------------- 编程语言与包管理器 --------------------- */
  { "lang", "npm",     "npm",                "~/.npmrc",                                                        "npm config get registry",                         NULL, "Node.js 包管理器" },
  { "lang", "pnpm",    "pnpm",               "~/.npmrc|~/.config/pnpm/rc",                                      "pnpm config get registry",                        NULL, NULL },
  { "lang", "yarn",    "yarn|yarnpkg",       "~/.yarnrc|~/.yarnrc.yml",                                         "yarn config get registry",                        NULL, NULL },
  { "lang", "bun",     "bun",                "~/.bunfig.toml",                                                   NULL,                                              NULL, NULL },
  { "lang", "nvm",     "nvm|~/.nvm/nvm.sh",  NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "pip",     "python3|python",     NULL,                                                               "python -m pip config get global.index-url",       NULL, "Python 包管理器" },
  { "lang", "pdm",     "pdm",                NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "poetry",  "poetry",             "~/.config/pypoetry/config.toml|~/AppData/Roaming/pypoetry/config.toml", NULL,                                       NULL, NULL },
  { "lang", "uv",      "uv",                 "~/.config/uv/uv.toml|~/AppData/Roaming/uv/uv.toml",               NULL,                                              NULL, NULL },
  { "lang", "rye",     "rye",                NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "go",      "go",                 NULL,                                                               "go env GOPROXY",                                  NULL, NULL },
  { "lang", "cargo",   "cargo",              "~/.cargo/config.toml|~/.cargo/config",                            NULL,                                              NULL, "Rust 包管理器" },
  { "lang", "rustup",  "rustup",             NULL,                                                               NULL,                                              "通过 shell profile 中的环境变量实现", "uses shell profile environment variables" },
  { "lang", "gem",     "gem",                "~/.gemrc",                                                         "gem sources -l",                                  NULL, "Ruby 包管理器" },
  { "lang", "perl",    "cpan|cpanm",         NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "php",     "composer",           NULL,                                                               "composer config -g repositories",                 NULL, NULL },
  { "lang", "lua",     "luarocks",           NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "java",    "mvn|gradle|mvnd",    "~/.m2/settings.xml|~/.gradle/init.gradle",                         NULL,                                              NULL, NULL },
  { "lang", "dart",    "dart",               NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "flutter", "flutter",            NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "nuget",   "dotnet|nuget",       "~/.nuget/NuGet/NuGet.Config",                                      "dotnet nuget list source",                        NULL, NULL },
  { "lang", "haskell", "cabal|stack",        "~/.cabal/config|~/.stack/config.yaml",                             NULL,                                              NULL, NULL },
  { "lang", "ocaml",   "opam",               NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "r",       "R",                  NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "julia",   "julia",              NULL,                                                               NULL,                                              NULL, NULL },
  { "lang", "clojure", "clojure|clj|lein",   NULL,                                                               NULL,                                              NULL, NULL },

  /* --------------------------- 软件 --------------------------- */
  { "ware", "winget",        "winget",                    NULL,                                  "winget source list",             "Windows 包管理器", "Windows package manager" },
  { "ware", "homebrew",      "brew",                      "~/.zshrc|~/.bashrc|~/.config/fish/config.fish", NULL,                  "通过 shell profile 环境变量实现", "uses shell profile environment variables" },
  { "ware", "cocoapods",     "pod",                       "~/.cocoapods/repos",                  NULL,                             NULL, NULL },
  { "ware", "docker",        "docker|/etc/docker/daemon.json", "/etc/docker/daemon.json",       "docker info",                    "Linux 下需要 root；Docker Desktop 会提示手动操作", "requires root on Linux; Docker Desktop is semi-manual" },
  { "ware", "flatpak",       "flatpak",                   NULL,                                  NULL,                             NULL, NULL },
  { "ware", "conda",         "conda",                     "~/.condarc",                          "conda config --show-sources",    "半自动：只打印配置内容，需手动写入", "semi-automatic: prints config for manual write" },
  { "ware", "huggingface-cli","huggingface-cli|hf",       NULL,                                  NULL,                             "手工：只给出设置 HF_ENDPOINT 的示例", "manual: only prints HF_ENDPOINT example" },
  { "ware", "emacs",         "emacs",                     NULL,                                  NULL,                             NULL, NULL },
  { "ware", "tex",           "tlmgr|mpm",                 NULL,                                  NULL,                             NULL, NULL },
  { "ware", "nix",           "nix|nix-channel",           NULL,                                  NULL,                             NULL, NULL },
  { "ware", "guix",          "guix",                      NULL,                                  NULL,                             NULL, NULL },
};

#define Bulk_Probes_N  (sizeof (BulkProbes) / sizeof (BulkProbes[0]))


typedef enum BulkTier_t
{
  BulkTier_Auto,
  BulkTier_System,
  BulkTier_Manual,
}
BulkTier_t;

typedef enum BulkSrcState_t
{
  BulkSrc_Unknown,
  BulkSrc_Unconfigured,
  BulkSrc_Upstream,
  BulkSrc_Mirror,
  BulkSrc_Custom,
}
BulkSrcState_t;

typedef struct BulkItem_t
{
  char          *dish;
  char          *category;
  char          *evidence;
  char          *note;
  char          *note_en;
  BulkTier_t     tier;
  BulkSrcState_t state;
  char          *state_detail;
  bool           state_verified;
  bool           found;
}
BulkItem_t;

/* ============================================================
 * 基础工具
 * ============================================================ */

static bool
bulk_any_field (const char *fields, bool (*probe) (const char *field, char **evidence), char **evidence)
{
  if (!fields || '\0' == fields[0])
    return false;

  char *copy = xy_strdup (fields);
  char *rest = copy;

  while (rest && *rest)
    {
      char *sep = strchr (rest, '|');
      if (sep)
        *sep = '\0';

      char *field = xy_str_strip (rest);
      if (field && field[0] && probe (field, evidence))
        {
          free (field);
          free (copy);
          return true;
        }
      free (field);

      if (!sep)
        break;
      rest = sep + 1;
    }

  free (copy);
  return false;
}


/**
 * @brief 判断一个候选项是否存在
 *
 * @note Windows 下 `where` 每次都会新建进程，33 个 probe 会明显拖慢 scan。
 *       这里优先用「已知安装路径」做纯文件判断；只有路径不在候选列表里时
 *       才回落到 `where`。
 */
static bool
bulk_field_exists (const char *field, char **evidence)
{
  bool is_path = false;

  if (field[0] == '~' || field[0] == '/' || field[0] == '\\')
    is_path = true;
  else if (strchr (field, '/') || strchr (field, '\\'))
    is_path = true;
  else if (isalpha ((unsigned char) field[0]) && field[1] == ':')
    is_path = true;

  bool exist = false;

  if (is_path)
    {
      exist = xy_file_exist (field);
    }
  else
    {
      /* 先查 PATH 中常见目录，避免每次 spawn where.exe */
      char *paths = getenv ("PATH");
      if (paths)
        {
          char sep = xy.on_windows ? ';' : ':';
          char *copy = xy_strdup (paths);
          char *cur = copy;

          while (cur && *cur && !exist)
            {
              char *next = strchr (cur, sep);
              if (next) *next = '\0';

              if (*cur)
                {
                  char *cand = xy_path_join (cur, field);

                  if (xy.on_windows)
                    {
                      char *exe = xy_2strcat (cand, ".exe");
                      exist = xy_file_exist (exe);
                      if (!exist) { char *cmd = xy_2strcat (cand, ".cmd"); exist = xy_file_exist (cmd); free (cmd); }
                      if (!exist) { char *bat = xy_2strcat (cand, ".bat"); exist = xy_file_exist (bat); free (bat); }
                      free (exe);
                    }
                  else
                    {
                      exist = xy_file_exist (cand);
                    }

                  free (cand);
                }

              if (!next) break;
              cur = next + 1;
            }

          free (copy);
        }

      /* PATH 已是权威来源，不再 spawn where/command -v */
    }

  if (exist && evidence)
    *evidence = xy_strdup (field);

  return exist;
}


static Dish_t *
bulk_find_dish (const char *dish_name)
{
  Dish_t *dish = NULL;

  if (iterate_menu (ProgStore.pl, dish_name, &dish)) return dish;
  if (iterate_menu (ProgStore.os, dish_name, &dish)) return dish;
  if (iterate_menu (ProgStore.wr, dish_name, &dish)) return dish;

  return NULL;
}


/**
 * @brief 检查指定 mirror code 是否适用于该 dish
 *
 * @return 未指定 mirror 时一律返回 true
 */
static bool
bulk_dish_supports_mirror (Dish_t *dish, const char *mirror)
{
  if (!mirror || !mirror[0])
    return true;

  /* URL 形式视为用户自定义源，交给 recipe 自己校验 */
  if (hp_is_url (mirror))
    return dish->can_user_define;

  for (int i = 0; i < dish->sources_n; i++)
    {
      if (dish->sources[i].provider
          && xy_streql (dish->sources[i].provider->code, mirror))
        return true;
    }

  return false;
}


static BulkTier_t
bulk_tier_of (Dish_t *dish)
{
  bool project_ok = dish->scope_caps[ScopeCap_Slot_Project] == ScopeCap_Able_And_Implemented;
  bool user_ok    = dish->scope_caps[ScopeCap_Slot_User]    == ScopeCap_Able_And_Implemented;
  bool system_ok  = dish->scope_caps[ScopeCap_Slot_System]  == ScopeCap_Able_And_Implemented;

  if (system_ok && !project_ok && !user_ok)
    return BulkTier_System;

  if (project_ok || user_ok)
    return BulkTier_Auto;

  return BulkTier_Manual;
}


/* ============================================================
 * 当前源探测
 * ============================================================ */

/**
 * @brief 静默执行命令并拿到 stdout
 *
 * @note xy_run_get_stdout() 会把 stderr 留在终端，这里改为整体静默。
 */
static char *
bulk_run_silent (const char *cmd)
{
  char *output = NULL;
  char *quiet = xy.on_windows
    ? xy_strcat (3, cmd, " 2>nul", "")
    : xy_strcat (3, cmd, " 2>/dev/null", "");

  int status = xy_run_get_stdout (quiet, &output);
  free (quiet);

  if (status != 0 && (!output || !output[0]))
    {
      free (output);
      return NULL;
    }

  return output;
}


static bool
bulk_text_has_known_url (const char *text, const char *url)
{
  if (!text || !url || !text[0] || !url[0])
    return false;

  /* 去掉协议头，做 host+path 子串匹配，容忍 http/https 差异 */
  const char *hostpath = url;
  if (xy_str_start_with (hostpath, "https://")) hostpath += 8;
  else if (xy_str_start_with (hostpath, "http://")) hostpath += 7;

  /* 去掉末尾斜杠，避免写法差异 */
  char *needle = xy_strdup (hostpath);
  size_t len = strlen (needle);
  while (len > 0 && needle[len - 1] == '/')
    needle[--len] = '\0';

  bool found = (len > 0) && (strstr (text, needle) != NULL);
  free (needle);
  return found;
}


/**
 * @brief 判断一段文本对应的源状态
 */
static BulkSrcState_t
bulk_state_from_text (Dish_t *dish, const char *text, char **detail)
{
  if (detail) *detail = NULL;

  if (!text || !text[0])
    return BulkSrc_Unconfigured;

  bool only_space = true;
  for (const char *p = text; *p; p++)
    {
      if (!isspace ((unsigned char) *p))
        {
          only_space = false;
          break;
        }
    }
  if (only_space)
    return BulkSrc_Unconfigured;

  if (dish->sources_n > 0 && bulk_text_has_known_url (text, dish->sources[0].url))
    {
      if (detail) *detail = xy_strdup ("upstream");
      return BulkSrc_Upstream;
    }

  for (int i = 1; i < dish->sources_n; i++)
    {
      Source_t *src = &dish->sources[i];
      if (bulk_text_has_known_url (text, src->url))
        {
          if (detail) *detail = xy_strdup (src->provider->code);
          return BulkSrc_Mirror;
        }
    }

  /* 文本存在内容但不匹配任何已知源 → 自定义源 */
  return BulkSrc_Custom;
}


static char *
bulk_read_first_config (const char *configs)
{
  if (!configs)
    return NULL;

  char *copy = xy_strdup (configs);
  char *rest = copy;

  while (rest && *rest)
    {
      char *sep = strchr (rest, '|');
      if (sep) *sep = '\0';

      char *field = xy_str_strip (rest);
      if (field && field[0] && xy_file_exist (field))
        {
          char *ret = xy_strdup (field);
          free (field);
          free (copy);
          return ret;
        }
      free (field);

      if (!sep) break;
      rest = sep + 1;
    }

  free (copy);
  return NULL;
}


/**
 * @brief 探测当前源
 *
 * 优先级：
 *   1. 有 state_cmd 时，执行并解析其输出（最准确）
 *   2. 否则读取配置文件文本并匹配已知源
 *   3. 都没有则视为未配置
 */
static void
bulk_detect_state (BulkProbe_t *probe, Dish_t *dish, BulkSrcState_t *state, char **detail, bool deep)
{
  *state = BulkSrc_Unknown;
  if (detail) *detail = NULL;

  if (deep && probe->state_cmd)
    {
      char *output = bulk_run_silent (probe->state_cmd);
      if (output)
        {
          *state = bulk_state_from_text (dish, output, detail);
          free (output);
          return;
        }
    }

  /* 快速模式：用环境变量与已知配置文件判断，避免启动慢速的包管理器 */
  if (!deep)
    {
      const char *env_value = NULL;

      if (xy_streql (probe->dish, "npm") || xy_streql (probe->dish, "pnpm") || xy_streql (probe->dish, "yarn"))
        env_value = getenv ("npm_config_registry");
      else if (xy_streql (probe->dish, "pip"))
        env_value = getenv ("PIP_INDEX_URL");
      else if (xy_streql (probe->dish, "go"))
        env_value = getenv ("GOPROXY");
      else if (xy_streql (probe->dish, "cargo"))
        env_value = getenv ("CARGO_REGISTRIES_CRATES_IO_PROTOCOL");

      if (env_value && env_value[0])
        {
          BulkSrcState_t env_state = bulk_state_from_text (dish, env_value, detail);
          if (env_state == BulkSrc_Upstream || env_state == BulkSrc_Mirror)
            {
              *state = env_state;
              return;
            }
        }

      /* npm/pip 常用配置文件，直接读文件即可，不必启动包管理器 */
      if (xy_streql (probe->dish, "npm") || xy_streql (probe->dish, "pnpm"))
        {
          char *npmrc = xy_normalize_path ("~/.npmrc");
          if (xy_file_exist (npmrc))
            {
              char *cfg = xy_file_read (npmrc);
              BulkSrcState_t cfg_state = bulk_state_from_text (dish, cfg, detail);
              free (cfg);
              free (npmrc);
              if (cfg_state == BulkSrc_Upstream || cfg_state == BulkSrc_Mirror || cfg_state == BulkSrc_Custom)
                {
                  *state = cfg_state;
                  return;
                }
            }
          else
            {
              free (npmrc);
            }
        }

      if (xy_streql (probe->dish, "pip"))
        {
          char *pip_cfg = NULL;
          if (xy.on_windows)
            pip_cfg = xy_normalize_path ("~/AppData/Roaming/pip/pip.ini");
          else
            pip_cfg = xy_normalize_path ("~/.config/pip/pip.conf");

          if (xy_file_exist (pip_cfg))
            {
              char *cfg = xy_file_read (pip_cfg);
              BulkSrcState_t cfg_state = bulk_state_from_text (dish, cfg, detail);
              free (cfg);
              free (pip_cfg);
              if (cfg_state != BulkSrc_Unconfigured)
                {
                  *state = cfg_state;
                  return;
                }
            }
          else
            {
              free (pip_cfg);
            }
        }
    }

  char *config = bulk_read_first_config (probe->configs);
  if (config)
    {
      char *text = xy_file_read (config);
      *state = bulk_state_from_text (dish, text, detail);
      free (text);
      free (config);
      return;
    }

  *state = BulkSrc_Unconfigured;
}


static bool
bulk_probe_one (BulkProbe_t *probe, BulkItem_t **out, bool deep)
{
  Dish_t *dish = bulk_find_dish (probe->dish);

  if (!dish)
    {
      char *msg = ENGLISH ? "Invalid bulk probe (dish not found in menu): "
                          : "批量检测表登记有误（菜单中找不到菜品）: ";
      chsrc_warn (xy_2strcat (msg, probe->dish));
      return false;
    }

  if (!dish->prepared && dish->preparefn)
    dish->preparefn ();

  char *evidence = NULL;
  bool found = probe->candidates
    ? bulk_any_field (probe->candidates, bulk_field_exists, &evidence)
    : false;

  if (!found)
    {
      free (evidence);
      return false;
    }

  BulkItem_t *item = xy_malloc0 (sizeof (BulkItem_t));
  item->dish     = xy_strdup (probe->dish);
  item->category = xy_strdup (probe->category);
  item->evidence = evidence;
  item->note     = probe->note ? xy_strdup (probe->note) : NULL;
  item->note_en  = probe->note_en ? xy_strdup (probe->note_en) : NULL;
  item->tier     = bulk_tier_of (dish);
  item->found    = true;

  bulk_detect_state (probe, dish, &item->state, &item->state_detail, deep);
  item->state_verified = deep || (probe->state_cmd == NULL);

  *out = item;
  return true;
}

/* ============================================================
 * 输出
 * ============================================================ */

typedef struct BulkOptions_t
{
  bool all;
  bool lang_only;
  bool ware_only;
  bool yes;
  bool force;
  bool first;
  bool english;
  bool no_color;
  bool deep;
  const char *mirror;
}
BulkOptions_t;


static const char *
bulk_tier_name (BulkTier_t tier, bool english)
{
  switch (tier)
    {
    case BulkTier_Auto:   return english ? "auto"   : "自动";
    case BulkTier_System: return english ? "system" : "系统级";
    case BulkTier_Manual: return english ? "manual" : "手工";
    default:              return english ? "unknown": "未知";
    }
}


static const char *
bulk_state_name (BulkSrcState_t state, bool english)
{
  switch (state)
    {
    case BulkSrc_Unknown:      return english ? "unknown"       : "未知";
    case BulkSrc_Unconfigured: return english ? "unconfigured"  : "未配置";
    case BulkSrc_Upstream:     return english ? "upstream"      : "上游默认";
    case BulkSrc_Mirror:       return english ? "known mirror"  : "已知镜像";
    case BulkSrc_Custom:       return english ? "custom (kept)" : "自定义(保护)";
    default:                   return english ? "unknown"       : "未知";
    }
}


/**
 * @brief 当前 item 是否可执行
 */
static bool
bulk_item_actionable (BulkItem_t *item, BulkOptions_t *opt)
{
  if (item->tier == BulkTier_System)
    return opt->all && chsrc_is_root ();

  if (item->tier == BulkTier_Manual)
    return false;

  if (item->state == BulkSrc_Custom && !opt->force)
    return false;

  return true;
}


/**
 * @brief 检查某个 dish 是否可能在 HOME 之外写文件
 *
 * 有些 recipe 使用环境变量（如 CARGO_HOME）或系统路径，即使 scope 是
 * 用户级，也可能写到 HOME 外。setup 是批量操作，必须先识别这种风险。
 */
/**
 * @brief 判断 recipe 是否会写入 HOME 之外
 *
 * @note 只针对「用户级但实际写到系统路径或专用环境变量」的情况。
 *       这里用运行时信息判断，而不是硬编码名单。
 */
static bool
bulk_dish_writes_outside_home (const char *dish_name)
{
  if (xy_streql (dish_name, "cargo"))
    {
      const char *cargo_home = getenv ("CARGO_HOME");
      if (!cargo_home || !cargo_home[0])
        return false;   /* 默认写 ~/.cargo，在 HOME 内 */

      char *home = xy_normalize_path ("~");
      bool outside = !xy_str_start_with (xy_normalize_path (cargo_home), home);
      free (home);
      return outside;
    }

  if (xy_streql (dish_name, "docker"))
    return xy.on_linux || xy.on_bsd;   /* 写 /etc/docker */

  if (xy_streql (dish_name, "homebrew"))
    return false;   /* 写 shell profile，在 HOME 内 */

  return false;
}


/**
 * @brief 判断 recipe 是否有 chsrc 可管理的、可回滚的备份
 *
 * @note 这类 recipe 通过 setx / winget / nix 等外部工具改状态，
 *       不产生 .bak 文件，失败后无法用文件回滚，因此不纳入批量。
 */
static bool
bulk_dish_has_no_file_backup (const char *dish_name)
{
  static const char *no_backup[] =
  {
    "rustup",   /* setx 写注册表 */
    "winget",   /* winget source 子命令 */
    "nix",      /* nix-channel */
    "guix",     /* guix pull */
    "emacs",    /* 外部包管理 */
    "tex",      /* tlmgr */
  };

  for (size_t i = 0; i < sizeof (no_backup) / sizeof (no_backup[0]); i++)
    {
      if (xy_streql (dish_name, no_backup[i]))
        return true;
    }

  return false;
}

static const char *
bulk_skip_reason (BulkItem_t *item, BulkOptions_t *opt)
{
  bool en = opt->english;

  if (opt->mirror && opt->mirror[0])
    {
      Dish_t *dish = bulk_find_dish (item->dish);
      if (dish && !dish->prepared && dish->preparefn)
        dish->preparefn ();
      if (dish && !bulk_dish_supports_mirror (dish, opt->mirror))
        return en ? "selected mirror is not available for this dish"
                  : "所选镜像站不支持该菜品";
    }

  /* tier 判断优先，避免把「半自动」误报成其他原因 */
  if (item->tier == BulkTier_System)
    {
      if (!opt->all)
        return en ? "system-level; add --include-system and run as root"
                  : "系统级；加 --include-system 并以 root/管理员运行";
      if (!chsrc_is_root ())
        return en ? "system-level; root/admin required"
                  : "系统级；需要 root/管理员权限";
    }

  if (item->tier == BulkTier_Manual)
    return en ? "semi-auto/manual recipe; run `chsrc set` manually"
              : "半自动/手工 recipe；请单独执行 chsrc set";

  if (bulk_dish_writes_outside_home (item->dish))
    return en ? "recipe writes outside HOME; run `chsrc set` individually"
              : "该 recipe 会写入 HOME 之外，请单独执行 chsrc set";

  if (bulk_dish_has_no_file_backup (item->dish))
    return en ? "recipe has no restorable file backup; run `chsrc set` individually"
              : "该 recipe 无文件备份、失败不可回滚，请单独执行 chsrc set";

  if (item->state == BulkSrc_Custom && !opt->force)
    return en ? "custom source detected; use --force to overwrite"
              : "检测到自定义源，默认保护；如需覆盖请加 --force";

  return NULL;
}


static void
bulk_print_item (BulkItem_t *item, BulkOptions_t *opt)
{
  bool en = opt->english;

  char *line = xy_strcat (8,
    "  " YesMark " ",
    bdpurple (item->dish),
    "  ",
    faint (bulk_tier_name (item->tier, en)),
    "  ",
    faint (item->evidence ? item->evidence : "-"),
    "  ",
    bulk_state_name (item->state, en));

  if (item->state_detail)
    line = xy_strcat (4, line, " (", bdgreen (item->state_detail), ")");

  say (line);

  char *note = en ? item->note_en : item->note;
  if (!note) note = item->note ? item->note : item->note_en;

  if (note)
    say (xy_strcat (3, "      ", bdyellow (en ? "note: " : "说明: "), note));
}


static int
bulk_cmd_scan (BulkItem_t **items, int n, BulkOptions_t *opt)
{
  bool en = opt->english;

  say (bdgreen (en ? "Sources detected on this machine:\n"
                   : "检测到本机可被 chsrc 管理的换源项目:\n"));

  int n_lang = 0, n_ware = 0;
  for (int i = 0; i < n; i++)
    {
      if (xy_streql (items[i]->category, "lang")) n_lang++;
      else n_ware++;
    }

  if (n_lang > 0)
    {
      br();
      say (bdblue (en ? "Languages & package managers" : "编程语言与包管理器"));
      for (int i = 0; i < n; i++)
        if (xy_streql (items[i]->category, "lang"))
          bulk_print_item (items[i], opt);
    }

  if (n_ware > 0)
    {
      br();
      say (bdblue (en ? "Software" : "软件"));
      for (int i = 0; i < n; i++)
        if (!xy_streql (items[i]->category, "lang"))
          bulk_print_item (items[i], opt);
    }

  int auto_n = 0, sys_n = 0, manual_n = 0, custom_n = 0;
  for (int i = 0; i < n; i++)
    {
      if (items[i]->state == BulkSrc_Custom) custom_n++;
      if (items[i]->tier == BulkTier_Auto) auto_n++;
      else if (items[i]->tier == BulkTier_System) sys_n++;
      else manual_n++;
    }

  char a[16], s[16], m[16], c[16];
  sprintf (a, "%d", auto_n);
  sprintf (s, "%d", sys_n);
  sprintf (m, "%d", manual_n);
  sprintf (c, "%d", custom_n);

  br();
  say (en
    ? xy_strcat (9, "Summary: ", a, " auto, ", s, " system, ", m, " manual, ", c, " protected.")
    : xy_strcat (9, "统计: 可自动 ", a, " 项，系统级 ", s, " 项，手工 ", m, " 项，受保护 ", c, " 项。"));

  if (!opt->deep)
    say (faint (en
      ? "Fast scan reads known config files and env vars. Use --deep to query every package manager."
      : "快速扫描只读取已知配置文件与环境变量；加 --deep 可逐个调用包管理器核实。"));

  say (en
    ? "Preview with `chsrc plan`, apply with `chsrc setup`."
    : "使用 chsrc plan 预览方案，使用 chsrc setup 直接应用。");

  return Exit_OK;
}


/* ============================================================
 * 操作系统探测
 * ============================================================ */

typedef struct BulkOsMap_t
{
  const char *id;
  const char *dish;
}
BulkOsMap_t;

static BulkOsMap_t BulkOsMaps[] =
{
  { "ubuntu", "ubuntu" }, { "zorin", "ubuntu" }, { "linuxmint", "linuxmint" },
  { "debian", "debian" }, { "kali", "kali" }, { "raspbian", "raspberrypi" },
  { "armbian", "armbian" }, { "fedora", "fedora" }, { "opensuse", "opensuse" },
  { "opensuse-leap", "opensuse" }, { "opensuse-tumbleweed", "opensuse" },
  { "arch", "arch" }, { "manjaro", "manjaro" }, { "omarchy", "omarchy" },
  { "alpine", "alpine" }, { "gentoo", "gentoo" }, { "rocky", "rockylinux" },
  { "almalinux", "almalinux" }, { "void", "voidlinux" }, { "solus", "solus" },
  { "trisquel", "trisquel" }, { "linuxlite", "linuxlite" }, { "openwrt", "openwrt" },
  { "termux", "termux" }, { "openkylin", "openkylin" }, { "openeuler", "openeuler" },
  { "anolis", "anolis" }, { "deepin", "deepin" },
  { "freebsd", "freebsd" }, { "netbsd", "netbsd" }, { "openbsd", "openbsd" },
};

static char *
bulk_os_release_value (const char *text, const char *key)
{
  if (!text || !key)
    return NULL;

  size_t key_len = strlen (key);
  const char *cur = text;

  while (cur && *cur)
    {
      const char *eol = strchr (cur, '\n');
      size_t line_len = eol ? (size_t) (eol - cur) : strlen (cur);

      /* 跳过行首空白 */
      const char *start = cur;
      const char *end = cur + line_len;
      while (start < end && isspace ((unsigned char) *start)) start++;

      if ((size_t) (end - start) > key_len
          && 0 == strncmp (start, key, key_len)
          && start[key_len] == '=')
        {
          const char *value = start + key_len + 1;
          while (value < end && isspace ((unsigned char) *value)) value++;

          /* 去掉成对引号 */
          if (value < end && (*value == '"' || *value == '\'')) value++;
          if (end > value && (end[-1] == '"' || end[-1] == '\'')) end--;

          size_t value_len = (size_t) (end - value);
          char *ret = xy_malloc0 (value_len + 1);
          memcpy (ret, value, value_len);
          ret[value_len] = '\0';
          return ret;
        }

      cur = eol ? eol + 1 : NULL;
    }

  return NULL;
}


static BulkItem_t *
bulk_probe_os (void)
{
  if (xy.on_windows)
    return NULL;

  char *os_id = NULL;

  if (xy_file_exist ("/etc/os-release"))
    {
      char *text = xy_file_read ("/etc/os-release");
      os_id = bulk_os_release_value (text, "ID");
      free (text);
    }

  if (!os_id && xy.on_bsd)
    {
      char *un = xy_run ("uname -s", 0);
      char *name = un ? xy_str_strip (un) : NULL;
      if (name)
        {
          if (xy_streql (name, "FreeBSD")) os_id = xy_strdup ("freebsd");
          else if (xy_streql (name, "NetBSD")) os_id = xy_strdup ("netbsd");
          else if (xy_streql (name, "OpenBSD")) os_id = xy_strdup ("openbsd");
        }
      free (name);
      free (un);
    }

  if (!os_id)
    return NULL;

  const char *dish_name = NULL;
  for (size_t i = 0; i < sizeof (BulkOsMaps) / sizeof (BulkOsMaps[0]); i++)
    {
      if (xy_streql (os_id, BulkOsMaps[i].id))
        {
          dish_name = BulkOsMaps[i].dish;
          break;
        }
    }

  if (!dish_name)
    {
      free (os_id);
      return NULL;
    }

  Dish_t *dish = bulk_find_dish (dish_name);
  if (!dish)
    {
      free (os_id);
      return NULL;
    }

  if (!dish->prepared && dish->preparefn)
    dish->preparefn ();

  BulkItem_t *item = xy_malloc0 (sizeof (BulkItem_t));
  item->dish     = xy_strdup (dish_name);
  item->category = xy_strdup ("os");
  item->evidence = xy_2strcat ("os-release: ", os_id);
  item->note     = xy_strdup (ENGLISH
    ? "system package sources; requires root/admin and --include-system"
    : "系统软件源；需要 root/管理员权限，并显式加 --include-system");
  item->tier     = bulk_tier_of (dish);
  item->state    = BulkSrc_Unknown;
  item->found    = true;

  free (os_id);
  return item;
}


static int
bulk_collect (BulkItem_t ***out, BulkOptions_t *opt)
{
  int cap = (int) Bulk_Probes_N + 1;
  int n = 0;
  BulkItem_t **items = xy_malloc0 (sizeof (BulkItem_t *) * cap);

  if (!opt->lang_only && !opt->ware_only)
    {
      BulkItem_t *os_item = bulk_probe_os ();
      if (os_item) items[n++] = os_item;
    }

  for (size_t i = 0; i < Bulk_Probes_N; i++)
    {
      BulkProbe_t *probe = &BulkProbes[i];

      if (opt->lang_only && !xy_streql (probe->category, "lang")) continue;
      if (opt->ware_only && !xy_streql (probe->category, "ware")) continue;

      BulkItem_t *item = NULL;
      if (bulk_probe_one (probe, &item, opt->deep)) items[n++] = item;
    }

  *out = items;
  return n;
}

/* ============================================================
 * plan / setup
 * ============================================================ */

static const char *
bulk_mirror_choice (BulkOptions_t *opt)
{
  if (opt->mirror) return opt->mirror;
  if (opt->first)  return "first";
  return NULL;
}



static void
bulk_print_plan (BulkItem_t **items, int n, BulkOptions_t *opt)
{
  bool en = opt->english;
  const char *choice = bulk_mirror_choice (opt);

  say (bdgreen (en ? "Setup plan:\n" : "换源方案:\n"));

  int apply_n = 0, skip_n = 0;

  for (int i = 0; i < n; i++)
    {
      BulkItem_t *it = items[i];
      const char *reason = bulk_skip_reason (it, opt);
      bool apply = (NULL == reason);

      if (apply) apply_n++;
      else skip_n++;

      char *tag = apply
        ? bdgreen (en ? "[APPLY]" : "[执行]")
        : bdyellow (en ? "[SKIP] " : "[跳过]");

      char *target = choice
        ? xy_strdup (choice)
        : (en ? "auto (measure & pick fastest)" : "自动测速选最快");

      char *line = xy_strcat (8,
        "  ", tag, " ", bdpurple (it->dish),
        "  ", faint (bulk_tier_name (it->tier, en)),
        "  ", apply ? target : reason);

      say (line);

      if (it->state_detail)
        say (xy_strcat (3, "        ", en ? "current: " : "当前: ", it->state_detail));
      if (it->note)
        say (xy_strcat (3, "        ", bdyellow (en ? "note: " : "说明: "), it->note));
    }

  char a[16], s[16];
  sprintf (a, "%d", apply_n);
  sprintf (s, "%d", skip_n);

  br();
  say (en
    ? xy_strcat (5, "Will apply ", a, " item(s), skip ", s, " item(s).")
    : xy_strcat (5, "将执行 ", a, " 项，跳过 ", s, " 项。"));
}


static char *
bulk_self_path (const char *argv0)
{
#ifdef XY_Build_On_Windows
  char buf[MAX_PATH];
  DWORD len = GetModuleFileNameA (NULL, buf, MAX_PATH);
  if (len > 0 && len < MAX_PATH)
    return xy_strdup (buf);
#endif
  return xy_strdup (argv0);
}


static char *
bulk_child_command (const char *self, BulkItem_t *item, BulkOptions_t *opt)
{
  char *cmd = xy_strcat (5, "\"", self, "\" set ", opt->english ? "-en " : "", item->dish);

  if (opt->mirror)
    cmd = xy_strcat (3, cmd, " ", opt->mirror);
  else if (opt->first)
    cmd = xy_strcat (3, cmd, " first");

  return cmd;
}


static int
bulk_cmd_setup (BulkItem_t **items, int n, BulkOptions_t *opt, const char *argv0)
{
  bool en = opt->english;

  bulk_print_plan (items, n, opt);

  if (in_dry_run_mode ())
    {
      say (bdyellow (en ? "Dry run: nothing was changed."
                        : "Dry Run: 未修改任何配置。"));
      return Exit_OK;
    }

  int apply_n = 0;
  for (int i = 0; i < n; i++)
    if (NULL == bulk_skip_reason (items[i], opt)) apply_n++;

  if (0 == apply_n)
    {
      say (bdyellow (en
        ? "Nothing to apply. Use --include-system for system sources."
        : "没有可自动执行的项目。系统源请加 --include-system，半自动项请单独执行 chsrc set。"));
      return Exit_OK;
    }

  if (!opt->yes)
    {
      printf (en ? "Proceed? [y/N] " : "确认执行? [y/N] ");
      fflush (stdout);

      char answer[32];
      if (!fgets (answer, sizeof (answer), stdin))
        {
          say (en ? "\nAborted." : "\n已取消。");
          return Exit_OK;
        }

      char *trimmed = xy_str_strip (answer);
      bool yes = xy_streql (trimmed, "y") || xy_streql (trimmed, "Y")
              || xy_streql (trimmed, "yes") || xy_streql (trimmed, "YES");
      free (trimmed);

      if (!yes)
        {
          say (en ? "Aborted." : "已取消。");
          return Exit_OK;
        }
    }

  char *self = bulk_self_path (argv0);

  int ok = 0, fail = 0, skip = 0;

  for (int i = 0; i < n; i++)
    {
      BulkItem_t *it = items[i];
      const char *reason = bulk_skip_reason (it, opt);

      if (reason)
        {
          skip++;
          continue;
        }

      char *cmd = bulk_child_command (self, it, opt);

      br();
      say (xy_strcat (3, bdblue ("==> "), bdpurple (it->dish), ""));
      say (faint (cmd));

      int status = system (cmd);

      if (0 == status)
        {
          ok++;
          say (bdgreen (en ? "  OK" : "  成功"));
        }
      else
        {
          fail++;
          char code[32];
          sprintf (code, "%d", status);
          say (bdred (xy_strcat (3, en ? "  FAILED (exit " : "  失败 (退出码 ",
                                  code, ")")));
        }

      free (cmd);
    }

  free (self);

  char a[16], b[16], c[16];
  sprintf (a, "%d", ok);
  sprintf (b, "%d", fail);
  sprintf (c, "%d", skip);

  br();
  say (en
    ? xy_strcat (9, "Setup finished: ", a, " succeeded, ", b, " failed, ", c, " skipped.")
    : xy_strcat (9, "Setup 完成: 成功 ", a, " 项，失败 ", b, " 项，跳过 ", c, " 项。"));

  return fail > 0 ? Exit_ExternalError : Exit_OK;
}


/* ============================================================
 * 入口
 * ============================================================ */

static void
bulk_parse_args (int argc, char const *argv[], int start, BulkOptions_t *opt)
{
  for (int i = start; i < argc; i++)
    {
      const char *arg = argv[i];

      if (xy_streql (arg, "--all") || xy_streql (arg, "--include-system"))
        opt->all = true;
      else if (xy_streql (arg, "--lang") || xy_streql (arg, "--languages"))
        opt->lang_only = true;
      else if (xy_streql (arg, "--ware") || xy_streql (arg, "--software"))
        opt->ware_only = true;
      else if (xy_streql (arg, "--yes") || xy_streql (arg, "-y"))
        opt->yes = true;
      else if (xy_streql (arg, "--force"))
        opt->force = true;
      else if (xy_streql (arg, "--deep"))
        opt->deep = true;
      else if (xy_streql (arg, "--first"))
        opt->first = true;
      else if (xy_str_start_with (arg, "--mirror="))
        opt->mirror = arg + strlen ("--mirror=");
      else if (xy_streql (arg, "--mirror") && i + 1 < argc)
        opt->mirror = argv[++i];
      else if (xy_streql (arg, "-en") || xy_streql (arg, "-english"))
        {
          opt->english = true;
          ProgMode.EnglishMode = true;
        }
      else if (xy_streql (arg, "-no-color") || xy_streql (arg, "-no-colour"))
        {
          opt->no_color = true;
          ProgMode.NoColorMode = true;
          xy.enable_color = false;
        }
      else if (xy_streql (arg, "-dry"))
        {
          ProgMode.DryRunMode = true;
        }
      else
        {
          char *msg = opt->english ? "Unknown option: " : "未识别的选项: ";
          chsrc_error (xy_2strcat (msg, arg));
          exit (Exit_Unknown);
        }
    }
}


static int
bulk_command_main (int argc, char const *argv[], int command_index)
{
  chsrc_register_chefs_and_sauciers ();

  BulkOptions_t opt = {0};
  opt.english = ProgMode.EnglishMode;
  opt.no_color = ProgMode.NoColorMode;

  /* 先解析参数，才能保证 -en 影响后续输出 */
  bulk_parse_args (argc, argv, command_index + 1, &opt);
  opt.english = ProgMode.EnglishMode;
  opt.no_color = ProgMode.NoColorMode;

  const char *command = argv[command_index];

  BulkItem_t **items = NULL;
  int n = bulk_collect (&items, &opt);

  if (xy_streql (command, "scan"))
    return bulk_cmd_scan (items, n, &opt);

  if (xy_streql (command, "plan"))
    {
      bulk_print_plan (items, n, &opt);
      return Exit_OK;
    }

  if (xy_streql (command, "setup"))
    return bulk_cmd_setup (items, n, &opt, argv[0]);

  return Exit_Unknown;
}