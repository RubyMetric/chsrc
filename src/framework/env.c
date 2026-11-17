/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : env.c
 * File Authors  : @Mikachu2333
 * Contributors  : @ccmywish
 *               |
 * Created On    : <2026-10-28>
 * Last Modified : <2026-11-08>
 *
 * 跨平台环境变量持久化管理模块
 * ------------------------------------------------------------*/

typedef enum EnvOp_t
{
  Env_OK,                /* 实际发生写入或删除 */
  Env_Unchanged,         /* 所有目标持久化值已与期望一致，未备份、未写盘 */
  Env_NoPermission,      /* SystemScope 权限不足，未做任何修改 */
  Env_TargetUnwritable,  /* 目标文件/目录/注册表键不可写或备份创建失败 */
  Env_BadName,           /* 变量名不匹配 ^[A-Za-z_][A-Za-z0-9_]*$ */
  Env_BadValue,          /* 值含目标方言危险字符（引号/$/`/换行等） */
  Env_AmbiguousTarget,   /* 未知 shell、多文件值冲突或现有行含复杂动态语法 */
  Env_ScopeUnsupported   /* ProjectScope 等不支持的作用域（防御性） */
}
EnvOp_t;

typedef struct EnvKV_t
{
  const char *name;
  const char *value;     /* NULL 表示删除该变量；"" 表示设为空串 */
}
EnvKV_t;

typedef enum PosixShell_t
{
  PosixShell_Unknown = 0,
  PosixShell_Bash    = 1,
  PosixShell_Zsh     = 2,
  PosixShell_Fish    = 3
}
PosixShell_t;

typedef enum EnvDialect_t
{
  EnvDialect_PosixShell,
  EnvDialect_Fish,
  EnvDialect_EtcEnv,
  EnvDialect_WinRegistry
}
EnvDialect_t;

typedef enum EnvLineMatch_t
{
  EnvLine_NoMatch,
  EnvLine_SafeMatch,
  EnvLine_ComplexMatch
}
EnvLineMatch_t;

/* 仅供 test/fw.c 单元测试注入使用，生产环境保持默认值 */
static int         Env_Test_Force_Shell  = -1;
static const char *Env_Test_Etc_Env_Path = NULL;


/* ============================================================
 * 动态字节缓冲与字符串切片辅助
 * ============================================================ */

static char *
env_strndup (const char *s, size_t n)
{
  char *out = xy_malloc0 (n + 1);
  if (n > 0)
    memcpy (out, s, n);
  out[n] = '\0';
  return out;
}

typedef struct EnvBuf_t
{
  char   *data;
  size_t  len;
  size_t  cap;
}
EnvBuf_t;

static void
env_buf_init (EnvBuf_t *b)
{
  b->cap  = 256;
  b->len  = 0;
  b->data = xy_malloc0 (b->cap);
}

static void
env_buf_reserve (EnvBuf_t *b, size_t extra)
{
  size_t needed = b->len + extra + 1;
  if (needed <= b->cap)
    return;

  size_t new_cap = b->cap;
  while (new_cap < needed)
    {
      new_cap *= 2;
    }

  char *new_data = realloc (b->data, new_cap);
  if (!new_data)
    {
      xy_throw ("Memory allocation failed in env_buf_reserve");
    }
  b->data = new_data;
  b->cap  = new_cap;
}

static void
env_buf_append_mem (EnvBuf_t *b, const char *s, size_t n)
{
  if (0 == n)
    return;
  env_buf_reserve (b, n);
  memcpy (b->data + b->len, s, n);
  b->len += n;
  b->data[b->len] = '\0';
}

static void
env_buf_append_str (EnvBuf_t *b, const char *s)
{
  if (!s)
    return;
  env_buf_append_mem (b, s, strlen (s));
}

static void
env_buf_append_char (EnvBuf_t *b, char c)
{
  env_buf_reserve (b, 1);
  b->data[b->len++] = c;
  b->data[b->len]   = '\0';
}

static char *
env_buf_take (EnvBuf_t *b)
{
  char *ret = b->data;
  b->data   = NULL;
  b->len    = 0;
  b->cap    = 0;
  return ret;
}

static void
env_buf_free (EnvBuf_t *b)
{
  if (b->data)
    {
      free (b->data);
      b->data = NULL;
    }
  b->len = 0;
  b->cap = 0;
}


/* ============================================================
 * 变量名与变量值合法性校验（防注入）
 * ============================================================ */

static bool
env_is_ident_start (char c)
{
  return (   ('A' <= c && c <= 'Z')
          || ('a' <= c && c <= 'z')
          || ('_' == c));
}

static bool
env_is_ident_char (char c)
{
  return (   env_is_ident_start (c)
          || ('0' <= c && c <= '9'));
}

static bool
env_validate_name (const char *name)
{
  if (!name || '\0' == name[0])
    return false;

  if (!env_is_ident_start (name[0]))
    return false;

  for (size_t i = 1; name[i] != '\0'; i++)
    {
      if (!env_is_ident_char (name[i]))
        return false;
    }
  return true;
}

static bool
env_validate_value (const char *value, EnvDialect_t dialect, char *out_bad_ch)
{
  if (NULL == value)
    return true; /* NULL 表示删除变量 */

  for (size_t i = 0; value[i] != '\0'; i++)
    {
      unsigned char uc = (unsigned char) value[i];
      char c = value[i];

      /* 所有平台均禁止换行与回车及控制字符（除制表符视方言而定） */
      if ('\r' == c || '\n' == c || (uc < 0x20 && '\t' != c) || 0x7F == uc)
        {
          if (out_bad_ch) *out_bad_ch = c;
          return false;
        }

      if (EnvDialect_WinRegistry == dialect)
        {
          continue;
        }

      /* POSIX Shell / fish / /etc/environment 共同危险字符 */
      if ('$' == c || '`' == c || '"' == c || '\\' == c)
        {
          if (out_bad_ch) *out_bad_ch = c;
          return false;
        }

      /* /etc/environment 使用无引号 NAME=value，额外禁止空白符、# 与单引号 */
      if (EnvDialect_EtcEnv == dialect)
        {
          if (' ' == c || '\t' == c || '#' == c || '\'' == c)
            {
              if (out_bad_ch) *out_bad_ch = c;
              return false;
            }
        }
    }

  return true;
}


/* ============================================================
 * POSIX 行级语法精确识别与改写引擎
 * ============================================================ */

static const char *
env_skip_ws (const char *p)
{
  while (' ' == *p || '\t' == *p)
    {
      p++;
    }
  return p;
}

/**
 * 检查值结尾后是否仅剩空白或合法行尾注释
 * @param require_ws_before_hash  POSIX shell 中引号/裸值后必须有空白才能将 # 视为注释
 */
static bool
env_valid_line_trailer (const char *p, bool require_ws_before_hash)
{
  if ('\0' == *p)
    return true;

  if (require_ws_before_hash && (' ' != *p && '\t' != *p))
    return false;

  p = env_skip_ws (p);
  return ('\0' == *p || '#' == *p);
}

static bool
env_is_unquoted_bad_shell_char (char c)
{
  return (   '$' == c || '`' == c || '"' == c || '\'' == c || '\\' == c
          || ';' == c || '&' == c || '|' == c || '<'  == c || '>'  == c
          || '(' == c || ')' == c || '*' == c || '?'  == c || '['  == c
          || ']' == c || '~' == c);
}

/**
 * 解析 NAME= 右侧字面值（供 PosixShell 与 EtcEnv 共用）
 */
static EnvLineMatch_t
env_parse_posix_rhs (const char *rhs, EnvDialect_t dialect, char **out_val)
{
  if ('\0' == *rhs)
    {
      if (out_val) *out_val = xy_strdup ("");
      return EnvLine_SafeMatch;
    }

  /* NAME= 后直接跟空白：仅当后面为空或 # 注释时才表示空串，否则如 `NAME= cmd` 属于命令前缀 */
  if (' ' == *rhs || '\t' == *rhs)
    {
      const char *after_ws = env_skip_ws (rhs);
      if ('\0' == *after_ws || '#' == *after_ws)
        {
          if (out_val) *out_val = xy_strdup ("");
          return EnvLine_SafeMatch;
        }
      return EnvLine_ComplexMatch;
    }

  if ('#' == *rhs)
    {
      /* `NAME=#foo` 在 shell 中并非注释而是字面 `#foo`，但对 /etc/environment 存在歧义 */
      if (EnvDialect_EtcEnv == dialect)
        return EnvLine_ComplexMatch;
    }

  if ('"' == *rhs)
    {
      const char *start = rhs + 1;
      const char *p     = start;
      while ('\0' != *p && '"' != *p)
        {
          if ('$' == *p || '`' == *p || '\\' == *p)
            return EnvLine_ComplexMatch;
          p++;
        }
      if ('"' != *p)
        return EnvLine_ComplexMatch; /* 未闭合双引号 */

      if (!env_valid_line_trailer (p + 1, true))
        return EnvLine_ComplexMatch;

      if (out_val) *out_val = env_strndup (start, (size_t) (p - start));
      return EnvLine_SafeMatch;
    }

  if ('\'' == *rhs)
    {
      const char *start = rhs + 1;
      const char *p     = start;
      while ('\0' != *p && '\'' != *p)
        {
          /* 若单引号内含有双引号写出时不安全的字符，拒绝自动改写以防语义变化 */
          if ('$' == *p || '`' == *p || '"' == *p || '\\' == *p)
            return EnvLine_ComplexMatch;
          p++;
        }
      if ('\'' != *p)
        return EnvLine_ComplexMatch; /* 未闭合单引号 */

      if (!env_valid_line_trailer (p + 1, true))
        return EnvLine_ComplexMatch;

      if (out_val) *out_val = env_strndup (start, (size_t) (p - start));
      return EnvLine_SafeMatch;
    }

  /* 裸值形式（如 export NVM_NODEJS_ORG_MIRROR=https://... 或 /etc/environment 的 K=V） */
  const char *start = rhs;
  const char *p     = start;
  while ('\0' != *p && ' ' != *p && '\t' != *p)
    {
      if (env_is_unquoted_bad_shell_char (*p))
        return EnvLine_ComplexMatch;
      if (EnvDialect_EtcEnv == dialect && '#' == *p)
        return EnvLine_ComplexMatch;
      p++;
    }

  if (!env_valid_line_trailer (p, true))
    return EnvLine_ComplexMatch;

  if (out_val) *out_val = env_strndup (start, (size_t) (p - start));
  return EnvLine_SafeMatch;
}

/**
 * 检查一行中在 export 之后是否以任意形式出现了目标变量名（用于捕获多变量同行定义，如 `export A=1 TARGET=2`）
 */
static bool
env_export_line_mentions_name (const char *p, const char *name, size_t name_len)
{
  bool in_squote = false;
  bool in_dquote = false;

  while ('\0' != *p)
    {
      if (!in_squote && !in_dquote && '#' == *p)
        break;

      if ('\'' == *p && !in_dquote)
        {
          in_squote = !in_squote;
          p++;
          continue;
        }
      if ('"' == *p && !in_squote)
        {
          in_dquote = !in_dquote;
          p++;
          continue;
        }
      if ('\\' == *p && !in_squote && '\0' != p[1])
        {
          p += 2;
          continue;
        }

      if (!in_squote && !in_dquote && env_is_ident_start (*p))
        {
          const char *id_start = p;
          while (env_is_ident_char (*p))
            p++;
          size_t id_len = (size_t) (p - id_start);
          if (   id_len == name_len
              && 0 == memcmp (id_start, name, name_len)
              && ('=' == *p || ' ' == *p || '\t' == *p || '\0' == *p || ';' == *p))
            {
              return true;
            }
          continue;
        }

      p++;
    }
  return false;
}

