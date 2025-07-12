/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : happy game <happygame1024@gmail.com>
 * Contributors  :    ccy     <icuichengyi@gmail.com>
 *               | Aoran Zeng <ccmywish@qq.com>
 *               |
 * Created On    : <2024-12-11>
 * Last Modified : <2025-07-11>
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

#define UV_CONFIG "uv.toml"
#define UV_LOCAL_CONFIG_PATH "./"
#define UV_USER_CONFIG_PATH  "~/.config/uv/"


char *
pl_python_find_uv_config (bool mkdir)
{
  if (chsrc_in_local_mode())
    {
      return xy_2strjoin (UV_LOCAL_CONFIG_PATH, UV_CONFIG);
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
          return xy_2strjoin (config_dir, UV_CONFIG);
        }
      else
        {
          /* config path on Linux or macOS */
          if (mkdir)
            {
              chsrc_ensure_dir (UV_USER_CONFIG_PATH);
            }
          return xy_2strjoin (UV_USER_CONFIG_PATH, UV_CONFIG);
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

  /**
   * grep -A 2 'index' config_file | sed -n 's/^url = "\(.*\)"/\1/p'
   * 获取 [[index]] 配置项的 url
   */
  char *cmd = xy_strjoin (3, "grep -A 2 'index' ",
                             uv_config,
                             " | sed -n 's/^url = \"\\(.*\\)\"/\\1/p'");
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

  chsrc_yield_source (pl_python);
  if (chsrc_in_standalone_mode())
    chsrc_confirm_source;

  char *uv_config = pl_python_find_uv_config (true);
  if (NULL==uv_config)
    {
      chsrc_error2 ("无法获取 uv 配置文件路径");
      return;
    }
  chsrc_backup (uv_config);

  const char *source_content = xy_strjoin (5,
    "[[index]]\\n",
    "url = \"", source.url, "\"\\n",
    "default = true\\n");

  /**
   * sed -i '/^\[\[index\]\]$/,/^default = true$/{s|^url = ".*"$|url = " source.url "|}' uv_config
   * 将 [[index]] 到 default = true 之间的 url = ".*" 替换为 url = "source.url"
   */
#if defined(XY_On_macOS) || defined(XY_On_BSD)
  char *sed_cmd = "sed -i '' ";
#else
  char *sed_cmd = "sed -i ";
#endif

  char *update_source_cmd = xy_strjoin (5, sed_cmd,
                            "'/^\\[\\[index\\]\\]$/,/^default = true$/{s|^url = \".*\"$|url = \"",
                            source.url,
                            "\"|;}' ",
                            uv_config);
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
           * run: grep -q '^[[index]]$' uv_config && update_source_cmd || append_source_cmd
           */
          cmd = xy_strjoin (6, "grep -q '^\\[\\[index\\]\\]$' ",
                              uv_config,
                              " && ",
                              update_source_cmd,
                              " || ",
                              append_source_cmd);
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
Target_t pl_python_uv_target = {def_target_inner_gsrf(pl_python_uv),def_target_sourcesn(pl_python)};
