/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Lib Authors   :  Mikachu2333 <linkchou@yandex.com>
 * Contributors  :  EXAMPLE     <EXAMPLE@EXAMPLE.com>
 *               |
 * Created On    : <2026-08-02>
 * Last Modified : <2026-08-02>
 *
 *
 * uv 配置 (uv.toml / pyproject.toml) 字符串改写工具
 * (由 uv.c #include, 可被 test 单独 #include)
 *
 * 仅处理 chsrc 关心的 uv 配置子集, 不是完整 TOML 解析器:
 *   - uv.toml:        顶层 [[index]] 数组表、顶层 python-install-mirror 键
 *   - pyproject.toml: [tool.uv] 表内的 [[tool.uv.index]] 数组表、
 *                     python-install-mirror 键
 *
 * 刻意不处理: 多行字符串、数组/内联表值、字符串转义 ——
 * 因为所有改写都按"整行替换/删除"进行, 无需解析值本身。
 * 已知限制: 多行字符串 (""") 内若出现行首 '[' 会被误认为段头, 罕见可忽略。
 *
 * 所有函数均为 static, 依赖调用方所在翻译单元提供 stdlib/string.h。
 * ------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * 判断行首 (调用方必须保证 p 位于行首) 是否以 key 作为键开始。
 * 键名后必须紧跟 =、空白或行尾, 避免误匹配 urlx 之类前缀键。
 */
static bool
uvh_key_prefix (const char *p, const char *key)
{
  size_t kl = strlen (key);
  if (strncmp (p, key, kl) != 0) return false;
  char c = p[kl];
  return c == '=' || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

static const char *
uvh_skip_indent (const char *p)
{
  while (*p == ' ' || *p == '\t') p++;
  return p;
}

/**
 * 判断行首 (调用方必须保证 p 位于行首) 是否为指定表头文本。
 * 允许缩进、CRLF 和行尾注释。
 */
static bool
uvh_header_match (const char *p, const char *header)
{
  p = uvh_skip_indent (p);
  size_t hl = strlen (header);
  if (strncmp (p, header, hl) != 0) return false;
  char c = p[hl];
  return c == '\n' || c == '\r' || c == '\0' || c == ' ' || c == '\t' || c == '#';
}

/* 跳过当前行 (含换行符), 返回下一行的行首 */
static const char *
uvh_next_line (const char *p)
{
  while (*p && *p != '\n') p++;
  if (*p == '\n') p++;
  return p;
}

/**
 * 从 start 开始找下一个段头行 (行首 '[') 的位置。
 * 返回 NULL 表示直到文件结尾都没有新段头。
 */
static const char *
uvh_find_section_end (const char *start)
{
  const char *p = start;
  while (*p)
    {
      if (*uvh_skip_indent (p) == '[') return p;
      p = uvh_next_line (p);
    }
  return NULL;
}

/**
 * 全文件查找第一个行首匹配 header 的表头位置; 未找到返回 NULL。
 */
static const char *
uvh_find_table (const char *content, const char *header)
{
  const char *p = content;
  while (*p)
    {
      const char *line = uvh_skip_indent (p);
      if (*line == '[' && uvh_header_match (line, header)) return p;
      p = uvh_next_line (p);
    }
  return NULL;
}

/**
 * 在 [first, end) 行区间内查找以 key 开头的键行; end 为 NULL 时到文件尾。
 */
static const char *
uvh_find_key_in_section (const char *first, const char *end, const char *key)
{
  for (const char *s = first; *s && (!end || s < end); s = uvh_next_line (s))
    {
      if (uvh_key_prefix (uvh_skip_indent (s), key)) return s;
    }
  return NULL;
}

/**
 * 从 "key = \"value\"" 或 "key = 'value'" 行提取字符串值。
 * @return malloc 的 caller-free 字符串; 值不是字符串时返回 NULL。
 */
static char *
uvh_extract_string_value (const char *line)
{
  const char *eq = strchr (uvh_skip_indent (line), '=');
  if (!eq) return NULL;
  const char *v = eq + 1;
  while (*v == ' ' || *v == '\t') v++;
  if (*v != '"' && *v != '\'') return NULL;
  char quote = *v;
  v++;
  const char *val = v;
  while (*v && *v != quote && *v != '\n') v++;
  size_t vl = v - val;
  char *ret = malloc (vl + 1);
  memcpy (ret, val, vl);
  ret[vl] = '\0';
  return ret;
}

static bool
uvh_value_is_true (const char *line)
{
  const char *eq = strchr (uvh_skip_indent (line), '=');
  if (!eq) return false;
  const char *v = uvh_skip_indent (eq + 1);
  return strncmp (v, "true", 4) == 0
      && (v[4] == '\0' || v[4] == '\r' || v[4] == '\n' || v[4] == ' ' || v[4] == '\t' || v[4] == '#');
}

/**
 * 查找要管理的 index。多个 index 中优先选择 default = true，
 * 否则选择第一个；表可以位于 [tool.uv.sources] 等子表之后。
 */
static const char *
uvh_find_managed_index (const char *content, const char *index_header)
{
  const char *first = NULL;
  for (const char *s = content; *s; s = uvh_next_line (s))
    {
      if (!uvh_header_match (s, index_header)) continue;
      if (!first) first = s;

      const char *body = uvh_next_line (s);
      const char *end = uvh_find_section_end (body);
      const char *default_line = uvh_find_key_in_section (body, end, "default");
      if (default_line && uvh_value_is_true (default_line)) return s;
    }
  return first;
}

/* 文件是否使用 CRLF 换行。 */
static bool
uvh_detect_crlf (const char *content)
{
  const char *nl = strchr (content, '\n');
  return nl && nl > content && nl[-1] == '\r';
}

/**
 * 按 content 的换行风格转换 str 中的 LF；content 为 CRLF 时返回 CRLF 版本。
 * 仅用于追加的纯 LF 文本，不要传入已含 CRLF 的 content 本身。
 *
 * @return caller-free
 */
static char *
uvh_apply_eol (const char *content, const char *str)
{
  bool crlf = uvh_detect_crlf (content);
  size_t n = strlen (str);
  char *ret = malloc (n + (crlf ? n : 0) + 1);
  size_t pos = 0;
  for (const char *s = str; *s; s++)
    {
      if (crlf && *s == '\n') ret[pos++] = '\r';
      ret[pos++] = *s;
    }
  ret[pos] = '\0';
  return ret;
}

/**
 * 用 new_line_text (不含换行) 替换 content 中从 line 开始的整行，
 * 保留原行缩进、行尾风格 (CRLF/LF) 与后续内容。
 *
 * @return 新内容 (caller-free)
 */
static char *
uvh_replace_line (const char *content, const char *line, const char *new_line_text)
{
  const char *line_end = strchr (line, '\n');
  if (!line_end) line_end = content + strlen (content);

  /* 原行为 CRLF 时，新行同样使用 CRLF */
  bool crlf = (*line_end == '\n' && line_end > line && line_end[-1] == '\r');

  /* 复用原行缩进 */
  const char *indent_end = uvh_skip_indent (line);
  size_t ind = indent_end - line;

  size_t len = strlen (content) + ind + strlen (new_line_text) + 8;
  char *ret = calloc (len, 1);
  size_t pos = 0;
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)(line - content), content);
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)ind, line);
  pos += snprintf (ret + pos, len - pos, "%s", new_line_text);
  if (crlf) ret[pos++] = '\r';
  strcpy (ret + pos, line_end);
  return ret;
}

/**
 * 在 insert_at 位置之前插入 insert_text (以 '\n' 结尾)。
 * 内容为 CRLF 时，插入文本与分隔换行同样转换为 CRLF。
 *
 * @return 新内容 (caller-free)
 */
static char *
uvh_insert_before (const char *content, const char *insert_at, const char *insert_text)
{
  size_t head = insert_at - content;

  bool need_sep = (head > 0 && content[head - 1] != '\n');
  bool crlf = need_sep ? uvh_detect_crlf (content)
                       : (head >= 2 && content[head - 1] == '\n' && content[head - 2] == '\r');

  size_t text_len = strlen (insert_text);
  size_t extra = (crlf ? text_len : 0) + (need_sep ? 1 : 0);
  size_t len = strlen (content) + text_len + extra + 16;
  char *ret = calloc (len, 1);
  size_t pos = 0;
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)head, content);
  if (need_sep)
    {
      if (crlf) ret[pos++] = '\r';
      ret[pos++] = '\n';
    }
  for (const char *t = insert_text; *t; t++)
    {
      if (crlf && *t == '\n') ret[pos++] = '\r';
      ret[pos++] = *t;
    }
  strcpy (ret + pos, content + head);
  return ret;
}


/**
 * 替换/创建 index 数组表 (如 [[index]] 或 [[tool.uv.index]]) 的 url 值。
 *
 * @param content      原始内容
 * @param url          新的索引 URL
 * @param index_header 段头文本, 如 "[[index]]" 或 "[[tool.uv.index]]"
 * @param parent_table 父表头, 如 NULL (顶层) 或 "[tool.uv]";
 *                     父表不存在时会在文件末尾创建父表并写入 index 段
 *
 * @return 新内容 (caller-free)
 */
static char *
replace_index_url (const char *content, const char *url, const char *index_header, const char *parent_table)
{
  /* default index 才是 uv 的 PyPI 替代源；不存在 default 时回退到第一个。 */
  const char *ih = uvh_find_managed_index (content, index_header);
  const char *boundary = content + strlen (content);
  if (!ih && parent_table)
    {
      const char *table = uvh_find_table (content, parent_table);
      if (!table)
        {
          /* 父表不存在: 在文件末尾创建父表并在其中创建 index 段,
           * 换行风格与 content 保持一致 */
          size_t clen = strlen (content);
          const char *sep = (clen > 0) ? "\n" : "";
          size_t seg_len = strlen (parent_table) + strlen (index_header) + strlen (url) + 48;
          char *seg = calloc (seg_len, 1);
          snprintf (seg, seg_len, "%s\n%s\nurl = \"%s\"\ndefault = true\n",
                    parent_table, index_header, url);
          char *seg_eol = uvh_apply_eol (content, seg);
          free (seg);
          char *sep_eol = uvh_apply_eol (content, sep);
          size_t len = clen + strlen (sep_eol) + strlen (seg_eol) + 1;
          char *ret = calloc (len, 1);
          snprintf (ret, len, "%s%s%s", content, sep_eol, seg_eol);
          free (sep_eol);
          free (seg_eol);
          return ret;
        }
    }

  /* 作用域内无 index 段: 追加到边界之前 */
  if (!ih)
    {
      size_t seglen = strlen (index_header) + strlen (url) + 48;
      char *seg = malloc (seglen);
      snprintf (seg, seglen, "%s\nurl = \"%s\"\ndefault = true\n", index_header, url);
      char *ret = uvh_insert_before (content, boundary, seg);
      free (seg);
      return ret;
    }

  /* 段内第一行与段边界 (下一个段头或文件尾) */
  const char *first = uvh_next_line (ih);
  const char *end = uvh_find_section_end (first);
  const char *url_line = uvh_find_key_in_section (first, end, "url");

  /* 段内没有 url 键: 在段内第一行之前插入 url (及缺失的 default) */
  if (!url_line)
    {
      bool has_default = uvh_find_key_in_section (first, end, "default") != NULL;
      size_t seglen = strlen (url) + (has_default ? 24 : 48);
      char *seg = malloc (seglen);
      size_t spos = 0;
      spos += snprintf (seg + spos, seglen - spos, "url = \"%s\"\n", url);
      if (!has_default)
        spos += snprintf (seg + spos, seglen - spos, "default = true\n");
      char *ret = uvh_insert_before (content, first, seg);
      free (seg);
      return ret;
    }

  /* 替换 url 行 */
  size_t newlen = strlen (url) + 32;
  char *new_line = malloc (newlen);
  snprintf (new_line, newlen, "url = \"%s\"", url);
  char *ret = uvh_replace_line (content, url_line, new_line);
  free (new_line);
  return ret;
}


/**
 * 替换/创建顶层键或指定表内键 (如 python-install-mirror) 的字符串值。
 *
 * @param content     原始内容
 * @param key         键名
 * @param url         新值
 * @param parent_table 父表头, 如 NULL (顶层) 或 "[tool.uv]";
 *                     父表不存在时会在文件末尾创建父表并写入键
 *
 * @return 新内容 (caller-free)
 */
static char *
replace_key_value (const char *content, const char *key, const char *url, const char *parent_table)
{
  const char *old_line = NULL;
  const char *insert_at = NULL;

  if (parent_table)
    {
      const char *table = uvh_find_table (content, parent_table);
      if (!table)
        {
          /* 父表不存在: 在文件末尾创建父表并写入键,
           * 换行风格与 content 保持一致 */
          size_t clen = strlen (content);
          const char *sep = (clen > 0) ? "\n" : "";
          size_t seg_len = strlen (parent_table) + strlen (key) + strlen (url) + 16;
          char *seg = calloc (seg_len, 1);
          snprintf (seg, seg_len, "%s\n%s = \"%s\"\n", parent_table, key, url);
          char *seg_eol = uvh_apply_eol (content, seg);
          free (seg);
          char *sep_eol = uvh_apply_eol (content, sep);
          size_t len = clen + strlen (sep_eol) + strlen (seg_eol) + 1;
          char *ret = calloc (len, 1);
          snprintf (ret, len, "%s%s%s", content, sep_eol, seg_eol);
          free (sep_eol);
          free (seg_eol);
          return ret;
        }

      const char *first = uvh_next_line (table);
      const char *key_zone_end = NULL;
      for (const char *s = first; *s; s = uvh_next_line (s))
        {
          const char *line = uvh_skip_indent (s);
          if (*line == '[')
            {
              key_zone_end = s;
              break;
            }
        }
      old_line = uvh_find_key_in_section (first, key_zone_end, key);
      insert_at = key_zone_end ? key_zone_end : content + strlen (content);
    }
  else
    {
      /* 顶层键区: 到第一个段头为止 */
      const char *p = content;
      while (*p)
        {
          if (*uvh_skip_indent (p) == '[') { insert_at = p; break; }
          if (uvh_key_prefix (uvh_skip_indent (p), key)) old_line = p;
          p = uvh_next_line (p);
        }
      if (!insert_at) insert_at = content + strlen (content);
    }

  if (old_line)
    {
      /* 替换旧值行 */
      size_t newlen = strlen (key) + strlen (url) + 32;
      char *new_line = malloc (newlen);
      snprintf (new_line, newlen, "%s = \"%s\"", key, url);
      char *ret = uvh_replace_line (content, old_line, new_line);
      free (new_line);
      return ret;
    }

  /* 键不存在: 追加到作用域末尾 */
  size_t seglen = strlen (key) + strlen (url) + 32;
  char *seg = malloc (seglen);
  snprintf (seg, seglen, "%s = \"%s\"\n", key, url);
  char *ret = uvh_insert_before (content, insert_at, seg);
  free (seg);
  return ret;
}


/**
 * 在指定表 (parent_table 为 NULL 表示顶层) 内提取键的字符串值。
 *
 * @return malloc 的 caller-free 字符串; 未找到返回 NULL。
 */
static char *
uvh_get_value_in_table (const char *content, const char *key, const char *parent_table)
{
  const char *p = content;
  if (parent_table)
    {
      const char *table = uvh_find_table (content, parent_table);
      if (!table) return NULL;
      p = uvh_next_line (table);
    }

  const char *limit = uvh_find_section_end (p);
  for (const char *s = p; *s && (!limit || s < limit); s = uvh_next_line (s))
    {
      if (uvh_key_prefix (uvh_skip_indent (s), key)) return uvh_extract_string_value (s);
    }
  return NULL;
}


/**
 * 提取 chsrc 管理的 index URL：优先 default = true，否则取第一个。
 *
 * @param index_header 段头文本, 如 "[[index]]" 或 "[[tool.uv.index]]"
 * @param parent_table 父表头, 如 NULL (顶层) 或 "[tool.uv]"
 *
 * @return malloc 的 caller-free 字符串; 未找到返回 NULL。
 */
static char *
uvh_get_index_url (const char *content, const char *index_header, const char *parent_table)
{
  if (parent_table && !uvh_find_table (content, parent_table)) return NULL;

  const char *ih = uvh_find_managed_index (content, index_header);
  if (!ih) return NULL;

  const char *first = uvh_next_line (ih);
  const char *end = uvh_find_section_end (first);
  const char *url_line = uvh_find_key_in_section (first, end, "url");
  if (!url_line) return NULL;
  return uvh_extract_string_value (url_line);
}


/* ============ uv.toml 便捷包装 (保持原函数名) ============ */

static char *
replace_pypi_index_url (const char *content, const char *url)
{
  return replace_index_url (content, url, "[[index]]", NULL);
}

static char *
replace_python_install_mirror (const char *content, const char *url)
{
  return replace_key_value (content, "python-install-mirror", url, NULL);
}

static char *
uvh_get_top_level_value (const char *content, const char *key)
{
  return uvh_get_value_in_table (content, key, NULL);
}
