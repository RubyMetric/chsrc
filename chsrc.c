/** ------------------------------------------------------------
 * File          : chsrc.c
 * License       : GPLv3
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-08-28>
 * Last modified : <2023-09-24>
 *
 * chsrc:
 *
 *   Change Source —— 命令行换源工具
 *
 *   该软件为自由软件，采用 GPLv3 许可证，请查阅 LICENSE.txt 文件
 * ------------------------------------------------------------*/

#define Chsrc_Version "v0.1.1-20230924"

#include "chsrc.h"

void
pl_ruby_getsrc (char* option)
{
  char* cmd = "gem sources";
  chsrc_runcmd(cmd);
  cmd = "bundle config get mirror.https://rubygems.org";
  chsrc_runcmd(cmd);
}

/**
 * Ruby换源，参考：https://gitee.com/RubyKids/rbenv-cn
 */
void
pl_ruby_setsrc (char* option)
{
  int index = 0;
  char* check_cmd = xy_str_to_quietcmd("gem -v");
  bool exist = does_the_program_exist (check_cmd, "gem");
  if (!exist) {
    xy_error ("chsrc: 未找到 gem 命令，请检查是否存在");
    return;
  }

  if (NULL!=option) {
    index = lets_find_mirror(pl_ruby, option);
  } else {
    index = lets_test_speed(pl_ruby);
  }

  source_info source = pl_ruby_sources[index];
  chsrc_say_selection (&source);

  char* cmd = NULL;

  FILE* fp = popen("gem sources -l", "r");
  char buf[512] = {0};
  while(NULL!=fgets(buf, 512, fp)) {
    if (xy_str_start_with(buf, "http")){
      cmd = xy_str_delete_suffix(buf, "\n");
      cmd = xy_2strjoin("gem sources -r ", cmd);
      chsrc_runcmd(cmd);
    }
    memset(buf, 0, 512);
  }
  pclose(fp);

  cmd = xy_2strjoin("gem source -a ", source.url);
  chsrc_runcmd(cmd);

  check_cmd = xy_str_to_quietcmd("bundle -v");
  exist = does_the_program_exist (check_cmd, "bundle");
  if (!exist) {
    xy_error ("chsrc: 未找到 bundle 命令，请检查是否存在");
    return;
  }

  cmd = xy_2strjoin("bundle config 'mirror.https://rubygems.org' ", source.url);
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
  puts("");
  xy_warn("chsrc: 维护者提醒您: Ruby的镜像源目前仅有腾讯和RubyChina实现正确");
  xy_warn("chsrc: 而其它如Tuna,Bfsu,Ali目前都实现的有问题，请勿使用");
}



/**
 * @param[out] prog 返回 Python 的可用名，如果不可用，则返回 NULL
 */
void
pl_python_check_cmd_ (char** prog)
{
  *prog = NULL;
  // 不要调用 python 自己，而是使用 python --version，避免Windows弹出Microsoft Store
  char* check_cmd = xy_str_to_quietcmd("python --version");
  bool exist = does_the_program_exist (check_cmd, "python");

  if (!exist) {
    check_cmd = xy_str_to_quietcmd("python3 --version");
    exist = does_the_program_exist (check_cmd, "python3");
    if (exist) *prog = "python3";
  }
  else {
    *prog = "python";
  }

  if (!exist) {
    xy_error ("chsrc: 未找到 Python 相关命令，请检查是否存在");
    exit(1);
  }
}

void
pl_python_getsrc (char* option)
{
  char* prog = NULL;
  pl_python_check_cmd_ (&prog);
  char* cmd = xy_2strjoin(prog, " -m pip config get global.index-url");
  chsrc_runcmd(cmd);
}

/**
 * Python换源，参考：https://mirrors.tuna.tsinghua.edu.cn/help/pypi/
 *
 * 经测试，Windows上调用换源命令，会写入 C:\Users\RubyMetric\AppData\Roaming\pip\pip.ini
 */
void
pl_python_setsrc (char* option)
{
  int index = 0;
  char* prog = NULL;
  pl_python_check_cmd_ (&prog);

  if (NULL!=option) {
    index = lets_find_mirror (pl_python, option);
  } else {
    index = lets_test_speed (pl_python);
  }

  source_info source = pl_python_sources[index];
  chsrc_say_selection(&source);

  char* cmd = xy_2strjoin(prog, xy_2strjoin(" -m pip config set global.index-url ", source.url));
  chsrc_runcmd(cmd);
  chsrc_say_thanks(&source);
}



void
pl_nodejs_check_cmd_ (bool* npm_exist, bool* yarn_exist)
{
  char* check_cmd = xy_str_to_quietcmd("npm -v");
  *npm_exist = does_the_program_exist (check_cmd, "npm");

  check_cmd = xy_str_to_quietcmd("yarn -v");
  *yarn_exist = does_the_program_exist (check_cmd, "yarn");

  if (!*npm_exist && !*yarn_exist) {
    xy_error ("chsrc: 未找到 npm 或 yarn 命令，请检查是否存在（其一）");
    exit(1);
  }
}


void
pl_nodejs_getsrc (char* option)
{
  bool npm_exist, yarn_exist;
  pl_nodejs_check_cmd_ (&npm_exist, &yarn_exist);

  if (npm_exist)
  {
    char* cmd = "npm config get registry";
    chsrc_runcmd(cmd);
  }
  if (yarn_exist)
  {
    char* cmd = "yarn config get registry";
    chsrc_runcmd(cmd);
  }
}

/**
 * NodeJS换源，参考：https://npmmirror.com/
 */
void
pl_nodejs_setsrc (char* option)
{
  bool npm_exist, yarn_exist;
  pl_nodejs_check_cmd_ (&npm_exist, &yarn_exist);

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_nodejs, option);
  } else {
    index = lets_test_speed (pl_nodejs);
  }

  source_info source = pl_nodejs_sources[index];
  chsrc_say_selection (&source);

  if (npm_exist)
  {
    char* cmd = xy_2strjoin("npm config set registry ", source.url);
    chsrc_runcmd(cmd);
  }

  if (yarn_exist)
  {
    char* cmd = xy_str_to_quietcmd(xy_2strjoin("yarn config set registry ", source.url));
    chsrc_runcmd(cmd);
  }
  chsrc_say_thanks(&source);
}



void
pl_perl_check_cmd_ ()
{
  char* check_cmd = xy_str_to_quietcmd("perl --version");
  bool exist = does_the_program_exist (check_cmd, "perl");

  if (!exist) {
    xy_error ("chsrc: 未找到 perl 命令，请检查是否存在");
    exit(1);
  }
}

void
pl_perl_getsrc (char* option)
{
  pl_perl_check_cmd_ ();
  // @ccmywish: 注意，prettyprint 仅仅是一个内部实现，可能不稳定，如果需要更稳定的，
  //            可以使用 CPAN::Shell->o('conf', 'urllist');
  //            另外，上述两种方法无论哪种，都要首先load()
  char* cmd = "perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->prettyprint('urllist')\" ";
  chsrc_runcmd(cmd);
}

/**
 * Perl换源，参考：https://help.mirrors.cernet.edu.cn/CPAN/
 */
void
pl_perl_setsrc (char* option)
{
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_perl, option);
  } else {
    index = lets_test_speed (pl_perl);
  }

  source_info source = pl_perl_sources[index];
  chsrc_say_selection (&source);

  char* cmd = xy_strjoin(3,
  "perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('urllist', 'unshift', '", source.url, "'); CPAN::HandleConfig->commit()\"");
  chsrc_runcmd(cmd);

  xy_warn ("chsrc: 请您使用 perl -v 以及 cpan -v，若 Perl >= v5.36 或 CPAN >= 2.29，请额外手动调用下面的命令");
  xy_warn ("       perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('pushy_https', 0);; CPAN::HandleConfig->commit()\"");
  chsrc_say_thanks(&source);
}



void
pl_php_check_cmd_()
{
  char* check_cmd = xy_str_to_quietcmd("composer --version");
  bool exist = does_the_program_exist (check_cmd, "composer");

  if (!exist) {
    xy_error ("chsrc: 未找到 composer 命令，请检查是否存在");
    exit(1);
  }
}

/**
 * 已在Windows上测试通过，待其他平台PHP用户确认
 */
void
pl_php_getsrc (char* option)
{
  pl_php_check_cmd_ ();
  char* cmd = "composer config -g repositories";
  chsrc_runcmd(cmd);
}

/**
 * PHP 换源，参考：https://developer.aliyun.com/composer
 */
void
pl_php_setsrc (char* option)
{
  pl_php_check_cmd_();

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_php, option);
  } else {
    index = lets_test_speed (pl_php);
  }

  source_info source = pl_php_sources[index];
  chsrc_say_selection (&source);

  char* cmd = xy_2strjoin("composer config -g repo.packagist composer ", source.url);
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
}



void
pl_go_check_cmd_ ()
{
  char* check_cmd = xy_str_to_quietcmd("go version");
  bool exist = does_the_program_exist (check_cmd, "go");

  if (!exist) {
    xy_error ("chsrc: 未找到 go 相关命令，请检查是否存在");
    exit(1);
  }
}

void
pl_go_getsrc (char* option)
{
  pl_go_check_cmd_ ();
  char* cmd = "go env GOPROXY";
  chsrc_runcmd(cmd);
}

/**
 * Go换源，参考：https://goproxy.cn/
 */