static EnvLineMatch_t
env_classify_posix_or_etc_line (const char *line, size_t line_len,
                                const char *name, EnvDialect_t dialect,
                                char **out_val)
{
  /* 行内含有嵌入 NUL 或孤立 CR 视为异常，仅在疑似提及时拒绝，否则忽略 */
  if (memchr (line, '\0', line_len) || memchr (line, '\r', line_len))
    {
      char *copy = xy_malloc0 (line_len + 1);
      for (size_t i = 0; i < line_len; i++)
        copy[i] = ('\0' == line[i]) ? ' ' : line[i];
      bool mentions = (NULL != strstr (copy, name));
      free (copy);
      return mentions ? EnvLine_ComplexMatch : EnvLine_NoMatch;
    }

  char *buf = env_strndup (line, line_len);
  const char *p = env_skip_ws (buf);
  size_t name_len = strlen (name);

  if ('\0' == *p || '#' == *p)
    {
      free (buf);
      return EnvLine_NoMatch;
    }

  bool had_export = false;
  if (0 == strncmp (p, "export", 6) && (' ' == p[6] || '\t' == p[6]))
    {
      had_export = true;
      p = env_skip_ws (p + 6);
    }

  /* 直接以 NAME= 开头 */
  if (0 == strncmp (p, name, name_len) && '=' == p[name_len])
    {
      EnvLineMatch_t res = env_parse_posix_rhs (p + name_len + 1, dialect, out_val);
      free (buf);
      return res;
    }

  /* `export NAME`（无 `=`）或 `export OTHER=1 NAME=2` 等复杂形式 */
  if (had_export && env_export_line_mentions_name (p, name, name_len))
    {
      free (buf);
      return EnvLine_ComplexMatch;
    }

  free (buf);
  return EnvLine_NoMatch;
}

static EnvLineMatch_t
env_classify_fish_line (const char *line, size_t line_len,
                        const char *name, char **out_val)
{
  if (memchr (line, '\0', line_len) || memchr (line, '\r', line_len))
    {
      char *copy = xy_malloc0 (line_len + 1);
      for (size_t i = 0; i < line_len; i++)
        copy[i] = ('\0' == line[i]) ? ' ' : line[i];
      bool mentions = (NULL != strstr (copy, name));
      free (copy);
      return mentions ? EnvLine_ComplexMatch : EnvLine_NoMatch;
    }

  char *buf = env_strndup (line, line_len);
  const char *p = env_skip_ws (buf);
  size_t name_len = strlen (name);

  if ('\0' == *p || '#' == *p)
    {
      free (buf);
      return EnvLine_NoMatch;
    }

  /* 必须是独立的 `set` 命令 */
  if (0 != strncmp (p, "set", 3) || (' ' != p[3] && '\t' != p[3]))
    {
      free (buf);
      return EnvLine_NoMatch;
    }

  p = env_skip_ws (p + 3);

  bool has_export   = false;
  bool has_bad_flag = false;

  while ('-' == *p)
    {
      if ('-' == p[1] && (' ' == p[2] || '\t' == p[2] || '\0' == p[2]))
        {
          p = env_skip_ws (p + 2);
          break;
        }
      else if ('-' == p[1])
        {
          /* 长选项 */
          const char *opt_start = p;
          while ('\0' != *p && ' ' != *p && '\t' != *p)
            p++;
          size_t opt_len = (size_t) (p - opt_start);
          if (8 == opt_len && 0 == memcmp (opt_start, "--export", 8))
            has_export = true;
          else if (8 == opt_len && 0 == memcmp (opt_start, "--global", 8))
            xy_noop ();
          else
            has_bad_flag = true;
        }
      else
        {
          /* 短选项组合，如 -x, -gx, -xg, -Ux, -e */
          p++;
          if ('\0' == *p || ' ' == *p || '\t' == *p)
            {
              has_bad_flag = true;
            }
          while ('\0' != *p && ' ' != *p && '\t' != *p)
            {
              if ('x' == *p)
                has_export = true;
              else if ('g' == *p)
                xy_noop ();
              else
                has_bad_flag = true;
              p++;
            }
        }
      p = env_skip_ws (p);
    }

  /* 读取变量名 token */
  const char *var_start = p;
  while ('\0' != *p && ' ' != *p && '\t' != *p && ';' != *p && '#' != *p)
    p++;
  size_t var_len = (size_t) (p - var_start);

  if (0 == var_len)
    {
      free (buf);
      return EnvLine_NoMatch;
    }

  bool matched_var = (var_len == name_len && 0 == memcmp (var_start, name, name_len));
  if (!matched_var)
    {
      /* 如 `set -e OTHER TARGET` 一类多变量操作 */
      if (has_bad_flag && env_export_line_mentions_name (p, name, name_len))
        {
          free (buf);
          return EnvLine_ComplexMatch;
        }
      free (buf);
      return EnvLine_NoMatch;
    }

  /* 命中了变量名，但选项不是纯导出（如 -Ux universal、-e erase、无 -x 等），拒绝自动覆写 */
  if (!has_export || has_bad_flag)
    {
      free (buf);
      return EnvLine_ComplexMatch;
    }

  if (';' == *p)
    {
      free (buf);
      return EnvLine_ComplexMatch;
    }

  p = env_skip_ws (p);
  if ('\0' == *p || '#' == *p)
    {
      /* `set -x NAME` 无参数在 fish 中为空列表而非空串，视为复杂语法保护 */
      free (buf);
      return EnvLine_ComplexMatch;
    }

  /* 解析单个值参数 */
  if ('"' == *p)
    {
      const char *vstart = p + 1;
      const char *vp     = vstart;
      while ('\0' != *vp && '"' != *vp)
        {
          if ('$' == *vp || '`' == *vp || '\\' == *vp)
            {
              free (buf);
              return EnvLine_ComplexMatch;
            }
          vp++;
        }
      if ('"' != *vp || !env_valid_line_trailer (vp + 1, true))
        {
          free (buf);
          return EnvLine_ComplexMatch;
        }
      if (out_val) *out_val = env_strndup (vstart, (size_t) (vp - vstart));
      free (buf);
      return EnvLine_SafeMatch;
    }

  if ('\'' == *p)
    {
      const char *vstart = p + 1;
      const char *vp     = vstart;
      while ('\0' != *vp && '\'' != *vp)
        {
          if ('$' == *vp || '`' == *vp || '"' == *vp || '\\' == *vp)
            {
              free (buf);
              return EnvLine_ComplexMatch;
            }
          vp++;
        }
      if ('\'' != *vp || !env_valid_line_trailer (vp + 1, true))
        {
          free (buf);
          return EnvLine_ComplexMatch;
        }
      if (out_val) *out_val = env_strndup (vstart, (size_t) (vp - vstart));
      free (buf);
      return EnvLine_SafeMatch;
    }

  const char *vstart = p;
  const char *vp     = vstart;
  while ('\0' != *vp && ' ' != *vp && '\t' != *vp)
    {
      if (env_is_unquoted_bad_shell_char (*vp) || '#' == *vp)
        {
          free (buf);
          return EnvLine_ComplexMatch;
        }
      vp++;
    }
  if (!env_valid_line_trailer (vp, true))
    {
      free (buf);
      return EnvLine_ComplexMatch;
    }

  if (out_val) *out_val = env_strndup (vstart, (size_t) (vp - vstart));
  free (buf);
  return EnvLine_SafeMatch;
}

static EnvLineMatch_t
env_classify_line (const char *line, size_t line_len,
                   const char *name, EnvDialect_t dialect,
                   char **out_val)
{
  if (out_val) *out_val = NULL;

  EnvLineMatch_t match;
  switch (dialect)
    {
    case EnvDialect_PosixShell:
    case EnvDialect_EtcEnv:
      match = env_classify_posix_or_etc_line (line, line_len, name, dialect, out_val);
      break;
    case EnvDialect_Fish:
      match = env_classify_fish_line (line, line_len, name, out_val);
      break;
    default:
      xy_unreached ();
      return EnvLine_NoMatch;
    }
  /* 未匹配的命令仍可能改动目标变量。复杂赋值、取消导出或删除不能当作
   * 无关行跳过，否则 reset 会漏删，get 也会报告错误的持久化值。 */
  if (EnvLine_NoMatch == match && EnvDialect_EtcEnv != dialect)
    {
      char *copy = env_strndup (line, line_len);
      const char *p = env_skip_ws (copy);
      const char *commands[] = { "readonly", "declare", "typeset", "local", "unset",
                                 "export", "set", "builtin", "command", "eval" };
      bool mutates = false;
      for (size_t i = 0; i < xy_c_array_len (commands); i++)
        {
          size_t n = strlen (commands[i]);
          if (0 == strncmp (p, commands[i], n) && (' ' == p[n] || '\t' == p[n]))
            { mutates = true; break; }
        }
      bool single = false, doub = false;
      size_t name_len = strlen (name);
      for (size_t i = 0; p[i]; i++)
        {
          char c = p[i];
          if (!single && '\\' == c && p[i + 1]) { i++; continue; }
          if (!single && !doub && '#' == c
              && (0 == i || strchr (" \t;|&", p[i - 1]))) break;
          if (!single && !doub && strchr (";|&", c))
            {
              const char *next = env_skip_ws (p + i + 1);
              for (size_t j = 0; j < xy_c_array_len (commands); j++)
                {
                  size_t n = strlen (commands[j]);
                  if (0 == strncmp (next, commands[j], n)
                      && (' ' == next[n] || '\t' == next[n])) mutates = true;
                }
            }
          if (!doub && '\'' == c) { single = !single; continue; }
          if (!single && '"' == c) { doub = !doub; continue; }
          if ((mutates || (!single && !doub)) && (0 == i || !env_is_ident_char (p[i - 1]))
              && 0 == strncmp (p + i, name, name_len)
              && !env_is_ident_char (p[i + name_len])
              && (mutates || '=' == p[i + name_len]))
            {
              match = EnvLine_ComplexMatch;
              break;
            }
        }
      free (copy);
    }
  return match;
}

static char *
env_format_canonical_line (const char *name, const char *value, EnvDialect_t dialect)
{
  switch (dialect)
    {
    case EnvDialect_PosixShell:
      return xy_strcat (5, "export ", name, "=\"", value, "\"");
    case EnvDialect_Fish:
      return xy_strcat (5, "set -x ", name, " \"", value, "\"");
    case EnvDialect_EtcEnv:
      return xy_strcat (3, name, "=", value);
    default:
      xy_unreached ();
      return NULL;
    }
}


/* ============================================================
 * 文件读写与内容转换
 * ============================================================ */

#include "env-shell.c"

/**
 * 在内存中对单个配置文件内容应用 kvs 修改：
 * - 第 1 处安全匹配原位替换为标准格式；
 * - 同文件后续重复安全匹配行剔除；
 * - value == NULL 删除所有安全匹配行；
 * - 未出现且 value != NULL 的变量追加在文件末尾（确保前导换行）；
 * - 遇到复杂动态行立即返回 Env_AmbiguousTarget 并携带精确文件路径与行号。
 */
