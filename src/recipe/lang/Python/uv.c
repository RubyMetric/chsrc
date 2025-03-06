/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : happy game <happygame1024@gmail.com>
 * Contributors  :  Nul None  <nul@none.org>
 * Created On    : <2024-12-11>
 * Last Modified : <2024-12-11>
 * ------------------------------------------------------------*/


/**
 * chsrc get uv
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
  if (CliOpt_Locally)
    {
      return xy_strjoin (2, UV_LOCAL_CONFIG_PATH, UV_CONFIG);
    }
  else
    {
      if (mkdir)
      {
        chsrc_ensure_dir (UV_USER_CONFIG_PATH);
      }
      return xy_strjoin (2, UV_USER_CONFIG_PATH, UV_CONFIG);
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
  // grep -A 2 'index' config_file | sed -n 's/^url = "\(.*\)"/\1/p'
  // 获取 [[index]] 配置项的 url
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
  chsrc_ensure_program("uv");

  Source_t source;
  chsrc_yield_for_the_source (pl_python);

  char *uv_config = pl_python_find_uv_config (true);
  chsrc_backup (uv_config);

  const char *source_content = xy_strjoin (5,
    "[[index]]\n",
    "url = \"", source.url, "\"\n",
    "default = true\n");

  // sed -i '/^\[\[index\]\]$/,/^default = true$/{s|^url = ".*"$|url = " source.url "|}' uv_config
  // 将 [[index]] 到 default = true 之间的 url = ".*" 替换为 url = "source.url"
  char *update_source_cmd = xy_strjoin (5, "sed -i ",
                            "'/^\\[\\[index\\]\\]$/,/^default = true$/{s|^url = \".*\"$|url = \"",
                            source.url,
                            "\"|}' ",
                            uv_config);

  char *append_source_cmd = xy_strjoin (4, "echo -e '", source_content, "' >> ", uv_config);

  // grep -q '^[[index]]$' uv_config && update_source_cmd || append_source_cmd
  // 如果 uv_config 中存在 [[index]] 则更新, 否则追加到文件末尾
  // 文件不存在也是追加到新文件末尾
  char *cmd = xy_strjoin (6, "grep -q '^\\[\\[index\\]\\]$' ",
                             uv_config,
                             " && ",
                             update_source_cmd,
                             " || ",
                             append_source_cmd);

  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
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