void
pl_go_setsrc (char* option)
{
  pl_go_check_cmd_();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_go, option);
  } else {
    index = lets_test_speed (pl_go);
  }

  source_info source = pl_go_sources[index];
  chsrc_say_selection (&source);

  char* cmd = "go env -w GO111MODULE=on";
  chsrc_runcmd(cmd);

  cmd = xy_strjoin(3, "go env -w GOPROXY=", source.url, ",direct");
  chsrc_runcmd(cmd);
  chsrc_say_thanks(&source);
}



void
pl_rust_getsrc (char* option)
{
  char* cmd = NULL;
  if(xy_on_windows) {
    cmd = "type %USERPROFILE%\\.cargo";
  } else {
    cmd = "cat  ~/.cargo";
  }
  chsrc_runcmd(cmd);
}

/**
 * Rust 换源，参考：https://mirrors.tuna.tsinghua.edu.cn/help/crates.io-index/
 */
void
pl_rust_setsrc (char* option)
{
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_rust, option);
  } else {
    index = lets_test_speed (pl_rust);
  }

  source_info source = pl_rust_sources[index];
  chsrc_say_selection(&source);

  const char* file = xy_strjoin (3,
    "[source.crates-io]\n"
    "replace-with = 'mirror'\n\n"

    "[source.mirror]\n"
    "registry = \"sparse+", source.url, "\"");

  xy_warn ("chsrc: 请您手动写入以下内容到 ~/.cargo 文件中");
  puts(file);
  chsrc_say_thanks(&source);
}



void
pl_dotnet_getsrc (char* option)
{
  xy_error ("chsrc: 暂时无法查看NuGet源，若有需求，请您提交issue");
}

/**
 * NuGet 换源
 */
void
pl_dotnet_setsrc (char* option)
{
  xy_error ("chsrc: 暂时无法为NuGet换源，若有需求，请您提交issue");
}



void
pl_java_check_cmd_(bool* maven_exist, bool* gradle_exist)
{
  char* check_cmd = NULL;
  check_cmd    = xy_str_to_quietcmd("mvn --version");
  *maven_exist = does_the_program_exist (check_cmd, "mvn");

  check_cmd     = xy_str_to_quietcmd("gradle --version");
  *gradle_exist = does_the_program_exist (check_cmd, "gradle");

  if (! *maven_exist && ! *gradle_exist) {
    xy_error ("chsrc: maven 与 gradle 命令均未找到，请检查是否存在（其一）");
    exit(1);
  }
}

char*
pl_java_find_maven_config_ ()
{
  FILE* fp = popen("mvn -v", "r");
  char buf[512];
  fgets(buf, 512, fp);
  memset(buf, 0, 512);
  fgets(buf, 512, fp);
  pclose(fp);
  char* maven_home = xy_str_delete_prefix(buf, "Maven home: ");
  // xy_info (buf);
  maven_home = xy_str_strip(maven_home);

  char* maven_config = NULL;

  if (xy_on_windows)
    maven_config = xy_2strjoin(maven_home, "\\conf\\settings.xml");
  else
    maven_config = xy_2strjoin(maven_home, "/conf/settings.xml");

  return maven_config;
}

void
pl_java_getsrc (char* option)
{
  bool maven_exist, gradle_exist;
  pl_java_check_cmd_ (&maven_exist, &gradle_exist);
  char* maven_config = pl_java_find_maven_config_();

  char* echo = xy_2strjoin("chsrc: 请查看 ", maven_config);
  xy_info (echo);
  // system(cmd);
}

/**
 * Java 换源，参考：https://developer.aliyun.com/mirror/maven
 */
void
pl_java_setsrc (char* option)
{
  bool maven_exist, gradle_exist;
  pl_java_check_cmd_ (&maven_exist, &gradle_exist);

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_java, option);
  } else {
    index = lets_test_speed (pl_java);
  }

  source_info source = pl_java_sources[index];
  chsrc_say_selection(&source);

  if (maven_exist) {
    const char* file = xy_strjoin(7,
    "<mirror>\n"
    "  <id>", source.mirror->code, "</id>\n"
    "  <mirrorOf>*</mirrorOf>\n"
    "  <name>", source.mirror->name, "</name>\n"
    "  <url>", source.url, "</url>\n"
    "</mirror>");

    char* maven_config = pl_java_find_maven_config_();
    char* echo = xy_strjoin(3, "chsrc: 请在您的 maven 配置文件 ", maven_config, " 中添加:");
    xy_info(echo);
    puts (file);
  }

  if (gradle_exist) {
    if (maven_exist) puts("");
    const char* file = xy_strjoin(3,
    "allprojects {\n"
    "  repositories {\n"
    "    maven { url '", source.url, "' }\n"
    "    mavenLocal()\n"
    "    mavenCentral()\n"
    "  }\n"
    "}");

    xy_info ("chsrc: 请在您的 build.gradle 中添加:");
    puts (file);
  }
  chsrc_say_thanks(&source);
}



void
pl_clojure_setsrc(char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(pl_clojure, option);
  } else {
    index = lets_test_speed(pl_clojure);
  }

  source_info source = pl_clojure_sources[index];
  chsrc_say_selection (&source);

  xy_warn("chsrc: 抱歉，Clojure换源较复杂，您可手动查阅并换源:");
  puts(source.url);
  chsrc_say_thanks (&source);
}



void
pl_dart_getsrc(char* option)
{
  char* cmd = NULL;
  if (xy_on_windows) {
    cmd = "set PUB_HOSTED_URL & set FLUTTER_STORAGE_BASE_URL";
    chsrc_runcmd(cmd);
  } else {
    cmd = "echo $PUB_HOSTED_URL; echo $FLUTTER_STORAGE_BASE_URL";
    chsrc_runcmd(cmd);
  }
}

/**
 * Dart pub 换源，参考：https://mirrors.tuna.tsinghua.edu.cn/help/dart-pub/
 */
void
pl_dart_setsrc (char* option)
{
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_dart, option);
  } else {
    index = lets_test_speed (pl_dart);
  }

  source_info source = pl_dart_sources[index];
  chsrc_say_selection(&source);

  char* cmd = NULL;

  char* pub = xy_2strjoin(source.url, "dart-pub");
  char* flutter = xy_2strjoin(source.url, "flutter");

  if (xy_on_windows)
  {
    if (xy_file_exist(xy_win_powershell_profile))
    {
      chsrc_runcmd(xy_strjoin(4, "echo $env:PUB_HOSTED_URL = \"", pub, "\" >> ", xy_win_powershell_profile));
      chsrc_runcmd(xy_strjoin(4, "echo $env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\" >> ", xy_win_powershell_profile));
    }

    if (xy_file_exist(xy_win_powershellv5_profile))
    {
      chsrc_runcmd(xy_strjoin(4, "echo $env:PUB_HOSTED_URL = \"", pub, "\" >> ", xy_win_powershellv5_profile));
      chsrc_runcmd(xy_strjoin(4, "echo $env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\" >> ", xy_win_powershell_profile));
    }
  }

  else
  {
    cmd = xy_strjoin(3, "echo 'export PUB_HOSTED_URL=\"", pub, "\"' >> ~/.bashrc >> ~/.zshrc");
    chsrc_runcmd(cmd);
    cmd = xy_strjoin(3, "export 'FLUTTER_STORAGE_BASE_URL=\"", flutter, "\"' >> ~/.bashrc >> ~/.zshrc");
    chsrc_runcmd(cmd);
  }
  chsrc_say_thanks(&source);
}



void
pl_haskell_setsrc(char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(pl_haskell, option);
  } else {
    index = lets_test_speed(pl_haskell);
  }

  source_info source = pl_haskell_sources[index];
  chsrc_say_selection (&source);

  char* file = xy_strjoin(3, "repository mirror\n"
                             "  url: ", source.url,
                          "\n  secure: True");

  char* config = NULL;
  if (xy_on_windows) {
    config = xy_2strjoin(xy_os_home, "\\AppData\\Roaming\\cabal\\config");
  } else {
    config = xy_2strjoin(xy_os_home, "/.cabal/config");
  }

  xy_info(xy_strjoin(3,"chsrc: 请向 ", config, " 中手动添加:"));
  puts(file); puts("");

  config = xy_2strjoin(xy_os_home, "/.stack/config.yaml");
  file = xy_strjoin(3, "package-indices:\n"
                       "  - download-prefix: ", source.url,
                     "\n    hackage-security:\n"
                       "        keyids:\n"
                       "        - 0a5c7ea47cd1b15f01f5f51a33adda7e655bc0f0b0615baa8e271f4c3351e21d\n"
                       "        - 1ea9ba32c526d1cc91ab5e5bd364ec5e9e8cb67179a471872f6e26f0ae773d42\n"
                       "        - 280b10153a522681163658cb49f632cde3f38d768b736ddbc901d99a1a772833\n"
                       "        - 2a96b1889dc221c17296fcc2bb34b908ca9734376f0f361660200935916ef201\n"
                       "        - 2c6c3627bd6c982990239487f1abd02e08a02e6cf16edb105a8012d444d870c3\n"
                       "        - 51f0161b906011b52c6613376b1ae937670da69322113a246a09f807c62f6921\n"
                       "        - 772e9f4c7db33d251d5c6e357199c819e569d130857dc225549b40845ff0890d\n"
                       "        - aa315286e6ad281ad61182235533c41e806e5a787e0b6d1e7eef3f09d137d2e9\n"
                       "        - fe331502606802feac15e514d9b9ea83fee8b6ffef71335479a2e68d84adc6b0\n"
                       "        key-threshold: 3\n"
                       "        ignore-expiry: no");
  xy_info(xy_strjoin(3,"chsrc: 请向 ", config, " 中手动添加:"));
  puts(file);
  chsrc_say_thanks (&source);
}



