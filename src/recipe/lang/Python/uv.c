/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_uv, "uv");

void
pl_python_uv_prelude (void)
{
  chef_prep_this (pl_python_uv, gsr);

  chef_set_recipe_created_on   (this, "2024-12-11");
  chef_set_recipe_last_updated (this, "2026-05-27");
  chef_set_sources_last_updated (this, "2026-05-27");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@happy-game", "@MingriLingran");
  chef_set_sauciers (this, 3, "@Kattos", "@ccmywish", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_But_Not_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_target_sources (this, &pl_python_group_target);
}


/**
 * chsrc get uv
 *
 * uv 配置文件查找优先级 (代码仅涵盖 ①②，③④为 uv 自身支持):
 * ① ./uv.toml                 (项目级)
 * ② ~/.config/uv/uv.toml      (用户级, Windows: %APPDATA%\uv\uv.toml)
 * ③ $workspace/pyproject.toml (项目级, 未实现)
 * ④ /etc/uv/uv.toml           (系统级, 未实现)
 */

#define PL_Python_uv_ConfigFile       "uv.toml"
#define PL_Python_uv_Local_ConfigPath "./"
#define PL_Python_uv_User_ConfigPath  "~/.config/uv/"


char *
pl_python_find_uv_config (bool mkdir)
{
  if (chsrc_in_project_scope_mode())
    {
      return xy_2strcat (PL_Python_uv_Local_ConfigPath, PL_Python_uv_ConfigFile);
    }
  else
    {
      if (xy.on_windows)
        {
          char *appdata = getenv ("APPDATA");

          if (!appdata)
            {
              chsrc_error2 ("未能获取 APPDATA 环境变量");
              return NULL;
            }

          char *config_dir = xy_2strcat(appdata, "\\uv\\");
          if (mkdir)
            {
              chsrc_ensure_dir (config_dir);
            }
          char *result = xy_2strcat (config_dir, PL_Python_uv_ConfigFile);
          free (config_dir);
          return result;
        }
      else
        {
          if (mkdir)
            {
              chsrc_ensure_dir (PL_Python_uv_User_ConfigPath);
            }
          return xy_2strcat (PL_Python_uv_User_ConfigPath, PL_Python_uv_ConfigFile);
        }
    }
}


void
pl_python_uv_getsrc (char *option)
{
  char *uv_config = pl_python_find_uv_config (false);

  if (!uv_config || !chsrc_check_file (uv_config))
    {
      if (!uv_config)
        chsrc_error2 ("无法获取 uv 配置文件路径");
      else
        chsrc_error2 ("未找到 uv 配置文件");
      free (uv_config);
      return;
    }

  /* 获取 [[index]] 配置项的 url */
  if (xy.on_windows)
    {
      char *script = xy_str_gsub (RAWSTR_pl_python_get_uv_config_on_windows, "@f@", uv_config);
      chsrc_run_as_powershell_file (script);
      free (script);
    }
  else
    {
      char *cmd = xy_str_gsub (RAWSTR_pl_python_get_uv_config, "@f@", uv_config);
      chsrc_run (cmd, RunOpt_Default);
      free (cmd);
    }

  /* 检查 Python 下载镜像 */
  char *content = xy_file_read (uv_config);
  if (content)
    {
      char *line = strstr (content, "python-install-mirror");
      if (line && (line == content || line[-1] == '\n'))
        {
          char *end = strchr (line, '\n');
          if (!end) end = line + strlen (line);
          printf ("%.*s\n", (int)(end - line), line);
        }
      free (content);
    }

  free (uv_config);
}


/*
 * Python下载镜像 (python-install-mirror)
*/

static MirrorSite_t
Py_GHRelease_NJU =
{
  IS_DedicatedMirrorSite,
  "nju", "NJU GitHub Release", "南京大学 GitHub 发布镜像",
  "https://mirrors.nju.edu.cn/github-release/astral-sh/python-build-standalone",
  {NotSkip, NA, NA,
   "https://mirror.nju.edu.cn/github-release/astral-sh/python-build-standalone/20260510/cpython-3.14.5+20260510-i686-pc-windows-msvc-install_only_stripped.tar.gz",
   ACCURATE}
};

/* 中科大的镜像由于仅保留最新的 Latest 且文件链接内含动态版本号导致无法精准测速 */
static MirrorSite_t
Py_GHRelease_USTC =
{
  IS_DedicatedMirrorSite,
  "ustc", "USTC GitHub Release", "中科大 GitHub 发布镜像",
  "https://mirrors.ustc.edu.cn/github-release/astral-sh/python-build-standalone",
  {NotSkip, NA, NA,
   "https://mirrors.ustc.edu.cn/github-release/astral-sh/python-build-standalone/LatestRelease/SHA256SUMS",
   ROUGH}
};

static MirrorSite_t
Py_GHRelease_LZU =
{
  IS_DedicatedMirrorSite,
  "lzu", "LZUOSS GitHub Release", "兰州大学 GitHub 发布镜像",
  "https://mirror.lzu.edu.cn/github-release/astral-sh/python-build-standalone",
  {NotSkip, NA, NA,
   "https://mirror.lzu.edu.cn/github-release/astral-sh/python-build-standalone/20260510/cpython-3.14.5+20260510-i686-pc-windows-msvc-install_only_stripped.tar.gz",
   ACCURATE}
};

static MirrorSite_t
Py_GHRelease_Aliyun =
{
  IS_DedicatedMirrorSite,
  "ali", "Aliyun GitHub Release", "阿里云 GitHub 发布镜像",
  "https://mirrors.aliyun.com/github/releases/astral-sh/python-build-standalone",
  {NotSkip, NA, NA,
   "https://mirrors.aliyun.com/github/releases/astral-sh/python-build-standalone/20260510/cpython-3.14.5+20260510-i686-pc-windows-msvc-install_only_stripped.tar.gz",
   ACCURATE}
};

/* 内部 target，不注册到 menu，仅用于 chsrc_yield_source 自动测速选取 */
static Source_t gh_release_sources[] = {
  {&UpstreamProvider,      "https://github.com/astral-sh/python-build-standalone/releases/download",  DelegateToUpstream},
  {&Py_GHRelease_NJU,      "https://mirrors.nju.edu.cn/github-release/astral-sh/python-build-standalone",    DelegateToMirror},
  {&Py_GHRelease_USTC,     "https://mirrors.ustc.edu.cn/github-release/astral-sh/python-build-standalone",   DelegateToMirror},
  {&Py_GHRelease_LZU,      "https://mirror.lzu.edu.cn/github-release/astral-sh/python-build-standalone",     DelegateToMirror},
  {&Py_GHRelease_Aliyun,   "https://mirrors.aliyun.com/github/releases/astral-sh/python-build-standalone",  DelegateToMirror}
};

static Target_t gh_release_target = {
  .sources_n = xy_c_array_len (gh_release_sources),
  .sources   = gh_release_sources,
  .inited    = true
};


static void
pl_python_uv_write_pypi_index (const char *uv_config, const char *url)
{
  char *source_content = xy_str_gsub (RAWSTR_pl_python_uv_config_source_content, "@url@", url);

  if (!xy_file_exist (uv_config))
    {
      chsrc_append_to_file (source_content, uv_config);
      free (source_content);
      return;
    }

  char *cmd = NULL;
  if (xy.on_windows)
    cmd = xy_str_gsub (RAWSTR_pl_python_test_uv_if_set_source_on_windows, "@f@", uv_config);
  else
    cmd = xy_str_gsub (RAWSTR_pl_python_test_uv_if_set_source, "@f@", uv_config);

  int status = xy_run_get_status (cmd);
  free (cmd);

  if (0 == status)
    {
      if (xy.on_windows)
        {
          char *ps_cmd = xy_str_gsub (RAWSTR_pl_python_set_uv_config_on_windows, "@f@", uv_config);
          char *tmp = xy_str_gsub (ps_cmd, "@url@", url);
          free (ps_cmd);
          chsrc_run (tmp, RunOpt_Default);
          free (tmp);
        }
      else
        {
#if defined(XY_Build_On_macOS) || defined(XY_Build_On_BSD)
          char *sed_prefix = "sed -i '' ";
#else
          char *sed_prefix = "sed -i ";
#endif
          char *cmd2 = xy_str_gsub (RAWSTR_pl_python_set_uv_config, "@sed@", sed_prefix);
          char *tmp  = xy_str_gsub (cmd2, "@f@", uv_config);
          free (cmd2);
          cmd2 = xy_str_gsub (tmp, "@url@", url);
          free (tmp);
          chsrc_run (cmd2, RunOpt_Default);
          free (cmd2);
        }
    }
  else
    {
      chsrc_append_to_file (source_content, uv_config);
    }

  free (source_content);
}


static void
pl_python_uv_write_python_download_mirror (const char *uv_config, Source_t gh_source)
{
  char *content = xy_file_read (uv_config);
  if (!content) content = xy_strdup ("");

  size_t estimate = strlen (content) + strlen (gh_source.url) + 128;
  char *new_content = xy_malloc0 (estimate);
  size_t pos = 0;

  const char *p = content;
  while (*p)
    {
      if (xy_str_start_with (p, "python-install-mirror"))
        {
          while (*p && *p != '\n') p++;
          if (*p == '\n') p++;
        }
      else
        {
          while (*p && *p != '\n') new_content[pos++] = *p++;
          if (*p == '\n') new_content[pos++] = *p++;
        }
    }

  pos += snprintf (new_content + pos, estimate - pos,
                   "python-install-mirror = \"%s\"\n", gh_source.url);
  new_content[pos] = '\0';

  chsrc_overwrite_file (new_content, uv_config);

  free (content);
  free (new_content);
}


/**
 * chsrc set uv
 *
 * 同时更换两部分:
 *   1. PyPI 索引源              ([[index]] 表)
 *   2. Python 解释器下载源       (python-install-mirror)
 *
 * @consult https://docs.astral.sh/uv/reference/settings/#python-install-mirror
 */
void
pl_python_uv_setsrc (char *option)
{
  chsrc_ensure_program ("uv");

  /* ---- 1. 先获取配置路径，再选取源 ---- */
  char *uv_config = pl_python_find_uv_config (true);
  if (NULL == uv_config)
    {
      chsrc_error2 ("无法获取 uv 配置文件路径");
      return;
    }

  Source_t source    = chsrc_yield_source (&pl_python_group_target, option);
  Source_t gh_source = chsrc_yield_source (&gh_release_target, NULL);

  if (chsrc_in_standalone_mode())
    chsrc_confirm_source (&source);

  /* ---- 2. 写入文件 ---- */
  chsrc_backup (uv_config);
  pl_python_uv_write_pypi_index (uv_config, source.url);
  pl_python_uv_write_python_download_mirror (uv_config, gh_source);

  free (uv_config);

  if (chsrc_in_standalone_mode())
    {
      chsrc_determine_chgtype (ChgType_Auto);
      chsrc_conclude (&source);
    }
}


void
pl_python_uv_resetsrc (char *option)
{
  pl_python_uv_setsrc (option);
}
