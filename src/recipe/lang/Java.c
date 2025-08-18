/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_java, "java/maven/mvn/gradle");

void
pl_java_prelude ()
{
  use_this(pl_java);
  chef_allow_gsr(pl_java);

  chef_set_created_on   (this, "2023-08-31");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-12-18");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 1,
    "BingChunMoLi", "bingchunmoli@bingchunmoli.com");

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
pl_java_check_cmd (bool *maven_exist, bool *gradle_exist)
{
  *maven_exist  = chsrc_check_program ("mvn");
  *gradle_exist = chsrc_check_program ("gradle");

  if (! *maven_exist && ! *gradle_exist)
    {
      chsrc_error ("maven 与 gradle 命令均未找到，请检查是否存在其一");
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


void
pl_java_getsrc (char *option)
{
  bool maven_exist, gradle_exist;
  pl_java_check_cmd (&maven_exist, &gradle_exist);
  char *maven_config = pl_java_find_maven_config ();
  chsrc_note2 (xy_2strjoin ("请查看 ", maven_config));
}


/**
 * @consult https://developer.aliyun.com/mirror/maven
 */
void
pl_java_setsrc (char *option)
{
  bool maven_exist, gradle_exist;
  pl_java_check_cmd (&maven_exist, &gradle_exist);

  use_this_source(pl_java);

  if (maven_exist)
    {
      char *file = xy_str_gsub (RAWSTR_pl_java_maven_config, "@1@", source.mirror->code);
            file = xy_str_gsub (file, "@name@", source.mirror->name);
            file = xy_str_gsub (file, "@url@", source.url);
      char *maven_config = pl_java_find_maven_config ();
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

  chsrc_conclude (&source);
}


void
pl_java_resetsrc (char *option)
{
  pl_java_setsrc (option);
}