void
pl_ocaml_check_cmd_()
{
  char* check_cmd = xy_str_to_quietcmd("opam --version");
  bool exist = does_the_program_exist (check_cmd, "opam");

  if (!exist) {
    xy_error ("chsrc: 未找到 opam 命令，请检查是否存在");
    exit(1);
  }
}

void
pl_ocaml_getsrc(char* option)
{
  pl_ocaml_check_cmd_();
  char* cmd = "opam repo get-url default";
  chsrc_runcmd(cmd);
}

/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/git/opam-repository.git
 */
void
pl_ocaml_setsrc(char* option)
{
  pl_ocaml_check_cmd_();

  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(pl_ocaml, option);
  } else {
    index = lets_test_speed(pl_ocaml);
  }

  source_info source = pl_ocaml_sources[index];
  chsrc_say_selection (&source);

  char* cmd = xy_strjoin(3, "opam repo set-url default ",
    source.url,
    " --all --set-default");

  chsrc_runcmd(cmd);

  xy_info("chsrc: 如果是首次使用 opam ，请使用以下命令进行初始化");
  puts(xy_2strjoin("opam init default ", source.url));

  chsrc_say_thanks (&source);
}



void
pl_r_getsrc (char* option)
{
  // 或参考：https://zhuanlan.zhihu.com/p/585036231
  //
  // options()$repos
  // options()$BioC_mirror
  //
  char* cmd = NULL;
  if(xy_on_windows) {
    cmd = "type %USERPROFILE%\\Documents\\.Rprofile";
  } else {
    cmd = "cat ~/.Rprofile";
  }
  chsrc_runcmd(cmd);
}

/**
 * R 换源，参考：https://help.mirrors.cernet.edu.cn/CRAN/
 */
void
pl_r_setsrc (char* option)
{
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_r, option);
  } else {
    index = lets_test_speed (pl_r);
  }

  source_info source = pl_r_sources[index];
  chsrc_say_selection(&source);

  char* bioconductor_url = xy_str_delete_suffix(xy_str_delete_suffix(source.url, "cran/"), "CRAN/");
  bioconductor_url = xy_2strjoin(bioconductor_url, "bioconductor");

  const char* file = xy_strjoin (3, "options(\"repos\" = c(CRAN=\"", source.url, "\"))" );

  char* cmd = NULL;
  // 或者我们调用 r.exe --slave -e 上面的内容
  if (xy_on_windows)
    cmd = xy_strjoin(3, "echo ", file, " >> %USERPROFILE%/Documents/.Rprofile");
  else
    cmd = xy_strjoin(3, "echo '", file, "' >> ~/.Rprofile");

  chsrc_runcmd(cmd);

  file = xy_strjoin (3, "options(BioC_mirror=\"", bioconductor_url, "\")" );
  // 或者我们调用 r.exe --slave -e 上面的内容
  if (xy_on_windows)
    cmd = xy_strjoin(3, "echo ", file, " >> %USERPROFILE%/Documents/.Rprofile");
  else
    cmd = xy_strjoin(3, "echo '", file, "' >> ~/.Rprofile");

  chsrc_runcmd(cmd);
  chsrc_say_thanks(&source);
}



/**
 * Julia的换源可以通过两种方式
 * 1. 写入 startup.jl
 * 2. 使用环境变量
 *
 * 我们采用第一种
 */
void
pl_julia_getsrc (char* option)
{
  char* cmd = NULL;
  if(xy_on_windows) {
    cmd = "type %USERPROFILE%\\.julia\\config\\startup.jl";
  } else {
    cmd = "cat  ~/.julia/config/startup.jl";
  }
  chsrc_runcmd(cmd);
}

/**
 * Julia 换源，参考：
 * 1. https://help.mirrors.cernet.edu.cn/julia/
 * 2. https://docs.julialang.org/en/v1/manual/command-line-interface/#Startup-file
 */
void
pl_julia_setsrc (char* option)
{
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_julia, option);
  } else {
    index = lets_test_speed (pl_julia);
  }

  source_info source = pl_julia_sources[index];
  chsrc_say_selection(&source);

  const char* file = xy_strjoin (3, "ENV[\"JULIA_PKG_SERVER\"] = \"", source.url, "\"");

  char* cmd = NULL;
  if (xy_on_windows)
    cmd = xy_strjoin(4, xy_str_to_quietcmd("md %USERPROFILE%\\.julia\\config"),
          "& echo ", file, " >> %USERPROFILE%/.julia/config/startup.jl");
  else
    cmd = xy_strjoin(4, xy_str_to_quietcmd("mkdir -p ~/.julia/config"),
        ";echo '", file, "' >> ~/.julia/config/startup.jl");

  chsrc_runcmd(cmd);
  chsrc_say_thanks(&source);
}







void
os_ubuntu_getsrc(char* option)
{
  char* cmd = "cat /etc/apt/sources.list";
  chsrc_runcmd(cmd);
}

/**
 * @note 不同架构下换源不一样
 */
void
os_ubuntu_setsrc (char* option)
{
  ensure_root();

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_ubuntu, option);
  } else {
    index = lets_test_speed(os_ubuntu);
  }

  source_info source = os_ubuntu_sources[index];
  chsrc_say_selection(&source);

  char* backup = "cp -f /etc/apt/sources.list /etc/apt/sources.list.bak --backup='t'";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* arch = xy_getcmd("arch",NULL);
  char* cmd;
  if(strncmp(arch, "x86_64", 6)==0)
  {
    cmd = xy_strjoin(3,
      "sed -E \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/ubuntu\\/@\\1",
      source.url,
      "/@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");

  }
  else {
    cmd = xy_strjoin(3,
      "sed -E \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/ubuntu\\/@\\1",
      source.url,
      "-ports/@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");
  }

  chsrc_runcmd(cmd);
  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



void
os_debian_getsrc(char* option)
{
  char* cmd = "cat /etc/apt/sources.list";
  chsrc_runcmd(cmd);
}

/**
 * Debian Buster 以上版本默认支持 HTTPS 源。如果遇到无法拉取 HTTPS 源的情况，请先使用 HTTP 源并安装
 * sudo apt install apt-transport-https ca-certificates
 */
void
os_debian_setsrc (char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_debian, option);
  } else {
    index = lets_test_speed(os_debian);
  }


  source_info source = os_debian_sources[index];
  chsrc_say_selection(&source);

  xy_info ("chsrc: 如果遇到无法拉取 HTTPS 源的情况，我们会使用 HTTP 源并 需要您 安装");
  xy_info ("chsrc: sudo apt install apt-transport-https ca-certificates");

  char* backup = "cp -f /etc/apt/sources.list /etc/apt/sources.list.bak --backup='t'";
  chsrc_runcmd(backup);

  char * cmd = xy_strjoin(3,"chsrc: 备份文件名: /etc/apt/.*)http[:|\\.|\\/|a-z|A-Z]*\\/debian\\/@\\1",
                          source.url,
                          "@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");

  chsrc_runcmd(cmd);

  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



void
os_deepin_getsrc(char* option)
{
  char* cmd = "cat /etc/apt/sources.list";
  chsrc_runcmd(cmd);
}

/**
 * HELP: 未经测试
 */
void
os_deepin_setsrc (char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_deepin, option);
  } else {
    index = lets_test_speed(os_deepin);
  }

  source_info source = os_deepin_sources[index];
  chsrc_say_selection(&source);

  char* backup = "cp -f /etc/apt/sources.list /etc/apt/sources.list.bak --backup='t'";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* cmd;
  cmd = xy_strjoin(3,
    "sed -E \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/deepin\\/@\\1",
    source.url,
    "@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");
  chsrc_logcmd(cmd);
  system(cmd);
  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



/**
 * @note fedora 29 及以下版本暂不支持
 */
void
os_fedora_setsrc (char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_fedora, option);
  } else {
    index = lets_test_speed(os_fedora);
  }


  source_info source = os_fedora_sources[index];
  chsrc_say_selection(&source);

  xy_warn ("chsrc: fedora 29 及以下版本暂不支持");

  char* backup = "cp -f /etc/yum.repos.d/fedora.repo /etc/yum.repos.d/fedora.repo.bak --backup='t'";
  chsrc_runcmd(backup);

  backup = "cp -f /etc/yum.repos.d/fedora-updates.repo /etc/yum.repos.d/fedora-updates.repo.bak";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名:1. /etc/yum.repos.d/fedora.repo.bak");
  xy_info ("chsrc: 备份文件名:2. /etc/yum.repos.d/fedora-updates.repo.bak");


  char* cmd = xy_strjoin(9, "sed -e 's|^metalink=|#metalink=|g' ",
         "-e 's|^#baseurl=http://download.example/pub/fedora/linux/|baseurl=",
         source.url,
         "|g' ",
         "-i.bak ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-modular.repo ",
         "/etc/yum.repos.d/fedora-updates.repo ",
         "/etc/yum.repos.d/fedora-updates-modular.repo");

  chsrc_runcmd(cmd);

  xy_info ("chsrc: 替换文件:/etc/yum.repos.d/fedora.repo");
  xy_info ("chsrc: 新增文件:/etc/yum.repos.d/fedora-modular.repo");
  xy_info ("chsrc: 替换文件:/etc/yum.repos.d/fedora-updates.repo");
  xy_info ("chsrc: 新增文件:/etc/yum.repos.d/fedora-updates-modular.repo");

  // char* rm = "rm -rf /etc/yum.repos.d/fedora.repo.bak";
  // system(rm);
  // char* rm = "rm -rf /etc/yum.repos.d/fedora-updates.repo.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