static EnvOp_t
env_transform_content (const char   *old_content,
                       size_t        old_len,
                       EnvDialect_t  dialect,
                       const EnvKV_t *kvs,
                       size_t        kv_count,
                       const char   *display_path,
                       char        **out_new_content,
                       size_t       *out_new_len,
                       bool         *out_changed,
                       char        **out_err)
{
  *out_new_content = NULL;
  *out_new_len     = 0;
  *out_changed     = false;

  bool *seen = xy_malloc0 (kv_count > 0 ? kv_count * sizeof (bool) : 1);
  EnvBuf_t out;
  env_buf_init (&out);

  size_t pos    = 0;
  size_t lineno = 0;
  EnvShellContext_t context = {0};

  while (pos < old_len)
    {
      lineno++;
      const char *nl = memchr (old_content + pos, '\n', old_len - pos);
      size_t raw_len = nl ? (size_t) (nl - (old_content + pos) + 1) : (old_len - pos);
      const char *raw_line = old_content + pos;

      size_t body_len = raw_len;
      const char *eol = "\n";
      size_t eol_len  = 1;

      if (body_len > 0 && '\n' == raw_line[body_len - 1])
        {
          body_len--;
          if (body_len > 0 && '\r' == raw_line[body_len - 1])
            {
              body_len--;
              eol     = "\r\n";
              eol_len = 2;
            }
        }

      bool top_level = env_shell_scan_line (&context, raw_line, body_len, dialect);
      int matched_idx = -1;
      for (size_t k = 0; k < kv_count; k++)
        {
          EnvLineMatch_t m = env_classify_line (raw_line, body_len, kvs[k].name, dialect, NULL);
          if (!top_level && env_shell_mentions_name (raw_line, body_len, kvs[k].name))
            {
              free (seen);
              env_buf_free (&out);
              return env_shell_context_error (display_path, lineno, out_err);
            }
          if (EnvLine_ComplexMatch == m)
            {
              char *line_str = xy_int2str ((int) lineno);
              if (out_err)
                {
                  *out_err = ENGLISH
                    ? xy_strcat (7, "Complex or dynamic syntax for `", kvs[k].name,
                                 "` in ", display_path, ":", line_str,
                                 "; refusing automatic modification to protect your configuration. Please edit it manually.")
                    : xy_strcat (7, "配置文件 ", display_path, ":", line_str,
                                 " 处的变量 `", kvs[k].name,
                                 "` 含有动态展开或复杂语法，为保护您的配置已拒绝自动修改，请手工编辑");
                }
              free (line_str);
              free (seen);
              env_buf_free (&out);
              return Env_AmbiguousTarget;
            }
          if (EnvLine_SafeMatch == m && matched_idx < 0)
            {
              matched_idx = (int) k;
            }
        }

      if (matched_idx < 0)
        {
          /* 无关行（含注释、历史 # ------ chsrc BLOCK 注释等）原样保留 */
          env_buf_append_mem (&out, raw_line, raw_len);
        }
      else
        {
          size_t k = (size_t) matched_idx;
          if (NULL == kvs[k].value)
            {
              /* 删除该行 */
              xy_noop ();
            }
          else if (!seen[k])
            {
              seen[k] = true;
              char *canon = env_format_canonical_line (kvs[k].name, kvs[k].value, dialect);
              env_buf_append_str (&out, canon);
              env_buf_append_mem (&out, eol, eol_len);
              free (canon);
            }
          else
            {
              /* 同文件后续重复定义直接折叠剔除 */
              xy_noop ();
            }
        }

      pos += raw_len;
    }

  if (EnvDialect_EtcEnv != dialect && !env_shell_at_top_level (&context))
    {
      free (seen);
      env_buf_free (&out);
      return env_shell_context_error (display_path, lineno, out_err);
    }

  /* 追加文件中尚未出现且非删除的变量 */
  for (size_t k = 0; k < kv_count; k++)
    {
      if (NULL != kvs[k].value && !seen[k])
        {
          if (out.len > 0 && '\n' != out.data[out.len - 1])
            {
              env_buf_append_char (&out, '\n');
            }
          char *canon = env_format_canonical_line (kvs[k].name, kvs[k].value, dialect);
          env_buf_append_str (&out, canon);
          env_buf_append_char (&out, '\n');
          free (canon);
        }
    }

  free (seen);

  bool changed = (out.len != old_len) || (old_len > 0 && 0 != memcmp (old_content, out.data, old_len));
  *out_changed     = changed;
  *out_new_len     = out.len;
  *out_new_content = env_buf_take (&out);
  return Env_OK;
}


/* ============================================================
 * POSIX Shell 检测与配置文件集合
 * ============================================================ */

#ifndef XY_Build_On_Windows
#include <sys/types.h>

static PosixShell_t
env_shell_from_name (const char *raw_name, bool *out_known_unsupported)
{
  if (out_known_unsupported) *out_known_unsupported = false;
  if (!raw_name || '\0' == raw_name[0])
    return PosixShell_Unknown;

  /* 取路径 basename */
  const char *base = strrchr (raw_name, '/');
  base = base ? (base + 1) : raw_name;

  /* 登录 shell 可能带有前导 '-'，如 -zsh、-bash */
  while ('-' == *base)
    base++;

  if (xy_streql (base, "bash"))
    return PosixShell_Bash;
  if (xy_streql (base, "zsh"))
    return PosixShell_Zsh;
  if (xy_streql (base, "fish"))
    return PosixShell_Fish;

  if (   xy_streql (base, "csh")      || xy_streql (base, "tcsh")
      || xy_streql (base, "nu")       || xy_streql (base, "nushell")
      || xy_streql (base, "pwsh")     || xy_streql (base, "powershell")
      || xy_streql (base, "elvish")   || xy_streql (base, "xonsh")
      || xy_streql (base, "ksh")      || xy_streql (base, "mksh"))
    {
      if (out_known_unsupported) *out_known_unsupported = true;
    }

  return PosixShell_Unknown;
}

static char *
env_get_parent_comm (void)
{
  pid_t ppid = getppid ();
  if (ppid <= 1)
    return NULL;

  /* Linux 优先直读 /proc/<ppid>/comm，零子进程开销且安静 */
  char proc_path[64];
  snprintf (proc_path, sizeof (proc_path), "/proc/%d/comm", (int) ppid);
  char *comm = xy_file_read (proc_path);
  char *name = xy_str_strip (comm);
  free (comm);
  if (*name)
    {
      return name;
    }
  free (name);

  /* macOS / BSD 回退至 ps */
  char ps_cmd[96];
  snprintf (ps_cmd, sizeof (ps_cmd), "ps -p %d -o comm= 2>/dev/null", (int) ppid);
  char *out = xy_run (ps_cmd, 1);
  if (out)
    {
      char *stripped = xy_str_strip (out);
      free (out);
      if ('\0' == stripped[0])
        {
          free (stripped);
          return NULL;
        }
      return stripped;
    }
  return NULL;
}

static PosixShell_t
env_detect_posix_shell (void)
{
  if (Env_Test_Force_Shell >= 0)
    return (PosixShell_t) Env_Test_Force_Shell;

  /* 1. 优先检查直接父进程名（支持用户在 bash 登录下临时进入交互式 zsh/fish/nu） */
  char *ppid_comm = env_get_parent_comm ();
  if (ppid_comm)
    {
      bool unsupported_interactive = false;
      PosixShell_t s = env_shell_from_name (ppid_comm, &unsupported_interactive);
      free (ppid_comm);
      if (PosixShell_Unknown != s)
        return s;
      if (unsupported_interactive)
        return PosixShell_Unknown;
    }

  /* 2. 父进程为 sh/dash 包装器或脚本解释器时，回退精确匹配 $SHELL */
  const char *shell_env = getenv ("SHELL");
  if (shell_env && '\0' != shell_env[0])
    {
      return env_shell_from_name (shell_env, NULL);
    }

  return PosixShell_Unknown;
}

static bool
env_is_migrated_legacy_fish_var (const char *name)
{
  static const char *const legacy_vars[] = {
    "RUSTUP_DIST_SERVER",
    "RUSTUP_UPDATE_ROOT",
    "PUB_HOSTED_URL",
    "FLUTTER_STORAGE_BASE_URL",
    "NVM_NODEJS_ORG_MIRROR",
    "HOMEBREW_API_DOMAIN",
    "HOMEBREW_BOTTLE_DOMAIN",
    "HOMEBREW_BREW_GIT_REMOTE",
    "HOMEBREW_CORE_GIT_REMOTE"
  };

  for (size_t i = 0; i < xy_c_array_len (legacy_vars); i++)
    {
      if (xy_streql (name, legacy_vars[i]))
        return true;
    }
  return false;
}

static char *
env_build_unknown_shell_error (const EnvKV_t *kvs, size_t kv_count)
{
  const char *shell_env = getenv ("SHELL");
  const char *shown     = (shell_env && '\0' != shell_env[0]) ? shell_env : "(unset)";

  EnvBuf_t hint;
  env_buf_init (&hint);

  if (ENGLISH)
    {
      env_buf_append_str (&hint, "Unable to confirm current shell (SHELL=");
      env_buf_append_str (&hint, shown);
      env_buf_append_str (&hint, "; supported shells: bash, zsh, fish). Refusing to guess target rc file.");
      if (kvs && kv_count > 0)
        {
          env_buf_append_str (&hint, " Please configure the following variable(s) manually in your shell profile:");
          for (size_t i = 0; i < kv_count; i++)
            {
              env_buf_append_str (&hint, "\n  ");
              if (kvs[i].value)
                {
                  env_buf_append_str (&hint, kvs[i].name);
                  env_buf_append_str (&hint, "=\"");
                  env_buf_append_str (&hint, kvs[i].value);
                  env_buf_append_str (&hint, "\"");
                }
              else
                {
                  env_buf_append_str (&hint, "unset ");
                  env_buf_append_str (&hint, kvs[i].name);
                }
            }
        }
    }
  else
    {
      env_buf_append_str (&hint, "无法精确确认当前 Shell 类型 (SHELL=");
      env_buf_append_str (&hint, shown);
      env_buf_append_str (&hint, "，当前仅自动支持 bash、zsh、fish)，已拒绝猜测写入配置文件。");
      if (kvs && kv_count > 0)
        {
          env_buf_append_str (&hint, "请手工在您的 Shell 配置文件中设置以下环境变量:");
          for (size_t i = 0; i < kv_count; i++)
            {
              env_buf_append_str (&hint, "\n  ");
              if (kvs[i].value)
                {
                  env_buf_append_str (&hint, kvs[i].name);
                  env_buf_append_str (&hint, "=\"");
                  env_buf_append_str (&hint, kvs[i].value);
                  env_buf_append_str (&hint, "\"");
                }
              else
                {
                  env_buf_append_str (&hint, "unset ");
                  env_buf_append_str (&hint, kvs[i].name);
                }
            }
        }
    }

  return env_buf_take (&hint);
}

typedef struct PosixFileSpec_t
{
  const char  *display_path;
  EnvDialect_t dialect;
  bool         create_if_missing;
  bool         legacy_only; /* synchronize old definitions, exclude from effective-value queries */
}
PosixFileSpec_t;

static char *
env_fish_path (const char *name)
{
  const char *root = getenv ("XDG_CONFIG_HOME");
  if (!root || !*root) root = "~/.config";
  char *dir = xy_path_join (root, "fish");
  char *path = xy_path_join (dir, name);
  free (dir);
  return path;
}

/* Read and write share shell startup paths. The caller owns each path. */
static size_t
env_user_file_specs (PosixShell_t shell, PosixFileSpec_t *specs)
{
  size_t n = 0;
  if (PosixShell_Bash == shell)
    {
      const char *login_paths[] = { "~/.bash_profile", "~/.bash_login", "~/.profile" };
      size_t active = 2;
      for (size_t i = 0; i < 3; i++)
        {
          char *path = xy_normalize_path (login_paths[i]);
          bool readable = xy_file_exist (path) && 0 == access (path, R_OK);
          free (path);
          if (readable) { active = i; break; }
        }
      specs[n++] = (PosixFileSpec_t) { xy_strdup ("~/.bashrc"), EnvDialect_PosixShell, true };
      specs[n++] = (PosixFileSpec_t) { xy_strdup (login_paths[active]), EnvDialect_PosixShell, true };
      for (size_t i = 0; i < 3; i++)
        if (i != active)
          specs[n++] = (PosixFileSpec_t) { xy_strdup (login_paths[i]), EnvDialect_PosixShell, false, true };
    }
  else if (PosixShell_Zsh == shell)
    {
      const char *root = getenv ("ZDOTDIR");
      if (!root) root = "~";
      if (!*root) root = ".";
      specs[n++] = (PosixFileSpec_t) { xy_path_join (root, ".zshrc"), EnvDialect_PosixShell, true };
      specs[n++] = (PosixFileSpec_t) { xy_path_join (root, ".zprofile"), EnvDialect_PosixShell, true };
    }
  else if (PosixShell_Fish == shell)
    {
      specs[n++] = (PosixFileSpec_t) { env_fish_path ("conf.d/chsrc.fish"), EnvDialect_Fish, true };
    }
  return n;
}

static void
env_free_file_specs (PosixFileSpec_t *specs, size_t count)
{
  for (size_t i = 0; i < count; i++) free ((void *) specs[i].display_path);
}

