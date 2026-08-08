/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Lib Authors   :  Mikachu2333   <linkchou@yandex.com>
 * Contributors  :  Nul None      <nul@none.org>
 *               |
 * Created On    : <2026-08-02>
 * Last Modified : <2026-08-02>
 *
 *
 * @file uv-helper.c
 * @brief uv 配置 (uv.toml / pyproject.toml) 字符串改写工具
 *        (由 uv.c #include, 可被 test 单独 #include)
 *
 * 仅处理 chsrc 关心的 uv 配置子集, 不是完整 TOML 解析器:
 *   - uv.toml:        顶层 [[index]] 数组表、顶层 python-install-mirror 键
 *   - pyproject.toml: [tool.uv] 表内的 [[tool.uv.index]] 数组表、
 *                     python-install-mirror 键
 *
 * 刻意不处理: 多行字符串、数组/内联表值、字符串转义 ——
 * 因为所有改写都按“整行替换/删除”进行, 无需解析值本身。
 * 已知限制: 多行字符串 (`"""`) 内若出现行首 `[` 会被误认为段头, 罕见可忽略。
 *
 * 所有函数均为 static, 依赖调用方所在翻译单元提供 stdlib/string.h。
 * ------------------------------------------------------------*/

/**
 * @brief 判断行首是否以 key 作为键开始。
 *
 * 支持 bare key 与单/双引号 quoted key (如 "url" 或 'url')。
 * 调用方必须保证 p 位于行首。键名后必须紧跟 =、空白或行尾,
 * 避免误匹配 urlx 之类前缀键。
 *
 * @param p   待检查的字符串, 必须位于行首
 * @param key 键名
 *
 * @return 行首以 key 开始且满足键边界时为 true, 否则为 false
 */
static bool
uvh_key_prefix (const char *p, const char *key)
{
  size_t kl = strlen (key);
  const char *end = NULL;
  if (strncmp (p, key, kl) == 0)
    end = p + kl;
  else if ((*p == '"' || *p == '\'')
           && strncmp (p + 1, key, kl) == 0 && p[kl + 1] == *p)
    end = p + kl + 2;
  else
    return false;

  char c = *end;
  return c == '=' || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

/**
 * @brief 跳过行首的空白字符 (空格与 Tab), 返回首个非空白字符位置。
 *
 * 不修改原字符串, 返回的是原字符串内部的指针。
 *
 * @param p 输入字符串
 *
 * @return 原字符串中第一个非空白字符的指针
 */
static const char *
uvh_skip_indent (const char *p)
{
  while (*p == ' ' || *p == '\t') p++;
  return p;
}

/**
 * @brief 判断行首是否为指定表头文本。
 *
 * 允许缩进、CRLF 和行尾注释; 额外把 [tool."uv"] / [tool.'uv'] 识别为
 * [tool.uv] 的等价形式。调用方必须保证 p 位于行首。
 *
 * @param p      待检查的字符串, 必须位于行首
 * @param header 表头文本, 如 "[[index]]" 或 "[tool.uv]"
 *
 * @return 匹配时返回 true, 否则返回 false
 */
static bool
uvh_header_match (const char *p, const char *header)
{
  p = uvh_skip_indent (p);
  size_t hl = strlen (header);
  if (strncmp (p, header, hl) == 0)
    {
      char c = p[hl];
      return c == '\n' || c == '\r' || c == '\0' || c == ' ' || c == '\t' || c == '#';
    }
  if (strcmp (header, "[tool.uv]") == 0
      && (strncmp (p, "[tool.\"uv\"]", 11) == 0
          || strncmp (p, "[tool.'uv']", 11) == 0))
    {
      char c = p[11];
      return c == '\n' || c == '\r' || c == '\0' || c == ' ' || c == '\t' || c == '#';
    }
  return false;
}

/**
 * @brief 跳过当前行 (含换行符), 返回下一行的行首。
 *
 * 若当前行是最后一行 (无换行符), 返回指向字符串结尾 '\0' 的指针。
 *
 * @param p 当前行的行首
 *
 * @return 下一行的行首指针 (或字符串结尾)
 */
static const char *
uvh_next_line (const char *p)
{
  while (*p && *p != '\n') p++;
  if (*p == '\n') p++;
  return p;
}

/**
 * @brief 从 start 开始查找下一个段头行 (行首 '[') 的位置。
 *
 * @param start 搜索起点
 *
 * @return 下一个段头行的位置; 直到文件结尾都没有新段头时返回 NULL
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
 * @brief 全文件查找第一个行首匹配 header 的表头位置。
 *
 * @param content 文件内容
 * @param header  要查找的表头文本
 *
 * @return 匹配的表头行位置; 未找到返回 NULL
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
 * @brief 在 [first, end) 行区间内查找以 key 开头的键行。
 *
 * @param first 区间起点 (行首)
 * @param end   区间终点 (不含); 为 NULL 时搜索到文件结尾
 * @param key   键名
 *
 * @return 匹配的键行位置; 未找到返回 NULL
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
 * @brief 将字符串转义为 TOML basic string 的双引号内容。
 *
 * 对反斜杠、双引号与 \b \t \n \f \r 进行转义, 其余字符原样保留;
 * 返回值直接放入 "..." 中即为合法 TOML 字符串。
 *
 * @param value 原始字符串
 *
 * @return malloc 的 caller-free 转义后字符串
 *
 * @memory SAFE
 *   return caller-free
 */
static char *
uvh_escape_basic_string (const char *value)
{
  size_t n = 0;
  for (const char *p = value; *p; p++)
    {
      switch (*p)
        {
        case '\\': case '"': case '\b': case '\t': case '\n':
        case '\f': case '\r': n += 2; break;
        default: n++;
        }
    }

  char *ret = malloc (n + 1);
  size_t pos = 0;
  for (const char *p = value; *p; p++)
    {
      switch (*p)
        {
        case '\\': ret[pos++] = '\\'; ret[pos++] = '\\'; break;
        case '"':  ret[pos++] = '\\'; ret[pos++] = '"'; break;
        case '\b': ret[pos++] = '\\'; ret[pos++] = 'b'; break;
        case '\t': ret[pos++] = '\\'; ret[pos++] = 't'; break;
        case '\n': ret[pos++] = '\\'; ret[pos++] = 'n'; break;
        case '\f': ret[pos++] = '\\'; ret[pos++] = 'f'; break;
        case '\r': ret[pos++] = '\\'; ret[pos++] = 'r'; break;
        default: ret[pos++] = *p;
        }
    }
  ret[pos] = '\0';
  return ret;
}

/**
 * @brief 将单个十六进制字符转换为数值。
 *
 * @param c 十六进制字符 (0-9, a-f, A-F)
 *
 * @return 对应的数值 0-15; 不是合法十六进制字符时返回 -1
 */
static int
uvh_hex_digit (char c)
{
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

/**
 * @brief 将 Unicode code point 编码为 UTF-8 并追加到输出缓冲区。
 *
 * 编码后为 1~4 字节; 代理区 (U+D800-U+DFFF) 与超出 U+10FFFF 的
 * 无效 code point 返回 false 且不写入任何字节。
 *
 * @param out       输出缓冲区 (调用方保证足够容纳 4 字节)
 * @param pos       输出缓冲区的当前写入位置 (会递增)
 * @param codepoint Unicode code point
 *
 * @return 编码成功返回 true, 无效 code point 返回 false
 */
static bool
uvh_append_utf8 (char **out, size_t *pos, unsigned long codepoint)
{
  if (codepoint <= 0x7f)
    (*out)[(*pos)++] = (char)codepoint;
  else if (codepoint <= 0x7ff)
    {
      (*out)[(*pos)++] = (char)(0xc0 | (codepoint >> 6));
      (*out)[(*pos)++] = (char)(0x80 | (codepoint & 0x3f));
    }
  else if (codepoint <= 0xffff)
    {
      if (codepoint >= 0xd800 && codepoint <= 0xdfff) return false;
      (*out)[(*pos)++] = (char)(0xe0 | (codepoint >> 12));
      (*out)[(*pos)++] = (char)(0x80 | ((codepoint >> 6) & 0x3f));
      (*out)[(*pos)++] = (char)(0x80 | (codepoint & 0x3f));
    }
  else if (codepoint <= 0x10ffff)
    {
      (*out)[(*pos)++] = (char)(0xf0 | (codepoint >> 18));
      (*out)[(*pos)++] = (char)(0x80 | ((codepoint >> 12) & 0x3f));
      (*out)[(*pos)++] = (char)(0x80 | ((codepoint >> 6) & 0x3f));
      (*out)[(*pos)++] = (char)(0x80 | (codepoint & 0x3f));
    }
  else return false;
  return true;
}

/**
 * @brief 从 "key = \"value\"" 或 "key = 'value'" 行提取字符串值。
 *
 * 支持 TOML basic string 的常见转义 (\", \\, \b, \t, \n, \f, \r,
 * \uXXXX, \UXXXXXXXX); literal string (单引号) 不处理转义。
 * 未闭合字符串、非法转义或值不是字符串时返回 NULL。
 *
 * @param line 包含 "key = value" 的行
 *
 * @return malloc 的 caller-free 解码后字符串; 失败返回 NULL
 *
 * @memory SAFE
 *   return caller-free
 */
static char *
uvh_extract_string_value (const char *line)
{
  const char *eq = strchr (uvh_skip_indent (line), '=');
  if (!eq) return NULL;
  const char *v = eq + 1;
  while (*v == ' ' || *v == '\t') v++;
  if (*v != '"' && *v != '\'') return NULL;
  char quote = *v++;

  size_t max_len = strcspn (v, "\n") + 1;
  char *ret = malloc (max_len * 4);
  size_t pos = 0;
  bool closed = false;
  while (*v && *v != '\n')
    {
      if (*v == quote)
        {
          closed = true;
          break;
        }
      if (quote == '"' && *v == '\\')
        {
          v++;
          if (*v == 'u' || *v == 'U')
            {
              int digits = (*v == 'u') ? 4 : 8;
              unsigned long codepoint = 0;
              v++;
              for (int i = 0; i < digits; i++, v++)
                {
                  int digit = uvh_hex_digit (*v);
                  if (digit < 0) { free (ret); return NULL; }
                  codepoint = (codepoint << 4) | (unsigned)digit;
                }
              if (!uvh_append_utf8 (&ret, &pos, codepoint))
                { free (ret); return NULL; }
              continue;
            }
          switch (*v)
            {
            case '"': ret[pos++] = '"'; break;
            case '\\': ret[pos++] = '\\'; break;
            case 'b': ret[pos++] = '\b'; break;
            case 't': ret[pos++] = '\t'; break;
            case 'n': ret[pos++] = '\n'; break;
            case 'f': ret[pos++] = '\f'; break;
            case 'r': ret[pos++] = '\r'; break;
            default: free (ret); return NULL;
            }
          if (*v) v++;
          continue;
        }
      ret[pos++] = *v++;
    }
  if (!closed)
    {
      free (ret);
      return NULL;
    }
  ret[pos] = '\0';
  return ret;
}

/**
 * @brief 判断行中布尔值是否为 true。
 *
 * 仅当值为小写 true 且后随空白、注释、行尾或结尾时返回 true。
 *
 * @param line 包含 "key = value" 的行
 *
 * @return 值为 true 时返回 true, 否则返回 false
 */
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
 * @brief 查找要管理的 index 段。
 *
 * 多个 index 中优先选择 default = true 的段, 不存在 default 时回退到
 * 第一个; 段可以位于 [tool.uv.sources] 等子表之后。
 *
 * @param content      文件内容
 * @param index_header index 段头文本, 如 "[[index]]" 或 "[[tool.uv.index]]"
 *
 * @return 选中的 index 段头位置; 不存在 index 段时返回 NULL
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

/**
 * @brief 用 new_line_text 替换 content 中从 line 开始的整行。
 *
 * 保留原行缩进与后续内容; new_line_text 不含换行。中间改写统一使用 LF,
 * 行尾由调用方在最终写盘前按系统平台转换 (Windows 为 CRLF, 其余 LF)。
 *
 * @param content       原始内容
 * @param line          content 中待替换行的行首指针
 * @param new_line_text 新行文本 (不含换行)
 *
 * @return malloc 的 caller-free 新内容
 *
 * @memory SAFE
 *   return caller-free
 */
static char *
uvh_replace_line (const char *content, const char *line, const char *new_line_text)
{
  const char *line_end = strchr (line, '\n');
  if (!line_end) line_end = content + strlen (content);

  /* 复用原行缩进 */
  const char *indent_end = uvh_skip_indent (line);
  size_t ind = indent_end - line;

  size_t len = strlen (content) + ind + strlen (new_line_text) + 8;
  char *ret = calloc (len, 1);
  size_t pos = 0;
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)(line - content), content);
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)ind, line);
  pos += snprintf (ret + pos, len - pos, "%s", new_line_text);
  strcpy (ret + pos, line_end);
  return ret;
}

