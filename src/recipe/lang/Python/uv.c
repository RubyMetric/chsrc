/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_python_uv, "uv");

#include "uv-helper.c"

/* 内部 target 前置声明 (定义在文件末尾 def_target 处) */
void pl_uv_github_release_prelude (void);
extern Target_t pl_uv_github_release_target;

void
pl_python_uv_prelude (void)
{
  chef_prep_this (pl_python_uv, gsr);

  chef_set_recipe_created_on   (this, "2024-12-11");
  chef_set_recipe_last_updated (this, "2026-08-02");

  chef_set_chefs (this, 2, "@happy-game", "@MingriLingran");
  chef_set_sauciers (this, 3, "@Kattos", "@ccmywish", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_But_Not_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);

  chef_use_other_target_sources (this, &pl_python_group_target);

  /* 内部 target 的 prelude 未通过 menu.c 的 add() 注册, 手动挂载 */
  pl_uv_github_release_target.preludefn = pl_uv_github_release_prelude;
}


/**
 * @consult: https://docs.astral.sh/uv/concepts/configuration-files
 * chsrc get uv
 *
 * uv 配置文件选择顺序 (chsrc 写入策略)：
 * 1. ./uv.toml        (项目级；与 pyproject.toml 并存时 uv 优先读取它)
 * 2. ./pyproject.toml (项目级 [tool.uv] 段)
 * 3. 用户级 uv.toml（Unix 下遵循 XDG_CONFIG_HOME）
 *
 * uv 还会向父目录查找最近的项目配置，并合并用户/系统配置；chsrc 为避免
 * 越界修改父项目，只管理当前目录或明确的用户级配置文件。
 * Unix 用户配置目录优先使用 $XDG_CONFIG_HOME/uv，未设置时回退到 ~/.config/uv。
 */

#define PL_Python_uv_ConfigFile          "uv.toml"
#define PL_Python_uv_PyprojectConfigFile "pyproject.toml"
/* 注意: 不能以 "./" 开头 —— Windows 上 chsrc_backup 的 copy 命令无法处理 */
#define PL_Python_uv_Local_ConfigPath    ""
#define PL_Python_uv_User_ConfigPath     "~/.config"

static const char *
pl_python_uv_user_config_path (void)
{
  const char *xdg = getenv ("XDG_CONFIG_HOME");
  return (xdg && *xdg) ? xdg : PL_Python_uv_User_ConfigPath;
}

static Source_t
pl_python_uv_yield_target_source (Target_t *target, char *option)
{
  if (!target->inited) target->preludefn ();

  if (hp_is_url (option))
    {
      Source_t user = { &UserDefinedProvider, option };
      return user;
    }

  int index = use_specific_mirror_or_auto_select (option, target);
  return target->sources[index];
}

static char *
pl_python_uv_read_config (const char *path)
{
  if (!xy_file_exist (path)) return xy_strdup ("");

  char *normalized = xy_normalize_path (path);
  FILE *fp = fopen (normalized, "rb");
  free (normalized);
  if (!fp || fseek (fp, 0, SEEK_END) != 0)
    {
      if (fp) fclose (fp);
      chsrc_error2 ("无法读取 uv 配置文件");
      return NULL;
    }

  long size = ftell (fp);
  if (size < 0)
    {
      fclose (fp);
      chsrc_error2 ("无法读取 uv 配置文件");
      return NULL;
    }
  rewind (fp);

  char *raw = xy_malloc0 ((size_t)size + 1);
  size_t nread = fread (raw, 1, (size_t)size, fp);
  bool failed = ferror (fp) != 0;
  fclose (fp);
  if (failed)
    {
      free (raw);
      chsrc_error2 ("无法读取 uv 配置文件");
      return NULL;
    }
  raw[nread] = '\0';

  char *content = xy_str_gsub (raw, "\r\n", "\n");
  free (raw);
  xy_ptr_replace (&content, xy_str_gsub (content, "\r", "\n"));
  return content;
}