/* 文件更新层不依赖 EnvOp_t；这里保留环境变量 API 的返回值约定。 */
static EnvOp_t
env_commit_file_updates (const FileUpdate_t *ops, size_t count, char **out_err)
{
  switch (chsrc_update_files (ops, count, out_err))
    {
    case FileUpdate_OK:        return Env_OK;
    case FileUpdate_Unchanged: return Env_Unchanged;
    case FileUpdate_Failed:    return Env_TargetUnwritable;
    default:                  xy_unreached (); return Env_TargetUnwritable;
    }
}

static void
env_free_prepared_ops (FileUpdate_t *ops, size_t count)
{
  if (!ops)
    return;
  for (size_t i = 0; i < count; i++)
    {
      if (ops[i].norm_path)   free (ops[i].norm_path);
      if (ops[i].display_path) free ((void *) ops[i].display_path);
      if (ops[i].old_content) free (ops[i].old_content);
      if (ops[i].new_content) free (ops[i].new_content);
    }
  free (ops);
}

/* 可选配置只允许“普通文件”或“确实不存在”；查询失败和目录不能当作空配置。 */
static EnvOp_t
env_read_optional_file (const char *path, bool *existed, char **content, size_t *len,
                        char **out_err)
{
  *content = NULL;
  *len = 0;
  enum XyStatError error;
  *existed = xy_stat_is_file (path, &error);
  if (*existed && xy_file_read_bytes (path, content, len)) return Env_OK;
  if (!*existed && XY_Stat_Not_Found == error)
    {
      *content = xy_strdup ("");
      return Env_OK;
    }
  if (out_err)
    *out_err = xy_2strcat (ENGLISH ? "Unable to read configuration file: " : "无法读取配置文件: ", path);
  return Env_TargetUnwritable;
}

static bool
env_file_mentions_any_kv (const char *display_path, EnvDialect_t dialect,
                          const EnvKV_t *kvs, size_t kv_count)
{
  char *norm = xy_normalize_path (display_path);
  char  *content = NULL;
  size_t len     = 0;
  bool existed = false;
  if (Env_OK != env_read_optional_file (norm, &existed, &content, &len, NULL))
    {
      free (norm);
      return false;
    }
  free (norm);

  bool found = false;
  size_t pos = 0;
  while (pos < len && !found)
    {
      const char *nl = memchr (content + pos, '\n', len - pos);
      size_t raw_len = nl ? (size_t) (nl - (content + pos) + 1) : (len - pos);
      size_t body_len = raw_len;
      if (body_len > 0 && '\n' == content[pos + body_len - 1])
        {
          body_len--;
          if (body_len > 0 && '\r' == content[pos + body_len - 1])
            body_len--;
        }

      for (size_t k = 0; k < kv_count; k++)
        {
          if (EnvLine_NoMatch != env_classify_line (content + pos, body_len, kvs[k].name, dialect, NULL))
            {
              found = true;
              break;
            }
        }
      pos += raw_len;
    }

  free (content);
  return found;
}

static EnvOp_t
env_apply_posix (const EnvKV_t *kvs, size_t kv_count, PosixShell_t *out_shell, char **out_err)
{
  if (out_shell) *out_shell = PosixShell_Unknown;

  /* SystemScope -> /etc/environment */
  if (chsrc_in_system_scope_mode ())
    {
      const char *etc_path = Env_Test_Etc_Env_Path ? Env_Test_Etc_Env_Path : "/etc/environment";

      if (!Env_Test_Etc_Env_Path && !chsrc_is_root () && !in_dry_run_mode ())
        {
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strdup ("Use sudo before the command or switch to root to ensure the necessary permissions")
                : xy_strdup ("请在命令前使用 sudo 或切换为root用户来保证必要的权限");
            }
          return Env_NoPermission;
        }

      for (size_t k = 0; k < kv_count; k++)
        {
          if (!env_validate_value (kvs[k].value, EnvDialect_EtcEnv, NULL))
            {
              if (out_err)
                {
                  *out_err = ENGLISH
                    ? xy_strcat (3, "Value for `", kvs[k].name,
                                 "` contains unsafe characters (whitespace, quotes, `#`, `$`, `` ` ``, `\\`, or newline) for /etc/environment")
                    : xy_strcat (3, "变量 `", kvs[k].name,
                                 "` 的值含有 /etc/environment 不安全字符（空白符、引号、`#`、`$`、反引号、反斜杠或换行）");
                }
              return Env_BadValue;
            }
        }

      char  *norm_path   = xy_normalize_path (etc_path);
      bool existed = false;
      char *old_content = NULL;
      size_t old_len = 0;
      EnvOp_t read_result = env_read_optional_file (norm_path, &existed, &old_content, &old_len, out_err);
      if (Env_OK != read_result)
        {
          free (norm_path);
          return read_result;
        }

      char  *new_content = NULL;
      size_t new_len     = 0;
      bool   changed     = false;

      EnvOp_t tr = env_transform_content (old_content, old_len, EnvDialect_EtcEnv,
                                          kvs, kv_count, etc_path,
                                          &new_content, &new_len, &changed, out_err);
      if (Env_OK != tr)
        {
          free (old_content);
          free (norm_path);
          return tr;
        }

      if (!changed || (!existed && 0 == new_len))
        {
          free (old_content);
          free (new_content);
          free (norm_path);
          return Env_Unchanged;
        }

      FileUpdate_t op = {
        .display_path = etc_path, .norm_path = norm_path,
        .existed = existed,
        .old_content = old_content, .old_len = old_len,
        .new_content = new_content, .new_len = new_len
      };
      EnvOp_t result = env_commit_file_updates (&op, 1, out_err);
      free (old_content);
      free (new_content);
      free (norm_path);
      return result;
    }

  /* UserScope */
  PosixShell_t shell = env_detect_posix_shell ();
  if (PosixShell_Unknown == shell)
    {
      if (out_err)
        *out_err = env_build_unknown_shell_error (kvs, kv_count);
      return Env_AmbiguousTarget;
    }
  if (out_shell) *out_shell = shell;

  EnvDialect_t user_dialect = (PosixShell_Fish == shell) ? EnvDialect_Fish : EnvDialect_PosixShell;
  for (size_t k = 0; k < kv_count; k++)
    {
      if (!env_validate_value (kvs[k].value, user_dialect, NULL))
        {
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strcat (3, "Value for `", kvs[k].name,
                             "` contains unsafe shell characters (`\"`, `\\`, `$`, `` ` ``, or newline)")
                : xy_strcat (3, "变量 `", kvs[k].name,
                             "` 的值含有不安全字符（`\"`、`\\`、`$`、反引号或换行）");
            }
          return Env_BadValue;
        }
    }

  PosixFileSpec_t write_specs[4];
  size_t          write_count = env_user_file_specs (shell, write_specs);
  PosixFileSpec_t check_specs[5];
  size_t          check_count = 0;

  if (PosixShell_Bash == shell)
    {
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.zshrc",                    EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.zprofile",                 EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.config/fish/conf.d/chsrc.fish", EnvDialect_Fish,  false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.config/fish/config.fish",  EnvDialect_Fish,       false };
    }
  else if (PosixShell_Zsh == shell)
    {
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.bashrc",                   EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.bash_profile",             EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.profile",                  EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.config/fish/conf.d/chsrc.fish", EnvDialect_Fish,  false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.config/fish/config.fish",  EnvDialect_Fish,       false };
    }
  else if (PosixShell_Fish == shell)
    {
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.bashrc",   EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.zshrc",    EnvDialect_PosixShell, false };
      check_specs[check_count++] = (PosixFileSpec_t) { "~/.profile",  EnvDialect_PosixShell, false };
    }

  /* 阶段 1：全部读取并在内存完成转换校验，任一文件遇复杂语法则零落盘中止 */
  FileUpdate_t *ops = xy_malloc0 ((write_count + 1) * sizeof (FileUpdate_t));
  size_t op_count = 0;

  for (size_t i = 0; i < write_count; i++)
    {
      char *norm = xy_normalize_path (write_specs[i].display_path);
      bool existed = false;
      char *old_content = NULL;
      size_t old_len = 0;
      EnvOp_t read_result = env_read_optional_file (norm, &existed, &old_content, &old_len, out_err);
      if (Env_OK != read_result)
        {
          free (norm);
          env_free_prepared_ops (ops, op_count);
          env_free_file_specs (write_specs, write_count);
          return read_result;
        }
      if (!existed && !write_specs[i].create_if_missing)
        {
          free (old_content);
          free (norm);
          continue;
        }

      char  *new_content = NULL;
      size_t new_len     = 0;
      bool   changed     = false;

      EnvOp_t tr = env_transform_content (old_content, old_len, write_specs[i].dialect,
                                          kvs, kv_count, write_specs[i].display_path,
                                          &new_content, &new_len, &changed, out_err);
      if (Env_OK != tr)
        {
          free (old_content);
          free (norm);
          env_free_prepared_ops (ops, op_count);
          env_free_file_specs (write_specs, write_count);
          return tr;
        }

      ops[op_count].display_path = xy_strdup (write_specs[i].display_path);
      ops[op_count].norm_path    = norm;
      ops[op_count].existed      = existed;
      ops[op_count].old_content  = old_content;
      ops[op_count].old_len      = old_len;
      ops[op_count].new_content  = new_content;
      ops[op_count].new_len      = new_len;
      op_count++;
    }

  env_free_file_specs (write_specs, write_count);

  /* Fish 一次性迁移清理：从 ~/.config/fish/config.fish 精确移除 5 个迁移 dish 的历史同名行 */
  EnvKV_t *non_legacy_fish_kvs = NULL;
  size_t   non_legacy_count    = 0;

  if (PosixShell_Fish == shell)
    {
      EnvKV_t *cleanup_kvs   = xy_malloc0 ((kv_count > 0 ? kv_count : 1) * sizeof (EnvKV_t));
      size_t   cleanup_count = 0;
      non_legacy_fish_kvs    = xy_malloc0 ((kv_count > 0 ? kv_count : 1) * sizeof (EnvKV_t));

      for (size_t k = 0; k < kv_count; k++)
        {
          if (env_is_migrated_legacy_fish_var (kvs[k].name))
            {
              cleanup_kvs[cleanup_count++] = (EnvKV_t) { kvs[k].name, NULL };
            }
          else
            {
              non_legacy_fish_kvs[non_legacy_count++] = kvs[k];
            }
        }

      if (cleanup_count > 0)
        {
          char       *fishrc_disp = env_fish_path ("config.fish");
          char       *fishrc_norm = xy_normalize_path (fishrc_disp);
          bool existed = false;
          char *old_content = NULL;
          size_t old_len = 0;
          EnvOp_t read_result = env_read_optional_file (fishrc_norm, &existed, &old_content, &old_len, out_err);
          if (Env_OK != read_result)
            {
              free (fishrc_norm);
              free (fishrc_disp);
              free (cleanup_kvs);
              free (non_legacy_fish_kvs);
              env_free_prepared_ops (ops, op_count);
              return read_result;
            }
          if (existed)
            {
              char  *new_content = NULL;
              size_t new_len     = 0;
              bool   changed     = false;

              EnvOp_t tr = env_transform_content (old_content, old_len, EnvDialect_Fish,
                                                  cleanup_kvs, cleanup_count, fishrc_disp,
                                                  &new_content, &new_len, &changed, out_err);
              if (Env_OK != tr)
                {
                  free (old_content);
                  free (fishrc_norm);
                  free (fishrc_disp);
                  free (cleanup_kvs);
                  free (non_legacy_fish_kvs);
                  env_free_prepared_ops (ops, op_count);
                  return tr;
                }

              ops[op_count].display_path = xy_strdup (fishrc_disp);
              ops[op_count].norm_path    = fishrc_norm;
              ops[op_count].existed      = true;
              ops[op_count].old_content  = old_content;
              ops[op_count].old_len      = old_len;
              ops[op_count].new_content  = new_content;
              ops[op_count].new_len      = new_len;
              op_count++;
            }
          else
            {
              free (old_content);
              free (fishrc_norm);
            }
          free (fishrc_disp);
        }
      free (cleanup_kvs);
    }

  /* 阶段 2：全部暂存成功后逐文件替换，后续失败则回滚已提交目标。 */
  EnvOp_t commit_result = env_commit_file_updates (ops, op_count, out_err);
  if (Env_OK != commit_result)
    {
      free (non_legacy_fish_kvs);
      env_free_prepared_ops (ops, op_count);
      return commit_result;
    }

  /* 检查集提醒：若其他 Shell 配置文件中也定义了相同变量，仅提示不越界修改 */
  for (size_t i = 0; i < check_count; i++)
    {
      char *fish_path = EnvDialect_Fish == check_specs[i].dialect
        ? env_fish_path (strstr (check_specs[i].display_path, "conf.d/") ? "conf.d/chsrc.fish" : "config.fish")
        : NULL;
      const char *check_path = fish_path ? fish_path : check_specs[i].display_path;
      if (env_file_mentions_any_kv (check_path, check_specs[i].dialect, kvs, kv_count))
        {
          char *msg = ENGLISH
            ? xy_strcat (3, "Detected matching variable definition in ", check_path,
                         "; switch to that shell if you also want chsrc to update it")
            : xy_strcat (3, "检测到 ", check_path,
                         " 中也存在同名环境变量定义，若您同时使用该 Shell 可切换后同步更新");
          chsrc_note2 (msg);
          free (msg);
        }
      free (fish_path);
    }

  if (PosixShell_Fish == shell && non_legacy_count > 0)
    {
      char *fish_path = env_fish_path ("config.fish");
      if (env_file_mentions_any_kv (fish_path, EnvDialect_Fish,
                                    non_legacy_fish_kvs, non_legacy_count))
        {
          char *msg = ENGLISH
            ? xy_strcat (3, "Detected matching variable definition in ", fish_path, "; verify whether it overrides conf.d/chsrc.fish")
            : xy_strcat (3, "检测到 ", fish_path, " 中也存在同名环境变量定义，请留意其加载顺序是否覆盖 conf.d/chsrc.fish");
          chsrc_note2 (msg);
          free (msg);
        }
      free (fish_path);
    }

  if (non_legacy_fish_kvs) free (non_legacy_fish_kvs);
  env_free_prepared_ops (ops, op_count);
  return Env_OK;
}

