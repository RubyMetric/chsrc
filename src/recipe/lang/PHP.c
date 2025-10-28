/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_php, "php/composer");

void
pl_php_prelude ()
{
  chef_prep_this (pl_php, gs);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-08-24");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@hezonglun");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://packagist.org/", DelegateToUpstream},
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

  chsrc_use_this_source (pl_php);

  char *where = " -g ";
  if (chsrc_in_local_mode())
    {
      where = " ";
    }

  char *cmd = xy_strcat (4, "composer config", where, "repo.packagist composer ", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source);
}
