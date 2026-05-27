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
 * uv 配置文件查找优先级：
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


/**
 * 在 content 中找到 [[index]] 段并替换其 url = "..." 行。
 * 找不到 [[index]] 则追加整个段到末尾。
 *
 * @return 新内容 (caller-free)
 */
static char *
replace_pypi_index_url (const char *content, const char *url)
{
  const char *index_tag = "[[index]]";

  char *index_pos = strstr (content, index_tag);
  if (!index_pos || (index_pos != content && index_pos[-1] != '\n'))
    {
      /* 文件中尚无 [[index]] 段，追加到末尾 */
      bool need_sep = (content[0] != '\0');
      size_t len = strlen (content) + strlen (url) + 80;
      char *ret = xy_malloc0 (len);
      if (need_sep)
        snprintf (ret, len, "%s\n[[index]]\nurl = \"%s\"\ndefault = true\n", content, url);
      else
        snprintf (ret, len, "[[index]]\nurl = \"%s\"\ndefault = true\n", url);
      return ret;
    }

  /* 找到 [[index]] 后紧跟的 url = "..." 行 (允许被 default = true 行隔开) */
  const char *search_start = index_pos + strlen (index_tag);
  const char *url_key = "url = \"";
  char *url_line = NULL;

  /* 只在当前 [[index]] 段内搜索: 遇到下一个 [[ 开头的行或文件结尾就停止 */
  const char *next_section = strstr (search_start, "\n[[");
  const char *limit = next_section ? next_section + 1 : content + strlen (content);

  for (const char *p = search_start; p < limit; p++)
    {
      if (*p == 'u' && xy_str_start_with (p, url_key) && (p == content || p[-1] == '\n'))
        {
          url_line = (char *)p;
          break;
        }
    }

  if (!url_line)
    {
      /* 有 [[index]] 段但没有 url = "..." 行，追加 url 行 */
      size_t len = strlen (content) + strlen (url) + 32;
      char *ret = xy_malloc0 (len);
      size_t pos = 0;

      /* 在 [[index]] 行的下一行插入 url */
      const char *insert_at = search_start;
      while (*insert_at == '\n') insert_at++;

      pos += snprintf (ret + pos, len - pos, "%.*s",
                       (int)(insert_at - content), content);
      pos += snprintf (ret + pos, len - pos, "url = \"%s\"\n", url);
      strcpy (ret + pos, insert_at);
      return ret;
    }

  /* 替换 url = "..." 行 */
  /* 找到该行结尾 */
  char *url_end = strchr (url_line, '\n');
  if (!url_end) url_end = (char *)content + strlen (content);

  size_t est = strlen (content) + strlen (url) + 32;
  char *ret = xy_malloc0 (est);
  size_t pos = 0;

  /* 拷贝 url_line 之前的内容 */
  pos += snprintf (ret + pos, est - pos, "%.*s",
                   (int)(url_line - content), content);
  /* 写入新的 url 行 */
  pos += snprintf (ret + pos, est - pos, "url = \"%s\"", url);
  /* 拷贝 url_line 之后的内容 (从该行原换行符开始, 保留 \n) */
  strcpy (ret + pos, url_end);

  return ret;
}


/**
 * 过滤 content 中的 python-install-mirror 行，追加新值。
 *
 * @return 新内容 (caller-free)
 */
static char *
replace_python_install_mirror (const char *content, const char *url)
{
  size_t estimate = strlen (content) + strlen (url) + 128;
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
                   "python-install-mirror = \"%s\"\n", url);
  new_content[pos] = '\0';
  return new_content;
}


/**
 * reset 专用: 删除所有 [[index]] 段内的 url/default 行 (保留 [[index]] 头
 * 与 name 等其余字段), 删除 python-install-mirror 行, 其余内容原样保留。
 *
 * @return 新内容 (caller-free)
 */
static char *
cleanup_config_for_reset (const char *content)
{
  size_t est = strlen (content) + 128;
  char *ret = xy_malloc0 (est);
  size_t pos = 0;

  const char *p = content;
  while (*p)
    {
      bool skip = false;

      if (xy_str_start_with (p, "url = \"") ||
          xy_str_start_with (p, "default = "))
        {
          skip = true;
        }
      else if (xy_str_start_with (p, "python-install-mirror"))
        {
          skip = true;
        }

      if (skip)
        {
          while (*p && *p != '\n') p++;
          if (*p == '\n') p++;
        }
      else
        {
          while (*p && *p != '\n') ret[pos++] = *p++;
          if (*p == '\n') ret[pos++] = *p++;
        }
    }

  ret[pos] = '\0';
  return ret;
}


/**
 * 一次性完成uv配置文件的全部文件操作 (set 路径)
 */
static void
pl_python_uv_write_all (const char *uv_config, const char *pypi_url, const char *py_dl_url)
{
  char *content = xy_file_read (uv_config);
  if (!content) content = xy_strdup ("");

  char *updated = replace_pypi_index_url (content, pypi_url);
  free (content);

  char *final = replace_python_install_mirror (updated, py_dl_url);
  free (updated);

  chsrc_overwrite_file (final, uv_config);
  free (final);
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

  char *uv_config = pl_python_find_uv_config (true);
  if (NULL == uv_config)
    {
      chsrc_error2 ("无法获取 uv 配置文件路径");
      return;
    }

  /* reset: 清理 [[index]] 段 url 与 python-install-mirror 行 */
  if (chsrc_in_reset_mode ())
    {
      if (!chsrc_check_file (uv_config))
        {
          chsrc_info ("没有 uv 配置文件, 无需重置");
          free (uv_config);
          return;
        }

      /* 读内容, 清理, 直接写回 */
      char *content = xy_file_read (uv_config);
      if (!content)
        {
          chsrc_error2 ("无法读取 uv 配置文件");
          free (uv_config);
          return;
        }

      char *cleaned = cleanup_config_for_reset (content);
      free (content);

      FILE *f = fopen (uv_config, "w");
      if (f)
        {
          fwrite (cleaned, 1, strlen (cleaned), f);
          fclose (f);
        }
      free (cleaned);

      free (uv_config);
      return;
    }

  /* set: 选取源并写入 */
  Source_t source    = chsrc_yield_source (&pl_python_group_target, option);
  Source_t gh_source = chsrc_yield_source (&gh_release_target, NULL);

  if (chsrc_in_standalone_mode())
    chsrc_confirm_source (&source);

  chsrc_backup (uv_config);
  pl_python_uv_write_all (uv_config, source.url, gh_source.url);

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