/**
 * 在单个文件中查询持久化变量值（文件内多次安全赋值以最后一次为准，与 Shell 顺序执行一致）
 */
static EnvOp_t
env_query_single_posix_file (const char *display_path, EnvDialect_t dialect,
                             const char *name, bool *out_found, char **out_val,
                             char **out_err)
{
  *out_found = false;
  *out_val   = NULL;

  char *norm = xy_normalize_path (display_path);
  char *content = NULL;
  size_t len = 0;
  bool existed = false;
  EnvOp_t read_result = env_read_optional_file (norm, &existed, &content, &len, out_err);
  free (norm);
  if (Env_OK != read_result) return read_result;

  size_t pos    = 0;
  size_t lineno = 0;
  EnvShellContext_t context = {0};

  while (pos < len)
    {
      lineno++;
      const char *nl = memchr (content + pos, '\n', len - pos);
      size_t raw_len = nl ? (size_t) (nl - (content + pos) + 1) : (len - pos);
      size_t body_len = raw_len;
      if (body_len > 0 && '\n' == content[pos + body_len - 1])
        {
          body_len--;
          if (body_len > 0 && '\r' == content[pos + body_len - 1])
            body_len--;
        }

      char *parsed = NULL;
      bool top_level = env_shell_scan_line (&context, content + pos, body_len, dialect);
      if (!top_level && env_shell_mentions_name (content + pos, body_len, name))
        {
          free (content);
          free (*out_val);
          *out_val = NULL;
          *out_found = false;
          return env_shell_context_error (display_path, lineno, out_err);
        }
      EnvLineMatch_t m = env_classify_line (content + pos, body_len, name, dialect, &parsed);
      if (EnvLine_ComplexMatch == m)
        {
          char *line_str = xy_int2str ((int) lineno);
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strcat (6, "Complex or dynamic syntax for `", name,
                             "` in ", display_path, ":", line_str)
                    : xy_strcat (7, "配置文件 ", display_path, ":", line_str,
                             " 处的变量 `", name, "` 含有动态展开或复杂语法，无法精确确认持久化值");
            }
          free (line_str);
          free (content);
          if (*out_val)
            {
              free (*out_val);
              *out_val   = NULL;
              *out_found = false;
            }
          return Env_AmbiguousTarget;
        }
      else if (EnvLine_SafeMatch == m)
        {
          if (*out_val) free (*out_val);
          *out_val   = parsed;
          *out_found = true;
        }

      pos += raw_len;
    }

  if (EnvDialect_EtcEnv != dialect && !env_shell_at_top_level (&context))
    {
      free (content);
      free (*out_val);
      *out_val = NULL;
      *out_found = false;
      return env_shell_context_error (display_path, lineno, out_err);
    }
  free (content);
  return Env_OK;
}

static EnvOp_t
env_query_persisted_posix (const char *name, bool *out_found, char **out_val, char **out_err)
{
  *out_found = false;
  *out_val   = NULL;

  if (chsrc_in_system_scope_mode ())
    {
      const char *etc_path = Env_Test_Etc_Env_Path ? Env_Test_Etc_Env_Path : "/etc/environment";
      return env_query_single_posix_file (etc_path, EnvDialect_EtcEnv, name, out_found, out_val, out_err);
    }

  PosixShell_t shell = env_detect_posix_shell ();
  if (PosixShell_Unknown == shell)
    {
      if (out_err)
        *out_err = env_build_unknown_shell_error (NULL, 0);
      return Env_AmbiguousTarget;
    }

  PosixFileSpec_t specs[4];
  size_t          spec_count = env_user_file_specs (shell, specs);
  if (PosixShell_Fish == shell)
    {
      specs[spec_count++] = (PosixFileSpec_t) { env_fish_path ("config.fish"), EnvDialect_Fish, false };
    }

  const char *first_file = NULL;
  char       *agreed_val = NULL;
  bool        any_found  = false;

  for (size_t i = 0; i < spec_count; i++)
    {
      if (specs[i].legacy_only) continue;
      bool  file_found = false;
      char *file_val   = NULL;

      EnvOp_t rc = env_query_single_posix_file (specs[i].display_path, specs[i].dialect,
                                                name, &file_found, &file_val, out_err);
      if (Env_OK != rc)
        {
          if (agreed_val) free (agreed_val);
          env_free_file_specs (specs, spec_count);
          return rc;
        }

      if (!file_found)
        continue;

      if (!any_found)
        {
          any_found  = true;
          agreed_val = file_val;
          first_file = specs[i].display_path;
        }
      else
        {
          if (!xy_streql (agreed_val, file_val))
            {
              if (out_err)
                {
                  *out_err = ENGLISH
                    ? xy_strcat (9, "Conflicting values for `", name, "` between ",
                                 first_file, " (`", agreed_val, "`) and ",
                                 specs[i].display_path, " — please resolve the conflict manually")
                    : xy_strcat (9, "变量 `", name, "` 在 ",
                                 first_file, " (`", agreed_val, "`) 与 ",
                                 specs[i].display_path, " 中存在不同值冲突，请先手工统一后再查看");
                }
              free (agreed_val);
              free (file_val);
              env_free_file_specs (specs, spec_count);
              return Env_AmbiguousTarget;
            }
          free (file_val);
        }
    }

  *out_found = any_found;
  *out_val   = agreed_val;
  env_free_file_specs (specs, spec_count);
  return Env_OK;
}

#endif /* !XY_Build_On_Windows */


/* Windows 字符串转换统一复用 xy.h；这里只处理 .reg 格式与注册表语义。 */
#ifdef XY_Build_On_Windows

typedef struct EnvRegSnapshot_t
{
  const char    *name;
  bool           existed;
  uint32_t       reg_type;     /* 1 = REG_SZ, 2 = REG_EXPAND_SZ 等 */
  char          *utf8_sz_val;  /* REG_SZ / REG_EXPAND_SZ 解码文本 */
  unsigned char *raw_bytes;    /* 原始字节快照 */
  size_t         raw_len;
}
EnvRegSnapshot_t;

static void
env_append_reg_escaped_str (EnvBuf_t *b, const char *s)
{
  if (!s)
    return;
  for (size_t i = 0; '\0' != s[i]; i++)
    {
      if ('\\' == s[i] || '"' == s[i])
        env_buf_append_char (b, '\\');
      env_buf_append_char (b, s[i]);
    }
}

/**
 * 生成带 UTF-16LE BOM (0xFF 0xFE) 的标准 Windows Registry Editor Version 5.00 备份字节流
 */
static unsigned char *
env_build_reg_backup_bytes (const char             *key_path,
                            const EnvRegSnapshot_t *snaps,
                            size_t                  snap_count,
                            size_t                 *out_byte_len)
{
  EnvBuf_t txt;
  env_buf_init (&txt);

  env_buf_append_str (&txt, "Windows Registry Editor Version 5.00\r\n\r\n[");
  env_buf_append_str (&txt, key_path);
  env_buf_append_str (&txt, "]\r\n");

  for (size_t i = 0; i < snap_count; i++)
    {
      env_buf_append_char (&txt, '"');
      env_append_reg_escaped_str (&txt, snaps[i].name);
      env_buf_append_str (&txt, "\"=");

      if (!snaps[i].existed)
        {
          env_buf_append_str (&txt, "-\r\n");
        }
      /* 实际快照优先按原始字节导出，保留非法编码、内嵌 NUL 和换行；
       * 只有没有字节快照的调用方才使用 .reg 字符串字面量。 */
      else if (1 == snaps[i].reg_type && NULL != snaps[i].utf8_sz_val && !snaps[i].raw_bytes)
        {
          env_buf_append_char (&txt, '"');
          env_append_reg_escaped_str (&txt, snaps[i].utf8_sz_val);
          env_buf_append_str (&txt, "\"\r\n");
        }
      else
        {
          char hdr[32];
          snprintf (hdr, sizeof (hdr), "hex(%x):", (unsigned int) snaps[i].reg_type);
          env_buf_append_str (&txt, hdr);
          for (size_t b = 0; b < snaps[i].raw_len; b++)
            {
              char hx[8];
              snprintf (hx, sizeof (hx), "%s%02x", (b > 0 ? "," : ""), snaps[i].raw_bytes[b]);
              env_buf_append_str (&txt, hx);
            }
          env_buf_append_str (&txt, "\r\n");
        }
    }

  env_buf_append_str (&txt, "\r\n");

  wchar_t *u16 = xy_utf8_to_wstr (txt.data);
  env_buf_free (&txt);
  if (!u16) { *out_byte_len = 0; return NULL; }
  size_t u16_len = wcslen (u16);

  size_t         total_bytes = 2 + u16_len * 2;
  unsigned char *bytes       = xy_malloc0 (total_bytes + 2);

  /* UTF-16LE BOM */
  bytes[0] = 0xFF;
  bytes[1] = 0xFE;

  for (size_t i = 0; i < u16_len; i++)
    {
      bytes[2 + i * 2]     = (unsigned char) (u16[i] & 0xFF);
      bytes[2 + i * 2 + 1] = (unsigned char) ((u16[i] >> 8) & 0xFF);
    }

  free (u16);
  *out_byte_len = total_bytes;
  return bytes;
}


/* ============================================================
 * Windows 注册表后端（Reg*W + .reg 备份与失败自动回滚 + WM_SETTINGCHANGE）
 * ============================================================ */

#define ENV_WIN_HKCU_SUBKEY   L"Environment"
#define ENV_WIN_HKLM_SUBKEY   L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
#define ENV_WIN_HKCU_REGPATH  "HKEY_CURRENT_USER\\Environment"
#define ENV_WIN_HKLM_REGPATH  "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"

static unsigned int Env_Win_Backup_Seq       = 0;
static char        *Env_Win_Backup_Dir       = NULL;  /* 保留备份供用户恢复 */