/**
 * HELP: 未经测试
 */
void
os_opensuse_setsrc (char* option)
{
  ensure_root(); // HELP: 不知道是否需要确保root权限
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_opensuse, option);
  } else {
    index = lets_test_speed(os_opensuse);
  }

  source_info source = os_opensuse_sources[index];
  chsrc_say_selection(&source);

  char* source_nselect = "zypper mr -da";
  chsrc_runcmd(source_nselect);

  char* cmd1 = xy_strjoin(3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/repo/oss/' mirror-oss");
  char* cmd2 = xy_strjoin(3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/repo/non-oss/' mirror-non-oss");
  char* cmd3 = xy_strjoin(3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/oss/' mirror-update");
  char* cmd4 = xy_strjoin(3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/non-oss/' mirror-update-non-oss");
  char* cmd5 = xy_strjoin(3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/sle/' mirror-sle-update");
  char* cmd6 = xy_strjoin(3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/backports/' mirror-backports-update");

  chsrc_runcmd(cmd1);
  chsrc_runcmd(cmd2);
  chsrc_runcmd(cmd3);
  chsrc_runcmd(cmd4);

  xy_info("chsrc: leap 15.3用户还需 要添加sle和backports源");
  xy_info("chsrc: 另外请确保系统在更新后仅启用了六个软件源，可以使用 zypper lr 检查软件源状态");
  xy_info("chsrc: 并使用 zypper mr -d 禁用多余的软件源");

  chsrc_runcmd(cmd5);
  chsrc_runcmd(cmd6);
  // char* rm = "rm -f /etc/apt/source.list.bak";
  // chsrc_runcmd(rm);
  chsrc_say_thanks(&source);
}


/**
 * HELP: 未经测试
 */
void
os_kali_setsrc(char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_kali, option);
  } else {
    index = lets_test_speed(os_kali);
  }

  source_info source = os_kali_sources[index];
  chsrc_say_selection(&source);

  char* backup = "cp -f /etc/apt/sources.list /etc/apt/sources.list.bak --backup='t'";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* cmd = xy_strjoin(3, "sed -i \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/kali\\/@\\1",
                          source.url,
                          "@g\' /etc/apt/sources.list");

  chsrc_runcmd(cmd);
  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



/**
 * HELP: 未经测试
 */
void
os_msys2_setsrc(char* option)
{
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_msys2, option);
  } else {
    index = lets_test_speed(os_msys2);
  }


  source_info source = os_msys2_sources[index];
  chsrc_say_selection(&source);


  char* backup = "cp -f /etc/pacman.d/mirrorlist.mingw32 /etc/pacman.d/mirrorlist.mingw32.bak";
  chsrc_runcmd(backup);

  backup = "cp -f /etc/pacman.d/mirrorlist.mingw64 /etc/pacman.d/mirrorlist.mingw64.bak";
  chsrc_runcmd(backup);

  backup = "cp -f /etc/pacman.d/mirrorlist.msys /etc/pacman.d/mirrorlist.msys.bak";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: 1. /etc/pacman.d/mirrorlist.mingw32.bak");
  xy_info ("chsrc: 备份文件名: 2. /etc/pacman.d/mirrorlist.mingw64.bak");
  xy_info ("chsrc: 备份文件名: 3. /etc/pacman.d/mirrorlist.msys.bak");

  char* prev = xy_strjoin(3,"chsrc: 请针对你的架构下载安装此目录下的文件:",
                            source.url,
                            "distrib/<架构>/");
  xy_info (prev);

  char* cmd = xy_strjoin(3,"sed -i \"s#https\?://mirror.msys2.org/#",
                            source.url,
                            "#g\" /etc/pacman.d/mirrorlist* ");

  chsrc_runcmd(cmd);

  // char* rm = "rm -rf /etc/pacman.d/mirrorlist.mingw32.bak";
  // system(rm);
  //       rm = "rm -rf /etc/pacman.d/mirrorlist.mingw64.bak";
  // system(rm);
  //       rm = "rm -rf /etc/pacman.d/mirrorlist.msys.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



/**
 * 参考: https://mirrors.tuna.tsinghua.edu.cn/help/archlinuxcn/
 */
void
os_arch_setsrc(char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_arch, option);
  } else {
    index = lets_test_speed(os_arch);
  }

  source_info source = os_arch_sources[index];
  chsrc_say_selection(&source);


  char* backup = "cp -f /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist.bak --backup='t'";
  chsrc_runcmd(backup);


  bool arch_flag = false;
  xy_info ("chsrc: 备份文件名: /etc/pacman.d/mirrorlist.bak");
  char* new_file ;


  char* arch = xy_getcmd("arch",NULL);
  char* cmd;
  if(strncmp(arch, "x86_64", 6)==0)
  {
    arch_flag = true;
    new_file = xy_strjoin(3,"Server = ",
                                source.url,
                                "archlinux/$repo/os/$arch");
  }
  else {
    arch_flag = false;
    new_file = xy_strjoin(3,"Server = ",
                                source.url,
                                "archlinuxarm/$repo/os/$arch");
  }

  cmd = xy_strjoin(3,"echo ",
                      new_file,
                      " > /etc/pacman.d/mirrorlist");
  chsrc_runcmd(cmd);

  xy_info("chsrc: 使用 archlinuxcn ");

  cmd = xy_strjoin(3, "cat [archlinuxcn] \r\n Server=",source.url,"archlinuxcn/$repo/os/$arch >> /etc/pacman.d/mirrorlist");
  chsrc_runcmd(cmd);

  cmd = "pacman -Sy archlinux-keyring";
  chsrc_runcmd(cmd);

  if(arch_flag) {
    chsrc_runcmd("pacman -Syyu");
  } else {
    chsrc_runcmd("pacman -Syy");
  }
  chsrc_say_thanks(&source);
}



/**
 * HELP: 未经测试
 */
void
os_gentoo_setsrc(char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_gentoo, option);
  } else {
    index = lets_test_speed(os_gentoo);
  }

  source_info source = os_arch_sources[index];
  chsrc_say_selection(&source);


  char* backup = "cp -f /etc/portage/repos.conf/gentoo.conf /etc/portage/repos.conf/gentoo.conf.bak";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/portage/repos.conf/gentoo.conf.bak");
  char* cmd = xy_strjoin(3,"sed -i \"s#rsync[:|\\.|\\/|a-z|A-Z]*/gentoo-portage#rsync://",
                            source.url,
                            "gentoo-portage#g");
  chsrc_runcmd(cmd);

  char * yuan = xy_strjoin(3,"GENTOO_MIRRORS=\"https://",
                              source.url,
                              "gentoo\"");
  cmd = xy_strjoin(3,"cat ",
                      yuan,
                      " >> /etc/portage/make.conf");
  chsrc_runcmd(cmd);

  // char* rm = "rm -rf /etc/portage/repos.conf/gentoo.conf.bak";
  // system(rm);
  chsrc_say_thanks(&source);
}



/**
 * 参考: https://help.mirrors.cernet.edu.cn/rocky/
 */
void
os_rocky_setsrc (char* option)
{
  ensure_root();

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_rocky, option);
  } else {
    index = lets_test_speed(os_rocky);
  }

  source_info source = os_rocky_sources[index];
  chsrc_say_selection(&source);

  char* cmd = xy_strjoin(3,
            "sudo sed -e 's|^mirrorlist=|#mirrorlist=|g' "
            "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
            "-i.bak /etc/yum.repos.d/rocky-extras.repo /etc/yum.repos.d/rocky.repo"
            );
  chsrc_runcmd(cmd);
  chsrc_say_thanks(&source);
}



/**
 * 参考: https://help.mirrors.cernet.edu.cn/alpine/
 */
void
os_alpine_setsrc (char* option)
{
  // ensure_root(); // HELP: 不确定是否需要root

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_alpine, option);
  } else {
    index = lets_test_speed(os_alpine);
  }

  source_info source = os_alpine_sources[index];
  chsrc_say_selection(&source);

  char* cmd = xy_strjoin(3,
            "sed -i 's#https\\?://dl-cdn.alpinelinux.org/alpine#", source.url, "#g' /etc/apk/repositories"
            );
  chsrc_runcmd(cmd);

  cmd = "apk update";
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
}



