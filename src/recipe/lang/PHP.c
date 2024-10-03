/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-30>
 * Last Modified : <2024-09-14>
 * ------------------------------------------------------------*/

/**
 * @update 2024-09-14
 * @note 缺少教育网或开源社区软件源
 */
static SourceInfo
pl_php_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/composer/"},
  {&Tencent,       "https://mirrors.tencent.com/composer/"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/composer/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/php/"}
};
def_sources_n(pl_php);


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
 * PHP 换源，参考：https://developer.aliyun.com/composer
 */
void
pl_php_setsrc (char *option)
{
  pl_php_check_cmd ();

  chsrc_yield_source_and_confirm (pl_php);

  char *where = " -g ";
  if (CliOpt_Locally==true)
    {
      where = " ";
    }

  char *cmd = xy_strjoin (4, "composer config", where, "repo.packagist composer ", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source, SetsrcType_SemiAuto);
}


FeatInfo
pl_php_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = false;

  fi.cap_locally = FullyCan;
  fi.locally = "composer 支持 (From v0.1.7)";
  fi.can_english = false;
  fi.can_user_define = true;

  return fi;
}

def_target_gsf (pl_php);
