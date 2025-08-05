/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng    <ccmywish@qq.com>
 * Contributors  : BingChunMoLi  <bingchunmoli@bingchunmoli.com>
 *               |
 * Created On    : <2023-08-31>
 * Last Modified : <2025-07-31>
 * ------------------------------------------------------------*/

static SourceProvider_t pl_java_upstream =
{
  def_upstream, "https://mvnrepository.com/",
  def_need_measure_info
};

/**
 * @update 2024-12-18
 */
static Source_t pl_java_sources[] =
{
  {&pl_java_upstream,  "https://repo1.maven.org/maven2/", NULL},
  {&Ali,              "https://maven.aliyun.com/repository/public/",       DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/maven/", DelegateToMirror},
  /* 网易的24小时更新一次 */
  {&Netease,          "http://mirrors.163.com/maven/repository/maven-public/", DelegateToMirror}
};
def_sources_n(pl_java);


void
pl_java_check_cmd (bool *maven_exist, bool *gradle_exist, bool *maven_daemon_exist)
{
  *maven_exist  = chsrc_check_program ("mvn");
  *gradle_exist = chsrc_check_program ("gradle");
  *gradle_exist = chsrc_check_program ("mvnd");

  if (! *maven_exist && ! *gradle_exist && ! *maven_daemon_exist)
    {
      chsrc_error ("maven(maven-daemon) 与 gradle 命令均未找到，请检查是否存在其一");
      exit (Exit_UserCause);
    }
}


char *
pl_java_find_maven_config ()
{
  char *buf = xy_run ("mvn -v", 2);
  char *maven_home = xy_str_delete_prefix (buf, "Maven home: ");
  maven_home = xy_str_strip (maven_home);

  char *maven_config = xy_normalize_path (xy_2strjoin (maven_home, "/conf/settings.xml"));
  return maven_config;
}


char *
pl_java_find_maven_daemon_config ()
{
  char *buf = xy_run ("mvnd -v", 2);
  char *maven_home = xy_str_delete_prefix (buf, "Maven daemon home: ");
  maven_home = xy_str_strip (maven_home);

  char *maven_config = xy_normalize_path (xy_2strjoin (maven_home, "/conf/settings.xml"));
  return maven_config;
}


void
pl_java_getsrc (char *option)
{
  bool maven_exist, gradle_exist, maven_daemon_exist;
  pl_java_check_cmd (&maven_exist, &gradle_exist, &maven_daemon_exist);
  if (maven_exist)
  {
    char *maven_config = pl_java_find_maven_config ();
    chsrc_note2 (xy_2strjoin ("请查看 ", maven_config));
  }
  if(maven_daemon_exist)
  {
    char *maven_config = pl_java_find_maven_daemon_config ();
    chsrc_note2 (xy_2strjoin ("请查看 ", maven_config));
  }
}


/**
 * @consult https://developer.aliyun.com/mirror/maven
 */
void
pl_java_setsrc (char *option)
{
  bool maven_exist, gradle_exist, maven_daemon_exist;
  pl_java_check_cmd (&maven_exist, &gradle_exist, &maven_daemon_exist);

  chsrc_yield_source_and_confirm (pl_java);

  if (maven_exist)
    {
      char *file = xy_str_gsub (RAWSTR_pl_java_maven_config, "@1@", source.mirror->code);
            file = xy_str_gsub (file, "@name@", source.mirror->name);
            file = xy_str_gsub (file, "@url@", source.url);
      char *maven_config = pl_java_find_maven_config ();
      chsrc_note2 (xy_strjoin (3, "请在 maven 配置文件 ", maven_config, " 中添加:"));
      println (file);
    }

  if (maven_daemon_exist)
    {
      char *file = xy_str_gsub (RAWSTR_pl_java_maven_config, "@1@", source.mirror->code);
              file = xy_str_gsub (file, "@name@", source.mirror->name);
              file = xy_str_gsub (file, "@url@", source.url);
      char *maven_config = pl_java_find_maven_daemon_config ();
      chsrc_note2 (xy_strjoin (3, "请在 maven 配置文件 ", maven_config, " 中添加:"));
      println (file);
    }

  if (gradle_exist)
    {
      if (maven_exist) br();
      char* file = xy_str_gsub (RAWSTR_pl_java_build_gradle, "@url@", source.url);
      chsrc_note2 ("请在 build.gradle 中添加:");
      println (file);
    }


  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


void
pl_java_resetsrc (char *option)
{
  pl_java_setsrc (option);
}


Feature_t
pl_java_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NA;
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(pl_java);