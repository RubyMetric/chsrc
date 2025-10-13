/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target (pl_python_uv, "uv");

void
pl_python_uv_prelude (void)
{
  chef_prep_this (pl_python_uv, gsr);

  chef_set_created_on   (this, "2024-12-11");
  chef_set_last_updated (this, "2025-10-11");
  chef_set_sources_last_updated (this, "2025-10-11");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@Mikachu2333", "@happy-game");
  chef_set_sauciers (this, 2, "@Kattos", "@ccmywish");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english (this);
  chef_allow_user_define (this);

  chef_use_other_target_sources (this, &pl_python_group_target);
}


/**
 * chsrc get uv
 *
 * uv的配置优先级顺序如下(高到低):
 * 1. $workspaces/uv.toml
 * 2. $workspaces/pyproject.toml
 * 3. ~/.config/uv/uv.toml
 * 4. /etc/uv/uv.toml
 */

#define PL_Python_uv_Proj_uv             "uv.toml"
#define PL_Python_uv_Proj_pyproj         "pyproject.toml"
#ifdef XY_Build_On_Windows
  #define PL_Python_uv_Local_ConfigPath  "\\uv\\uv.toml"
  #define PL_Python_uv_User_ConfigPath   "\\uv\\uv.toml"
#else
  #define PL_Python_uv_Local_ConfigPath  "~/.config/uv/uv.toml"
  #define PL_Python_uv_User_ConfigPath   "/etc/uv/uv.toml"
#endif

typedef struct
{
  bool is_empty;
  char *url;
  char *file_path;
}
PlPythonUvConfig;

PlPythonUvConfig
pl_python_uv_config_define_python (char *path)
{
  PlPythonUvConfig config = {true, NULL, NULL};

  char *content = xy_file_read (path);
  if (!content || strlen (content) == 0)
    return config;

  char *formatted = xy_str_gsub (content, " ", "");
  formatted = xy_str_gsub (formatted, "'", "\"");

  XyStrFindResult_t result = xy_str_find (formatted, "python-install-mirror");
  if (result.found)
    {
      char *url = xy_str_next_nonempty_line (formatted + result.end + 1);
      url = xy_str_gsub (url, "\"", "");
      url = xy_str_delete_prefix (url, "=");
      config.url = url;
      config.file_path = xy_strdup (path);
      config.is_empty = !config.url;
    }
  return config;
}

PlPythonUvConfig
pl_python_uv_config_define_pypi (char *path)
{
  PlPythonUvConfig config = {true, NULL, NULL};

  char *content = xy_file_read (path);
  if (!content || strlen (content) == 0)
    return config;

  char *formatted = xy_str_gsub (content, " ", "");
  formatted = xy_str_gsub (formatted, "'", "\"");

  XyStrFindResult_t old_sytle_result = xy_str_find (formatted, "[[index]]");
  XyStrFindResult_t new_sytle_result = xy_str_find (formatted, "index-url");
  if (old_sytle_result.found && new_sytle_result.found)
    {
      chsrc_error2 (path);
      char *msg = ENGLISH ? "UV config conflicted with old sytle and new style."
                          : "UV 配置文件因同时存在新旧两种格式而冲突，请自行检查。";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }

  if (old_sytle_result.found)
    {
      char *current_pos = formatted + old_sytle_result.end + 1;
      char *url = NULL;

      /* 循环读取下一行，直到找到包含 "url" 的行 */
      while (*current_pos != '\0')
        {
          char *line = xy_str_next_nonempty_line (current_pos);
          if (line && xy_str_find (line, "url").found)
            {
              url = line;
              break;
            }

          /* 移动到下一行的位置 */
          current_pos = strchr (current_pos, '\n');
          if (!current_pos)
            break;
          current_pos++;
        }

      if (url)
        {
          url = xy_str_gsub (url, "\"", "");
          url = xy_str_delete_prefix (url, "url=");
          config.url = url;
          config.file_path = xy_strdup (path);
          config.is_empty = !config.url;
        }
    }
  else
    {
      char *url = xy_str_next_nonempty_line (formatted + new_sytle_result.end + 1);
      url = xy_str_gsub (url, "\"", "");
      url = xy_str_delete_prefix (url, "=");
      config.url = url;
      config.file_path = xy_strdup (path);
      config.is_empty = !config.url;
    }
  return config;
}

