/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

/**
 * @consult: https://docs.astral.sh/uv/concepts/configuration-files
 *
 *
 * uv 配置文件选择顺序 (chsrc 写入策略)：
 *
 *   1. ./uv.toml        (项目级；与 pyproject.toml 并存时 uv 优先读取它)
 *   2. ./pyproject.toml (项目级 [tool.uv] 段)
 *   3. 用户级 uv.toml（Unix 下遵循 XDG_CONFIG_HOME）
 *
 * uv 还会向父目录查找最近的项目配置，并合并用户/系统配置；chsrc 为避免
 * 越界修改父项目，只管理当前目录或明确的用户级配置文件。
 * Unix 用户配置目录优先使用 $XDG_CONFIG_HOME/uv，未设置时回退到 ~/.config/uv。
 */

#define PL_uv_ConfigFile          "uv.toml"
#define PL_uv_PyprojectConfigFile "pyproject.toml"
// 注意: 不能以 "./" 开头 —— Windows 上 chsrc_backup 的 copy 命令无法处理
#define PL_uv_Local_ConfigPath    ""
#define PL_uv_User_ConfigPath     "~/.config"

static const char *
pl_uv_user_config_path (void)
{
  const char *xdg = getenv ("XDG_CONFIG_HOME");
  return (xdg && *xdg) ? xdg : PL_uv_User_ConfigPath;
}



/**
 * @the-return-is-NULLable
 */
char *
pl_uv_find_uv_config (bool mkdir)
{
  if (chsrc_in_project_scope_mode())
    {
      // uv.toml 与 pyproject.toml 并存时，uv 会忽略后者中的 [tool.uv]。
      char *uv_toml = xy_2strcat (PL_uv_Local_ConfigPath, PL_uv_ConfigFile);
      if (xy_file_exist (uv_toml))
        return uv_toml;

      char *pyproject = xy_2strcat (PL_uv_Local_ConfigPath, PL_uv_PyprojectConfigFile);
      if (xy_file_exist (pyproject))
        return pyproject;
      return xy_2strcat (PL_uv_Local_ConfigPath, PL_uv_ConfigFile);
    }
  else
    {
      if (xy.on_windows)
        {
          // config path on Windows
          char *appdata = xy_env_get ("APPDATA");

          if (!appdata)
            {
              chsrc_error2 ("未能获取 APPDATA 环境变量");
              return NULL;
            }

          char *config_dir = xy_path_join (appdata, "uv");
          free (appdata);
          if (mkdir)
            {
              chsrc_ensure_dir (config_dir);
            }
          char *result = xy_path_join (config_dir, PL_uv_ConfigFile);
          return result;
        }
      else
        {
          // config path on Linux or macOS
          const char *config_path = pl_uv_user_config_path ();
          char *config_dir = xy_path_join (config_path, "uv");
          if (mkdir)
            {
              chsrc_ensure_dir (config_dir);
            }
          char *result = xy_path_join (config_dir, PL_uv_ConfigFile);
          return result;
        }
    }
}