void
os_void_getsrc (char* option)
{
  char* cmd = "xbps-query -L";
  chsrc_runcmd(cmd);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/voidlinux/
 */
void
os_void_setsrc (char* option)
{
  // ensure_root(); // HELP: 不确定是否需要root

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_void, option);
  } else {
    index = lets_test_speed(os_void);
  }

  source_info source = os_void_sources[index];
  chsrc_say_selection(&source);

  char* cmd = "mkdir -p /etc/xbps.d";
  chsrc_runcmd(cmd);

  cmd = "cp /usr/share/xbps.d/*-repository-*.conf /etc/xbps.d/";
  chsrc_runcmd(cmd);

  cmd = xy_strjoin(3,
            "sed -i 's|https://repo-default.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );
  chsrc_runcmd(cmd);

  cmd = xy_strjoin(3,
            "sed -i 's|https://alpha.de.repo.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );

  xy_warn("chsrc: 若报错可尝试使用以下命令");
  puts(cmd);
  chsrc_say_thanks(&source);
}



/**
 * 似乎会弹出GUI，待确定
 */
void
os_manjaro_setsrc(char* option)
{
  ensure_root();
  char* cmd = "sudo pacman-mirrors -i -c China -m rank";
  chsrc_runcmd(cmd);

  chsrc_runcmd("sudo pacman -Syy");
}



/**
 * HELP: 未经测试
 */
void
os_openeuler_setsrc (char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_openeuler, option);
  } else {
    index = lets_test_speed(os_openeuler);
  }

  source_info source = os_openeuler_sources[index];
  chsrc_say_selection(&source);

  char* backup = "cp -f /etc/yum.repos.d/openEuler.repot /etc/yum.repos.d/openEuler.repo.bak --backup='t'";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/yum.repos.d/openEuler.repo.bak");

  char* cmd;
  cmd = xy_strjoin(3,
    "s#http://repo.openeuler.org#",
    source.url,
    "#\'< /etc/yum.repos.d/openEuler.repo.bak | cat > /etc/yum.repos.d/openEuler.repo");

  chsrc_runcmd(cmd);

  // char* rm = "rm -rf /etc/yum.repos.d/openEuler.repo.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



/**
 * HELP: 未经测试
 */
void
os_openkylin_setsrc (char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_openkylin, option);
  } else {
    index = lets_test_speed(os_openkylin);
  }

  source_info source = os_openkylin_sources[index];
  chsrc_say_selection(&source);

  char* backup = "cp -f /etc/apt/sources.list /etc/apt/sources.list.bak --backup='t'";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* cmd;
  cmd = xy_strjoin(3,
    "sed -E \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/openkylin\\/@\\1",
    source.url,
    "@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");

  chsrc_runcmd(cmd);
  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  chsrc_say_thanks(&source);
}



/**
 * 参考: https://book.bsdcn.org/di-3-zhang-ruan-jian-yuan-ji-bao-guan-li-qi/di-3.2-jie-freebsd-huan-yuan-fang-shi.html
 *
 * 据 @ykla,
 *   FreeBSD 有四类源：pkg、ports、portsnap、update，其中 portsnap 在 FreeBSD 14 已经被移除了
 *
 *   目前缺少update源，所以我们暂时只提供 pkg 和 ports 源的替换
 *
 * HELP: 未经测试
 */
void
os_freebsd_setsrc (char* option)
{
  ensure_root(); // HELP: 不知道是否需要确保root权限
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_freebsd, option);
  } else {
    index = lets_test_speed(os_freebsd);
  }

  source_info source = os_freebsd_sources[index];
  chsrc_say_selection(&source);

  xy_info("chsrc: 1. 添加 freebsd-pkg 源 (二进制安装包)");
  char* pkg_mkdir = "mkdir -p /usr/local/etc/pkg/repos";
  char* pkg_createconf = xy_strjoin(3, "ee /usr/local/etc/pkg/repos/", source.mirror->code, ".conf");
  chsrc_runcmd(pkg_mkdir);
  chsrc_runcmd(pkg_createconf);


  char* pkg_content = xy_strjoin(4,
                      source.mirror->code, ": { \
                      url: \"pkg+=http://", source.url, "/freebsd-pkg/${ABI}/latest\",\
                      mirror_type: \"srv\",\
                      signature_type: \"none\",\
                      fingerprints: \"/usr/share/keys/pkg\",\
                      enabled: yes\
                    }");

  char* pkg_cmd = xy_strjoin(3, "cat ", pkg_content, "> /usr/local/etc/pkg/repos/", source.mirror->code , ".conf");
  chsrc_runcmd(pkg_cmd);

  xy_warn("chsrc: 若要使用HTTPS源，请先安装securtiy/ca_root_ns，并将 'http' 改成 'https' ，最后使用 'pkg update -f' 刷新缓存即可\n");



  xy_info("chsrc: 2. 修改 freebsd-ports 源");
  bool git_exist = does_the_program_exist (xy_str_to_quietcmd("git version"), "git");
  if (git_exist) {
    char* git_cmd = xy_strjoin(3, "git clone --depth 1 https://", source.url, "/freebsd-ports/ports.git /usr/ports");
    chsrc_runcmd(git_cmd);
  } else {
    char* fetch  = xy_strjoin(3, "fetch https://", source.url, "/freebsd-ports/ports.tar.gz");  // 70多MB
    char* unzip  = "tar -zxvf ports.tar.gz -C /usr/ports";
    char* delete = "rm ports.tar.gz";
    chsrc_runcmd(fetch);
    chsrc_runcmd(unzip);
    chsrc_runcmd(delete);
  }

  /* https://help.mirrors.cernet.edu.cn/FreeBSD-ports/ 的换源方法 */
  /*
  char* ports_cp="cp -f /etc/make.conf /etc/make.conf.bak";
  chsrc_runcmd(ports_cp);

  char* ports_cmd =xy_strjoin(3, "cat MASTER_SITE_OVERRIDE?=http://",
                                 source.url,
                                 "/freebsd-ports/ >> /etc/make.conf");
  chsrc_runcmd(ports_cmd);
  */



  /* 不再换 portsnap */
  /*
  char* portsnap_cp="cp -f /etc/portsnap.conf /etc/portsnap.conf.bak";
  chsrc_runcmd(portsnap_cp);

  char* portsnap_cmd =xy_strjoin(3,"s@(.*)SERVERNAME=[\\.|a-z|A-Z]*@\\1SERVERNAME=",
                                source.url,
                                "@g < /etc/portsnap.conf.bak | cat > /etc/portsnap.conf");
  chsrc_runcmd(portsnap_cmd);


  xy_info("chsrc: portsnap sources changed");
  xy_info("chsrc: 获取portsnap更新使用此命令: 'portsnap fetch extract'");
  */



  // HELP: 暂时似乎没有源提供
  /*
  xy_info("chsrc: 3. 修改 freebsd-update 源");
  char* update_cp="cp -f /etc/freebsd-update.conf /etc/freebsd-update.conf.bak";
  chsrc_runcmd(update_cp);


  char* update_cmd =xy_strjoin(3,"s@(.*)SERVERNAME [\\.|a-z|A-Z]*@\\1SERVERNAME ",
                                source.url,
                                "@g < /etc/freebsd-update.conf.bak | cat > /etc/freebsd-update.conf");
  chsrc_runcmd(update_cmd);
   */

  chsrc_say_thanks(&source);
}



void
os_netbsd_getsrc (char* option)
{
  char* cmd = "cat /usr/pkg/etc/pkgin/repositories.conf";
  chsrc_runcmd(cmd);
}

/**
 * 参考:
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/pkgsrc/
 * 2. https://book.bsdcn.org/di-27-zhang-netbsd/di-27.2-jie-huan-yuan-yu-bao-guan-li-qi.html
 *
 * 根据 @ykla (https://github.com/ykla)
 *    NetBSD 默认状态下没有 pkgsrc，用户可能安装了也可能没安装
 *
 * HELP: 未经测试
 */
void
os_netbsd_setsrc(char* option)
{
  ensure_root(); // HELP: 不知道是否需要确保root权限

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_netbsd, option);
  } else {
    index = lets_test_speed(os_netbsd);
  }

  source_info source = os_netbsd_sources[index];
  chsrc_say_selection(&source);


  char* backup = "cp -f /usr/pkg/etc/pkgin/repositories.conf /usr/pkg/etc/pkgin/repositories.conf.bak";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /usr/pkg/etc/pkgin/repositories.conf.bak");

  char* arch = xy_getcmd("arch",NULL);
  char* version = "cat /etc/os-release | grep \"VERSION=\" | grep -Po [8-9].[0-9]+";
  char* cmd = xy_strjoin(6,"echo ",
                            source.url,
                            arch,
                            "/",
                            version,
                            "/All > /usr/pkg/etc/pkgin/repositories.conf");
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
}



void
os_openbsd_getsrc (char* option)
{
  char* cmd = "cat /etc/installurl";
  chsrc_runcmd(cmd);
}

/**
 * 参考：
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/openbsd/
 * 2. https://book.bsdcn.org/di-26-zhang-openbsd/di-26.2-jie-pei-zhi.html
 */
void
os_openbsd_setsrc(char* option)
{
  ensure_root();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror(os_openbsd, option);
  } else {
    index = lets_test_speed(os_openbsd);
  }

  source_info source = os_openbsd_sources[index];
  chsrc_say_selection(&source);

  char* backup = "cp -f /etc/installurl /etc/installurl.bak --backup='t'";
  chsrc_runcmd(backup);

  xy_info ("chsrc: 备份文件名: /etc/installurl.bak");

  char* cmd = xy_strjoin(3,"echo ", source.url, " > /etc/installurl");
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
}






void
wr_tex_check_cmd_ (bool* tlmgr_exist, bool* mpm_exist)
{
  char* check_cmd = xy_str_to_quietcmd("tlmgr --version");
  *tlmgr_exist = does_the_program_exist (check_cmd, "tlmgr");

  check_cmd = xy_str_to_quietcmd("mpm --version");
  *mpm_exist = does_the_program_exist (check_cmd, "mpm");

  if (!*tlmgr_exist && !*mpm_exist) {
    xy_error ("chsrc: 未找到 tlmgr 或 mpm 命令，请检查是否存在（其一）");
    exit(1);
  }
}