typedef struct
{
  bool  pypi;
  char *pypi_url;
  char *pypi_path;
  bool  python;
  char *python_url;
  char *python_path;
}
PlPythonUvConfigSummary;

bool
pl_python_uv_config_summary_struct_is_full (PlPythonUvConfigSummary *config)
{
  return config->pypi == true &&
         config->python == true;
}

/**
 * @brief 按配置文件优先级依次查找镜像配置
 *
 * @return 返回优先级最高的有效配置，其余配置将被忽略。可能为空。
 *
 * @note 特殊情况（Local mode）时直接返回项目一级的配置，无论是否为空
 */
PlPythonUvConfigSummary
pl_python_uv_config_find ()
{
  PlPythonUvConfigSummary result = {false, NULL, NULL, false, NULL, NULL};

  // 第一优先级
  PlPythonUvConfig result1 = pl_python_uv_config_define_python (PL_Python_uv_Proj_uv);
  if (!result1.is_empty)
    {
      result.python = true;
      result.python_url = result1.url;
      result.python_path = result1.file_path;
    }

  PlPythonUvConfig result2 = pl_python_uv_config_define_pypi (PL_Python_uv_Proj_uv);
  if (!result2.is_empty)
    {
      result.pypi = true;
      result.pypi_url = result2.url;
      result.pypi_path = result2.file_path;
    }
  if (pl_python_uv_config_summary_struct_is_full (&result) || chsrc_in_local_mode ()) return result;

  PlPythonUvConfig result3 = pl_python_uv_config_define_python (PL_Python_uv_Proj_pyproj);
  if (!result3.is_empty)
    {
      result.python = true;
      result.python_url = result3.url;
      result.python_path = result3.file_path;
    }
  if (pl_python_uv_config_summary_struct_is_full (&result)) return result;

  PlPythonUvConfig result4 = pl_python_uv_config_define_pypi (PL_Python_uv_Proj_pyproj);
  if (!result4.is_empty)
    {
      result.pypi = true;
      result.pypi_url = result4.url;
      result.pypi_path = result4.file_path;
    }
  if (pl_python_uv_config_summary_struct_is_full (&result)) return result;

  // 第二优先级
  char *uv_local_config_path = PL_Python_uv_Local_ConfigPath;
  if (xy.on_windows)
    {
      uv_local_config_path = xy_2strcat (getenv ("APPDATA"), uv_local_config_path);
    }

  PlPythonUvConfig result5 = pl_python_uv_config_define_python (uv_local_config_path);
  if (!result5.is_empty)
    {
      result.python = true;
      result.python_path = result5.file_path;
      result.python_url = result5.url;
    }
  if (pl_python_uv_config_summary_struct_is_full (&result)) return result;

  PlPythonUvConfig result6 = pl_python_uv_config_define_pypi (uv_local_config_path);
  if (!result6.is_empty)
    {
      result.pypi = true;
      result.pypi_path = result6.file_path;
      result.pypi_url = result6.url;
    }
  if (pl_python_uv_config_summary_struct_is_full (&result)) return result;

  // 第三优先级
  char *uv_user_config_path = PL_Python_uv_User_ConfigPath;
  if (xy.on_windows)
    {
      uv_user_config_path = xy_2strcat (getenv ("APPDATA"), uv_user_config_path);
    }
  PlPythonUvConfig result7 = pl_python_uv_config_define_python (uv_user_config_path);
  if (!result7.is_empty)
    {
      result.python = true;
      result.python_path = result7.file_path;
      result.python_url = result7.url;
    }
  if (pl_python_uv_config_summary_struct_is_full (&result)) return result;

  PlPythonUvConfig result8 = pl_python_uv_config_define_pypi (uv_user_config_path);
  if (!result8.is_empty)
    {
      result.pypi = true;
      result.pypi_path = result8.file_path;
      result.pypi_url = result8.url;
    }

  return result;
}

