/* --------------------------------------------------------------
* File          : chsrc.c
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-28>
* Last modified : <2023-09-05>
*
* chsrc:
*
*   Change Source —— 命令行换源工具
* -------------------------------------------------------------*/

#include "chsrc.h"

#define Chsrc_Version "v0.1.0.20230910.pre"


/**
 * 检测二进制程序是否存在
 *
 * @param  check_cmd  检测 `progname` 是否存在的一段命令，一般来说，填 `progname` 本身即可，
 *                    但是某些情况下，需要使用其他命令绕过一些特殊情况，比如 python 这个命令在Windows上
 *                    会自动打开 Microsoft Store，需避免
 *
 * @param  progname   要检测的二进制程序名
 */
bool
does_the_program_exist (char* check_cmd, char* progname)
{
  char* which = check_cmd;

  int ret = system(which);

  // char buf[32] = {0}; sprintf(buf, "错误码: %d", ret);

  if (0!=ret) {
    // xy_warn (xy_strjoin(4, "× 命令 ", progname, " 不存在，", buf));
    xy_warn (xy_strjoin(3, "× 命令 ", progname, " 不存在"));
    return false;
  } else {
    xy_success (xy_strjoin(3, "√ 命令 ", progname, " 存在"));
    return true;
  }
}



/**
 * 用于 _setsrc 函数，检测用户输入的镜像站code，是否存在于该target可用源中
 *
 * @param  target  目标名
 * @param  input   如果用户输入 default 或者 def，则选择第一个源
 */
#define lets_find_mirror(s, input) does_the_input_mirror_exist(s##_sources, s##_sources_n, #s+3, input)
int
does_the_input_mirror_exist (source_info* sources, size_t size, char* target, char* input)
{
  if (0==size) {
    xy_error(xy_strjoin(3, "chsrc: 当前 ", target, " 无任何可用源，请联系维护者"));
    exit(1);
  }

  if (1==size) {
    xy_success(xy_strjoin(5, "chsrc: ", sources[0].mirror->name, " 是 ", target, " 目前唯一可用镜像站，感谢你们的慷慨支持"));
  }

  if (xy_streql("default", input) || xy_streql("def", input)) {
    xy_info ("chsrc: 默认使用维护团队测速第一的源");
    return 0;
  }

  int idx = 0;
  source_info source = sources[0];

  bool exist = false;
  for (int i=0; i<size; i++)
  {
    source = sources[i];
    if (xy_streql(source.mirror->code, input)) {
      idx = i;
      exist = true;
      break;
    }
  }
  if (!exist) {
    xy_error (xy_strjoin(3, "chsrc: 镜像站 ", input, " 不存在"));
    xy_error (xy_2strjoin("chsrc: 查看可使用源，请使用 chsrc list ", target));
    exit(1);
  }
  return idx;
}

char*
to_human_readable_speed (double speed)
{
  char* scale[] = {"Byte/s", "KByte/s", "MByte/s", "GByte/s", "TByte/s"};
  int i = 0;
  while (speed > 1024.0)
  {
    i += 1;
    speed /= 1024.0;
  }
  char* buf = xy_malloc0(64);
  sprintf(buf, "%.2f %s", speed, scale[i]);

  char* new = NULL;
  if (i <= 1 ) new = xy_str_to_red(buf);
  else
  {
    if (i == 2 && speed < 2.00) new = xy_str_to_yellow(buf);
    else new = xy_str_to_green(buf);
  }
  return new;
}


/**
 * 测速代码参考自 https://github.com/mirrorz-org/oh-my-mirrorz/blob/master/oh-my-mirrorz.py
 * 修改为C语言，一切功劳属于原作者
 *
 * @return 返回测得的速度，若出错，返回-1
 */
