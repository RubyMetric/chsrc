/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(wr_homebrew, "brew/homebrew");

static void
wr_homebrew_check_platform (void)
{
  if (xy.on_windows)
    {
      char *msg = ENGLISH
        ? "Homebrew is only supported on macOS and Linux, not native Windows"
        : "Homebrew 仅支持 macOS 与 Linux，不支持原生 Windows";
      chsrc_error (msg);
      exit (Exit_Unsupported);
    }
}

void
wr_homebrew_prepare (void)
{
  chef_prep_this_dish (wr_homebrew, gsr);

  chef_set_recipe_created_on   (this, "2023-09-10");
  chef_set_recipe_last_updated (this, "2026-10-28");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@Word2VecT", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_And_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english (this);
  chef_deny_user_define (this);

  chef_set_note (this,
    "Homebrew 仅支持 macOS 与 Linux，通过持久化环境变量实现幂等换源",
    "Homebrew is supported on macOS and Linux only, configured idempotently via persistent environment variables");

  def_sources_begin()
  {&UpstreamProvider, "https://github.com/Homebrew/brew.git",  DelegateToUpstream},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/", DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/",          DelegateToMirror},
  {&Nju,              "https://mirror.nju.edu.cn/",            DelegateToMirror},
  {&Nyist,            "https://mirror.nyist.edu.cn/",          DelegateToMirror}
  def_sources_end()
}


void
wr_homebrew_getsrc (char *option)
{
  wr_homebrew_check_platform ();
  chsrc_get_env (
    "HOMEBREW_API_DOMAIN",
    "HOMEBREW_BOTTLE_DOMAIN",
    "HOMEBREW_BREW_GIT_REMOTE",
    "HOMEBREW_CORE_GIT_REMOTE",
    NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/homebrew/
 *
 * 自 brew 4.0.0 (2023 年 2 月 16 日) 起，
 * HOMEBREW_INSTALL_FROM_API 会成为默认行为，无需设置。大部分用户无需再克隆 homebrew-core 仓库，故无需设置 HOMEBREW_CORE_GIT_REMOTE 环境变量；
 * 但是为了以防万一，我们还是为用户设置该环境变量
 */
void
wr_homebrew_setsrc (char *option)
{
  wr_homebrew_check_platform ();

  chsrc_use_this_source (wr_homebrew);

  if (source_is_upstream (&source))
    {
      chsrc_set_env_block (
        "HOMEBREW_BREW_GIT_REMOTE", NULL,
        "HOMEBREW_CORE_GIT_REMOTE", NULL,
        "HOMEBREW_API_DOMAIN",      NULL,
        "HOMEBREW_BOTTLE_DOMAIN",   NULL,
        NULL);
    }
  else
    {
      char *base       = hp_remove_trailing_slash (source.url);
      char *brew_git   = xy_2strcat (base, "/git/homebrew/brew.git");
      char *core_git   = xy_2strcat (base, "/git/homebrew/homebrew-core.git");
      char *api_domain = xy_2strcat (base, "/homebrew-bottles/api");
      char *bottle_dom = xy_2strcat (base, "/homebrew-bottles");

      chsrc_set_env_block (
        "HOMEBREW_BREW_GIT_REMOTE", brew_git,
        "HOMEBREW_CORE_GIT_REMOTE", core_git,
        "HOMEBREW_API_DOMAIN",      api_domain,
        "HOMEBREW_BOTTLE_DOMAIN",   bottle_dom,
        NULL);

      free (bottle_dom);
      free (api_domain);
      free (core_git);
      free (brew_git);
      free (base);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
wr_homebrew_resetsrc (char *option)
{
  wr_homebrew_check_platform ();

  chsrc_set_env_block (
    "HOMEBREW_BREW_GIT_REMOTE", NULL,
    "HOMEBREW_CORE_GIT_REMOTE", NULL,
    "HOMEBREW_API_DOMAIN",      NULL,
    "HOMEBREW_BOTTLE_DOMAIN",   NULL,
    NULL);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