void
pl_python_uv_getsrc (char *option)
{
  PlPythonUvConfigSummary uv_config = pl_python_uv_config_find ();

  if (uv_config.pypi)
    {
      chsrc_note2 ("uv pypi:");
      say (xy_2strcat (ENGLISH ? "Config Path: " : "文件路径：", uv_config.pypi_path));
      say (uv_config.pypi_url);
    }
  else
    {
      char *msg = ENGLISH ? "Can't find uv config for pypi, show default."
                          : "未找到 uv 关于 pypi 的配置文件，显示默认值";
      chsrc_note2 (msg);
      say ("https://pypi.org/simple");
    }

  if (uv_config.python)
    {
      chsrc_note2 ("uv Python:");
      say (xy_2strcat (ENGLISH ? "Config Path: " : "文件路径：", uv_config.python_path));
      say (uv_config.python_url);
    }
  else
    {
      char *msg = ENGLISH ? "Can't find uv config for Python, show default."
                          : "未找到 uv 关于 Python 的配置文件，显示默认值";
      chsrc_note2 (msg);
      say ("https://github.com/astral-sh/python-build-standalone/releases/download");
    }
}


static bool
pl_python_uv_path_is_pyproject (const char *path)
{
  if (!path)
    return false;
  return xy_str_end_with (path, "pyproject.toml");
}


static char *
pl_python_uv_replace_first_literal (const char *content, const char *old_literal, const char *new_literal)
{
  if (!content || !old_literal || !new_literal)
    return NULL;

  char *position = strstr (content, old_literal);
  if (!position)
    return NULL;

  size_t prefix_len = position - content;
  size_t old_len = strlen (old_literal);
  size_t suffix_len = strlen (position + old_len);

  size_t result_len = prefix_len + strlen (new_literal) + suffix_len;
  char *result = xy_malloc0 (result_len + 1);
  memcpy (result, content, prefix_len);
  memcpy (result + prefix_len, new_literal, strlen (new_literal));
  memcpy (result + prefix_len + strlen (new_literal), position + old_len, suffix_len);
  result[result_len] = '\0';

  return result;
}


static char *
pl_python_uv_replace_key_value (const char *content, const char *key, const char *new_value)
{
  if (!content || !key || !new_value)
    return NULL;

  char *key_pos = strstr (content, key);
  if (!key_pos)
    return NULL;

  char *line_end = strchr (key_pos, '\n');
  if (!line_end)
    line_end = (char *) content + strlen (content);

  char *equal_pos = strchr (key_pos, '=');
  if (!equal_pos || equal_pos > line_end)
    return NULL;

  char *value_begin = equal_pos + 1;
  while (value_begin < line_end && (*value_begin == ' ' || *value_begin == '\t'))
    value_begin++;

  char quote = '\0';
  if (value_begin < line_end && (*value_begin == '"' || *value_begin == '\''))
    {
      quote = *value_begin;
      value_begin++;
    }

  char *value_end = value_begin;
  while (value_end < line_end)
    {
      if (quote)
        {
          if (*value_end == quote)
            break;
        }
      else if (*value_end == ' ' || *value_end == '\t' || *value_end == '#')
        {
          break;
        }
      value_end++;
    }

  char *after_value = value_end;
  if (quote && after_value < line_end && *after_value == quote)
    after_value++;

  size_t prefix_len = value_begin - content;
  size_t suffix_len = strlen (after_value);
  size_t quote_len = quote ? 1 : 0;
  size_t new_len = prefix_len + quote_len + strlen (new_value) + quote_len + suffix_len;

  char *result = xy_malloc0 (new_len + 1);
  memcpy (result, content, prefix_len);

  char *cursor = result + prefix_len;
  if (quote)
    {
      *cursor = quote;
      cursor++;
    }
  memcpy (cursor, new_value, strlen (new_value));
  cursor += strlen (new_value);
  if (quote)
    {
      *cursor = quote;
      cursor++;
    }

  memcpy (cursor, after_value, suffix_len);
  result[new_len] = '\0';

  return result;
}


static char *
pl_python_uv_append_line (const char *content, const char *line_with_newline)
{
  if (!content)
    content = "";
  if (!line_with_newline)
    return NULL;

  size_t len = strlen (content);
  bool need_line_break = len > 0 && content[len - 1] != '\n';

  if (need_line_break)
    {
      char *tmp = xy_strcat (2, content, "\n");
      char *result = xy_strcat (2, tmp, line_with_newline);
      free (tmp);
      return result;
    }

  return xy_strcat (2, content, line_with_newline);
}