static void
env_free_reg_snapshots (EnvRegSnapshot_t *snaps, size_t count)
{
  if (!snaps)
    return;
  for (size_t i = 0; i < count; i++)
    {
      if (snaps[i].utf8_sz_val) free (snaps[i].utf8_sz_val);
      if (snaps[i].raw_bytes)   free (snaps[i].raw_bytes);
    }
  free (snaps);
}

static EnvOp_t
env_read_win_snapshot_one (HKEY hkey, const char *name, EnvRegSnapshot_t *snap, char **out_err)
{
  snap->name        = name;
  snap->existed     = false;
  snap->reg_type    = 0;
  snap->utf8_sz_val = NULL;
  snap->raw_bytes   = NULL;
  snap->raw_len     = 0;

  if (!hkey)
    return Env_OK;

  wchar_t *wname = xy_utf8_to_wstr (name);
  if (!wname)
    {
      if (out_err) *out_err = xy_strdup (ENGLISH ? "Unable to encode Registry value name"
                                               : "无法转换注册表变量名");
      return Env_TargetUnwritable;
    }

  DWORD reg_type = 0;
  DWORD cb_data  = 0;
  LONG  st       = RegQueryValueExW (hkey, (LPCWSTR) wname, NULL, &reg_type, NULL, &cb_data);

  if (ERROR_FILE_NOT_FOUND == st)
    {
      free (wname);
      return Env_OK;
    }
  if (ERROR_SUCCESS != st)
    {
      free (wname);
      char *code_str = xy_int2str ((int) st);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strcat (4, "Failed to query Windows Registry value `", name, "` (error ", code_str)
            : xy_strcat (4, "读取 Windows 注册表变量 `", name, "` 失败，错误码: ", code_str);
        }
      free (code_str);
      return Env_TargetUnwritable;
    }

  unsigned char *raw = xy_malloc0 ((size_t) cb_data + sizeof (WCHAR));
  DWORD cb_read = cb_data;
  st = RegQueryValueExW (hkey, (LPCWSTR) wname, NULL, &reg_type, (LPBYTE) raw, &cb_read);
  free (wname);

  if (ERROR_SUCCESS != st)
    {
      free (raw);
      char *code_str = xy_int2str ((int) st);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strcat (4, "Failed to read Windows Registry value `", name, "` (error ", code_str)
            : xy_strcat (4, "读取 Windows 注册表变量 `", name, "` 内容失败，错误码: ", code_str);
        }
      free (code_str);
      return Env_TargetUnwritable;
    }

  snap->existed   = true;
  snap->reg_type  = (uint32_t) reg_type;
  snap->raw_bytes = raw;
  snap->raw_len   = (size_t) cb_read;

  if ((REG_SZ == reg_type || REG_EXPAND_SZ == reg_type) && 0 == cb_read % sizeof (WCHAR))
    {
      /* RegQueryValueExW 的字符串可能未以 NUL 结尾；raw 已额外分配一个
       * WCHAR。只接受单个字符串，异常内容保留原始字节供备份恢复。 */
      const wchar_t *wide = (const wchar_t *) raw;
      size_t units = cb_read / sizeof (WCHAR);
      size_t len = wcslen (wide);
      while (len < units && 0 == wide[len]) len++;
      if (len == units) snap->utf8_sz_val = xy_wstr_to_utf8 (wide);
    }

  return Env_OK;
}

/**
 * 将本批变量的原有值连同本次新增变量的删除标记 ("NAME"=-) 导出到 %TEMP%\chsrc-<随机名>\env-hkcu|hklm-<seq>.reg
 */
static bool
env_write_win_reg_backup (bool                    is_system,
                          const EnvRegSnapshot_t *snaps,
                          size_t                  snap_count,
                          char                  **out_reg_utf8_path,
                          char                  **out_err)
{
  *out_reg_utf8_path = NULL;

  /* 每批保存本批修改前的值；组合操作按序号倒序恢复，后续新增变量也有删除标记。 */
  if (NULL == Env_Win_Backup_Dir)
    {
      Env_Win_Backup_Dir = chsrc_make_tmpdir ("chsrc", out_err);
      if (NULL == Env_Win_Backup_Dir)
        return false;
    }

  Env_Win_Backup_Seq++;
  char file_name[64];
  snprintf (file_name, sizeof (file_name), "env-%s-%u.reg",
            is_system ? "hklm" : "hkcu", Env_Win_Backup_Seq);

  char *reg_utf8 = xy_path_join (Env_Win_Backup_Dir, file_name);
  size_t reg_byte_len = 0;
  unsigned char *reg_bytes = env_build_reg_backup_bytes (
    is_system ? ENV_WIN_HKLM_REGPATH : ENV_WIN_HKCU_REGPATH,
    snaps, snap_count, &reg_byte_len);
  if (!reg_bytes)
    {
      if (out_err) *out_err = xy_strdup (ENGLISH ? "Unable to encode Registry backup"
                                               : "无法编码注册表备份");
      free (reg_utf8);
      return false;
    }

  /* 二进制备份也使用统一暂存、刷盘和提交逻辑。此目录由本进程独占创建，
   * 目标必须是新文件；写入失败不开始修改注册表。 */
  FileUpdate_t op = {
    .display_path = reg_utf8, .norm_path = reg_utf8,
    .new_content = (char *) reg_bytes, .new_len = reg_byte_len
  };
  FileUpdateResult_t result = chsrc_update_files (&op, 1, out_err);
  free (reg_bytes);
  if (FileUpdate_OK != result)
    {
      free (reg_utf8);
      return false;
    }

  char *bk_msg = ENGLISH
    ? xy_2strcat ("Backed up Windows Registry environment to: ", reg_utf8)
    : xy_2strcat ("已备份 Windows 注册表环境变量至: ", reg_utf8);
  chsrc_log2 (bk_msg);
  free (bk_msg);

  *out_reg_utf8_path = reg_utf8;
  return true;
}

static bool
env_rollback_win_registry (HKEY hkey_write, const EnvRegSnapshot_t *snaps, size_t count)
{
  bool ok = true;
  for (size_t i = 0; i < count; i++)
    {
      wchar_t *wname = xy_utf8_to_wstr (snaps[i].name);
      if (!wname) { ok = false; continue; }

      LONG st;
      if (!snaps[i].existed)
        {
          st = RegDeleteValueW (hkey_write, (LPCWSTR) wname);
          if (ERROR_FILE_NOT_FOUND == st) st = ERROR_SUCCESS;
        }
      else
        {
          st = RegSetValueExW (hkey_write, (LPCWSTR) wname, 0,
                          (DWORD) snaps[i].reg_type,
                          (const BYTE *) snaps[i].raw_bytes,
                          (DWORD) snaps[i].raw_len);
        }
      free (wname);
      if (ERROR_SUCCESS != st) ok = false;
    }
  return ok;
}

static EnvOp_t
env_apply_windows (const EnvKV_t *kvs, size_t kv_count, char **out_err)
{
  bool is_system = chsrc_in_system_scope_mode ();

  if (is_system && !chsrc_is_root () && !in_dry_run_mode ())
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Please run the terminal as Administrator to ensure the necessary permissions")
            : xy_strdup ("请以管理员身份运行终端来保证必要的权限");
        }
      return Env_NoPermission;
    }

  for (size_t k = 0; k < kv_count; k++)
    {
      wchar_t *wide = kvs[k].value ? xy_utf8_to_wstr (kvs[k].value) : NULL;
      bool valid_encoding = !kvs[k].value || wide;
      free (wide);
      if (!valid_encoding || !env_validate_value (kvs[k].value, EnvDialect_WinRegistry, NULL))
        {
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strcat (3, "Value for `", kvs[k].name,
                             "` contains invalid UTF-8, control characters or newlines for Windows Registry")
                : xy_strcat (3, "变量 `", kvs[k].name,
                             "` 的值含有非法 UTF-8、换行或控制字符，无法写入 Windows 注册表");
            }
          return Env_BadValue;
        }
    }

  HKEY    root_key = is_system ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
  LPCWSTR sub_key  = is_system ? ENV_WIN_HKLM_SUBKEY : ENV_WIN_HKCU_SUBKEY;

  HKEY hkey_read = NULL;
  LONG st = RegOpenKeyExW (root_key, sub_key, 0, KEY_QUERY_VALUE, &hkey_read);
  if (ERROR_FILE_NOT_FOUND == st)
    {
      hkey_read = NULL;
    }
  else if (ERROR_ACCESS_DENIED == st)
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Access denied when opening Windows Registry Environment key")
            : xy_strdup ("打开 Windows 注册表环境变量键时权限不足");
        }
      return Env_NoPermission;
    }
  else if (ERROR_SUCCESS != st)
    {
      char *code_s = xy_int2str ((int) st);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_2strcat ("Failed to open Windows Registry Environment key, error: ", code_s)
            : xy_2strcat ("打开 Windows 注册表环境变量键失败，错误码: ", code_s);
        }
      free (code_s);
      return Env_TargetUnwritable;
    }

  EnvRegSnapshot_t *snaps = xy_malloc0 (kv_count * sizeof (EnvRegSnapshot_t));
  for (size_t k = 0; k < kv_count; k++)
    {
      EnvOp_t qrc = env_read_win_snapshot_one (hkey_read, kvs[k].name, &snaps[k], out_err);
      if (Env_OK != qrc)
        {
          if (hkey_read) RegCloseKey (hkey_read);
          env_free_reg_snapshots (snaps, k);
          return qrc;
        }
    }

  /* 幂等判定：检查是否所有目标变量在注册表中已完全一致 */
  bool any_changed = false;
  for (size_t k = 0; k < kv_count; k++)
    {
      if (NULL == kvs[k].value)
        {
          if (snaps[k].existed)
            {
              any_changed = true;
              break;
            }
        }
      else
        {
          if (   !snaps[k].existed
              || REG_SZ != snaps[k].reg_type
              || NULL == snaps[k].utf8_sz_val
              || !xy_streql (snaps[k].utf8_sz_val, kvs[k].value))
            {
              any_changed = true;
              break;
            }
        }
    }

  if (!any_changed)
    {
      if (hkey_read) RegCloseKey (hkey_read);
      env_free_reg_snapshots (snaps, kv_count);
      return Env_Unchanged;
    }

  /* Dry-run 模式不创建 .reg 文件、不修改注册表、不发送广播 */
  if (in_dry_run_mode ())
    {
      if (hkey_read) RegCloseKey (hkey_read);
      env_free_reg_snapshots (snaps, kv_count);
      const char *reg_disp = is_system ? ENV_WIN_HKLM_REGPATH : ENV_WIN_HKCU_REGPATH;
      log_for_file_write (reg_disp, true);
      return Env_OK;
    }

  char *backup_path = NULL;
  if (!env_write_win_reg_backup (is_system, snaps, kv_count, &backup_path, out_err))
    {
      if (hkey_read) RegCloseKey (hkey_read);
      env_free_reg_snapshots (snaps, kv_count);
      return Env_TargetUnwritable;
    }

  if (hkey_read)
    {
      RegCloseKey (hkey_read);
      hkey_read = NULL;
    }

  HKEY  hkey_write = NULL;
  DWORD disp       = 0;
  st = RegCreateKeyExW (root_key, sub_key, 0, NULL, REG_OPTION_NON_VOLATILE,
                        KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hkey_write, &disp);
  if (ERROR_ACCESS_DENIED == st)
    {
      if (backup_path) free (backup_path);
      env_free_reg_snapshots (snaps, kv_count);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Access denied when opening Windows Registry Environment key for writing")
            : xy_strdup ("以写入权限打开 Windows 注册表环境变量键被拒绝");
        }
      return Env_NoPermission;
    }
  if (ERROR_SUCCESS != st)
    {
      char *code_s = xy_int2str ((int) st);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_2strcat ("Failed to open Windows Registry Environment key for writing, error: ", code_s)
            : xy_2strcat ("打开 Windows 注册表写入句柄失败，错误码: ", code_s);
        }
      free (code_s);
      if (backup_path) free (backup_path);
      env_free_reg_snapshots (snaps, kv_count);
      return Env_TargetUnwritable;
    }

  for (size_t k = 0; k < kv_count; k++)
    {
      wchar_t *wname = xy_utf8_to_wstr (kvs[k].name);
      LONG op_st = ERROR_SUCCESS;
      if (!wname)
        op_st = ERROR_NOT_ENOUGH_MEMORY;
      else if (NULL == kvs[k].value)
        {
          op_st = RegDeleteValueW (hkey_write, (LPCWSTR) wname);
          if (ERROR_FILE_NOT_FOUND == op_st)
            op_st = ERROR_SUCCESS;
        }
      else
        {
          wchar_t *wval = xy_utf8_to_wstr (kvs[k].value);
          if (!wval)
            op_st = ERROR_NOT_ENOUGH_MEMORY;
          else
            {
              DWORD cb = (DWORD) ((wcslen (wval) + 1) * sizeof (WCHAR));
              op_st = RegSetValueExW (hkey_write, wname, 0, REG_SZ, (const BYTE *) wval, cb);
              free (wval);
            }
        }
      free (wname);

      if (ERROR_SUCCESS != op_st)
        {
          /* 立即回滚已修改的前序键值 */
          bool restored = env_rollback_win_registry (hkey_write, snaps, k);
          RegCloseKey (hkey_write);

          char *code_s  = xy_int2str ((int) op_st);
          const char *b = backup_path;
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strcat (6, "Failed to update Registry value `", kvs[k].name,
                             "` (error ", code_s,
                             restored ? "); rolled back previous changes. Backup saved at: "
                                      : "); rollback incomplete. Recover using: ", b)
                : xy_strcat (6, "更新注册表变量 `", kvs[k].name,
                             "` 失败（错误码 ", code_s,
                             restored ? "），已自动回滚前序修改。原始备份位于: "
                                      : "），回滚未完成，请使用备份恢复: ", b);
            }
          free (code_s);
          if (backup_path) free (backup_path);
          env_free_reg_snapshots (snaps, kv_count);
          return Env_TargetUnwritable;
        }
    }

  RegCloseKey (hkey_write);
  if (backup_path) free (backup_path);
  env_free_reg_snapshots (snaps, kv_count);

  const char *reg_disp = is_system ? ENV_WIN_HKLM_REGPATH : ENV_WIN_HKCU_REGPATH;
  log_for_file_write (reg_disp, true);

  /* 广播 WM_SETTINGCHANGE，通知 Explorer 与现代终端刷新环境块（超时或失败静默忽略） */
  DWORD_PTR bc_res = 0;
  SendMessageTimeoutW (HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                       (LPARAM) L"Environment", SMTO_ABORTIFHUNG,
                       1500, &bc_res);

  return Env_OK;
}