double
test_speed_url (const char* url)
{
  // 我们用 —L，因为Ruby China源会跳转到其他地方
  char* curl_cmd = xy_strjoin(4, "curl -qsL -o ", xy_os_devnull, " -w \"%{http_code} %{speed_download}\" -m6 -A chsrc/" Chsrc_Version
                   "  ", url);

  // xy_info (xy_2strjoin("chsrc: 测速 ", url));

  FILE* fp = popen(curl_cmd, "r");
  char buf[64] = {0};
  while(NULL!=fgets(buf, 64, fp));
  // puts(buf);

  // 如果尾部有换行，删除
  char* last_lf = strrchr(buf, '\n');
  if (last_lf) *last_lf = '\0';

  char* split = strchr(buf, ' ');
  if (split) *split = '\0';

  // puts(buf); puts(split+1);
  int http_code = atoi(buf);
  double speed  = atof(split+1);
  char* speedstr = to_human_readable_speed(speed);

  if (200!=http_code) {
    char* httpcodestr = xy_str_to_yellow(xy_2strjoin("HTTP码  ", buf));
    puts (xy_strjoin(3, speedstr, " | ",  httpcodestr));
  } else {
    puts (speedstr);
  }
  return speed;
}


#define lets_test_speed(s) lets_test_speed_(s##_sources, s##_sources_n, #s+3)
int
lets_test_speed_ (source_info* sources, size_t size, const char* target)
{
  if (0==size) {
    xy_error(xy_strjoin(3, "chsrc: 当前 ", target, " 无任何可用源，请联系维护者"));
    exit(1);
  }

  bool onlyone = false;
  if (1==size) onlyone = true;

  double speeds[size];
  double speed = 0.0;
  for (int i=0;i<size;i++)
  {
    source_info src = sources[i];
    const char* url = src.mirror->__bigfile_url;
    if (NULL==url) {
      xy_warn ("chsrc: 跳过该站点");
      speed = 0;
    } else {
      printf (xy_strjoin(3, "chsrc: 测速 ", src.mirror->site , " ... "));
      speed = test_speed_url (url);
    }
    speeds[i] = speed;
  }
  int fastidx = dblary_maxidx (speeds, size);

  if (onlyone)
    xy_success(xy_strjoin(5, "chsrc: ", sources[fastidx].mirror->name, " 是 ", target, " 目前唯一可用镜像站，感谢你们的慷慨支持"));
  else
    xy_success (xy_2strjoin("chsrc: 最快镜像站: ", sources[fastidx].mirror->name));

  return fastidx;
}


/***************************************** 换源 *********************************************/

void
pl_ruby_getsrc (char* option)
{
  char* cmd = "gem sources";
  chsrc_logcmd(cmd);
  system(cmd);
  cmd = "bundle config get mirror.https://rubygems.org";
  chsrc_logcmd(cmd);
  system(cmd);
}


/**
 * Ruby换源，参考：https://gitee.com/RubyKids/rbenv-cn
 */