static char *
pl_python_uv_insert_into_section (const char *content, const char *section_header, const char *line_with_newline)
{
  if (!content || !section_header || !line_with_newline)
    return NULL;

  char *section_pos = strstr (content, section_header);
  if (!section_pos)
    return NULL;

  char *after_header = strchr (section_pos, '\n');
  if (!after_header)
    {
      char *line_block = xy_strcat (2, "\n", line_with_newline);
      char *result = xy_strcat (2, content, line_block);
      free (line_block);
      return result;
    }

  after_header++;
  char *scan = after_header;
  while (*scan != '\0')
    {
      if (*scan == '\n' && scan[1] == '[')
        break;
      scan++;
    }

  char *insert_pos = scan;
  size_t prefix_len = insert_pos - content;
  size_t suffix_len = strlen (insert_pos);

  bool need_leading_newline = prefix_len > 0 && content[prefix_len - 1] != '\n';
  char *line_block = need_leading_newline ? xy_strcat (2, "\n", line_with_newline)
                                          : xy_strdup (line_with_newline);

  size_t block_len = strlen (line_block);
  char *result = xy_malloc0 (prefix_len + block_len + suffix_len + 1);
  memcpy (result, content, prefix_len);
  memcpy (result + prefix_len, line_block, block_len);
  memcpy (result + prefix_len + block_len, insert_pos, suffix_len);
  result[prefix_len + block_len + suffix_len] = '\0';

  free (line_block);
  return result;
}


static char *
pl_python_uv_dirname (const char *path)
{
  if (!path)
    return NULL;

  const char *slash1 = strrchr (path, '/');
  const char *slash2 = strrchr (path, '\\');
  const char *slash = slash1 > slash2 ? slash1 : slash2;
  if (!slash)
    return NULL;

  size_t len = slash - path + 1;
  char *dir = xy_malloc0 (len + 1);
  memcpy (dir, path, len);
  dir[len] = '\0';
  return dir;
}


static char *
pl_python_uv_dup_normalized (const char *path)
{
  if (!path)
    return NULL;

  char *dup = xy_strdup (path);
  return xy_normalize_path (dup);
}


static void
pl_python_uv_upsert_python (char **content,
                            const char *existing_url,
                            const char *new_url,
                            bool is_pyproject)
{
  bool updated = false;

  if (existing_url)
    {
      char *old_literal = xy_strcat (3, "\"", existing_url, "\"");
      char *new_literal = xy_strcat (3, "\"", new_url, "\"");
      char *replaced = pl_python_uv_replace_first_literal (*content, old_literal, new_literal);
      free (old_literal);
      free (new_literal);
      if (replaced)
        {
          free (*content);
          *content = replaced;
          updated = true;
        }
    }

  if (!updated)
    {
      char *replaced = pl_python_uv_replace_key_value (*content, "python-install-mirror", new_url);
      if (replaced)
        {
          free (*content);
          *content = replaced;
          updated = true;
        }
    }

  if (!updated)
    {
      char *line = xy_strcat (4, "python-install-mirror = \"", new_url, "\"", "\n");

      if (is_pyproject)
        {
          if (xy_str_find (*content, "[tool.uv]").found)
            {
              char *inserted = pl_python_uv_insert_into_section (*content, "[tool.uv]", line);
              if (inserted)
                {
                  free (*content);
                  *content = inserted;
                  updated = true;
                }
            }

          if (!updated)
            {
              char *block = xy_strcat (3, "\n[tool.uv]\n", line, "");
              char *appended = pl_python_uv_append_line (*content, block);
              free (block);
              if (appended)
                {
                  free (*content);
                  *content = appended;
                  updated = true;
                }
            }
        }
      else
        {
          char *appended = pl_python_uv_append_line (*content, line);
          if (appended)
            {
              free (*content);
              *content = appended;
              updated = true;
            }
        }

      free (line);
    }
}


