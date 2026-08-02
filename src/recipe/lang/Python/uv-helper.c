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
 * uv.toml 配置字符串改写工具 (由 uv.c #include, 可被 test 单独 #include)
 *
 * 仅处理 chsrc 关心的 uv 配置子集, 不是完整 TOML 解析器:
 *   - [[index]] 数组表: 替换/清理其 url、default 行 (保留段头与其他键)
 *   - 顶层 python-install-mirror 键: 替换/删除
 *
 * 刻意不处理: 多行字符串、数组/内联表值、字符串转义 ——
 * 因为所有改写都按整行“替换/删除”进行, 无需解析值本身。
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

/**
 * 判断行首是否为 [[index]] 数组表头 (允许行尾注释, 如 "[[index]] # note")。
 */
static bool
uvh_is_index_header (const char *p)
{
  if (strncmp (p, "[[index]]", 9) != 0) return false;
  char c = p[9];
  return c == '\n' || c == '\0' || c == ' ' || c == '\t';
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
 * 从 start (段内第一行) 开始找下一个段头行 (行首 '[') 的位置。
 * 返回 NULL 表示直到文件结尾都没有新段头。
 */
static const char *
uvh_find_section_end (const char *start)
{
  const char *p = start;
  while (*p)
    {
      if (*p == '[') return p;
      p = uvh_next_line (p);
    }
  return NULL;
}


/**
 * 在 content 中找到第一个 [[index]] 段, 替换其 url = "..." 行 (或单引号值)。
 * 文件中没有 [[index]] 段时, 追加整个段到末尾。
 *
 * 只修改第一个 [[index]] 段; 段内 url/default 键的语义由调用者 (set) 决定。
 *
 * @return 新内容 (caller-free)
 */
static char *
replace_pypi_index_url (const char *content, const char *url)
{
  /* 找第一个行首 [[index]] 段头 (跳过注释等非段头匹配) */
  const char *p = content;
  const char *index_header = NULL;
  while (*p)
    {
      if (*p == '[' && uvh_is_index_header (p)) { index_header = p; break; }
      p = uvh_next_line (p);
    }

  /* 文件中尚无 [[index]] 段, 追加到末尾 */
  if (!index_header)
    {
      size_t len = strlen (content) + strlen (url) + 96;
      char *ret = calloc (len, 1);
      size_t clen = strlen (content);
      if (clen > 0 && content[clen - 1] == '\n')
        snprintf (ret, len, "%s[[index]]\nurl = \"%s\"\ndefault = true\n", content, url);
      else if (clen > 0)
        snprintf (ret, len, "%s\n[[index]]\nurl = \"%s\"\ndefault = true\n", content, url);
      else
        snprintf (ret, len, "[[index]]\nurl = \"%s\"\ndefault = true\n", url);
      return ret;
    }

  /* 段内第一行与段边界 (下一个段头或文件尾) */
  const char *first = uvh_next_line (index_header);
  const char *end = uvh_find_section_end (first);

  /* 在当前 [[index]] 段内找 url 键行 */
  const char *url_line = NULL;
  for (const char *s = first; *s && (!end || s < end); s = uvh_next_line (s))
    {
      if (uvh_key_prefix (s, "url")) { url_line = s; break; }
    }

  /* 段内没有 url 键: 在段内第一行之前插入 url 行 (及缺失的 default 行) */
  if (!url_line)
    {
      bool has_default = false;
      for (const char *s = first; *s && (!end || s < end); s = uvh_next_line (s))
        {
          if (uvh_key_prefix (s, "default")) { has_default = true; break; }
        }

      size_t len = strlen (content) + strlen (url) + 64;
      char *ret = calloc (len, 1);
      size_t pos = 0;
      pos += snprintf (ret + pos, len - pos, "%.*s", (int)(first - content), content);
      pos += snprintf (ret + pos, len - pos, "url = \"%s\"\n", url);
      if (!has_default)
        pos += snprintf (ret + pos, len - pos, "default = true\n");
      strcpy (ret + pos, first);
      return ret;
    }

  /* 替换 url 行, 保留该行原有的换行符与后续内容 */
  const char *line_end = strchr (url_line, '\n');
  if (!line_end) line_end = content + strlen (content);

  size_t len = strlen (content) + strlen (url) + 32;
  char *ret = calloc (len, 1);
  size_t pos = 0;
  pos += snprintf (ret + pos, len - pos, "%.*s", (int)(url_line - content), content);
  pos += snprintf (ret + pos, len - pos, "url = \"%s\"", url);
  strcpy (ret + pos, line_end);
  return ret;
}


/**
 * 替换顶层 python-install-mirror 键的值; 顶层无该键时, 追加到顶层区域末尾
 * (第一个段头之前), 保证新键仍是顶层键而非被归入某个表。
 *
 * 段内 (非顶层) 的同名键不受影响 —— chsrc 只管理顶层键。
 *
 * @return 新内容 (caller-free)
 */
static char *
replace_python_install_mirror (const char *content, const char *url)
{
  /* 只扫描顶层区域 (第一个段头之前) */
  const char *p = content;
  const char *old_line = NULL;
  const char *first_section = NULL;
  while (*p)
    {
      if (*p == '[')
        {
          first_section = p;
          break;
        }
      if (uvh_key_prefix (p, "python-install-mirror")) old_line = p;
      p = uvh_next_line (p);
    }

  size_t base = strlen (content) + strlen (url) + 64;

  /* 顶层已有旧行: 就地替换 */
  if (old_line)
    {
      const char *line_end = strchr (old_line, '\n');
      if (!line_end) line_end = content + strlen (content);

      char *ret = calloc (base, 1);
      size_t pos = 0;
      pos += snprintf (ret + pos, base - pos, "%.*s", (int)(old_line - content), content);
      pos += snprintf (ret + pos, base - pos, "python-install-mirror = \"%s\"", url);
      strcpy (ret + pos, line_end);
      return ret;
    }

  /* 顶层无旧行: 追加到顶层区域末尾 (无段头则文件末尾) */
  const char *insert_at = first_section ? first_section : content + strlen (content);
  size_t head = insert_at - content;

  char *ret = calloc (base, 1);
  size_t pos = 0;
  pos += snprintf (ret + pos, base - pos, "%.*s", (int)head, content);
  if (head > 0 && content[head - 1] != '\n') ret[pos++] = '\n';
  pos += snprintf (ret + pos, base - pos, "python-install-mirror = \"%s\"\n", url);
  strcpy (ret + pos, content + head);
  return ret;
}


/**
 * reset 专用: 删除每个 [[index]] 段内的 url/default 行 (保留段头与
 * name 等其余字段), 删除顶层 python-install-mirror 行, 其余内容原样保留。
 *
 * 段边界严格限定: 其他表 (如 [python]) 内同名的 url/default 键不受影响。
 *
 * @return 新内容 (caller-free)
 */
static char *
cleanup_config_for_reset (const char *content)
{
  size_t est = strlen (content) + 128;
  char *ret = calloc (est, 1);
  size_t pos = 0;

  bool in_table = false;  /* 是否已进入任何表 (用于限定 python-install-mirror 仅顶层) */
  bool in_index = false;  /* 当前是否处于 [[index]] 段 (用于限定 url/default) */

  const char *p = content;
  while (*p)
    {
      bool skip = false;

      if (*p == '[')
        {
          in_table = true;
          in_index = uvh_is_index_header (p);
        }
      else if (in_index && (uvh_key_prefix (p, "url") || uvh_key_prefix (p, "default")))
        {
          skip = true;
        }
      else if (!in_table && uvh_key_prefix (p, "python-install-mirror"))
        {
          skip = true;
        }

      if (skip)
        {
          while (*p && *p != '\n') p++;
          if (*p == '\n') p++;
        }
      else
        {
          while (*p && *p != '\n') ret[pos++] = *p++;
          if (*p == '\n') ret[pos++] = *p++;
        }
    }

  ret[pos] = '\0';
  return ret;
}


/**
 * 提取顶层键的字符串值 (双引号或单引号), 返回 malloc 的 caller-free 字符串。
 * 键不存在、值不是字符串、或键位于段内 (非顶层) 时返回 NULL。
 */
static char *
uvh_get_top_level_value (const char *content, const char *key)
{
  const char *p = content;
  bool in_table = false;
  while (*p)
    {
      if (*p == '[')
        {
          in_table = true;
        }
      else if (!in_table && uvh_key_prefix (p, key))
        {
          const char *v = p + strlen (key);
          while (*v == ' ' || *v == '\t') v++;
          if (*v != '=') return NULL;
          v++;
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
      p = uvh_next_line (p);
    }
  return NULL;
}
