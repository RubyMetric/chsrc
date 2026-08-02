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
 * chsrc get uv
 *
 * uv 配置文件查找优先级：
 * 1 ./uv.toml                    (项目级，已实现)
 * 2. $workspaces/pyproject.toml  (项目级，未实现，因过于罕见不考虑适配)
 * 3. ~/.config/uv/uv.toml        (用户级，已实现)
 * 4. /etc/uv/uv.toml             (系统级，考虑到权限问题不予考虑)
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
          /* config path on Windows */
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
          return result;
        }
      else
        {
          /* config path on Linux or macOS */
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
      return;
    }

  /* 获取 [[index]] 配置项的 url */
  if (xy.on_windows)
    {
      /* 在 Windows 上使用 PowerShell 替代 grep */
      char *script = xy_str_gsub (RAWSTR_pl_python_get_uv_config_on_windows, "@f@", uv_config);
      chsrc_run_as_powershell_file (script);
    }
  else
    {
      /* 在类 Unix 系统上使用 grep */
      char *cmd = xy_str_gsub (RAWSTR_pl_python_get_uv_config, "@f@", uv_config);
      chsrc_run (cmd, RunOpt_Default);
    }

  /* 检查 Python 下载镜像 (顶层 python-install-mirror 键) */
  char *content = xy_file_read (uv_config);
  if (content)
    {
      char *mirror = uvh_get_top_level_value (content, "python-install-mirror");
      if (mirror)
        {
          say (mirror);
          free (mirror);   /* uvh_get_top_level_value 返回 caller-free */
        }
      free (content);
    }
}


/*
 * Python下载镜像 (python-install-mirror)
 *
 * 内部 target, 不注册到 menu, 仅用于 chsrc_yield_source 自动测速选取。
 * 针对 python-build-standalone 的测速链接。
*/

def_target (pl_uv_github_release, NULL);

/* 内部 target, 无 CLI 入口, 以下为占位 */
void pl_uv_github_release_getsrc (char *o) { (void)o; }
void pl_uv_github_release_setsrc (char *o) { (void)o; }
void pl_uv_github_release_resetsrc (char *o) { (void)o; }

void
pl_uv_github_release_prelude (void)
{
  chef_prep_this (pl_uv_github_release, gsr);

  chef_set_recipe_created_on   (this, "2026-05-31");
  chef_set_recipe_last_updated (this, "2026-05-31");

  chef_set_chefs (this, 1, "@Mikachu2333");
  chef_set_sauciers (this, 0);

  chef_allow_english (this);
  chef_allow_user_define (this);

  def_sources_begin ()
  {&UpstreamProvider, "https://github.com/astral-sh/python-build-standalone/releases/download",       DelegateToUpstream},
  {&Nju,              "https://mirrors.nju.edu.cn/github-release/astral-sh/python-build-standalone",  FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/github-release/astral-sh/python-build-standalone", FeedByPrelude},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/github-release/astral-sh/python-build-standalone",   FeedByPrelude}
  def_sources_end ()

#define GH_SM_POSTFIX  "/20260510/cpython-3.14.5+20260510-i686-pc-windows-msvc-install_only_stripped.tar.gz"
  chef_set_smURL_with_postfix (this, &Nju,    GH_SM_POSTFIX);
  chef_set_smURL_with_postfix (this, &Lzuoss, GH_SM_POSTFIX);
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
  char *content = xy_file_read (uv_config);
  if (!content) content = xy_strdup ("");

  char *updated = replace_pypi_index_url (content, pypi_url);

  char *final = replace_python_install_mirror (updated, py_dl_url);

  chsrc_overwrite_file (final, uv_config);
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
          return;
        }

      /* 读内容, 清理, 写回 */
      char *content = xy_file_read (uv_config);
      if (!content)
        {
          chsrc_error2 ("无法读取 uv 配置文件");
          return;
        }

      char *cleaned = cleanup_config_for_reset (content);
      chsrc_overwrite_file (cleaned, uv_config);

      return;
    }

  /* set: 选取源并写入 */
  Source_t source = chsrc_yield_source (&pl_python_group_target, option);

  /* 若 option 命中了 GitHub release 源则直接用, 否则自动测速 */
  char *gh_opt = option;
  if (gh_opt && !chsrc_in_reset_mode () && !hp_is_url (gh_opt))
    {
      bool found = false;
      if (!pl_uv_github_release_target.inited)
        pl_uv_github_release_target.preludefn ();
      for (int i = 0; i < pl_uv_github_release_target.sources_n; i++)
        if (xy_streql (pl_uv_github_release_target.sources[i].mirror->code, gh_opt))
          { found = true; break; }
      if (!found) gh_opt = NULL;
    }
  Source_t gh_source = chsrc_yield_source (&pl_uv_github_release_target, gh_opt);

  if (chsrc_in_standalone_mode())
    chsrc_confirm_source (&source);

  chsrc_backup (uv_config);
  pl_python_uv_write_all (uv_config, source.url, gh_source.url);

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
