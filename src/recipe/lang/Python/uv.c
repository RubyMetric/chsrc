/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_uv, "uv");

void
pl_python_uv_prelude (void)
{
  chef_prep_this (pl_python_uv, gsr);

  chef_set_created_on   (this, "2024-12-11");
  chef_set_last_updated (this, "2025-10-11");
  chef_set_sources_last_updated (this, "2025-10-11");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@mikachu2333", "@happy-game");
  chef_set_sauciers (this, 2, "@Kattos", "@ccmywish");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

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
  #define PL_Python_uv_Local_ConfigPath  "~/.config/uv/"
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
  if (pl_python_uv_config_summary_struct_is_full (&result) || chsrc_in_local_mode()) return result;

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


/**
 * @consult https://docs.astral.sh/uv/configuration/files/
 *          https://github.com/RubyMetric/chsrc/issues/139
 */
void
pl_python_uv_setsrc (char *option)
{
  chsrc_ensure_program ("uv");


}


void
pl_python_uv_resetsrc (char *option)
{
  pl_python_uv_setsrc (option);
}
