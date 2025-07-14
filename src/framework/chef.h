/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : chef.h
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Peng Gao  <gn3po4g@outlook.com>
 * Created On    : <2025-07-14>
 * Last Modified : <2025-07-14>
 *
 * For chefs (recipe makers) and sometimes framewoker
 * ------------------------------------------------------------*/

#pragma once


bool
is_url (const char *str)
{
  return (xy_str_start_with (str, "http://") || xy_str_start_with (str, "https://"));
}