void
wr_tex_getsrc(char* option)
{
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd_(&tlmgr_exist, &mpm_exist);

  if (tlmgr_exist) {
    chsrc_runcmd("tlmgr option repository");
  }
  if (mpm_exist) {
    chsrc_runcmd("mpm --get-repository");
  }
}

/**
 * 参考 https://help.mirrors.cernet.edu.cn/CTAN/
 */
void
wr_tex_setsrc(char* option)
{
  int index = 0;
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd_(&tlmgr_exist, &mpm_exist);

  if (NULL!=option) {
    index = lets_find_mirror(wr_tex, option);
  } else {
    index = lets_test_speed(wr_tex);
  }

  source_info source = wr_tex_sources[index];
  chsrc_say_selection (&source);

  char* cmd = NULL;

  if (tlmgr_exist) {
    cmd = xy_2strjoin("tlmgr option repository ", source.url);
    chsrc_runcmd(cmd);
  }

  if (mpm_exist) {
    char* miktex_url = xy_2strjoin(xy_str_delete_suffix(source.url, "texlive/tlnet"), "win32/miktex/tm/packages/");
    cmd = xy_2strjoin("mpm --set-repository=", miktex_url);
    chsrc_runcmd(cmd);
  }

  chsrc_say_thanks(&source);
}




void
wr_emacs_setsrc(char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(wr_emacs, option);
  } else {
    index = lets_test_speed(wr_emacs);
  }

  source_info source = wr_emacs_sources[index];
  chsrc_say_selection (&source);

  xy_warn("chsrc: 抱歉，Emacs换源涉及Elisp，您可手动查阅并换源:");
  puts(source.url);

  chsrc_say_thanks (&source);
}



void
wr_brew_getsrc(char* option)
{
  char* cmd = "echo HOMEBREW_API_DOMAIN=$HOMEBREW_API_DOMAIN;"
              "echo HOMEBREW_BOTTLE_DOMAIN=$HOMEBREW_BOTTLE_DOMAIN;"
              "echo HOMEBREW_BREW_GIT_REMOTE=$HOMEBREW_BREW_GIT_REMOTE;"
              "echo HOMEBREW_CORE_GIT_REMOTE=$HOMEBREW_CORE_GIT_REMOTE;";
  system(cmd);
}

/**
 * 参考自: https://mirrors.tuna.tsinghua.edu.cn/help/homebrew/
 *
 * 自brew 4.0.0 (2023 年 2 月 16日) 起，
 * HOMEBREW_INSTALL_FROM_API 会成为默认行为，无需设置。大部分用户无需再克隆 homebrew-core 仓库，故无需设置 HOMEBREW_CORE_GIT_REMOTE 环境变量；
 * 但是为了以防万一，我们还是为用户设置该环境变量
 */
void
wr_brew_setsrc(char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(wr_brew, option);
  } else {
    index = lets_test_speed(wr_brew);
  }

  source_info source = wr_brew_sources[index];
  chsrc_say_selection (&source);

  char* api_domain      = xy_strjoin(3, "export HOMEBREW_API_DOMAIN=\"",      xy_2strjoin(source.url, "homebrew-bottles/api"), "\"");
  char* bottle_domain   = xy_strjoin(3, "export HOMEBREW_BOTTLE_DOMAIN=\"",   xy_2strjoin(source.url, "homebrew-bottles"), "\"");
  char* brew_git_remote = xy_strjoin(3, "export HOMEBREW_BREW_GIT_REMOTE=\"", xy_2strjoin(source.url, "git/homebrew/brew.git"), "\"");
  char* core_git_remote = xy_strjoin(3, "export HOMEBREW_CORE_GIT_REMOTE=\"", xy_2strjoin(source.url, "git/homebrew/homebrew-core.git"), "\"");

  chsrc_runcmd(xy_strjoin(3,"echo ", api_domain,      " >> ~/.bashrc >> ~/.zshrc"));
  chsrc_runcmd(xy_strjoin(3,"echo ", bottle_domain,   " >> ~/.bashrc >> ~/.zshrc"));
  chsrc_runcmd(xy_strjoin(3,"echo ", brew_git_remote, " >> ~/.bashrc >> ~/.zshrc"));
  chsrc_runcmd(xy_strjoin(3,"echo ", core_git_remote, " >> ~/.bashrc >> ~/.zshrc"));

  chsrc_say_thanks (&source);
  puts(""); xy_warn("chsrc: 请您重启终端使环境变量生效");
}



/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/guix
 */
void
wr_guix_setsrc (char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(wr_guix, option);
  } else {
    index = lets_test_speed(wr_guix);
  }

  source_info source = wr_guix_sources[index];
  chsrc_say_selection (&source);


  char* file =  xy_strjoin(3, "(list (channel\n"
                              "       (inherit (car %default-channels))\n"
                              "       (url \"", source.url, "\")))");

  xy_warn ("chsrc: 为防止扰乱配置文件，请您手动写入以下内容到 ~/.config/guix/channels.scm 文件中");
  puts(file);
  chsrc_say_thanks(&source);
}


void
wr_nix_check_cmd_()
{
  char* check_cmd = xy_str_to_quietcmd("nix-channel --version");
  bool exist = does_the_program_exist (check_cmd, "nix-channel");

  if (!exist) {
    xy_error ("chsrc: 未找到 nix-channel 命令，请检查是否存在");
    exit(1);
  }
}

/**
 * 参考: https://mirrors.bfsu.edu.cn/help/nix-channels/
 */
void
wr_nix_setsrc (char* option)
{
  wr_nix_check_cmd_();

  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(wr_nix, option);
  } else {
    index = lets_test_speed(wr_nix);
  }

  source_info source = wr_nix_sources[index];
  chsrc_say_selection (&source);

  char* cmd = xy_strjoin(3, "nix-channel --add ", source.url, "nixpkgs-unstable nixpkgs");
  chsrc_runcmd(cmd);

  cmd = "nix-channel --update";
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
}



/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/flathub
 */
void
wr_flathub_setsrc(char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(wr_flathub, option);
  } else {
    index = lets_test_speed(wr_flathub);
  }

  source_info source = wr_flathub_sources[index];
  chsrc_say_selection (&source);

  xy_warn ("chsrc: 若出现问题，可先调用以下命令:");
  char* note = xy_strjoin(3,
    "wget ", source.url, "/flathub.gpg\n"
    "sudo flatpak remote-modify --gpg-import=flathub.gpg flathub"
  );
  puts(note);

  char* cmd = xy_2strjoin("sudo flatpak remote-modify flathub --url=", source.url);
  chsrc_runcmd(cmd);

  chsrc_say_thanks(&source);
}



void
wr_anaconda_setsrc(char* option)
{
  int index = 0;
  if (NULL!=option) {
    index = lets_find_mirror(wr_anaconda, option);
  } else {
    index = lets_test_speed(wr_anaconda);
  }

  source_info source = wr_anaconda_sources[index];
  chsrc_say_selection (&source);


  char* main  = xy_2strjoin(source.url, "pkgs/main");
  char* r     = xy_2strjoin(source.url, "pkgs/r");
  char* msys2 = xy_2strjoin(source.url, "pkgs/msys2");
  char* cloud = xy_2strjoin(source.url, "cloud");

  char* file = xy_strjoin(22,
               "channels:\n  - defaults\n"
               "show_channel_urls: true\ndefault_channels:"
             "\n  - ", main,
             "\n  - ", r,
             "\n  - ", msys2,
             "\ncustom_channels:\n"
               "  conda-forge: ", cloud,
             "\n  msys2: ",        cloud,
             "\n  bioconda: ",     cloud,
             "\n  menpo: ",        cloud,
             "\n  pytorch: ",      cloud,
             "\n  pytorch-lts: ",  cloud,
             "\n  simpleitk: ",    cloud,
             "\n  deepmodeling: ", cloud);


  // TODO: 待确认 windows 上也是这里吗？
  char* config = xy_2strjoin(xy_os_home, "/.condarc");

  if (xy_on_windows) {
    char* check_cmd = xy_str_to_quietcmd("conda --version");
    bool exist = does_the_program_exist (check_cmd, "conda");
    if (!exist) {
      xy_error ("chsrc: 未找到 conda 命令，请检查是否存在");
      exit(1);
    }
    chsrc_runcmd("conda config --set show_channel_urls yes");
  }

  xy_info(xy_strjoin(3, "chsrc: 请向 ", config, " 中手动添加:"));
  puts(file);

  xy_info("chsrc: 然后运行 conda clean -i 清除索引缓存，保证用的是镜像站提供的索引");
  chsrc_say_thanks(&source);
}



/************************************** Begin Target Matrix ****************************************/
def_target_info(pl_ruby);
def_target_info(pl_python);
def_target_info(pl_nodejs);
def_target_info(pl_perl);
def_target_info(pl_php);
def_target_info(pl_go);
def_target_info(pl_rust);
def_target_info(pl_java);
def_target_info(pl_dart);
def_target_info(pl_ocaml);
def_target_info(pl_r);
def_target_info(pl_julia);

target_info
  pl_clojure_target = {pl_clojure_setsrc, NULL,  pl_clojure_sources, pl_clojure_sources_n},
  pl_dotnet_target  = {pl_dotnet_setsrc,  NULL,  pl_dotnet_sources,  pl_dotnet_sources_n},
  pl_haskell_target = {pl_haskell_setsrc, NULL,  pl_haskell_sources, pl_haskell_sources_n};