char *
pl_python_find_uv_config (bool mkdir)
{
  if (chsrc_in_project_scope_mode())
    {
      /* uv.toml 与 pyproject.toml 并存时，uv 会忽略后者中的 [tool.uv]。 */
      char *uv_toml = xy_2strcat (PL_Python_uv_Local_ConfigPath, PL_Python_uv_ConfigFile);
      if (xy_file_exist (uv_toml))
        return uv_toml;
      free (uv_toml);

      char *pyproject = xy_2strcat (PL_Python_uv_Local_ConfigPath, PL_Python_uv_PyprojectConfigFile);
      if (xy_file_exist (pyproject))
        return pyproject;
      free (pyproject);
      return xy_2strcat (PL_Python_uv_Local_ConfigPath, PL_Python_uv_ConfigFile);
    }
  else
    {
      if (xy.on_windows)
        {
          /* config path on Windows */
          char *appdata = getenv ("APPDATA");

          if (!appdata)
            {
              chsrc_error2 ("未能获取 APPDATA 环境变量");
              return NULL;
            }

          char *config_dir = xy_path_join (appdata, "uv");
          if (mkdir)
            {
              chsrc_ensure_dir (config_dir);
            }
          char *result = xy_path_join (config_dir, PL_Python_uv_ConfigFile);
          free (config_dir);
          return result;
        }
      else
        {
          /* config path on Linux or macOS */
          const char *config_path = pl_python_uv_user_config_path ();
          char *config_dir = xy_path_join (config_path, "uv");
          if (mkdir)
            {
              chsrc_ensure_dir (config_dir);
            }
          char *result = xy_path_join (config_dir, PL_Python_uv_ConfigFile);
          free (config_dir);
          return result;
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

  /* uv.toml 与 pyproject.toml 均使用同一套受限 TOML 读取逻辑。 */
  char *content = pl_python_uv_read_config (uv_config);
  if (!content)
    {
      chsrc_error2 ("无法读取 uv 配置文件");
      free (uv_config);
      return;
    }

  bool pyproject = xy_str_end_with (uv_config, PL_Python_uv_PyprojectConfigFile);
  const char *index_header = pyproject ? "[[tool.uv.index]]" : "[[index]]";
  const char *parent_table = pyproject ? "[tool.uv]" : NULL;

  char *url = uvh_get_index_url (content, index_header, parent_table);
  if (url)
    {
      say (url);
      free (url);
    }
  else
    {
      if (ENGLISH)
        chsrc_note2 ("No source configured in uv, showing default upstream source:");
      else
        chsrc_note2 ("uv 中未配置源，显示默认上游源：");
      Source_t default_source = chsrc_yield_source (&pl_python_group_target, "upstream");
      say (default_source.url);
    }

  char *mirror = pyproject
               ? uvh_get_value_in_table (content, "python-install-mirror", parent_table)
               : uvh_get_top_level_value (content, "python-install-mirror");
  if (mirror)
    {
      say (mirror);
      free (mirror);
    }

  free (content);
  free (uv_config);
}


/*
 * Python下载镜像 (python-install-mirror)
 *
 * 内部 target, 不注册到 menu, 仅用于本 recipe 独立选择 Python 下载镜像。
 * 针对 python-build-standalone 的测速链接。
*/

def_target (pl_uv_github_release, NULL);

/* CNB (Cloud Native Build, 腾讯) 托管的 python-build-standalone releases 镜像 */
static MirrorSite_t CnbPython =
{
  IS_DedicatedMirrorSite,
  "cnb", "CNB", "Cloud Native Build (腾讯)", "https://cnb.cool/",
  {NotSkip, NA, NA, "https://cnb.cool/astral-sh/python-build-standalone/-/releases/download/20260728/cpython-3.14.6+20260728-i686-pc-windows-msvc-install_only_stripped.tar.gz", ACCURATE}
};

/* 内部 target, 无 CLI 入口, 以下为占位 */
void pl_uv_github_release_getsrc (char *o) { (void)o; }
void pl_uv_github_release_setsrc (char *o) { (void)o; }
void pl_uv_github_release_resetsrc (char *o) { (void)o; }

void
pl_uv_github_release_prelude (void)
{
  chef_prep_this (pl_uv_github_release, gsr);

  chef_set_recipe_created_on   (this, "2026-08-02");
  chef_set_recipe_last_updated (this, "2026-08-02");

  chef_set_chefs (this, 1, "@Mikachu2333");
  chef_set_sauciers (this, 0);

  chef_allow_english (this);
  chef_allow_user_define (this);

  def_sources_begin ()
  {&UpstreamProvider, "https://github.com/astral-sh/python-build-standalone/releases/download",       DelegateToUpstream},
  {&Nju,              "https://mirrors.nju.edu.cn/github-release/astral-sh/python-build-standalone",  FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/github-release/astral-sh/python-build-standalone", FeedByPrelude},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/github-release/astral-sh/python-build-standalone",   FeedByPrelude},
  {&CnbPython,        "https://cnb.cool/astral-sh/python-build-standalone/-/releases/download",       FeedByPrelude}
  def_sources_end ()

#define GH_SM_POSTFIX  "/20260728/cpython-3.14.6+20260728-i686-pc-windows-msvc-install_only_stripped.tar.gz"
  chef_set_smURL_with_postfix (this, &Nju,       GH_SM_POSTFIX);
  chef_set_smURL_with_postfix (this, &Lzuoss,    GH_SM_POSTFIX);
  chef_set_smURL_with_postfix (this, &CnbPython, GH_SM_POSTFIX);
#undef GH_SM_POSTFIX

  /* 2026-5-31: USTC 仅保留 Latest, 只能用 SHA256SUMS 粗略测速 */
  chef_set_smURL_with_postfix (this, &Ustc, "/LatestRelease/SHA256SUMS");

  /* 中科大仅保留 Latest 且文件内含动态版本号, 使用模糊测速 */
  chef_set_provider_sm_accuracy (&Ustc, ROUGH);
}


/**
 * 一次性完成uv配置文件的全部文件操作 (set 路径)
 */
static void
pl_python_uv_write_all (const char *uv_config, const char *pypi_url, const char *py_dl_url)
{
  char *content = pl_python_uv_read_config (uv_config);
  if (!content) return;

  char *final = NULL;
  if (xy_str_end_with (uv_config, PL_Python_uv_PyprojectConfigFile))
    {
      /* pyproject.toml: 配置位于 [tool.uv] 表内 */
      char *updated = replace_index_url (content, pypi_url, "[[tool.uv.index]]", "[tool.uv]");
      final = replace_key_value (updated, "python-install-mirror", py_dl_url, "[tool.uv]");
      free (updated);
    }
  else
    {
      /* uv.toml: 配置位于顶层 */
      char *updated = replace_pypi_index_url (content, pypi_url);
      final = replace_python_install_mirror (updated, py_dl_url);
      free (updated);
    }

  chsrc_overwrite_file (final, uv_config);
  free (final);
  free (content);
}


/**
 * chsrc set uv
 *
 * 同时更换两部分:
 *   1. PyPI 索引源               ([[index]] 表)
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

  /**
   * reset: 把 index 与 python-install-mirror 写回
   * 默认上游（PyPI 与 python-build-standalone）， 不依赖 .bak。
   */
  if (chsrc_in_reset_mode ())
    {
      if (!chsrc_check_file (uv_config))
        {
          chsrc_info ("没有 uv 配置文件，无需重置");
          free (uv_config);
          return;
        }

      Source_t default_pypi = pl_python_uv_yield_target_source (&pl_python_group_target, "upstream");
      Source_t default_gh   = pl_python_uv_yield_target_source (&pl_uv_github_release_target, "upstream");

      chsrc_backup (uv_config);
      pl_python_uv_write_all (uv_config, default_pypi.url, default_gh.url);
      free (uv_config);

      return;
    }

  /**
   * set: 选取源并写入。
   * 两个 URL 的语义不同：自定义 URL 只作为 PyPI index，Python 下载镜像仍自动测速。 */
  char *pypi_opt = option;
  char *gh_opt = NULL;

  if (option && !hp_is_url (option))
    {
      if (!pl_uv_github_release_target.inited)
        pl_uv_github_release_target.preludefn ();
      bool gh_found = false;
      bool pypi_found = false;
      for (int i = 0; i < pl_uv_github_release_target.sources_n; i++)
        if (xy_streql (pl_uv_github_release_target.sources[i].mirror->code, option))
          { gh_found = true; break; }
      if (!pl_python_group_target.inited)
        pl_python_group_target.preludefn ();
      for (int i = 0; i < pl_python_group_target.sources_n; i++)
        if (xy_streql (pl_python_group_target.sources[i].mirror->code, option))
          { pypi_found = true; break; }

      if (gh_found && !pypi_found)
        {
          pypi_opt = NULL;
          gh_opt = option;
        }
      else if (gh_found)
        gh_opt = option; /* 共有 code: 两个 target 都使用同一 code */
    }

  Source_t source = chsrc_yield_source (&pl_python_group_target, pypi_opt);
  /* 内部 target 不得复用 Python group leader 的数组下标。 */
  Source_t gh_source = pl_python_uv_yield_target_source (&pl_uv_github_release_target, gh_opt);

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
