/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_java, "java/maven/mvn/maven-daemon/mvnd/gradle");

void
pl_java_prelude ()
{
  chef_prep_this (pl_java, gsr);

  chef_set_created_on   (this, "2023-08-31");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-12-18");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@BingChunMoLi");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://repo1.maven.org/maven2/",                                  DelegateToUpstream},
  {&Ali,              "https://maven.aliyun.com/repository/public/",                      DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/maven/",                "https://mirrors.huaweicloud.com/repository/maven/com/tencentcloudapi/tencentcloud-sdk-java/3.1.1033/tencentcloud-sdk-java-3.1.1033-javadoc.jar"},
  {&HuaweiCdn,        "https://repo.huaweicloud.com/repository/maven/",                   "https://repo.huaweicloud.com/repository/maven/com/tencentcloudapi/tencentcloud-sdk-java/3.1.1033/tencentcloud-sdk-java-3.1.1033-javadoc.jar"},
  {&Tencent,          "https://mirrors.cloud.tencent.com/nexus/repository/maven-public/", "https://mirrors.cloud.tencent.com/nexus/repository/maven-public/com/tencentcloudapi/tencentcloud-sdk-java/3.1.1033/tencentcloud-sdk-java-3.1.1033-javadoc.jar"},
  // 网易的24小时更新一次
  {&Netease,          "http://mirrors.163.com/maven/repository/maven-public/",            "https://mirrors.163.com/maven/repository/maven-public/com/tencentcloudapi/tencentcloud-sdk-java/3.1.1033/tencentcloud-sdk-java-3.1.1033-javadoc.jar"}
  def_sources_end()
}


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

bool
pl_java_is_home (const char *str)
{
  return (xy_str_start_with (str, "Maven home:"));
}

bool
pl_java_find_maven_home (const char *line)
{
  if(pl_java_is_home(line))
    {
      return true;
    }
  return false;
}

char *
pl_java_find_maven_config ()
{
  char *output;
  int status = xy_run_capture ("mvn -v", &output);
  if (0==status)
  {
    char *maven_home_line = xy_run_iter_lines ("mvn -v", 0, pl_java_find_maven_home);
    char *maven_home = xy_str_delete_prefix (maven_home_line, "Maven home: ");
    char *maven_config = xy_normalize_path (xy_2strcat (maven_home, "/conf/settings.xml"));
    return maven_config;
  }
  else
  {
    printf("no maven home");
  }
  return "~/.m2/settings.xml";
}

char *
pl_java_find_maven_daemon_config ()
{
  char *output;
  int status = xy_run_capture ("mvnd -v", &output);
  if (0==status)
  {
     char *maven_home_line = xy_run_iter_lines ("mvnd -v", 0, pl_java_find_maven_home);
     char *maven_home = xy_str_delete_prefix (maven_home_line, "Maven home: ");
     char *maven_config = xy_normalize_path (xy_2strcat (maven_home, "/conf/settings.xml"));
     return maven_config;
  }
  else
  {
    printf ("no maven home");
  }
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
  if(maven_daemon_exist)
  {
    char *maven_config = pl_java_find_maven_daemon_config ();
    chsrc_note2 (xy_2strcat ("请查看 ", maven_config));
  }
}


void use_custom_user_agent() {
  chsrc_custom_user_agent("Maven/3.9.11");
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
  use_custom_user_agent();
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
      char *maven_config = pl_java_find_maven_daemon_config ();
      chsrc_note2 (xy_strcat (3, "请在 maven 配置文件 ", maven_config, " 中添加:"));
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