/**
 * @brief 在 insert_at 位置之前插入 insert_text (以 '\n' 结尾)。
 *
 * 中间改写统一使用 LF, 行尾由调用方在最终写盘前按系统平台转换;
 * insert_at 之前没有换行时会先补一个分隔换行。
 *
 * @param content     原始内容
 * @param insert_at   content 内的插入位置指针
 * @param insert_text 以 '\n' 结尾的待插入文本
 *
 * @return malloc 的 caller-free 新内容
 *
 * @memory SAFE
 *   return caller-free
 */
static char *
uvh_insert_before (const char *content, const char *insert_at, const char *insert_text)
{
  size_t head = insert_at - content;

  bool need_sep = (head > 0 && content[head - 1] != '\n');

  size_t text_len = strlen (insert_text);
  size_t len = strlen (content) + text_len + (need_sep ? 1 : 0) + 16;
  char *ret = calloc (len, 1);
  size_t pos = 0;
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)head, content);
  if (need_sep)
    {
      ret[pos++] = '\n';
    }
  memcpy (ret + pos, insert_text, text_len);
  pos += text_len;
  strcpy (ret + pos, content + head);
  return ret;
}

/**
 * @brief 以 LF 分隔，将完整 TOML 片段追加到文件末尾。
 *
 * @return malloc 的 caller-free 新内容
 */
