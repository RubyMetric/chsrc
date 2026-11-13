/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Conservative shell context tracking, included by env.c.
 * Never executes user configuration.
 * ------------------------------------------------------------*/

typedef struct EnvShellContext_t
{
  size_t blocks, braces, parens;
  char quote;
  bool continued, opaque;
}
EnvShellContext_t;

static bool
env_shell_at_top_level (const EnvShellContext_t *ctx)
{
  return !ctx->blocks && !ctx->braces && !ctx->parens
      && !ctx->quote && !ctx->continued && !ctx->opaque;
}

static bool
env_shell_word_is (const char *word, size_t len, const char *expected)
{
  return strlen (expected) == len && 0 == memcmp (word, expected, len);
}

/* Skip a complete, single-line command substitution without interpreting it.
 * Its quote state is independent from the surrounding double-quoted word. */
static bool
env_shell_skip_command (const char *line, size_t len, size_t *at)
{
  char quotes[64] = {0};
  size_t depth = 1;
  for (size_t i = *at + 2; i < len; i++)
    {
      char c = line[i];
      char *quote = &quotes[depth - 1];
      if ('\\' == c && '\'' != *quote) { i++; continue; }
      if ('\'' == *quote)
        {
          if ('\'' == c) *quote = 0;
          continue;
        }
      if ('$' == c && i + 1 < len && '(' == line[i + 1])
        {
          if (depth == sizeof (quotes)) return false;
          quotes[depth++] = 0;
          i++;
          continue;
        }
      if ('"' == *quote)
        {
          if ('"' == c) *quote = 0;
          continue;
        }
      if ('\'' == c || '"' == c) { *quote = c; continue; }
      if ('`' == c) return false;
      if ('(' == c)
        {
          if (depth == sizeof (quotes)) return false;
          quotes[depth++] = 0;
        }
      else if (')' == c && 0 == --depth)
        {
          *at = i;
          return true;
        }
    }
  return false;
}

/* Only standalone top-level assignments may be rewritten. Unsupported
 * constructs make the context opaque; do not guess where code resumes. */
static bool
env_shell_scan_line (EnvShellContext_t *ctx, const char *line, size_t len,
                     EnvDialect_t dialect)
{
  if (EnvDialect_EtcEnv == dialect) return true;
  bool top = env_shell_at_top_level (ctx);
  bool command_start = !ctx->continued;
  ctx->continued = false;
  if (memchr (line, '\0', len)) ctx->opaque = true;

  for (size_t i = 0; i < len; i++)
    {
      char c = line[i];
      if ('\'' == ctx->quote)
        {
          if ('\'' == c) ctx->quote = 0;
          continue;
        }
      if ('\\' == c)
        {
          if (i + 1 == len) ctx->continued = true;
          else i++;
          continue;
        }
      if ('$' == c && i + 1 < len && '(' == line[i + 1])
        {
          if (!env_shell_skip_command (line, len, &i)) ctx->opaque = true;
          command_start = false;
          continue;
        }
      if ('`' == c)
        {
          size_t end = i + 1;
          for (; end < len && '`' != line[end]; end++)
            if ('\\' == line[end] && end + 1 < len) end++;
          if (end == len) ctx->opaque = true;
          else i = end;
          continue;
        }
      if ('"' == ctx->quote)
        {
          if ('"' == c) ctx->quote = 0;
          continue;
        }
      if ('#' == c && (0 == i || strchr (" \t;|&()", line[i - 1]))) break;
      if ('\'' == c || '"' == c)
        {
          ctx->quote = c;
          command_start = false;
          continue;
        }
      if ('<' == c && i + 1 < len && '<' == line[i + 1])
        ctx->opaque = true;
      if ('{' == c || '(' == c)
        {
          if ('{' == c) ctx->braces++;
          else ctx->parens++;
          top = false;
          command_start = true;
          continue;
        }
      if ('}' == c || ')' == c)
        {
          size_t *depth = ('}' == c) ? &ctx->braces : &ctx->parens;
          if (*depth) (*depth)--;
          else if (!ctx->blocks) ctx->opaque = true;
          top = false;
          command_start = true;
          continue;
        }
      if (';' == c || '|' == c || '&' == c)
        {
          command_start = true;
          continue;
        }
      if (' ' == c || '\t' == c) continue;
      size_t start = i;
      if (env_is_ident_start (c))
        {
          while (i + 1 < len && env_is_ident_char (line[i + 1])) i++;
          size_t n = i - start + 1;
          bool boundary = i + 1 == len || strchr (" \t;|&(){}", line[i + 1]);
          if (command_start && boundary)
            {
              const char *w = line + start;
              bool fish = EnvDialect_Fish == dialect;
              bool opens = env_shell_word_is (w, n, "if")
                        || env_shell_word_is (w, n, "for")
                        || env_shell_word_is (w, n, "while")
                        || (!fish && (env_shell_word_is (w, n, "until")
                                   || env_shell_word_is (w, n, "case")
                                   || env_shell_word_is (w, n, "select")))
                        || (fish && (env_shell_word_is (w, n, "begin")
                                  || env_shell_word_is (w, n, "switch")
                                  || env_shell_word_is (w, n, "function")));
              bool closes = fish ? env_shell_word_is (w, n, "end")
                                 : (env_shell_word_is (w, n, "fi")
                                 || env_shell_word_is (w, n, "done")
                                 || env_shell_word_is (w, n, "esac"));
              if (opens) { ctx->blocks++; top = false; }
              if (closes)
                {
                  if (ctx->blocks) ctx->blocks--;
                  else ctx->opaque = true;
                  top = false;
                }
              command_start = env_shell_word_is (w, n, "then")
                           || env_shell_word_is (w, n, "do")
                           || env_shell_word_is (w, n, "else")
                           || env_shell_word_is (w, n, "elif")
                           || env_shell_word_is (w, n, "if")
                           || env_shell_word_is (w, n, "while")
                           || env_shell_word_is (w, n, "until");
              continue;
            }
        }
      command_start = false;
    }
  return top && env_shell_at_top_level (ctx);
}

static bool
env_shell_mentions_name (const char *line, size_t len, const char *name)
{
  size_t n = strlen (name);
  size_t start = 0;
  while (start < len && (' ' == line[start] || '\t' == line[start])) start++;
  if (start < len && '#' == line[start]) return false;
  for (size_t i = start; i + n <= len; i++)
    {
      if (0 == memcmp (line + i, name, n)
          && (0 == i || !env_is_ident_char (line[i - 1]))
          && (i + n == len || !env_is_ident_char (line[i + n])))
        return true;
    }
  return false;
}

static EnvOp_t
env_shell_context_error (const char *path, size_t lineno, char **out_err)
{
  if (out_err)
    {
      char *number = xy_int2str ((int) lineno);
      *out_err = ENGLISH
        ? xy_strcat (5, "Cannot safely resolve shell syntax in ", path, ":", number,
                     "; please configure the variable manually. No configuration was written.")
        : xy_strcat (5, "无法安全解析配置文件 ", path, ":", number,
                     " 的 Shell 上下文，请手工配置该变量；未写入任何配置");
      free (number);
    }
  return Env_AmbiguousTarget;
}