void
pl_ruby_setsrc (char* option)
{
  int index = 0;
  char* check_cmd = xy_str_to_quietcmd("gem -v");
  bool exist_b = does_the_program_exist (check_cmd, "gem");
  if (!exist_b) {
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


  char* cmd = "gem source -r https://rubygems.org/";
  chsrc_logcmd(cmd);
  system(cmd);

  cmd = xy_2strjoin("gem source -a ", source.url);
  chsrc_logcmd(cmd);
  system(cmd);


  check_cmd = xy_str_to_quietcmd("bundle -v");
  exist_b = does_the_program_exist (check_cmd, "bundle");
  if (!exist_b) {
    xy_error ("chsrc: 未找到 bundle 命令，请检查是否存在");
    return;
  }

  cmd = xy_2strjoin("bundle config 'mirror.https://rubygems.org' ", source.url);
  chsrc_logcmd(cmd);
  system(cmd);

  chsrc_say_thanks(&source);
}



/**
 * @param[out] prog 返回 Python 的可用名，如果不可用，则返回 NULL
 */
void
_pl_python_check_cmd (char** prog)
{
  *prog = NULL;
  // 不要调用 python 自己，而是使用 python --version，避免Windows弹出Microsoft Store
  char* check_cmd = xy_str_to_quietcmd("python --version");
  bool exist_b = does_the_program_exist (check_cmd, "python");

  if (!exist_b) {
    check_cmd = xy_str_to_quietcmd("python3 --version");
    exist_b = does_the_program_exist (check_cmd, "python3");
    if (exist_b) *prog = "python3";
  }
  else {
    *prog = "python";
  }

  if (!exist_b) {
    xy_error ("chsrc: 未找到 Python 相关命令，请检查是否存在");
    exit(1);
  }
}


void
pl_python_getsrc (char* option)
{
  char* prog = NULL;
  _pl_python_check_cmd (&prog);
  char* cmd = xy_2strjoin(prog, " -m pip config get global.index-url");
  chsrc_logcmd(cmd);
  system(cmd);
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
  _pl_python_check_cmd (&prog);

  if (NULL!=option) {
    index = lets_find_mirror (pl_python, option);
  } else {
    index = lets_test_speed (pl_python);
  }

  source_info source = pl_python_sources[index];
  chsrc_say_selection(&source);

  char* cmd = xy_2strjoin(prog, xy_2strjoin(" -m pip config set global.index-url ", source.url));
  chsrc_logcmd(cmd);
  system(cmd);
  chsrc_say_thanks(&source);
}



void
_pl_nodejs_check_cmd ()
{
  char* check_cmd = xy_str_to_quietcmd("npm -v");
  bool exist_b = does_the_program_exist (check_cmd, "npm");
  if (!exist_b) {
    xy_error ("chsrc: 未找到 npm 命令，请检查是否存在");
    exit(1);
  }
}


void
pl_nodejs_getsrc (char* option)
{
  _pl_nodejs_check_cmd ();
  char* cmd = "npm config get registry";
  chsrc_logcmd(cmd);
  system(cmd);
}

/**
 * NodeJS换源，参考：https://npmmirror.com/
 */
void
pl_nodejs_setsrc (char* option)
{
  _pl_nodejs_check_cmd();

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_nodejs, option);
  } else {
    index = lets_test_speed (pl_nodejs);
  }

  source_info source = pl_nodejs_sources[index];
  chsrc_say_selection (&source);

  char* cmd = xy_2strjoin("npm config set registry ", source.url);
  chsrc_logcmd(cmd);
  system(cmd);
}



void
_pl_perl_check_cmd ()
{
  char* check_cmd = xy_str_to_quietcmd("perl --version");
  bool exist_b = does_the_program_exist (check_cmd, "perl");

  if (!exist_b) {
    xy_error ("chsrc: 未找到 perl 命令，请检查是否存在");
    exit(1);
  }
}

void
pl_perl_getsrc (char* option)
{
  _pl_perl_check_cmd ();
  // @ccmywish: 注意，prettyprint 仅仅是一个内部实现，可能不稳定，如果需要更稳定的，
  //            可以使用 CPAN::Shell->o('conf', 'urllist');
  //            另外，上述两种方法无论哪种，都要首先load()
  char* cmd = "perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->prettyprint('urllist')\" ";
  chsrc_logcmd(cmd);
  system(cmd);
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
  chsrc_logcmd(cmd);
  system(cmd);

  xy_warn ("chsrc: 请您使用 perl -v 以及 cpan -v，若 Perl >= v5.36 或 CPAN >= 2.29，请额外手动调用下面的命令");
  xy_warn ("       perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('pushy_https', 0);; CPAN::HandleConfig->commit()\"");
  chsrc_say_thanks(&source);
}



void
_pl_php_check_cmd()
{
  char* check_cmd = xy_str_to_quietcmd("composer --version");
  bool exist_b = does_the_program_exist (check_cmd, "composer");

  if (!exist_b) {
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
  _pl_php_check_cmd ();
  char* cmd = "composer config -g repositories";
  chsrc_logcmd(cmd);
  system(cmd);
}

/**
 * PHP 换源，参考：https://developer.aliyun.com/composer
 */
void
pl_php_setsrc (char* option)
{
  _pl_php_check_cmd();

  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_php, option);
  } else {
    index = lets_test_speed (pl_php);
  }

  source_info source = pl_php_sources[index];
  chsrc_say_selection (&source);

  char* cmd = xy_2strjoin("composer config -g repo.packagist composer ", source.url);
  chsrc_logcmd(cmd);
  system(cmd);

  chsrc_say_thanks(&source);
}