static void
pl_python_uv_upsert_pypi (char **content,
                          const char *existing_url,
                          const char *new_url,
                          bool is_pyproject)
{
  bool updated = false;

  if (existing_url)
    {
      char *old_literal = xy_strcat (3, "\"", existing_url, "\"");
      char *new_literal = xy_strcat (3, "\"", new_url, "\"");
      char *replaced = pl_python_uv_replace_first_literal (*content, old_literal, new_literal);
      free (old_literal);
      free (new_literal);
      if (replaced)
        {
          free (*content);
          *content = replaced;
          updated = true;
        }
    }

  if (!updated)
    {
      char *replaced = pl_python_uv_replace_key_value (*content, "index-url", new_url);
      if (replaced)
        {
          free (*content);
          *content = replaced;
          updated = true;
        }
    }

  if (!updated)
    {
      if (xy_str_find (*content, "[tool.uv.pip]").found)
        {
          char *line = xy_strcat (4, "index-url = \"", new_url, "\"", "\n");
          char *inserted = pl_python_uv_insert_into_section (*content, "[tool.uv.pip]", line);
          free (line);
          if (inserted)
            {
              free (*content);
              *content = inserted;
              updated = true;
            }
        }

      if (!updated && xy_str_find (*content, "[pip]").found)
        {
          char *line = xy_strcat (4, "index-url = \"", new_url, "\"", "\n");
          char *inserted = pl_python_uv_insert_into_section (*content, "[pip]", line);
          free (line);
          if (inserted)
            {
              free (*content);
              *content = inserted;
              updated = true;
            }
        }

      if (!updated && xy_str_find (*content, "[[index]]").found)
        {
          char *line = xy_strcat (4, "url = \"", new_url, "\"", "\n");
          char *inserted = pl_python_uv_insert_into_section (*content, "[[index]]", line);
          free (line);
          if (inserted)
            {
              free (*content);
              *content = inserted;
              updated = true;
            }
        }
    }

  if (!updated)
    {
      char *line = xy_strcat (4, "index-url = \"", new_url, "\"", "\n");

      if (is_pyproject)
        {
          char *block = xy_strcat (3, "\n[tool.uv.pip]\n", line, "");
          char *appended = pl_python_uv_append_line (*content, block);
          free (block);
          if (appended)
            {
              free (*content);
              *content = appended;
              updated = true;
            }
        }
      else
        {
          char *block = xy_strcat (3, "\n[pip]\n", line, "");
          char *appended = pl_python_uv_append_line (*content, block);
          free (block);
          if (appended)
            {
              free (*content);
              *content = appended;
              updated = true;
            }
        }

      free (line);
    }
}


typedef struct
{
  char *path;
  bool is_pyproject;
  bool update_python;
  bool update_pypi;
  const char *existing_python_url;
  const char *existing_pypi_url;
}
PlPythonUvTarget;

static int
pl_python_uv_target_index (PlPythonUvTarget targets[], int count, const char *path)
{
  for (int i = 0; i < count; i++)
    {
      if (xy_streql (targets[i].path, path))
        return i;
    }
  return -1;
}


static void
pl_python_uv_target_add (PlPythonUvTarget targets[],
                         int *count,
                         const char *path,
                         bool update_python,
                         bool update_pypi,
                         const char *existing_python_url,
                         const char *existing_pypi_url)
{
  if (!path)
    return;

  int index = pl_python_uv_target_index (targets, *count, path);
  if (index == -1)
    {
      index = *count;
      targets[index].path = xy_strdup (path);
      targets[index].is_pyproject = pl_python_uv_path_is_pyproject (path);
      targets[index].update_python = update_python;
      targets[index].update_pypi = update_pypi;
      targets[index].existing_python_url = existing_python_url;
      targets[index].existing_pypi_url = existing_pypi_url;
      (*count)++;
      return;
    }

  if (update_python)
    {
      targets[index].update_python = true;
      if (existing_python_url)
        targets[index].existing_python_url = existing_python_url;
    }

  if (update_pypi)
    {
      targets[index].update_pypi = true;
      if (existing_pypi_url)
        targets[index].existing_pypi_url = existing_pypi_url;
    }
}


/**
 * @consult https://docs.astral.sh/uv/configuration/files/
 *          https://github.com/RubyMetric/chsrc/issues/139
 */
