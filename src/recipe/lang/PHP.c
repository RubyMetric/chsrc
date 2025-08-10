/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_php);

void
pl_php_prelude ()
{
  use_this(pl_php);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-09-14");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nil Null", "nil@null.org");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_allow_local_mode (this, Can, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&upstream,         NULL, DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com/composer/",     DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/composer/",    DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/composer/", DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/php/", DelegateToMirror}
  def_sources_end()
}


void
pl_php_check_cmd ()
{
  chsrc_ensure_program ("composer");
}

/**
 * 已在Windows上测试通过，待其他平台PHP用户确认
 */
void
pl_php_getsrc (char *option)
{
  pl_php_check_cmd ();
  chsrc_run ("composer config -g repositories", RunOpt_Default);
}

/**
 * @consult https://developer.aliyun.com/composer
 */
void
pl_php_setsrc (char *option)
{
  pl_php_check_cmd ();

  chsrc_yield_source_and_confirm (pl_php);

  char *where = " -g ";
  if (chsrc_in_local_mode())
    {
      where = " ";
    }

  char *cmd = xy_strjoin (4, "composer config", where, "repo.packagist composer ", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source);
}