void
_pl_go_check_cmd ()
{
  char* check_cmd = xy_str_to_quietcmd("go version");
  bool exist_b = does_the_program_exist (check_cmd, "go");

  if (!exist_b) {
    xy_error ("chsrc: 未找到 go 相关命令，请检查是否存在");
    exit(1);
  }
}

void
pl_go_getsrc (char* option)
{
  _pl_go_check_cmd ();
  char* cmd = "go env GOPROXY";
  chsrc_logcmd(cmd);
  system(cmd);
}

/**
 * Go换源，参考：https://goproxy.cn/
 */
void
pl_go_setsrc (char* option)
{
  _pl_go_check_cmd();
  int index = 0;

  if (NULL!=option) {
    index = lets_find_mirror (pl_go, option);
  } else {
    index = lets_test_speed (pl_go);
  }

  source_info source = pl_go_sources[index];
  chsrc_say_selection (&source);

  char* cmd = "go env -w GO111MODULE=on";
  chsrc_logcmd(cmd);
  system(cmd);

  cmd = xy_strjoin(3, "go env -w GOPROXY=", source.url, ",direct");
  chsrc_logcmd(cmd);
  system(cmd);
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
  chsrc_logcmd(cmd);
  system(cmd);
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
  int index = 0; char* check_cmd = NULL;

  xy_error ("chsrc: 暂时无法为NuGet换源，若有需求，请您提交issue");
}



void
_pl_java_check_cmd(bool* maven_exist, bool* gradle_exist)
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
_pl_java_find_maven_config ()
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
  _pl_java_check_cmd (&maven_exist, &gradle_exist);
  char* maven_config = _pl_java_find_maven_config();

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
  _pl_java_check_cmd (&maven_exist, &gradle_exist);

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

    char* maven_config = _pl_java_find_maven_config();
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
  chsrc_logcmd(cmd);
  system(cmd);
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
    cmd = xy_strjoin(3, "echo ", file, " >> ~/.Rprofile");

  chsrc_logcmd(cmd);
  system(cmd);


  file = xy_strjoin (3, "options(BioC_mirror=\"", bioconductor_url, "\")" );
  // 或者我们调用 r.exe --slave -e 上面的内容
  if (xy_on_windows)
    cmd = xy_strjoin(3, "echo ", file, " >> %USERPROFILE%/Documents/.Rprofile");
  else
    cmd = xy_strjoin(3, "echo ", file, " >> ~/.Rprofile");

  chsrc_logcmd(cmd);
  system(cmd);
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
  chsrc_logcmd(cmd);
  system(cmd);
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
        ";echo ", file, " >> ~/.julia/config/startup.jl");

  chsrc_logcmd(cmd);
  system(cmd);
  chsrc_say_thanks(&source);
}




/**
 * ubuntu不同架构下的换源是不一样的,这个针对x86架构
 */