static EnvOp_t
env_query_persisted_windows (const char *name, bool *out_found, char **out_val, char **out_err)
{
  *out_found = false;
  *out_val   = NULL;

  bool    is_system = chsrc_in_system_scope_mode ();
  HKEY    root_key  = is_system ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
  LPCWSTR sub_key   = is_system ? ENV_WIN_HKLM_SUBKEY : ENV_WIN_HKCU_SUBKEY;

  HKEY hkey = NULL;
  LONG st   = RegOpenKeyExW (root_key, sub_key, 0, KEY_QUERY_VALUE, &hkey);
  if (ERROR_FILE_NOT_FOUND == st)
    return Env_OK;
  if (ERROR_ACCESS_DENIED == st)
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Access denied when reading Windows Registry Environment key")
            : xy_strdup ("读取 Windows 注册表环境变量键权限不足");
        }
      return Env_NoPermission;
    }
  if (ERROR_SUCCESS != st)
    {
      char *code_s = xy_int2str ((int) st);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_2strcat ("Failed to open Windows Registry Environment key, error: ", code_s)
            : xy_2strcat ("打开 Windows 注册表环境变量键失败，错误码: ", code_s);
        }
      free (code_s);
      return Env_TargetUnwritable;
    }

  EnvRegSnapshot_t snap;
  EnvOp_t qrc = env_read_win_snapshot_one (hkey, name, &snap, out_err);
  RegCloseKey (hkey);
  if (Env_OK != qrc)
    return qrc;

  if (!snap.existed)
    return Env_OK;

  if ((REG_SZ != snap.reg_type && REG_EXPAND_SZ != snap.reg_type) || !snap.utf8_sz_val)
    {
      if (snap.raw_bytes) free (snap.raw_bytes);
      if (snap.utf8_sz_val) free (snap.utf8_sz_val);
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strcat (3, "Windows Registry value `", name, "` has unsupported type or invalid string encoding")
            : xy_strcat (3, "Windows 注册表变量 `", name, "` 的类型或字符串编码无效，无法作为镜像地址解析");
        }
      return Env_AmbiguousTarget;
    }

  *out_found = true;
  *out_val   = snap.utf8_sz_val;
  if (snap.raw_bytes) free (snap.raw_bytes);
  return Env_OK;
}

#endif /* XY_Build_On_Windows */


/* ============================================================
 * 跨平台统一内部入口（供公开 API 与 test/fw.c 共用）
 * ============================================================ */

/**
 * @brief 持久化写入引擎的内部入口，供公开 API 与 `test/fw.c` 单元测试共用
 *
 * @details 与 `chsrc_set_env()` / `chsrc_set_env_block()` 的区别仅在于错误处理策略：
 *          本函数**不打印、不退出**，而是把完整的 `EnvOp_t` 状态原样返回，
 *          以便单元测试覆盖全部失败分支（公开 API 依赖它"遇错即退出"的行为）
 *
 * @param  raw_kvs   (name, value) 数组；`value == NULL` 表示删除
 * @param  raw_count 数组元素个数。为 0 或 `raw_kvs == NULL` 时直接返回 `Env_Unchanged`
 * @param  out_shell [out] 实际使用的 POSIX shell，仅 UserScope 且 shell 被成功识别时赋值；
 *                   SystemScope、失败路径以及 Windows 构建下保持 `PosixShell_Unknown`。
 *                   可为 `NULL`。调用方需在返回 `Env_OK` 后把它交给重启提示逻辑
 * @param  out_err   [out] 失败原因文案，**caller-free**；仅在非 `Env_OK`/`Env_Unchanged`
 *                   时被赋值，成功时为 `NULL`。可为 `NULL`
 *
 * @return 完整的 `EnvOp_t`，包括 `Env_NoPermission` 这类公开 API 会转成退出的状态
 *
 * @note 入参数组与其中的字符串由调用方持有；本函数内部只做同名去重
 *       （后者覆盖前者的值，位置保持首次出现），不产生所有权转移
 */
static EnvOp_t
env_apply_kvs (const EnvKV_t *raw_kvs, size_t raw_count,
               PosixShell_t *out_shell, char **out_err)
{
  if (out_err)   *out_err   = NULL;
  if (out_shell) *out_shell = PosixShell_Unknown;

  if (chsrc_in_project_scope_mode ())
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Project scope is not supported for environment variable recipes")
            : xy_strdup ("环境变量换源不支持项目级作用域 (ProjectScope)");
        }
      return Env_ScopeUnsupported;
    }

  if (0 == raw_count || NULL == raw_kvs)
    return Env_Unchanged;

  /* 1. 校验所有变量名 */
  for (size_t i = 0; i < raw_count; i++)
    {
      if (!env_validate_name (raw_kvs[i].name))
        {
          const char *bad = raw_kvs[i].name ? raw_kvs[i].name : "(null)";
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strcat (3, "Invalid environment variable name: `", bad, "`")
                : xy_strcat (3, "非法的环境变量名: `", bad, "`");
            }
          return Env_BadName;
        }
    }

  /* 2. 去重：同名变量保留最后一次指定的值，保持首次出现的顺序 */
  EnvKV_t *kvs      = xy_malloc0 (raw_count * sizeof (EnvKV_t));
  size_t   kv_count = 0;

  for (size_t i = 0; i < raw_count; i++)
    {
      bool updated = false;
      for (size_t j = 0; j < kv_count; j++)
        {
          if (
#ifdef XY_Build_On_Windows
              xy_streql_ic (kvs[j].name, raw_kvs[i].name)
#else
              xy_streql (kvs[j].name, raw_kvs[i].name)
#endif
             )
            {
              kvs[j].value = raw_kvs[i].value;
              updated      = true;
              break;
            }
        }
      if (!updated)
        {
          kvs[kv_count++] = raw_kvs[i];
        }
    }

#ifdef XY_Build_On_Windows
  EnvOp_t rc = env_apply_windows (kvs, kv_count, out_err);
  free (kvs);
  return rc;
#else
  EnvOp_t rc = env_apply_posix (kvs, kv_count, out_shell, out_err);
  free (kvs);
  return rc;
#endif
}

/**
 * @brief 查询单个变量持久化值的内部入口，供 `chsrc_get_env()` 与单元测试共用
 *
 * @param  name    变量名，必须匹配 `^[A-Za-z_][A-Za-z0-9_]*$`
 * @param  out_found [out] 是否在持久化目标中找到该变量；未找到时 `*out_val` 保持 `NULL`。可为 `NULL`
 * @param  out_val [out] 解析出的值，**caller-free**。值为空串时返回的也是空串而非 `NULL`。可为 `NULL`
 * @param  out_err [out] 失败原因文案，**caller-free**；仅在非 `Env_OK` 时被赋值。可为 `NULL`
 *
 * @return 完整 `EnvOp_t`，不打印、不退出
 * @retval Env_OK               查询完成（`*out_found` 区分"已设置"与"未设置"）
 * @retval Env_AmbiguousTarget  无法唯一定位：shell 未识别、多文件值冲突、行含复杂语法或注册表非字符串类型
 * @retval Env_TargetUnwritable 目标文件无法读取
 * @retval Env_NoPermission     注册表键读取权限不足
 * @retval Env_BadName          变量名非法
 * @retval Env_ScopeUnsupported 当前为 `ProjectScope`
 *
 * @note 同一文件内多次安全赋值时以**最后一次**为准，与 shell 顺序执行语义一致
 */
static EnvOp_t
env_query_persisted (const char *name, bool *out_found, char **out_val, char **out_err)
{
  if (out_err)   *out_err   = NULL;
  if (out_found) *out_found = false;
  if (out_val)   *out_val   = NULL;

  if (chsrc_in_project_scope_mode ())
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Project scope is not supported for environment variable recipes")
            : xy_strdup ("环境变量换源不支持项目级作用域 (ProjectScope)");
        }
      return Env_ScopeUnsupported;
    }

  if (!env_validate_name (name))
    {
      const char *bad = name ? name : "(null)";
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strcat (3, "Invalid environment variable name: `", bad, "`")
            : xy_strcat (3, "非法的环境变量名: `", bad, "`");
        }
      return Env_BadName;
    }

  bool found = false;
  char *value = NULL;
#ifdef XY_Build_On_Windows
  EnvOp_t rc = env_query_persisted_windows (name, &found, &value, out_err);
#else
  EnvOp_t rc = env_query_persisted_posix (name, &found, &value, out_err);
#endif
  if (out_found) *out_found = found;
  if (out_val) *out_val = value;
  else free (value);
  return rc;
}

