/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * uv-helper.c (uv.toml 字符串改写工具) 单元测试
 *
 * 独立编译: 不依赖 xy.h / framework, 直接 #include uv-helper.c
 * ------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/recipe/lang/Python/uv-helper.c"

static int total = 0;
static int failures = 0;

#define CHECK(cond) \
  do { \
    total++; \
    if (!(cond)) \
      { \
        failures++; \
        printf ("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      } \
  } while (0)

#define CHECK_STR(got, expect) \
  do { \
    total++; \
    if (strcmp ((got), (expect)) != 0) \
      { \
        failures++; \
        printf ("FAIL %s:%d:\n  expected [%s]\n  got      [%s]\n", \
                __FILE__, __LINE__, (expect), (got)); \
      } \
  } while (0)


static void
test_replace_pypi_index_url (void)
{
  char *r;

  /* 空文件: 追加整个段 */
  r = replace_pypi_index_url ("", "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* 已有 [[index]] 与 url: 仅替换 url 行 */
  r = replace_pypi_index_url (
      "[[index]]\nurl = \"https://old\"\ndefault = true\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* default = false 保留 */
  r = replace_pypi_index_url (
      "[[index]]\nurl = \"https://old\"\ndefault = false\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\ndefault = false\n");
  free (r);

  /* [[index]] 段内没有 url 键: 在段内第一行之前插入 url + default */
  r = replace_pypi_index_url (
      "[[index]]\nname = \"tuna\"\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\ndefault = true\nname = \"tuna\"\n");
  free (r);

  /* 注释中的 [[index]] 不应被当作真实段: 追加新段 */
  r = replace_pypi_index_url (
      "# [[index]]\nfoo = 1\n",
      "https://mirror");
  CHECK_STR (r, "# [[index]]\nfoo = 1\n[[index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* 注释与真实段并存: 修改真实段 */
  r = replace_pypi_index_url (
      "# [[index]]\n[[index]]\nurl = \"https://old\"\n",
      "https://mirror");
  CHECK_STR (r, "# [[index]]\n[[index]]\nurl = \"https://mirror\"\n");
  free (r);

  /* 单引号字符串值也能被替换 (替换后统一为双引号) */
  r = replace_pypi_index_url (
      "[[index]]\nurl = 'https://old'\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\n");
  free (r);

  /* 多个 [[index]] 段: 优先修改 default = true 的 PyPI 替代源 */
  r = replace_pypi_index_url (
      "[[index]]\nurl = \"https://private\"\ndefault = false\n\n[[index]]\nurl = \"https://old\"\ndefault = true\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://private\"\ndefault = false\n\n[[index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* 段边界: [[index]] 之后其他表的 url 键不被误改 */
  r = replace_pypi_index_url (
      "[[index]]\nurl = \"https://old\"\n\n[python]\nurl = \"https://python\"\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\n\n[python]\nurl = \"https://python\"\n");
  free (r);
}


static void
test_replace_python_install_mirror (void)
{
  char *r;

  /* 空文件 */
  r = replace_python_install_mirror ("", "https://mirror");
  CHECK_STR (r, "python-install-mirror = \"https://mirror\"\n");
  free (r);

  /* 顶层已有: 就地替换 */
  r = replace_python_install_mirror (
      "python-install-mirror = \"https://old\"\nother = 1\n",
      "https://mirror");
  CHECK_STR (r, "python-install-mirror = \"https://mirror\"\nother = 1\n");
  free (r);

  /* 顶层键在段头之前: 替换后仍是顶层键 */
  r = replace_python_install_mirror (
      "python-install-mirror = \"https://old\"\n\n[python]\nx = 1\n",
      "https://mirror");
  CHECK_STR (r, "python-install-mirror = \"https://mirror\"\n\n[python]\nx = 1\n");
  free (r);

  /* 顶层无旧键但有段: 在第一个段头之前插入, 保证新键仍属顶层 */
  r = replace_python_install_mirror (
      "[python]\nx = 1\n",
      "https://mirror");
  CHECK_STR (r, "python-install-mirror = \"https://mirror\"\n[python]\nx = 1\n");
  free (r);

  /* 段内同名键不受影响 (chsrc 只管理顶层键) */
  r = replace_python_install_mirror (
      "[python]\npython-install-mirror = \"https://old\"\n",
      "https://mirror");
  CHECK_STR (r, "python-install-mirror = \"https://mirror\"\n[python]\npython-install-mirror = \"https://old\"\n");
  free (r);

  /* 注释行不误删 */
  r = replace_python_install_mirror (
      "# python-install-mirror = \"comment\"\npython-install-mirror = \"https://old\"\n",
      "https://mirror");
  CHECK_STR (r, "# python-install-mirror = \"comment\"\npython-install-mirror = \"https://mirror\"\n");
  free (r);
}


static void
test_uvh_get_top_level_value (void)
{
  char *v;

  v = uvh_get_top_level_value (
      "python-install-mirror = \"https://x\"\n", "python-install-mirror");
  CHECK (v != NULL);
  if (v) { CHECK_STR (v, "https://x"); free (v); }

  /* 单引号 */
  v = uvh_get_top_level_value (
      "python-install-mirror = 'https://x'\n", "python-install-mirror");
  CHECK (v != NULL);
  if (v) { CHECK_STR (v, "https://x"); free (v); }

  /* 等号周围无空格 */
  v = uvh_get_top_level_value (
      "python-install-mirror=\"https://x\"\n", "python-install-mirror");
  CHECK (v != NULL);
  if (v) { CHECK_STR (v, "https://x"); free (v); }

  /* 键位于段内 (非顶层) 时返回 NULL */
  v = uvh_get_top_level_value (
      "[python]\npython-install-mirror = \"https://x\"\n", "python-install-mirror");
  CHECK (v == NULL);

  /* 键不存在时返回 NULL */
  v = uvh_get_top_level_value ("other = 1\n", "python-install-mirror");
  CHECK (v == NULL);

  /* 注释行中的键不匹配 */
  v = uvh_get_top_level_value (
      "# python-install-mirror = \"comment\"\n", "python-install-mirror");
  CHECK (v == NULL);
}


static void
test_replace_index_url_pyproject (void)
{
  char *r;

  /* 空文件: 创建 [tool.uv] 表 + [[tool.uv.index]] 段 */
  r = replace_index_url ("", "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\n[[tool.uv.index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* 已有 [tool.uv] 无 index 段: 表内追加 (在下一个段头前) */
  r = replace_index_url (
      "[tool.uv]\nsome = 1\n[tool.poetry]\nx = 1\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\nsome = 1\n[tool.poetry]\nx = 1\n[[tool.uv.index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* 已有 [tool.uv] + [[tool.uv.index]]: 替换 url */
  r = replace_index_url (
      "[tool.uv]\n[[tool.uv.index]]\nurl = \"https://old\"\ndefault = true\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\n[[tool.uv.index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);

  /* 有 [project] 等其他表时正确找到 [tool.uv] */
  r = replace_index_url (
      "[project]\nname = \"x\"\n\n[tool.uv]\n[[tool.uv.index]]\nurl = \"https://old\"\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r, "[project]\nname = \"x\"\n\n[tool.uv]\n[[tool.uv.index]]\nurl = \"https://mirror\"\n");
  free (r);

  /* [[tool.uv.index]] 段内无 url 键: 段内插入 url + default */
  r = replace_index_url (
      "[tool.uv]\n[[tool.uv.index]]\nname = \"mirror\"\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\n[[tool.uv.index]]\nurl = \"https://mirror\"\ndefault = true\nname = \"mirror\"\n");
  free (r);

  /* CRLF 表头与缩进键应被识别，不能产生 duplicate table/key */
  r = replace_index_url (
      "  [tool.uv]\r\n  python-install-mirror = \"https://old-python\"\r\n  [[tool.uv.index]]\r\n  url = \"https://old\"\r\n  default = true\r\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r,
      "  [tool.uv]\r\n  python-install-mirror = \"https://old-python\"\r\n  [[tool.uv.index]]\r\n  url = \"https://mirror\"\r\n  default = true\r\n");
  free (r);

  /* CRLF 文件中无 [tool.uv]: 新建段同样使用 CRLF */
  r = replace_index_url (
      "[project]\r\nname = \"x\"\r\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r,
      "[project]\r\nname = \"x\"\r\n\r\n[tool.uv]\r\n[[tool.uv.index]]\r\nurl = \"https://mirror\"\r\ndefault = true\r\n");
  free (r);

  /* [tool.uv.sources] 不会阻止后续 index 被找到 */
  r = replace_index_url (
      "[tool.uv]\n[tool.uv.sources]\nfoo = { path = \"foo\" }\n[[tool.uv.index]]\nurl = \"https://old\"\ndefault = true\n",
      "https://mirror", "[[tool.uv.index]]", "[tool.uv]");
  CHECK_STR (r,
      "[tool.uv]\n[tool.uv.sources]\nfoo = { path = \"foo\" }\n[[tool.uv.index]]\nurl = \"https://mirror\"\ndefault = true\n");
  free (r);
}


static void
test_replace_key_value_pyproject (void)
{
  char *r;

  /* 空文件: 创建 [tool.uv] 表 + 键 */
  r = replace_key_value ("", "python-install-mirror", "https://mirror", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\npython-install-mirror = \"https://mirror\"\n");
  free (r);

  /* [tool.uv] 内已有键: 替换 */
  r = replace_key_value (
      "[tool.uv]\npython-install-mirror = \"https://old\"\n",
      "python-install-mirror", "https://mirror", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\npython-install-mirror = \"https://mirror\"\n");
  free (r);

  /* [tool.uv] 内无该键但已有 [[tool.uv.index]] 子表: 键插在子表头前 */
  r = replace_key_value (
      "[tool.uv]\n[[tool.uv.index]]\nurl = \"https://pypi\"\n",
      "python-install-mirror", "https://mirror", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\npython-install-mirror = \"https://mirror\"\n[[tool.uv.index]]\nurl = \"https://pypi\"\n");
  free (r);

  /* 缩进键就地替换，不重复定义 */
  r = replace_key_value (
      "[tool.uv]\r\n  python-install-mirror = \"https://old\"\r\n",
      "python-install-mirror", "https://mirror", "[tool.uv]");
  CHECK_STR (r, "[tool.uv]\r\n  python-install-mirror = \"https://mirror\"\r\n");
  free (r);

  /* CRLF 文件中无 [tool.uv]: 新建表同样使用 CRLF */
  r = replace_key_value (
      "[project]\r\nname = \"x\"\r\n",
      "python-install-mirror", "https://mirror", "[tool.uv]");
  CHECK_STR (r,
      "[project]\r\nname = \"x\"\r\n\r\n[tool.uv]\r\npython-install-mirror = \"https://mirror\"\r\n");
  free (r);
}


static void
test_uvh_get_value_pyproject (void)
{
  char *v;

  /* 表内键提取 */
  v = uvh_get_value_in_table (
      "[tool.uv]\npython-install-mirror = \"https://x\"\n", "python-install-mirror", "[tool.uv]");
  CHECK (v != NULL);
  if (v) { CHECK_STR (v, "https://x"); free (v); }

  /* 其他表内同名键: 找不到 */
  v = uvh_get_value_in_table (
      "[tool.other]\npython-install-mirror = \"https://x\"\n", "python-install-mirror", "[tool.uv]");
  CHECK (v == NULL);

  /* index url 提取 */
  v = uvh_get_index_url (
      "[tool.uv]\n[[tool.uv.index]]\nurl = \"https://pypi\"\ndefault = true\n",
      "[[tool.uv.index]]", "[tool.uv]");
  CHECK (v != NULL);
  if (v) { CHECK_STR (v, "https://pypi"); free (v); }

  /* 无 index 段: NULL */
  v = uvh_get_index_url (
      "[tool.uv]\nsome = 1\n", "[[tool.uv.index]]", "[tool.uv]");
  CHECK (v == NULL);

  /* 表不存在: NULL */
  v = uvh_get_index_url ("", "[[tool.uv.index]]", "[tool.uv]");
  CHECK (v == NULL);
}


static void
test_uvh_edge_cases (void)
{
  char *r;

  r = replace_pypi_index_url ("", "https://example.test/a\\b\"c");
  CHECK_STR (r, "[[index]]\nurl = \"https://example.test/a\\\\b\\\"c\"\ndefault = true\n");
  free (r);

  r = replace_pypi_index_url (
      "[[index]]\n\"url\" = \"https://old\"\ndefault = true\n",
      "https://new");
  CHECK_STR (r, "[[index]]\nurl = \"https://new\"\ndefault = true\n");
  free (r);

  r = replace_key_value (
      "[tool.\"uv\"]\npython-install-mirror = \"https://old\"\n",
      "python-install-mirror", "https://new", "[tool.uv]");
  CHECK_STR (r, "[tool.\"uv\"]\npython-install-mirror = \"https://new\"\n");
  free (r);

  r = replace_key_value (
      "[tool.'uv']\n'python-install-mirror' = \"https://old\"\n",
      "python-install-mirror", "https://new", "[tool.uv]");
  CHECK_STR (r, "[tool.'uv']\npython-install-mirror = \"https://new\"\n");
  free (r);

  char *v = uvh_get_index_url (
      "[[index]]\nurl = \"https://example/\\u0073imple/\\U0001F600\"\ndefault = true\n",
      "[[index]]", NULL);
  CHECK (v != NULL);
  if (v) { CHECK_STR (v, "https://example/simple/\xF0\x9F\x98\x80"); free (v); }

  r = replace_pypi_index_url (
      "[[index]]\nurl = \"https://old\"", "https://new");
  CHECK_STR (r, "[[index]]\nurl = \"https://new\"");
  free (r);
}


int
main (void)
{
  test_replace_pypi_index_url ();
  test_replace_python_install_mirror ();
  test_uvh_get_top_level_value ();
  test_replace_index_url_pyproject ();
  test_replace_key_value_pyproject ();
  test_uvh_get_value_pyproject ();
  test_uvh_edge_cases ();

  if (failures)
    {
      printf ("uv-helper-test: %d/%d checks FAILED\n", failures, total);
      return 1;
    }

  printf ("uv-helper-test: all %d checks passed\n", total);
  return 0;
}