void
os_ubuntu_setsrc (char* option)
{
  int selected = 0;
  for (int i=0;i<sizeof(os_ubuntu_sources);i++) {
    // 循环测速
  }
  const char* source_name = os_ubuntu_sources[selected].mirror->name;
  const char* source_abbr = os_ubuntu_sources[selected].mirror->abbr;
  const char* source_url  = os_ubuntu_sources[selected].url;

  char* backup = "cp -rf /etc/apt/sources.list /etc/apt/sources.list.bak";
  system(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* cmd = xy_strjoin(3, "sed -E \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/ubuntu\\/@\\1",
                          source_url,
                          "@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");
  system(cmd);
  free(cmd);

  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  xy_info ("chsrc: 为 ubuntu 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}
/**
 * Debian Buster 以上版本默认支持 HTTPS 源。如果遇到无法拉取 HTTPS 源的情况，请先使用 HTTP 源并安装
 * sudo apt install apt-transport-https ca-certificates
 */
void
os_debian_setsrc (char* option)
{
  int selected = 0;
  for (int i=0;i<sizeof(os_ubuntu_sources);i++) {
    // 循环测速
  }
  const char* source_name = os_ubuntu_sources[selected].mirror->name;
  const char* source_abbr = os_ubuntu_sources[selected].mirror->abbr;
  const char* source_url  = os_ubuntu_sources[selected].url;

  char* backup = "cp -rf /etc/apt/sources.list /etc/apt/sources.list.bak";
  system(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* cmd = xy_strjoin(3, "sed -E \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/debian\\/@\\1",
                          source_url,
                          "@\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");
  system(cmd);
  free(cmd);

  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  xy_info ("chsrc: 为 debian 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}
/**
 * fedora29版本及以下暂不支持
 */
void
os_fedora_setsrc (char* option)
{
  int selected = 0;
  for (int i=0;i<sizeof(os_fedora_sources);i++) {
    // 循环测速
  }
  const char* source_name = os_fedora_sources[selected].mirror->name;
  const char* source_abbr = os_fedora_sources[selected].mirror->abbr;
  const char* source_url  = os_fedora_sources[selected].url;

  char* backup = "cp -rf /etc/yum.repos.d/fedora.repo /etc/yum.repos.d/fedora.repo.bak";
  system(backup);
  backup = "cp -rf /etc/yum.repos.d/fedora-updates.repo /etc/yum.repos.d/fedora-updates.repo.bak";
  system(backup);

  xy_info ("chsrc: 备份文件名:1. /etc/yum.repos.d/fedora.repo.bak");
  xy_info ("chsrc: 备份文件名:2. /etc/yum.repos.d/fedora-updates.repo.bak");


  char* cmd = xy_strjoin(9, "sed -e 's|^metalink=|#metalink=|g' ",
         "-e 's|^#baseurl=http://download.example/pub/fedora/linux/|baseurl=",
         source_url,
         "|g' ",
         "-i.bak ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-modular.repo ",
         "/etc/yum.repos.d/fedora-updates.repo ",
         "/etc/yum.repos.d/fedora-updates-modular.repo");
  system(cmd);
  free(cmd);

  xy_info("替换文件:/etc/yum.repos.d/fedora.repo");
  xy_info("新增文件:/etc/yum.repos.d/fedora-modular.repo");
  xy_info("替换文件:/etc/yum.repos.d/fedora-updates.repo");
  xy_info("新增文件:/etc/yum.repos.d/fedora-updates-modular.repo");

  // char* rm = "rm -rf /etc/yum.repos.d/fedora.repo.bak";
  // system(rm);
  // char* rm = "rm -rf /etc/yum.repos.d/fedora-updates.repo.bak";
  // system(rm);

  xy_info ("chsrc: 为 fedora 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}




void
os_kali_setsrc(char* option)
{
  int selected = 0;
  for (int i=0;i<sizeof(os_kali_sources);i++) {
    // 循环测速
  }
  const char* source_name = os_kali_sources[selected].mirror->name;
  const char* source_abbr = os_kali_sources[selected].mirror->abbr;
  const char* source_url  = os_kali_sources[selected].url;

  char* backup = "cp -rf /etc/apt/sources.list /etc/apt/sources.list.bak";
  system(backup);

  xy_info ("chsrc: 备份文件名: /etc/apt/sources.list.bak");

  char* cmd = xy_strjoin(3, "sed -i \'s@(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/kali\\/@\\1",
                          source_url,
                          "@g\' /etc/apt/sources.list");
  system(cmd);
  free(cmd);

  // char* rm = "rm -rf /etc/apt/source.list.bak";
  // system(rm);

  xy_info ("chsrc: 为 kali 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}


void
os_openbsd_setsrc(char* option)
{
  int selected = 0;
  for (int i=0;i<sizeof(os_openbsd_sources);i++) {
    // 循环测速
  }
  const char* source_name = os_openbsd_sources[selected].mirror->name;
  const char* source_abbr = os_openbsd_sources[selected].mirror->abbr;
  const char* source_url  = os_openbsd_sources[selected].url;

  char* backup = "cp -rf /etc/installurl /etc/installurl.bak";
  system(backup);

  xy_info ("chsrc: 备份文件名: /etc/installurl.bak");

  char* cmd = xy_strjoin(3,"echo ",source_url," > /etc/installurl");
  system(cmd);
  free(cmd);

  // char* rm = "rm -rf /etc/installurl.bak";
  // system(rm);

  xy_info ("chsrc: 为 openbsd 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}


void
os_mysys2_setsrc(char* option)
{
  int selected = 0;
  for (int i=0;i<sizeof(os_mysys2_sources);i++) {
    // 循环测速
  }
  const char* source_name = os_mysys2_sources[selected].mirror->name;
  const char* source_abbr = os_mysys2_sources[selected].mirror->abbr;
  const char* source_url  = os_mysys2_sources[selected].url;

  char* backup = "cp -rf /etc/pacman.d/mirrorlist.mingw32 /etc/pacman.d/mirrorlist.mingw32.bak";
  system(backup);
        backup = "cp -rf /etc/pacman.d/mirrorlist.mingw64 /etc/pacman.d/mirrorlist.mingw64.bak";
  system(backup);
        backup = "cp -rf /etc/pacman.d/mirrorlist.msys /etc/pacman.d/mirrorlist.msys.bak";
  system(backup);

  xy_info ("chsrc: 备份文件名:1. /etc/pacman.d/mirrorlist.mingw32.bak");
  xy_info ("chsrc: 备份文件名:2. /etc/pacman.d/mirrorlist.mingw64.bak");
  xy_info ("chsrc: 备份文件名:3. /etc/pacman.d/mirrorlist.msys.bak");

  char* cmd = xy_strjoin(3,"sed -i \"s#https\?://mirror.msys2.org/#",source_url,"#g\" /etc/pacman.d/mirrorlist* ");
  system(cmd);
  free(cmd);

  // char* rm = "rm -rf /etc/pacman.d/mirrorlist.mingw32.bak";
  // system(rm);
  //       rm = "rm -rf /etc/pacman.d/mirrorlist.mingw64.bak";
  // system(rm);
  //       rm = "rm -rf /etc/pacman.d/mirrorlist.msys.bak";
  // system(rm);

  xy_info ("chsrc: 为 mysys2 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
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
def_target_info(pl_r);
def_target_info(pl_julia);

target_info
  pl_dotnet_target = {pl_dotnet_setsrc, NULL,  pl_dotnet_sources, pl_dotnet_sources_n};


#define targetinfo(t) (const char const*)t
static const char const
*pl_ruby  [] = {"gem",   "ruby",    "rb",    "rubygems", NULL,  targetinfo(&pl_ruby_target)},
*pl_python[] = {"pip",   "python",  "py",    "pypi",     NULL,  targetinfo(&pl_python_target)},
*pl_nodejs[] = {"npm",   "node",    "js",    "nodejs",   NULL,  targetinfo(&pl_nodejs_target)},
*pl_perl  [] = {"perl",  "cpan",                         NULL,  targetinfo(&pl_perl_target)},
*pl_rust  [] = {"rust",  "cargo",   "crate",  "crates",  NULL,  targetinfo(&pl_rust_target)},
*pl_go    [] = {"go",    "golang",  "goproxy",           NULL,  targetinfo(&pl_go_target)} ,
*pl_dotnet[] = {"nuget", "net",     ".net",   "dotnet",  NULL,  targetinfo(&pl_dotnet_target)},
*pl_java  [] = {"java",  "maven",   "gradle",            NULL,  targetinfo(&pl_java_target)},
*pl_php   [] = {"php",   "composer",                     NULL,  targetinfo(&pl_php_target)},
*pl_r     [] = {"r",     "cran",                         NULL,  targetinfo(&pl_r_target)},
*pl_julia [] = {"julia",                                 NULL,  targetinfo(&pl_julia_target)},
**pl_packagers[] =
{
  pl_ruby,    pl_python,  pl_nodejs,  pl_perl,
  pl_rust,    pl_go,      pl_dotnet,  pl_java,   pl_php,
  pl_r,       pl_julia
};


target_info
  os_ubuntu_target  = {os_ubuntu_setsrc, NULL, os_ubuntu_sources, 7},
  os_debian_target  = {os_debian_setsrc, NULL, os_debian_sources, 7},
  os_fedora_target  = {os_ubuntu_setsrc, NULL, os_ubuntu_sources, 7},
  os_kali_target    = {os_ubuntu_setsrc, NULL, os_ubuntu_sources, 7},
  os_openbsd_target = {os_ubuntu_setsrc, NULL, os_ubuntu_sources, 7},
  os_mysys2_target  = {os_ubuntu_setsrc, NULL, os_ubuntu_sources, 7};
static const char const
*os_ubuntu   [] = {"ubuntu", NULL,  targetinfo(&os_ubuntu_target)},
*os_debian   [] = {"debian", NULL,  targetinfo(&os_debian_target)},
*os_fedora   [] = {"debian", NULL,  targetinfo(&os_debian_target)},
*os_kali     [] = {"debian", NULL,  targetinfo(&os_debian_target)},
*os_openbsd  [] = {"debian", NULL,  targetinfo(&os_debian_target)},
*os_mysys2   [] = {"debian", NULL,  targetinfo(&os_debian_target)},
**os_systems[] =
{
  os_ubuntu, os_debian
};


target_info
  wr_anaconda_target = {NULL, NULL, NULL, 0},
  wr_emacs_target    = {NULL, NULL, NULL, 0},
  wr_tex_target      = {NULL, NULL, NULL, 0},
  wr_brew_target     = {NULL, NULL, NULL, 0};

static const char const
*wr_anaconda[] = {"conda", "anaconda",         NULL,  targetinfo(&wr_anaconda_target)},
*wr_emacs   [] = {"emacs",                     NULL,  targetinfo(&wr_emacs_target)},
*wr_tex     [] = {"latex", "ctan",     "tex",  NULL,  targetinfo(&wr_tex_target) },
*wr_brew    [] = {"brew",  "homebrew",         NULL,  targetinfo(&wr_brew_target)},
**wr_softwares[] =
{
  wr_anaconda, wr_emacs, wr_tex, wr_brew
};
#undef targetinfo
/************************************** End Target Matrix ****************************************/


static const char const*
usage[] = {
  "维护: https://gitee.com/RubyMetric/chsrc\n",

  "使用: chsrc <command> [target] [mirror]",
  "help                      打印此帮助，或 h, -h, --help",
  "list (或 ls, 或 l)        查看可用镜像源，和可换源软件",
  "list mirror(s)            查看可用镜像源",
  "list target(s)            查看可换源软件",
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
print_supported_targets_ (const char const*** array, size_t size)
{
  for (int i=0; i<size; i++)
  {
    const char ** target = array[i];
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



/**
 * 用于 chsrc list <target>
 */
void
print_supported_sources_for_target (source_info sources[])
{
  for (int i=0;i<4;i++)
  {
    source_info src = sources[i];
    const mirror_info* mir = src.mirror;
    printf ("%-14s%-18s%-50s ",mir->code, mir->abbr, src.url);
    puts(mir->name);
  }
}




int
print_help ()
{
  puts(xy_strjoin(4, "Change Source ",
                      xy_str_to_magenta(Chsrc_Version),
                      " by ",
                      xy_str_to_underline(xy_str_to_bold(xy_str_to_red("RubyMetric\n")))
                      ));
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
iterate_targets_(const char const*** array, size_t size, const char* input, const char const*** target_info)
{
  int matched = 0;

  const char const** target = NULL;
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
  const char const** target_tmp = NULL;

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
    print_supported_sources_for_target (target->sources);
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
  const char* target  = NULL;

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