static void
env_emit_post_write_hint (PosixShell_t shell)
{
  if (in_dry_run_mode ())
    return;

#ifdef XY_Build_On_Windows
  (void) shell;
  char *msg = ENGLISH
    ? "Environment variables updated in Windows Registry. Please restart your terminal for changes to take effect."
    : "已更新 Windows 注册表环境变量，请重启终端使新环境变量生效";
  chsrc_note2 (msg);
#else
  if (chsrc_in_system_scope_mode ())
    {
      char *msg = ENGLISH
        ? "Updated /etc/environment. Please re-login or start a new session for system-wide changes to take effect."
        : "已更新 /etc/environment，请重新登录系统或开启新会话使全局环境变量生效";
      chsrc_note2 (msg);
      return;
    }

  const char *reload_cmd = "source ~/.profile";
  if (PosixShell_Bash == shell)
    {
      reload_cmd = xy_file_exist ("~/.bashrc") ? "source ~/.bashrc" : "source ~/.profile";
    }
  else if (PosixShell_Zsh == shell)
    {
      /* ZDOTDIR may contain shell metacharacters; avoid interpolating it. */
      reload_cmd = "source \"${ZDOTDIR-$HOME}/.zshrc\"";
    }
  else if (PosixShell_Fish == shell)
    {
      reload_cmd = "source \"$__fish_config_dir/conf.d/chsrc.fish\"";
    }

  char *msg = ENGLISH
    ? xy_strcat (3, "Please restart your terminal or run `", reload_cmd, "` to apply environment variable changes immediately")
    : xy_strcat (3, "请重启终端或执行 `", reload_cmd, "` 以立即生效环境变量更改");
  chsrc_note2 (msg);
  free (msg);
#endif
}

static void
env_abort_on_error (EnvOp_t op, char *err_msg)
{
  if (err_msg)
    {
      chsrc_error2 (err_msg);
      free (err_msg);
    }
  else
    {
      chsrc_error2 (ENGLISH ? "Environment variable operation failed" : "环境变量操作失败");
    }

  if (Env_ScopeUnsupported == op)
    exit (Exit_Unsupported);
  else
    exit (Exit_UserCause);
}


/* ============================================================
 * Recipe 公开 API
 * ============================================================ */

/**
 * @brief 持久化写入或删除**单个**环境变量
 *
 * @details 写入目标由当前 `ProgMode.Scope` 与平台共同决定：
 *          - Windows：`HKCU\Environment`（UserScope）或
 *            `HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment`（SystemScope）
 *          - POSIX UserScope：精确识别出的当前 shell 对应的一组 rc 文件
 *          - POSIX SystemScope：`/etc/environment`
 *
 * @note 绝不修改当前进程的环境块（不调用 `setenv`/`_putenv`），
 *       因此写入后必须由用户重启终端或重新加载配置文件才会生效
 *
 * @note **幂等**：若所有目标的持久化值已经等于期望值，则不备份、不写盘、不广播、
 *       不打印重启提示，直接返回 `Env_Unchanged`
 *
 * @note 实际发生写入时（`Env_OK`），本函数会在末尾输出且仅输出 1 次重启终端提示；
 *       Windows 侧还会广播 `WM_SETTINGCHANGE` 通知 Explorer 与现代终端刷新
 *
 * @param  name  变量名，必须匹配 `^[A-Za-z_][A-Za-z0-9_]*$`
 * @param  value 变量值。`NULL` 表示**删除**该变量；`""` 表示设置为空串。
 *               含目标方言的危险字符（POSIX 双引号内的 `"` `\\` `$` `` ` ``、
 *               以及所有平台的 `\r` `\n` 与控制字符）会被拒绝
 *
 * @return 只会是下面两种之一，其余状态不返回
 * @retval Env_OK          实际发生了写入或删除
 * @retval Env_Unchanged   目标值已与期望一致
 *
 * @warning 其余 `EnvOp_t` 状态（权限不足、目标不可写、非法名/值、无法确认 shell、
 *          多文件冲突或行含复杂语法、ProjectScope）会先打印中英双语错误，
 *          再 `exit (Exit_UserCause)`（ProjectScope 为 `Exit_Unsupported`），
 *          以防止 recipe 漏检返回值而打印假的"换源成功"结论
 */
static EnvOp_t
chsrc_set_env (const char *name, const char *value)
{
  EnvKV_t      kv    = { name, value };
  PosixShell_t shell = PosixShell_Unknown;
  char        *err   = NULL;

  EnvOp_t op = env_apply_kvs (&kv, 1, &shell, &err);
  if (Env_OK == op)
    {
      if (err) free (err);
      env_emit_post_write_hint (shell);
      return Env_OK;
    }
  if (Env_Unchanged == op)
    {
      if (err) free (err);
      return Env_Unchanged;
    }

  env_abort_on_error (op, err);
  return op;
}

/**
 * @brief 一次性持久化写入或删除**多个**环境变量，供一个 dish 需要同时设置若干变量的场景使用
 *
 * @details 语义上等价于对每个变量调用一次 `chsrc_set_env()`，但代价只有一份：
 *          单次备份、单次落盘、Windows 侧单次注册表写入序列与单次 `WM_SETTINGCHANGE`
 *          广播，且最多输出 1 次重启终端提示。同一 dish 内分多次调用本函数
 *          也不会互相抹掉对方已写入的变量（按变量名增量合并/删除）。
 *
 * @param name1 第 1 个变量名。传 `NULL` 表示空操作，直接返回 `Env_Unchanged`
 * @param ...   紧跟 `name1` 的是它的值，之后交替出现 `(name, value)`，
 *              并以**一个额外的 `NULL`** 结束（即结束符占用的是 name 槽位）
 *
 * @note **参数总数必须是奇数**。`name` 与 `value` 成对，末尾单独一个 `NULL` 作为哨兵：
 * @code
 *   chsrc_set_env_block ("FOO", "1",           NULL);              // 写 1 个
 *   chsrc_set_env_block ("FOO", "1", "BAR", NULL, NULL);           // 写 FOO，删 BAR
 * @endcode
 *       若误传成偶数个参数，最后的 `NULL` 会被当成某个变量的值消费掉，
 *       随后 `va_arg()` 将越过参数表末尾读取，属未定义行为
 *
 * @note 同一名字出现多次时，**最后一次**的值生效，但其在参数表中的位置保持首次出现的位置
 *
 * @note 校验阶段是"全有或全无"：任一变量名/值非法，或任一目标文件里的同名行含
 *       复杂动态语法，则在任何落盘动作之前中止；Windows 侧若写入中途失败，
 *       会自动把已改过的值回滚为原状态并给出 `.reg` 备份路径
 *
 * @return 同 `chsrc_set_env()`，只会是 `Env_OK` 或 `Env_Unchanged`；
 *         其余状态同样打印双语错误后 `exit`，详见 `chsrc_set_env()` 的 `@warning`
 */
static EnvOp_t
chsrc_set_env_block (const char *name1, ...)
{
  if (NULL == name1)
    return Env_Unchanged;

  size_t   cap   = 8;
  size_t   count = 0;
  EnvKV_t *kvs   = xy_malloc0 (cap * sizeof (EnvKV_t));

  va_list ap;
  va_start (ap, name1);

  const char *cur_name = name1;
  while (NULL != cur_name)
    {
      const char *cur_val = va_arg (ap, const char *);
      if (count >= cap)
        {
          cap *= 2;
          EnvKV_t *grown = realloc (kvs, cap * sizeof (EnvKV_t));
          if (!grown)
            {
              va_end (ap);
              free (kvs);
              xy_throw ("Memory allocation failed in chsrc_set_env_block");
            }
          kvs = grown;
        }
      kvs[count++] = (EnvKV_t) { cur_name, cur_val };
      cur_name     = va_arg (ap, const char *);
    }

  va_end (ap);

  PosixShell_t shell = PosixShell_Unknown;
  char        *err   = NULL;

  EnvOp_t op = env_apply_kvs (kvs, count, &shell, &err);
  free (kvs);

  if (Env_OK == op)
    {
      if (err) free (err);
      env_emit_post_write_hint (shell);
      return Env_OK;
    }
  if (Env_Unchanged == op)
    {
      if (err) free (err);
      return Env_Unchanged;
    }

  env_abort_on_error (op, err);
  return op;
}

/**
 * @brief 读取并打印若干个环境变量的值，优先取持久化配置，缺失时回退到当前终端会话
 *
 * @details 这是 `chsrc get` 对环境变量型 dish 的展示入口。它先按当前平台与作用域解析
 *          真正的持久化落点（POSIX 逐行解析 shell rc 文件与 `/etc/environment`，
 *          Windows 读注册表），因此打印出的就是重启终端之后才会生效的值；
 *          若持久化配置中没有该变量，则以当前进程环境为准，方便用户在首次 `chsrc set` 之前
 *          查看机器上已有的值；读取时使用 `xy_env_get()`，以保证 Windows 非 ASCII 值按 UTF-8 返回。
 *
 * @param var1 第 1 个要查看的变量名，不可为 `NULL`
 * @param ...  后续变量名，以 `NULL` 结束：`chsrc_get_env ("A", "B", NULL);`
 *
 * @note 只读，不改文件，不生成备份
 *
 * @note 每个变量只输出一行 `NAME=value`：
 *       - 持久化配置中存在 → 输出持久化值
 *       - 持久化配置中不存在 → 输出当前终端会话中的值；会话中也没有则为 `NAME=`
 *
 * @note 仅当**持久化值与会话值同时存在且不一致**时才追加一条提示，说明需重启终端
 *       或重新加载配置文件才会同步；只有单侧存在时不做任何额外提示
 *
 * @important
 *       本函数 **只打印、不返回值** ；若 recipe 需要在内存中取用当前进程的环境变量，
 *       请调用 `xy_env_get()`（返回 UTF-8 副本，调用者负责释放），不要把本函数当作取值接口
 *
 * @warning 所有变量先完成读取校验，任何一个无法唯一定位持久化值时**不输出半截结果**，
 *          而是打印双语错误后 `exit`。拒绝的情形包括：无法确认当前 shell、
 *          多个配置文件对同一变量给出不同值、同名行含动态展开或复杂语法、
 *          注册表值不是字符串类型、`ProjectScope`、变量名非法
 */
static void
chsrc_get_env (const char *var1, ...)
{
  if (!var1)
    {
      xy_unreached ();
      return;
    }

  size_t       cap   = 8;
  size_t       count = 0;
  const char **vars  = xy_malloc0 (cap * sizeof (const char *));

  va_list ap;
  va_start (ap, var1);
  const char *cur = var1;
  while (cur)
    {
      if (count >= cap)
        {
          cap *= 2;
          const char **grown = realloc (vars, cap * sizeof (const char *));
          if (!grown)
            {
              va_end (ap);
              free (vars);
              xy_throw ("Memory allocation failed in chsrc_get_env");
            }
          vars = grown;
        }
      vars[count++] = cur;
      cur = va_arg (ap, const char *);
    }
  va_end (ap);

  /* 先对全部待查变量完成持久化读取校验，遇到未知 Shell / 冲突 / 复杂语法立即中止，避免输出半截结果 */
  bool  *founds = xy_malloc0 (count * sizeof (bool));
  char **vals   = xy_malloc0 (count * sizeof (char *));

  for (size_t i = 0; i < count; i++)
    {
      char   *err = NULL;
      EnvOp_t rc  = env_query_persisted (vars[i], &founds[i], &vals[i], &err);
      if (Env_OK != rc)
        {
          for (size_t j = 0; j < i; j++)
            {
              if (vals[j]) free (vals[j]);
            }
          free (founds);
          free (vals);
          free (vars);
          env_abort_on_error (rc, err);
          return;
        }
      if (err) free (err);
    }

  for (size_t i = 0; i < count; i++)
    {
      char *proc_val = xy_env_get (vars[i]);

      /* 持久化配置中没有该项时以当前进程环境为准，不做额外提示 */
      const char *shown = founds[i] ? vals[i] : (proc_val ? proc_val : "");
      printf ("%s=%s\n", vars[i], shown);

      /* 仅当持久化值与当前会话值同时存在且不一致时才提示 */
      if (founds[i] && proc_val && !xy_streql (vals[i], proc_val))
        {
          char *note = ENGLISH
            ? xy_strcat (5, "Current terminal session has a different value for `", vars[i],
                         "` (`", proc_val, "`); restart terminal or reload shell profile to sync")
            : xy_strcat (5, "当前终端会话中的 `", vars[i],
                         "` 值为 `", proc_val, "`（与已持久化配置不同），重启终端或重新加载配置文件后同步");
          chsrc_note2 (note);
          free (note);
        }

      if (vals[i]) free (vals[i]);
      if (proc_val) free (proc_val);
    }

  free (founds);
  free (vals);
  free (vars);
}