static char *
uvh_append_segment (const char *content, const char *segment)
{
  return uvh_insert_before (content, content + strlen (content), segment);
}


/**
 * @brief 替换/创建 index 数组表 (如 [[index]] 或 [[tool.uv.index]]) 的 url 值。
 *
 * 优先修改 default = true 的 index (uv 的 PyPI 替代源), 不存在时回退到
 * 第一个; 作用域内无 index 段时新建, 父表不存在时一并创建。
 *
 * @param content      原始内容
 * @param url          新的索引 URL
 * @param index_header 段头文本, 如 "[[index]]" 或 "[[tool.uv.index]]"
 * @param parent_table 父表头, 如 NULL (顶层) 或 "[tool.uv]";
 *                     父表不存在时会在文件末尾创建父表并写入 index 段
 *
 * @return malloc 的 caller-free 新内容
 *
 * @memory SAFE
 *   return caller-free
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
           * 中间改写统一 LF, 行尾由最终写入前按系统平台转换 */
          char *escaped_url = uvh_escape_basic_string (url);
          size_t seg_len = strlen (parent_table) + strlen (index_header) + strlen (escaped_url) + 48;
          char *seg = calloc (seg_len, 1);
          snprintf (seg, seg_len, "%s\n%s\nurl = \"%s\"\ndefault = true\n",
                    parent_table, index_header, escaped_url);
          free (escaped_url);
          char *ret = uvh_append_segment (content, seg);
          free (seg);
          return ret;
        }
    }

  /* 作用域内无 index 段: 追加到边界之前 */
  if (!ih)
    {
      char *escaped_url = uvh_escape_basic_string (url);
      size_t seglen = strlen (index_header) + strlen (escaped_url) + 48;
      char *seg = malloc (seglen);
      snprintf (seg, seglen, "%s\nurl = \"%s\"\ndefault = true\n", index_header, escaped_url);
      free (escaped_url);
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
      char *escaped_url = uvh_escape_basic_string (url);
      size_t seglen = strlen (escaped_url) + (has_default ? 24 : 48);
      char *seg = malloc (seglen);
      size_t spos = 0;
      spos += snprintf (seg + spos, seglen - spos, "url = \"%s\"\n", escaped_url);
      if (!has_default)
        spos += snprintf (seg + spos, seglen - spos, "default = true\n");
      char *ret = uvh_insert_before (content, first, seg);
      free (seg);
      free (escaped_url);
      return ret;
    }

  /* 替换 url 行 */
  char *escaped_url = uvh_escape_basic_string (url);
  size_t newlen = strlen (escaped_url) + 32;
  char *new_line = malloc (newlen);
  snprintf (new_line, newlen, "url = \"%s\"", escaped_url);
  char *ret = uvh_replace_line (content, url_line, new_line);
  free (new_line);
  free (escaped_url);
  return ret;
}


