/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : happy game <happygame1024@gmail.com>
 * Contributors  :    ccy     <icuichengyi@gmail.com>
 *               | Aoran Zeng <ccmywish@qq.com>
 *               |
 * Created On    : <2024-12-11>
 * Major Revision :      1
 * Last Modified : <2025-07-14>
 * ------------------------------------------------------------*/

/**
 * chsrc get uv
 *
 * uv的配置优先级顺序如下(高到低):
 * 1. $workspaces/uv.toml
 * 2. $workspaces/pyproject.toml
 * 3. ~/.config/uv/uv.toml
 * 4. /etc/uv/uv.toml
 */

#define PL_Python_uv_ConfigFile       "uv.toml"
#define PL_Python_uv_Local_ConfigPath "./"
#define PL_Python_uv_User_ConfigPath  "~/.config/uv/"


char *
pl_python_find_uv_config (bool mkdir)
{
  if (chsrc_in_local_mode())
    {
      return xy_2strjoin (PL_Python_uv_Local_ConfigPath, PL_Python_uv_ConfigFile);
    }
  else
    {
      if (xy_on_windows)
        {
          /* config path on Windows */
          char *appdata = getenv ("APPDATA");

          if (!appdata)
            {
              chsrc_error2 ("未能获取 APPDATA 环境变量");
              return NULL;
            }

          char *config_dir = xy_2strjoin(appdata, "\\uv\\");
          if (mkdir)
            {
              chsrc_ensure_dir (config_dir);
            }
          return xy_2strjoin (config_dir, PL_Python_uv_ConfigFile);
        }
      else
        {
          /* config path on Linux or macOS */
          if (mkdir)
            {
              chsrc_ensure_dir (PL_Python_uv_User_ConfigPath);
            }
          return xy_2strjoin (PL_Python_uv_User_ConfigPath, PL_Python_uv_ConfigFile);
        }
    }
}

void
pl_python_uv_getsrc (char *option)
{
  char *uv_config = pl_python_find_uv_config (false);

  if (!chsrc_check_file (uv_config))
    {
      chsrc_error2 ("未找到 uv 配置文件");
      return;
    }

  /* 获取 [[index]] 配置项的 url */
  char *cmd = xy_str_gsub (RAWSTR_pl_python_get_uv_config, "@f@", uv_config);
  chsrc_run (cmd, RunOpt_Default);
}


/**
 * @consult https://docs.astral.sh/uv/configuration/files/
 *          https://github.com/RubyMetric/chsrc/issues/139
 *
 * chsrc set uv
 */
void
pl_python_uv_setsrc (char *option)
{
  chsrc_ensure_program ("uv");

  chsrc_yield_source (pl_python_group);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source();

  char *uv_config = pl_python_find_uv_config (true);
  if (NULL==uv_config)
    {
      chsrc_error2 ("无法获取 uv 配置文件路径");
      return;
    }
  chsrc_backup (uv_config);

  const char *source_content = xy_str_gsub (RAWSTR_pl_python_uv_config_source_content, "@url@", source.url);

#if defined(XY_On_macOS) || defined(XY_On_BSD)
  char *sed_cmd = "sed -i '' ";
#else
  char *sed_cmd = "sed -i ";
#endif

  /**
   * 将 [[index]] 到 default = true 之间的 url = ".*" 替换为 url = "source.url"
   */
  char *update_source_cmd = xy_str_gsub (RAWSTR_pl_python_set_uv_config, "@sed@", sed_cmd);
        update_source_cmd = xy_str_gsub (update_source_cmd, "@f@", uv_config);
        update_source_cmd = xy_str_gsub (update_source_cmd, "@url@", source.url);

  char *append_source_cmd = xy_strjoin (4, "printf '", source_content, "' >> ", uv_config);

  char *cmd = NULL;
  if (!xy_file_exist (uv_config))
    {
      /**
       * uv_config 不存在，追加到新文件末尾
       * run: append_source_cmd
       */
      cmd = append_source_cmd;
    }
  else
    {
      if (xy_on_windows)
        {
          /* TODO: Windows 下替换源暂不支持 */
          chsrc_note2 ("Windows 下暂不支持修改 uv.toml，请手动修改配置文件");
        }
      else
        {
          /**
           * uv_config 存在，如果存在 [[index]] 则更新，否则追加到文件末尾
           */
          cmd = xy_str_gsub (RAWSTR_pl_python_final_uv_cmd, "@f@", uv_config);
          cmd = xy_str_gsub (cmd, "@ucmd@", update_source_cmd);
          cmd = xy_str_gsub (cmd, "@acmd@", append_source_cmd);
        }
    }
  if (NULL==cmd)
    {
      chsrc_note2 (xy_strjoin (4, "请手动为 ", uv_config, "添加或修改 [[index]] 配置项的 url = ", source.url));
    }
  else
    {
      chsrc_run (cmd, RunOpt_Default);
    }


  if(chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


/**
 * chsrc reset uv
 */
void
pl_python_uv_resetsrc (char *option)
{
  pl_python_uv_setsrc (option);
}


/**
 * chsrc ls uv
 */
Feature_t
pl_python_uv_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = true;
  f.cap_locally_explain = NULL;

  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

// def_target_gsrf(pl_python_uv);
Target_t pl_python_uv_target = {def_target_inner_gsrf(pl_python_uv),def_target_sourcesn(pl_python_group)};
