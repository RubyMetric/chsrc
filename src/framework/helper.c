/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : helper.c
 * File Authors  : 曾奥然 <ccmywish@qq.com>
 * Contributors  : Nul None <nul@none.org>
 * Created On    : <2025-07-14>
 * Last Modified : <2025-10-28>
 *
 * For chefs (recipe makers) and sometimes framewoker
 * to do some work not releated to OS operations
 * ------------------------------------------------------------*/

bool
hp_is_url (const char *str)
{
  return (xy_str_start_with (str, "http://") || xy_str_start_with (str, "https://"));
}

/**
 * @return 一律返回新字符串
 */
char *
hp_remove_trailing_slash (char *str)
{
  char *newstr = xy_strdup (str);
  size_t len = strlen (newstr);
  if (len > 0 && newstr[len - 1] == '/')
    newstr[len - 1] = '\0';
  return newstr;
}

/**
 * @return 一律返回新字符串
 */
char *
hp_ensure_trailing_slash (char *str)
{
  size_t len = strlen (str);
  if (len == 0 || str[len - 1] == '/')
    return xy_strdup (str);

  return xy_2strcat (str, "/");
}
