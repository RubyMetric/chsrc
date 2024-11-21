/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-31>
 * Last Modified : <2024-08-09>
 * ------------------------------------------------------------*/

/**
 * @time 2024-04-18 更新
 * @note 缺少教育网或开源社区软件源
 */
static SourceInfo
pl_java_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://maven.aliyun.com/repository/public/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/maven/"},
  {&Netease,       "http://mirrors.163.com/maven/repository/maven-public/"} // 网易的24小时更新一次
};
def_sources_n(pl_java);


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
  char *buf = xy_run ("mvn -v", 2, NULL);
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
 * Java 换源，参考：https://developer.aliyun.com/mirror/maven
 */
void
pl_java_setsrc (char *option)
{
  bool maven_exist, gradle_exist;
  pl_java_check_cmd (&maven_exist, &gradle_exist);

  chsrc_yield_source_and_confirm (pl_java);

  if (maven_exist)
    {
      const char *file = xy_strjoin (7,
      "<mirror>\n"
      "  <id>", source.mirror->code, "</id>\n"
      "  <mirrorOf>*</mirrorOf>\n"
      "  <name>", source.mirror->name, "</name>\n"
      "  <url>", source.url, "</url>\n"
      "</mirror>");

      char *maven_config = pl_java_find_maven_config ();
      chsrc_note2 (xy_strjoin (3, "请在您的 maven 配置文件 ", maven_config, " 中添加:"));
      puts (file);
    }

  if (gradle_exist)
    {
      if (maven_exist) br();
      const char* file = xy_strjoin (3,
      "allprojects {\n"
      "  repositories {\n"
      "    maven { url '", source.url, "' }\n"
      "    mavenLocal()\n"
      "    mavenCentral()\n"
      "  }\n"
      "}");

      chsrc_note2 ("请在您的 build.gradle 中添加:");
      puts (file);
    }
  chsrc_conclude (&source, SetsrcType_Manual);
}

def_target(pl_java);
