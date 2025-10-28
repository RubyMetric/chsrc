/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_java, "java/maven/mvn/maven-daemon/mvnd/gradle");

void
pl_java_prelude ()
{
  chef_prep_this (pl_java, gsr);

  chef_set_created_on   (this, "2023-08-31");
  chef_set_last_updated (this, "2025-08-27");
  chef_set_sources_last_updated (this, "2024-12-18");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@BingChunMoLi", "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_deny_english(this);
  chef_allow_user_define(this);

  // 阿里巴巴开源镜像站需要修改为此才能测速
  // https://github.com/RubyMetric/chsrc/issues/238#issuecomment-3162367686
  chef_set_user_agent ("Maven/3.9.11");

  def_sources_begin()
  {&UpstreamProvider, "https://repo1.maven.org/maven2/",                                 FeedByPrelude},
  {&Ali,              "https://maven.aliyun.com/repository/public/",                     FeedByPrelude},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/maven/",               FeedByPrelude},
  {&HuaweiCDN,        "https://repo.huaweicloud.com/repository/maven/",                  FeedByPrelude},
  {&Tencent,          "https://mirrors.cloud.tencent.com/nexus/repository/maven-public/",FeedByPrelude},
  // 网易的24小时更新一次
  {&Netease,          "http://mirrors.163.com/maven/repository/maven-public/",           FeedByPrelude}
  def_sources_end()

  // 220MB
  chef_set_sources_speed_measure_url_with_postfix (this, "com/tencentcloudapi/tencentcloud-sdk-java/3.1.1033/tencentcloud-sdk-java-3.1.1033-javadoc.jar");
}


void
pl_java_check_cmd (bool *maven_exist, bool *gradle_exist, bool *maven_daemon_exist)
{
  *maven_exist  = chsrc_check_program ("mvn");
  *gradle_exist = chsrc_check_program ("gradle");
  *maven_daemon_exist = chsrc_check_program ("mvnd");

  if (! *maven_exist && ! *gradle_exist && ! *maven_daemon_exist)
    {
      chsrc_error ("maven(maven-daemon) 与 gradle 命令均未找到，请检查是否存在其一");
      exit (Exit_UserCause);
    }
}


bool
pl_java_is_maven_home_line (const char *line)
{
  if (xy_str_start_with (line, "Maven home:"))
    return true;
  else
    return false;
}


/**
 * @consult https://github.com/RubyMetric/chsrc/pull/268#issuecomment-3209071819
 */
char *
pl_java_find_maven_config ()
{
  char *output;
  int status = xy_run_get_stdout ("mvn -v", &output);
  if (0==status)
    {
      char *maven_home_line = xy_run_iter_lines ("mvn -v", 0, pl_java_is_maven_home_line);
      char *maven_home = xy_str_delete_prefix (maven_home_line, "Maven home: ");
      char *maven_config = xy_normalize_path (xy_2strcat (maven_home, "/conf/settings.xml"));
      return maven_config;
    }

  chsrc_alert2 ("未找到 maven home, 将使用用户配置文件");
  return "~/.m2/settings.xml";
}

char *
pl_java_find_maven_daemon_config ()
{
  char *output;
  int status = xy_run_get_stdout ("mvnd -v", &output);
  if (0==status)
    {
      char *maven_home_line = xy_run_iter_lines ("mvnd -v", 0, pl_java_is_maven_home_line);
      char *maven_home = xy_str_delete_prefix (maven_home_line, "Maven home: ");
      char *maven_config = xy_normalize_path (xy_2strcat (maven_home, "/conf/settings.xml"));
      return maven_config;
    }

  chsrc_alert2 ("未找到 maven home, 将使用用户配置文件");
  return "~/.m2/settings.xml";
}


void
pl_java_getsrc (char *option)
{
  bool maven_exist, gradle_exist, maven_daemon_exist;
  pl_java_check_cmd (&maven_exist, &gradle_exist, &maven_daemon_exist);
  if (maven_exist)
    {
      char *maven_config = pl_java_find_maven_config ();
      chsrc_note2 (xy_2strcat ("请查看 ", maven_config));
    }
  if (maven_daemon_exist)
    {
      char *maven_config = pl_java_find_maven_daemon_config ();
      chsrc_note2 (xy_2strcat ("请查看 ", maven_config));
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

  chsrc_use_this_source(pl_java);
  if (maven_exist)
    {
      char *file = xy_str_gsub (RAWSTR_pl_java_maven_config, "@1@", source.mirror->code);
            file = xy_str_gsub (file, "@name@", source.mirror->name);
            file = xy_str_gsub (file, "@url@", source.url);
      char *maven_config = pl_java_find_maven_config ();
      chsrc_note2 (xy_strcat (3, "请在 maven 配置文件 ", maven_config, " 中添加:"));
      println (file);
    }

  if (maven_daemon_exist)
    {
      char *file = xy_str_gsub (RAWSTR_pl_java_maven_config, "@1@", source.mirror->code);
            file = xy_str_gsub (file, "@name@", source.mirror->name);
            file = xy_str_gsub (file, "@url@", source.url);
      char *maven_daemon_config = pl_java_find_maven_daemon_config ();
      chsrc_note2 (xy_strcat (3, "请在 maven daemon 配置文件 ", maven_daemon_config, " 中添加:"));
      println (file);
    }

  if (gradle_exist)
    {
      if (maven_exist) br();
      char* file = xy_str_gsub (RAWSTR_pl_java_build_gradle, "@url@", source.url);
      chsrc_note2 ("请在 build.gradle 中添加:");
      println (file);
    }

  chsrc_conclude (&source);
}


void
pl_java_resetsrc (char *option)
{
  pl_java_setsrc (option);
}