/**
 * @brief 替换/创建顶层键或指定表内键 (如 python-install-mirror) 的字符串值。
 *
 * 键已存在时原地替换, 不存在时追加到作用域末尾; 父表不存在时一并创建。
 *
 * @param content      原始内容
 * @param key          键名
 * @param url          新值
 * @param parent_table 父表头, 如 NULL (顶层) 或 "[tool.uv]";
 *                     父表不存在时会在文件末尾创建父表并写入键
 *
 * @return malloc 的 caller-free 新内容
 *
 * @memory SAFE
 *   return caller-free
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
           * 中间改写统一 LF, 行尾由最终写入前按系统平台转换 */
          char *escaped_url = uvh_escape_basic_string (url);
          size_t seg_len = strlen (parent_table) + strlen (key) + strlen (escaped_url) + 16;
          char *seg = calloc (seg_len, 1);
          snprintf (seg, seg_len, "%s\n%s = \"%s\"\n", parent_table, key, escaped_url);
          free (escaped_url);
          char *ret = uvh_append_segment (content, seg);
          free (seg);
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
      char *escaped_url = uvh_escape_basic_string (url);
      size_t newlen = strlen (key) + strlen (escaped_url) + 32;
      char *new_line = malloc (newlen);
      snprintf (new_line, newlen, "%s = \"%s\"", key, escaped_url);
      char *ret = uvh_replace_line (content, old_line, new_line);
      free (new_line);
      free (escaped_url);
      return ret;
    }

  /* 键不存在: 追加到作用域末尾 */
  char *escaped_url = uvh_escape_basic_string (url);
  size_t seglen = strlen (key) + strlen (escaped_url) + 32;
  char *seg = malloc (seglen);
  snprintf (seg, seglen, "%s = \"%s\"\n", key, escaped_url);
  char *ret = uvh_insert_before (content, insert_at, seg);
  free (seg);
  free (escaped_url);
  return ret;
}