#define targetinfo(t) (const char*)t
static const char
*pl_ruby  [] = {"gem",   "ruby",    "rb",    "rubygems", NULL,  targetinfo(&pl_ruby_target)},
*pl_python[] = {"pip",   "python",  "py",    "pypi",     NULL,  targetinfo(&pl_python_target)},
*pl_nodejs[] = {"npm",   "node",    "js",    "nodejs",   NULL,  targetinfo(&pl_nodejs_target)},
*pl_perl  [] = {"perl",  "cpan",                         NULL,  targetinfo(&pl_perl_target)},
*pl_php   [] = {"php",   "composer",                     NULL,  targetinfo(&pl_php_target)},
*pl_go    [] = {"go",    "golang",  "goproxy",           NULL,  targetinfo(&pl_go_target)} ,
*pl_rust  [] = {"rust",  "cargo",   "crate",  "crates",  NULL,  targetinfo(&pl_rust_target)},
*pl_java  [] = {"java",  "maven",   "gradle",            NULL,  targetinfo(&pl_java_target)},
*pl_clojure[] ={"clojure","clojars","leiningen", "lein", NULL,  targetinfo(&pl_clojure_target)},
*pl_dart  [] = {"dart",  "pub",     "flutter",           NULL,  targetinfo(&pl_dart_target)},
*pl_dotnet[] = {"nuget", "net",     ".net",   "dotnet",  NULL,  targetinfo(&pl_dotnet_target)},
*pl_haskell[] ={"haskell", "cabal", "stack",  "hackage", NULL,  targetinfo(&pl_haskell_target)},
*pl_ocaml[] =  {"ocaml", "opam",                         NULL,  targetinfo(&pl_ocaml_target)},
*pl_r     [] = {"r",     "cran",                         NULL,  targetinfo(&pl_r_target)},
*pl_julia [] = {"julia",                                 NULL,  targetinfo(&pl_julia_target)},
**pl_packagers[] =
{
  pl_ruby,    pl_python,    pl_nodejs,      pl_perl,    pl_php,
  pl_rust,    pl_go,        /*pl_dotnet,*/  pl_java,    pl_clojure,  pl_dart,
  pl_haskell, pl_ocaml,
  pl_r,       pl_julia
};


def_target_info(os_ubuntu);
def_target_info(os_debian);
def_target_info(os_deepin);
def_target_info(os_void);
def_target_info(os_netbsd);
def_target_info(os_openbsd);


target_info
  os_fedora_target      = {os_fedora_setsrc,      NULL, os_fedora_sources,    os_fedora_sources_n},
  os_opensuse_target    = {os_opensuse_setsrc,    NULL, os_opensuse_sources,  os_opensuse_sources_n},
  os_kali_target        = {os_kali_setsrc,        NULL, os_kali_sources,      os_kali_sources_n},
  os_msys2_target       = {os_msys2_setsrc,       NULL, os_msys2_sources,     os_msys2_sources_n},
  os_arch_target        = {os_arch_setsrc,        NULL, os_arch_sources,      os_arch_sources_n},
  os_manjaro_target     = {os_manjaro_setsrc,     NULL, NULL,                       0},
  os_gentoo_target      = {os_gentoo_setsrc,      NULL, os_gentoo_sources,    os_gentoo_sources_n},
  os_rocky_target       = {os_rocky_setsrc,       NULL, os_rocky_sources,     os_rocky_sources_n},
  os_alpine_target      = {os_alpine_setsrc,      NULL, os_alpine_sources,    os_alpine_sources_n},
  os_freebsd_target     = {os_freebsd_setsrc,     NULL, os_freebsd_sources,   os_freebsd_sources_n},
  os_openeuler_target   = {os_openeuler_setsrc,   NULL, os_openeuler_sources, os_openeuler_sources_n},
  os_openkylin_target   = {os_openkylin_setsrc,   NULL, os_openkylin_sources, os_openkylin_sources_n};

static const char
*os_ubuntu        [] = {"ubuntu",               NULL,  targetinfo(&os_ubuntu_target)},
*os_debian        [] = {"debian",  "deb",       NULL,  targetinfo(&os_debian_target)},
*os_fedora        [] = {"fedora",               NULL,  targetinfo(&os_fedora_target)},
*os_opensuse      [] = {"opensuse","suse",      NULL,  targetinfo(&os_opensuse_target)},
*os_kali          [] = {"kali",                 NULL,  targetinfo(&os_kali_target)},
*os_msys2         [] = {"msys2",   "msys",      NULL,  targetinfo(&os_msys2_target)},
*os_arch          [] = {"arch",                 NULL,  targetinfo(&os_arch_target)},
*os_manjaro       [] = {"manjaro",              NULL,  targetinfo(&os_manjaro_target)},
*os_gentoo        [] = {"gentoo",               NULL,  targetinfo(&os_gentoo_target)},
*os_rocky         [] = {"rocky",  "rockylinux", NULL,  targetinfo(&os_rocky_target)},
*os_alpine        [] = {"alpine",               NULL,  targetinfo(&os_alpine_target)},
*os_void          [] = {"void",   "voidlinux",  NULL,  targetinfo(&os_void_target)},
*os_freebsd       [] = {"freebsd",              NULL,  targetinfo(&os_freebsd_target)},
*os_netbsd        [] = {"netbsd",               NULL,  targetinfo(&os_netbsd_target)},
*os_openbsd       [] = {"openbsd",              NULL,  targetinfo(&os_openbsd_target)},
*os_deepin        [] = {"deepin",               NULL,  targetinfo(&os_deepin_target)},
*os_openeuler     [] = {"openeuler", "euler",   NULL, targetinfo(&os_openeuler_target)},
*os_openkylin     [] = {"openkylin",            NULL, targetinfo(&os_openkylin_target)},
**os_systems[] =
{
  os_ubuntu,  os_debian,  os_fedora,  os_opensuse, os_kali,
  os_arch,    os_manjaro, os_gentoo,
  os_rocky,
  os_alpine,
  os_freebsd, os_netbsd,  os_openbsd,
  os_msys2,
  os_deepin, os_openeuler, os_openkylin,
};


def_target_info(wr_brew);
def_target_info(wr_tex);

target_info
  wr_flathub_target  = {wr_flathub_setsrc,  NULL,  wr_flathub_sources,  wr_flathub_sources_n},
  wr_nix_target      = {wr_nix_setsrc,      NULL,  wr_nix_sources,      wr_nix_sources_n},
  wr_guix_target     = {wr_guix_setsrc,     NULL,  wr_guix_sources,     wr_guix_sources_n},
  wr_emacs_target    = {wr_emacs_setsrc,    NULL,  wr_emacs_sources,    wr_emacs_sources_n},
  wr_anaconda_target = {wr_anaconda_setsrc, NULL,  wr_anaconda_sources, wr_anaconda_sources_n};

static const char
*wr_brew    [] = {"brew",  "homebrew",     NULL,  targetinfo(&wr_brew_target)},
*wr_flathub [] = {"flathub", NULL,                targetinfo(&wr_flathub_target)},
*wr_nix     [] = {"nix",     NULL,                targetinfo(&wr_nix_target)},
*wr_guix    [] = {"guix",    NULL,                targetinfo(&wr_guix_target)},
*wr_emacs   [] = {"emacs", "elpa",         NULL,  targetinfo(&wr_emacs_target)},
*wr_tex     [] = {"latex", "ctan", "tex", "texlive", "miktex", "tlmgr", "mpm", NULL, targetinfo(&wr_tex_target)},
*wr_anaconda[] = {"conda", "anaconda",     NULL,  targetinfo(&wr_anaconda_target)},
**wr_softwares[] =
{
  wr_brew, wr_flathub, wr_nix, wr_guix, wr_emacs, wr_tex, wr_anaconda
};
#undef targetinfo
/************************************** End Target Matrix ****************************************/


static const char*
usage[] = {
  "维护: https://gitee.com/RubyMetric/chsrc\n",

  "使用: chsrc <command> [target] [mirror]",
  "help                      打印此帮助，或 h, -h, --help",
  "list (或 ls, 或 l)        列出可用镜像源，和可换源软件",
  "list mirror/target        列出可用镜像源，或可换源软件",
  "list os/lang/ware         列出可换源的操作系统/编程语言/软件",
  "list <target>             查看该软件可以使用哪些源",
  "cesu <target>             对该软件所有源测速",
  "get  <target>             查看当前软件的源使用情况",
  "set  <target>             换源，自动测速后挑选最快源",
  "set  <target> def(ault)   换源，默认使用维护团队测速第一的源",
  "set  <target> <mirror>    换源，指定使用某镜像站\n"
};


void
call_cmd (void* cmdptr, const char* arg)
{
  void (*cmd_func)(const char*) = cmdptr;
  if (NULL==arg) {
    xy_info("chsrc: 将使用默认镜像");
  }
  cmd_func(arg);
}



void
print_available_mirrors ()
{
  xy_info ("chsrc: 支持以下镜像站，荣耀均归属于这些站点，以及它们的开发/维护者们");
  xy_warn ("chsrc: 下方 code 列，可用于指定使用某镜像站，请使用 chsrc set <target> <code>");
  printf ("%-14s%-30s%-41s ", "code", "服务商缩写", "服务商URL"); puts("服务商名称");
  puts   ("-------------------------------------------------------------------------------------------------");
  for (int i=0; i<xy_arylen(available_mirrors); i++)
  {
    mirror_info* mir = available_mirrors[i];
    printf ("%-14s%-18s%-41s ", mir->code, mir->abbr, mir->site); puts(mir->name);
  }
}