void
pl_python_uv_setsrc (char *option)
{
  chsrc_ensure_program ("uv");
  PlPythonUvConfigSummary uv_config = pl_python_uv_config_find ();
  chsrc_use_this_source (pl_python_uv);

  const char *PL_CPYTHON_URL = "https://mirror.nju.edu.cn/github-release/astral-sh/python-build-standalone/";

  char *summary_python_path = pl_python_uv_dup_normalized (uv_config.python_path);
  char *summary_pypi_path = pl_python_uv_dup_normalized (uv_config.pypi_path);

  char *uv_local_config_path = NULL;
  if (xy.on_windows)
    {
      uv_local_config_path = xy_2strcat (getenv ("APPDATA"), PL_Python_uv_Local_ConfigPath);
    }
  else
    {
      uv_local_config_path = xy_strdup (PL_Python_uv_Local_ConfigPath);
    }
  uv_local_config_path = xy_normalize_path (uv_local_config_path);

  char *proj_uv_path = pl_python_uv_dup_normalized (PL_Python_uv_Proj_uv);
  char *proj_pyproj_path = pl_python_uv_dup_normalized (PL_Python_uv_Proj_pyproj);

  bool proj_uv_exist = xy_file_exist (proj_uv_path);
  bool proj_pyproj_exist = xy_file_exist (proj_pyproj_path);
  bool local_mode = chsrc_in_local_mode ();

  PlPythonUvTarget targets[3] = {0};
  int target_count = 0;

  if (local_mode)
    {
      const char *existing_python_url = (summary_python_path && xy_streql (summary_python_path, proj_uv_path))
                                           ? uv_config.python_url
                                           : NULL;
      const char *existing_pypi_url = (summary_pypi_path && xy_streql (summary_pypi_path, proj_uv_path))
                                          ? uv_config.pypi_url
                                          : NULL;
      pl_python_uv_target_add (targets, &target_count, proj_uv_path, true, true,
                               existing_python_url, existing_pypi_url);
    }
  else
    {
      if (summary_python_path)
        {
          pl_python_uv_target_add (targets, &target_count, summary_python_path, true, false,
                                   uv_config.python_url, NULL);
        }
      else if (proj_uv_exist)
        {
          pl_python_uv_target_add (targets, &target_count, proj_uv_path, true, false, NULL, NULL);
        }
      else if (proj_pyproj_exist)
        {
          pl_python_uv_target_add (targets, &target_count, proj_pyproj_path, true, false, NULL, NULL);
        }
      else
        {
          pl_python_uv_target_add (targets, &target_count, uv_local_config_path, true, false, NULL, NULL);
        }

      if (summary_pypi_path)
        {
          pl_python_uv_target_add (targets, &target_count, summary_pypi_path, false, true,
                                   NULL, uv_config.pypi_url);
        }
      else if (summary_python_path)
        {
          pl_python_uv_target_add (targets, &target_count, summary_python_path, false, true,
                                   NULL, NULL);
        }
      else if (proj_uv_exist)
        {
          pl_python_uv_target_add (targets, &target_count, proj_uv_path, false, true, NULL, NULL);
        }
      else if (proj_pyproj_exist)
        {
          pl_python_uv_target_add (targets, &target_count, proj_pyproj_path, false, true, NULL, NULL);
        }
      else
        {
          pl_python_uv_target_add (targets, &target_count, uv_local_config_path, false, true, NULL, NULL);
        }
    }

  for (int i = 0; i < target_count; i++)
    {
      char *path = targets[i].path;
      bool file_exists = xy_file_exist (path);
      char *content = file_exists ? xy_file_read (path) : NULL;
      if (!content)
        content = xy_strdup ("");

      if (file_exists)
        {
          chsrc_backup (path);
        }
      else
        {
          char *dir = pl_python_uv_dirname (path);
          if (dir)
            {
              chsrc_ensure_dir (dir);
              free (dir);
            }
        }

      if (targets[i].update_python)
        {
          const char *existing_python_url = targets[i].existing_python_url;
          if (!existing_python_url && summary_python_path && xy_streql (summary_python_path, path))
            existing_python_url = uv_config.python_url;
          pl_python_uv_upsert_python (&content, existing_python_url, PL_CPYTHON_URL,
                                      targets[i].is_pyproject);
        }

      if (targets[i].update_pypi)
        {
          const char *existing_pypi_url = targets[i].existing_pypi_url;
          if (!existing_pypi_url && summary_pypi_path && xy_streql (summary_pypi_path, path))
            existing_pypi_url = uv_config.pypi_url;
          pl_python_uv_upsert_pypi (&content, existing_pypi_url, source.url,
                                    targets[i].is_pyproject);
        }

      chsrc_overwrite_file (content, path);
      free (content);
    }

  for (int i = 0; i < target_count; i++)
    free (targets[i].path);

  free (summary_python_path);
  free (summary_pypi_path);
  free (uv_local_config_path);
  free (proj_uv_path);
  free (proj_pyproj_path);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

void
pl_python_uv_resetsrc (char *option)
{
  pl_python_uv_setsrc (option);
}