/**
 * @brief 在指定表 (parent_table 为 NULL 表示顶层) 内提取键的字符串值。
 *
 * 搜索范围限制在该表到下一个段头之间。
 *
 * @param content      文件内容
 * @param key          键名
 * @param parent_table 父表头, 如 NULL (顶层) 或 "[tool.uv]"
 *
 * @return malloc 的 caller-free 解码后字符串; 未找到返回 NULL
 *
 * @memory SAFE
 *   return caller-free
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
 * @brief 提取 chsrc 管理的 index URL。
 *
 * 优先 default = true 的 index, 否则取第一个; fully-qualified
 * [[tool.uv.index]] 会隐式创建父路径, 不要求显式 [tool.uv]。
 *
 * @param content      文件内容
 * @param index_header 段头文本, 如 "[[index]]" 或 "[[tool.uv.index]]"
 *
 * @return malloc 的 caller-free 解码后 URL; 未找到返回 NULL
 *
 * @memory SAFE
 *   return caller-free
 */
static char *
uvh_get_index_url (const char *content, const char *index_header)
{
  /* fully-qualified [[tool.uv.index]] 会隐式创建父路径，不要求显式 [tool.uv]。 */
  const char *ih = uvh_find_managed_index (content, index_header);
  if (!ih) return NULL;

  const char *first = uvh_next_line (ih);
  const char *end = uvh_find_section_end (first);
  const char *url_line = uvh_find_key_in_section (first, end, "url");
  if (!url_line) return NULL;
  return uvh_extract_string_value (url_line);
}