void
print_supported_targets_ (const char*** array, size_t size)
{
  for (int i=0; i<size; i++)
  {
    const char** target = array[i];
    const char* alias = target[0];
    for (int k=1; alias!=NULL; k++)
    {
      printf ("%s\t", alias);
      alias = target[k];
    }
    puts("");
  }
  puts("");
}

void
print_supported_targets ()
{
  xy_info ("chsrc: 支持对以下目标换源 (同一行表示这几个命令兼容)");
  xy_warn ("编程语言开发");
  print_supported_targets_ (pl_packagers, xy_arylen(pl_packagers));
  xy_warn ("操作系统");
  print_supported_targets_ (os_systems,   xy_arylen(os_systems));
  xy_warn ("软件");
  print_supported_targets_ (wr_softwares, xy_arylen(wr_softwares));
}

void
print_supported_pl ()
{
  xy_info ("chsrc: 支持对以下编程语言生态换源 (同一行表示这几个命令兼容)");
  print_supported_targets_ (pl_packagers,   xy_arylen(pl_packagers));
}

void
print_supported_os ()
{
  xy_info ("chsrc: 支持对以下操作系统换源 (同一行表示这几个命令兼容)");
  print_supported_targets_ (os_systems,   xy_arylen(os_systems));
}

void
print_supported_wr ()
{
  xy_info ("chsrc: 支持对以下软件换源 (同一行表示这几个命令兼容)");
  print_supported_targets_ (wr_softwares,   xy_arylen(wr_softwares));
}



/**
 * 用于 chsrc list <target>
 */
void
print_supported_sources_for_target (source_info sources[], size_t size)
{
  for (int i=0;i<size;i++)
  {
    source_info src = sources[i];
    const mirror_info* mir = src.mirror;
    printf ("%-14s%-18s%-50s ",mir->code, mir->abbr, src.url);
    puts(mir->name);
  }
}



void
print_help ()
{
  puts(xy_strjoin(3, "chsrc: Change Source (GPLv3) ",
                      xy_str_to_magenta(Chsrc_Version), " by RubyMetric\n"));
  for (int i=0; i<xy_arylen(usage); i++) {
    puts (usage[i]);
  }
}



/**
 * 遍历我们内置的targets列表，查询用户输入`input`是否与我们支持的某个target匹配
 *
 * @param[out]  target_info  如果匹配到，则返回内置targets列表中最后的target_info信息
 *
 * @return 匹配到则返回true，未匹配到则返回false
 */
bool
iterate_targets_(const char*** array, size_t size, const char* input, const char*** target_info)
{
  int matched = 0;

  const char** target = NULL;
  int k = 0;
  const char* alias = NULL;

  for (int i=0; i<size; i++) {
    target = array[i];
    alias = target[k];
    while (NULL!=alias) {
      if (xy_streql(input, alias)) {
        matched = 1; break;
      }
      k++;
      alias = target[k];
    }
    if (!matched) k = 0;
    if (matched) break;
  }

  if(!matched) {
    *target_info = NULL;
    return false;
  }

  do {
    k++;
    alias = target[k];
  } while (NULL!=alias);
  *target_info = target + k + 1;
  return true;
}

#define iterate_targets(ary, input, target) iterate_targets_(ary, xy_arylen(ary), input, target)

#define Target_Set_Source  1
#define Target_Get_Source  2
#define Target_Cesu_Source 3
#define Target_List_Source 4

/**
 * 寻找target，并根据`code`执行相应的操作
 *
 * @param  input   用户输入的目标
 * @param  code    对target要执行的操作
 * @param  option  额外的指示，可为NULL
 *
 * @return 找到目标返回true，未找到返回false
 */
bool
get_target (const char* input, int code, char* option)
{
  const char** target_tmp = NULL;

           bool matched = iterate_targets(pl_packagers, input, &target_tmp);
  if (!matched) matched = iterate_targets(os_systems,   input, &target_tmp);
  if (!matched) matched = iterate_targets(wr_softwares, input, &target_tmp);

  if (!matched) {
    return false;
  }

  target_info* target = (target_info*) *target_tmp;

  if (Target_Set_Source==code)
  {
    if (target->setfn) target->setfn(option);
    else xy_error (xy_strjoin(3, "chsrc: 暂未对 ", input, " 实现set功能，欢迎贡献"));
  }
  else if (Target_Get_Source==code)
  {
    if (target->getfn) target->getfn("");
    else xy_error (xy_strjoin(3, "chsrc: 暂未对 ", input, " 实现get功能，欢迎贡献"));
  }
  else if (Target_List_Source==code)
  {
    xy_info (xy_strjoin(3,"chsrc: 对 ", input ," 支持以下镜像站，荣耀均归属于这些站点，以及它们的开发/维护者们"));
    xy_warn (xy_strjoin(3, "chsrc: 下方 code 列，可用于指定使用某源，请使用 chsrc set ", input, " <code>"));
    printf ("%-14s%-35s%-45s ", "code", "服务商缩写", "服务源URL"); puts("服务商名称");
    puts   ("--------------------------------------------------------------------------------------------------------");
    print_supported_sources_for_target (target->sources, target->sources_n);
  }
  else if (Target_Cesu_Source==code)
  {
    char* check_cmd = xy_str_to_quietcmd("curl --version");
    bool exist_b = does_the_program_exist (check_cmd, "curl");
    if (!exist_b) {
      xy_error ("chsrc: 没有curl命令，无法测速");
      exit(1);
    }
    lets_test_speed_ (target->sources, target->sources_n, input-3);
    return true;

  }
  return true;
}



int
main (int argc, char const *argv[])
{
  xy_useutf8(); argc -= 1;

  if (argc==0) {
    print_help(); return 0;
  }

  const char* command = argv[1];

  bool matched = false;

  /* chsrc help */
  if (xy_streql(command, "h")  ||
      xy_streql(command, "-h") ||
      xy_streql(command, "help") ||
      xy_streql(command, "--help"))
  {
    print_help();
    return 0;
  }

  /* chsrc list */
  else if (xy_streql(command, "list") ||
           xy_streql(command, "l")    ||
           xy_streql(command, "ls"))
  {
    if (argc < 2) {
      print_available_mirrors();
      puts("");
      print_supported_targets();
    } else {

      if (xy_streql(argv[2],"mirrors")) {
        print_available_mirrors(); return 0;
      }
      if (xy_streql(argv[2],"mirror"))  {
        print_available_mirrors(); return 0;
      }
      if (xy_streql(argv[2],"targets")) {
        print_supported_targets(); return 0;
      }
      if (xy_streql(argv[2],"target"))  {
        print_supported_targets(); return 0;
      }
      if (xy_streql(argv[2],"os")) {
        print_supported_os(); return 0;
      }
      if (xy_streql(argv[2],"lang")) {
        print_supported_pl(); return 0;
      }
      if (xy_streql(argv[2],"pl")) {
        print_supported_pl(); return 0;
      }
      if (xy_streql(argv[2],"language")) {
        print_supported_pl(); return 0;
      }
      if (xy_streql(argv[2],"software")) {
        print_supported_wr(); return 0;
      }
      if (xy_streql(argv[2],"ware")) {
        print_supported_wr(); return 0;
      }
      matched = get_target(argv[2], Target_List_Source, NULL);
      if (!matched) goto not_matched;
    }
    return 0;
  }


  /* chsrc cesu */
  else if (xy_streql(command, "cesu") ||
           xy_streql(command, "ce")   ||
           xy_streql(command, "c"))
  {
    if (argc < 2) {
      xy_error ("chsrc: 请您提供想要测速源的软件名; 使用 chsrc list targets 查看所有支持的软件");
      return 1;
    }
    matched = get_target(argv[2], Target_Cesu_Source, NULL);
    if (!matched) goto not_matched;
    return 0;
  }


  /* chsrc get */
  else if (xy_streql(command, "get") ||
           xy_streql(command, "g"))
  {
    if (argc < 2) {
      xy_error ("chsrc: 请您提供想要查看源的软件名; 使用 chsrc list targets 查看所有支持的软件");
      return 1;
    }
    matched = get_target(argv[2], Target_Get_Source, NULL);
    if (!matched) goto not_matched;
    return 0;
  }

  /* chsrc set */
  else if (xy_streql(command, "set") ||
           xy_streql(command, "s"))
  {
    if (argc < 2) {
      xy_error ("chsrc: 请您提供想要设置源的软件名; 使用 chsrc list targets 查看所有支持的软件");
      return 1;
    }

    char* option = NULL;
    if (argc >= 3) {
      option = (char*) argv[3]; // 暂时我们只接受最多三个参数
    }
    matched = get_target(argv[2], Target_Set_Source, option);
    if (!matched) goto not_matched;
    return 0;
  }

  /* 不支持的命令 */
  else
  {
    xy_error ("chsrc: 不支持的命令，请使用 chsrc help 查看使用方式");
    return 1;
  }

not_matched:
  if (!matched) {
    xy_info("chsrc: 暂不支持的换源目标，请使用 chsrc list targets 查看可换源软件");
    return 1;
  }
}
