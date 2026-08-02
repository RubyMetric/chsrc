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

  /* 多个 [[index]] 段: 只修改第一个 */
  r = replace_pypi_index_url (
      "[[index]]\nurl = \"https://old1\"\n\n[[index]]\nurl = \"https://old2\"\n",
      "https://mirror");
  CHECK_STR (r, "[[index]]\nurl = \"https://mirror\"\n\n[[index]]\nurl = \"https://old2\"\n");
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
test_cleanup_config_for_reset (void)
{
  char *r;

  /* 空内容 */
  r = cleanup_config_for_reset ("");
  CHECK_STR (r, "");
  free (r);

  /* 只清理 [[index]] 的 url/default 与顶层 python-install-mirror;
   * 其他表 ([python]) 内的 url 键必须保留 */
  r = cleanup_config_for_reset (
      "python-install-mirror = \"https://old\"\n"
      "[[index]]\n"
      "name = \"tuna\"\n"
      "url = \"https://old\"\n"
      "default = true\n"
      "\n"
      "[python]\n"
      "url = \"https://python\"\n");
  CHECK_STR (r,
      "[[index]]\n"
      "name = \"tuna\"\n"
      "\n"
      "[python]\n"
      "url = \"https://python\"\n");
  free (r);

  /* 顶层其他键保留 */
  r = cleanup_config_for_reset (
      "a = 1\npython-install-mirror = \"https://old\"\nb = 2\n");
  CHECK_STR (r, "a = 1\nb = 2\n");
  free (r);

  /* 注释行保留 (注释中的 url 键不误删) */
  r = cleanup_config_for_reset (
      "# url = \"comment\"\n[[index]]\nurl = \"real\"\n");
  CHECK_STR (r, "# url = \"comment\"\n[[index]]\n");
  free (r);

  /* 多个 [[index]] 段都清理 */
  r = cleanup_config_for_reset (
      "[[index]]\nurl = \"https://old1\"\n\n[[index]]\nurl = \"https://old2\"\n");
  CHECK_STR (r, "[[index]]\n\n[[index]]\n");
  free (r);

  /* 其他表内的 default 键保留 */
  r = cleanup_config_for_reset (
      "[python]\ndefault = \"x\"\n");
  CHECK_STR (r, "[python]\ndefault = \"x\"\n");
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


int
main (void)
{
  test_replace_pypi_index_url ();
  test_replace_python_install_mirror ();
  test_cleanup_config_for_reset ();
  test_uvh_get_top_level_value ();

  if (failures)
    {
      printf ("uv-helper-test: %d/%d checks FAILED\n", failures, total);
      return 1;
    }

  printf ("uv-helper-test: all %d checks passed\n", total);
  return 0;
}
