/* --------------------------------------------------------------
* File          : chsrc.c
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-28>
* Last modified : <2023-08-31>
*
* chsrc:
*
*   Change Source —— 换源命令行工具
* -------------------------------------------------------------*/

#include "chsrc.h"

#define Chsrc_Version "v0.1.0.20230910.pre"


/**
 * 检测二进制程序是否存在
 *
 * @param[in]  check_cmd  检测 `progname` 是否存在的一段命令，一般来说，填 `progname` 本身即可，
 *                        但是某些情况下，需要使用其他命令绕过一些特殊情况，比如 python 这个命令在Windows上
 *                        会自动打开 Microsoft Store，需避免
 *
 * @param[in]  progname   要检测的二进制程序名
 */
bool
does_the_program_exist (char* check_cmd, char* progname)
{
  char* which = check_cmd;

  int ret = system(which);

  char buf[32] = {0};
  sprintf(buf, "错误码: %d", ret);

  if (0!=ret) {
    xy_warn (xy_strjoin(4, "× 命令 ", progname, " 不存在，", buf));
    return false;
  } else {
    xy_success (xy_strjoin(3, "√ 命令 ", progname, " 存在"));
    return true;
  }
}




/**
 * Perl换源
 *
 * 参考：https://help.mirrors.cernet.edu.cn/CPAN/
 */
void
pl_perl_chsrc (char* option)
{
  int selected = 0; char* check_cmd, *prog = NULL;

  if (xy_on_windows) check_cmd = "perl --version >nul 2>nul";
  else               check_cmd = "perl --version 1>/dev/null 2>&1";

  bool exist_b = does_the_program_exist (check_cmd, "perl");

  if (!exist_b) {
    xy_error ("chsrc: 未找到 perl 相关命令，请检查是否存在");
    return;
  }

  for (int i=0;i<sizeof(pl_perl_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_perl_sources[selected].mirror->name;
  const char* source_abbr = pl_perl_sources[selected].mirror->abbr;
  const char* source_url  = pl_perl_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  char* cmd = xy_strjoin(3,
  "perl -MCPAN -e 'CPAN::HandleConfig->edit(\"pushy_https\", 0); CPAN::HandleConfig->edit(\"urllist\", \"unshift\", \"",
   source_url,
  "\"); mkmyconfig'");

  system(cmd);
  free(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}



/**
 * NodeJS换源
 *
 * 参考：https://npmmirror.com/
 */
void
pl_nodejs_chsrc (char* option)
{
  int selected = 0; char* check_cmd, *prog = NULL;

  if (xy_on_windows) check_cmd = "npm -v >nul 2>nul";
  else               check_cmd = "npm -v 1>/dev/null 2>&1";

  bool exist_b = does_the_program_exist (check_cmd, "npm");

  if (!exist_b) {
    xy_error ("chsrc: 未找到 npm 相关命令，请检查是否存在");
    return;
  }

  for (int i=0;i<sizeof(pl_nodejs_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_nodejs_sources[selected].mirror->name;
  const char* source_abbr = pl_nodejs_sources[selected].mirror->abbr;
  const char* source_url  = pl_nodejs_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  char* cmd = xy_2strjoin("npm config set registry  ", source_url);
  system(cmd);
  free(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}


/**
 * Python换源
 *
 * 参考：https://mirrors.tuna.tsinghua.edu.cn/help/pypi/
 *
 * 经测试，Windows上调用换源命令，会写入 C:\Users\RubyMetric\AppData\Roaming\pip\pip.ini
 */
void
pl_python_chsrc (char* option)
{
  int selected = 0; char* check_cmd, *prog = NULL;

  // 不要调用 python 自己，而是使用 python --version，避免Windows弹出Microsoft Store
  if (xy_on_windows) check_cmd = "python --version >nul 2>nul";
  else               check_cmd = "python --version 1>/dev/null 2>&1";

  bool exist_b = does_the_program_exist (check_cmd, "python");

  if (!exist_b) {
    if (xy_on_windows) check_cmd = "python3 --version >nul 2>nul";
    else               check_cmd = "python3 --version 1>/dev/null 2>&1";
    exist_b = does_the_program_exist (check_cmd, "python3");
    if (exist_b) prog = "python3";
  }
  else {
    prog = "python";
  }

  if (!exist_b) {
    xy_error ("chsrc: 未找到 Python 相关命令，请检查是否存在");
    return;
  }

  for (int i=0;i<sizeof(pl_python_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_python_sources[selected].mirror->name;
  const char* source_abbr = pl_python_sources[selected].mirror->abbr;
  const char* source_url  = pl_python_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  char* cmd = xy_2strjoin(prog, xy_2strjoin(" -m pip config set global.index-url ", source_url));
  system(cmd);
  free(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}


/**
 * Ruby换源
 *
 * 参考：https://gitee.com/RubyKids/rbenv-cn
 */
void
pl_ruby_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;
  for (int i=0;i<sizeof(pl_ruby_sources);i++) {
    // 循环测速
  }

  if (xy_on_windows) check_cmd = "gem -v >nul 2>nul";
  else               check_cmd = "gem -v 1>/dev/null 2>&1";
  bool exist_b = does_the_program_exist (check_cmd, "gem");
  if (!exist_b) {
    xy_error ("chsrc: 未找到 gem 相关命令，请检查是否存在");
    return;
  }

  const char* source_name = pl_ruby_sources[selected].mirror->name;
  const char* source_abbr = pl_ruby_sources[selected].mirror->abbr;
  const char* source_url  = pl_ruby_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  xy_info("chsrc: 为 gem 命令换源");
  system("gem source -r https://rubygems.org/");

  char* cmd = xy_2strjoin("gem source -a ", source_url);
  system(cmd);
  free(cmd);


  if (xy_on_windows) check_cmd = "bundle -v >nul 2>nul";
  else               check_cmd = "bundle -v 1>/dev/null 2>&1";
  exist_b = does_the_program_exist (check_cmd, "bundle");
  if (!exist_b) {
    xy_error ("chsrc: 未找到 bundle 相关命令，请检查是否存在");
    return;
  }

  cmd = xy_2strjoin("bundle config 'mirror.https://rubygems.org' ", source_url);
  xy_info("chsrc: 为 bundler 命令换源");
  system(cmd);
  free(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}



/**
 * Go换源
 *
 * 参考：https://goproxy.cn/
 */
void
pl_go_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  if (xy_on_windows) check_cmd = "go --version >nul 2>nul";
  else               check_cmd = "go --version 1>/dev/null 2>&1";

  bool exist_b = does_the_program_exist (check_cmd, "go");

  if (!exist_b) {
    xy_error ("chsrc: 未找到 go 相关命令，请检查是否存在");
    return;
  }

  for (int i=0;i<sizeof(pl_go_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_go_sources[selected].mirror->name;
  const char* source_abbr = pl_go_sources[selected].mirror->abbr;
  const char* source_url  = pl_go_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));
  char* cmd = "go env -w GO111MODULE=on";
  system(cmd);

  cmd = xy_strjoin(3, "go env -w GOPROXY=", source_url, ",direct");
  system(cmd);
  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}



/**
 * Rust 换源
 *
 * 参考：https://help.mirrors.cernet.edu.cn/crates.io-index.git
 */
void
pl_rust_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  for (int i=0;i<sizeof(pl_rust_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_rust_sources[selected].mirror->name;
  const char* source_abbr = pl_rust_sources[selected].mirror->abbr;
  const char* source_url  = pl_rust_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));


  const char* file = xy_strjoin (3,
    "[source.crates-io]\n"
    "replace-with = 'mirror'\n\n"

    "[source.mirror]\n"
    "registry = \"", source_url, "\"");


  char* cmd = NULL;
  if (xy_on_windows)
    cmd = xy_strjoin(3, "echo ", file, ">> \%USERPROFILE%\\.cargo");
  else
    cmd = xy_strjoin(3, "echo ", file, ">> $HOME/.cargo");

  system(cmd);
  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}




/**
 * NuGet 换源
 *
 */
void
pl_dotnet_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  xy_error ("chsrc: 暂时无法为NuGet换源，若有需求，请您提交issue");
}




/**
 * PHP 换源
 *
 * 参考：https://developer.aliyun.com/composer
 */
void
pl_php_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  if (xy_on_windows) check_cmd = "composer --version >nul 2>nul";
  else               check_cmd = "composer --version 1>/dev/null 2>&1";

  bool exist_b = does_the_program_exist (check_cmd, "composer");

  if (!exist_b) {
    xy_error ("chsrc: 未找到 composer 相关命令，请检查是否存在");
    return;
  }

  for (int i=0;i<sizeof(pl_php_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_php_sources[selected].mirror->name;
  const char* source_abbr = pl_php_sources[selected].mirror->abbr;
  const char* source_url  = pl_php_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  char* cmd = xy_2strjoin("composer config repo.packagist composer ", source_url);
  system(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}



/**
 * Java 换源
 *
 * 参考：https://developer.aliyun.com/mirror/maven
 */
void
pl_java_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  if (xy_on_windows) check_cmd = "mvn --version >nul 2>nul";
  else               check_cmd = "mvn --version 1>/dev/null 2>&1";
  bool mvn_exist_b    = does_the_program_exist (check_cmd, "mvn");

  if (xy_on_windows) check_cmd = "gradle --version >nul 2>nul";
  else               check_cmd = "gradle --version 1>/dev/null 2>&1";
  bool gradle_exist_b = does_the_program_exist (check_cmd, "gradle");

  if (!mvn_exist_b && !gradle_exist_b) {
    xy_error ("chsrc: maven 与 gradle 命令均未找到，请检查是否存在（其一）");
    return;
  }

  for (int i=0;i<sizeof(pl_java_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_java_sources[selected].mirror->name;
  const char* source_abbr = pl_java_sources[selected].mirror->abbr;
  const char* source_url  = pl_java_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  if (mvn_exist_b) {
    const char* file = xy_strjoin(3,
    "<mirror>\n"
    "  <id>aliyunmaven</id>\n"
    "  <mirrorOf>*</mirrorOf>\n"
    "  <name>阿里云公共仓库</name>\n"
    "  <url>", source_url, "</url>\n"
    "</mirror>");

    xy_info ("chsrc: 请在您的 maven安装目录/conf/settings.xml 中添加:\n");
    puts (file);
  }

  if (gradle_exist_b) {
    const char* file = xy_strjoin(3,
    "allprojects {\n"
    "  repositories {\n"
    "  maven { url '", source_url, "' }\n"
    "  mavenLocal()\n"
    "  mavenCentral()\n"
    "  }\n"
    "}");

    xy_info ("chsrc: 请在您的 build.gradle 中添加:\n");
    puts (file);
  }

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}



/**
 * R 换源
 *
 * 参考：https://help.mirrors.cernet.edu.cn/CRAN/
 */
void
pl_r_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  for (int i=0;i<sizeof(pl_r_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_r_sources[selected].mirror->name;
  const char* source_abbr = pl_r_sources[selected].mirror->abbr;
  const char* source_url  = pl_r_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));


  const char* file = xy_strjoin (3, "options(\"repos\" = c(CRAN=\"", source_url, "\"))" );


  char* cmd = NULL;
  // TODO: 待确认，Windows 下是否也是该文件
  if (xy_on_windows)
    cmd = xy_strjoin(3, "echo ", file, " >> %USERPROFILE%/.Rprofile");
  else
    cmd = xy_strjoin(3, "echo ", file, " >> ~/.Rprofile");

  system(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}




/**
 * Julia 换源
 *
 * 参考：https://help.mirrors.cernet.edu.cn/julia/
 */
void
pl_julia_chsrc (char* option)
{
  int selected = 0; char* check_cmd = NULL;

  for (int i=0;i<sizeof(pl_r_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_r_sources[selected].mirror->name;
  const char* source_abbr = pl_r_sources[selected].mirror->abbr;
  const char* source_url  = pl_r_sources[selected].url;

  xy_info (xy_2strjoin("chsrc: 选中镜像站：", source_abbr));

  const char* file = xy_strjoin (3, "ENV[\"JULIA_PKG_SERVER\"] = \"", source_url, "\"");

  char* cmd = NULL;
  // TODO: $JULIA_DEPOT_PATH/config/startup.jl 是否要考虑环境变量
  if (xy_on_windows)
    cmd = xy_strjoin(3, "echo ", file, " >> %USERPROFILE%/.julia/config/startup.jl");
  else
    cmd = xy_strjoin(3, "echo ", file, " >> ~/.julia/config/startup.jl");
  system(cmd);

  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}




void
os_ubuntu_chsrc (char* option)
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
  const char* current_url = xy_strch(source_url,'/',"\\/");

  char* cmd = xy_strjoin(3, "sed -E \'s/(^[^#]* .*)http[:|\\.|\\/|a-z|A-Z]*\\/ubuntu\\//\\1",
                          current_url,
                          "/\'< /etc/apt/sources.list.bak | cat > /etc/apt/sources.list");
  system(cmd);
  free(cmd);

  char* rm = "rm -rf /etc/apt/source.list.bak";
  system(rm);
  // free(rm);

  xy_info ("chsrc: 为 ubuntu 命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}



#define chsrcfunc(func) (const char const*)func
static const char const
*pl_ruby[]   = {"gem",  "ruby",    "rb",                NULL,  chsrcfunc(pl_ruby_chsrc)  },
*pl_python[] = {"pip",  "python",  "py",      "pypi",   NULL,  chsrcfunc(pl_python_chsrc)},
*pl_nodejs[] = {"npm",  "node",    "nodejs",  "js",     NULL,  chsrcfunc(pl_nodejs_chsrc)},
*pl_perl[]   = {"perl", "cpan",                         NULL,  chsrcfunc(pl_perl_chsrc)},

*pl_rust[]   = {"rust", "cargo",   "crate",   "crates", NULL, chsrcfunc(pl_rust_chsrc)},
*pl_go[]     = {"go",   "golang",  "goproxy",           NULL, chsrcfunc(pl_go_chsrc)},
*pl_dotnet[] = {"nuget","net",     "dotnet",  ".net",   NULL, chsrcfunc(pl_dotnet_chsrc)},
*pl_java[]   = {"maven", "gradle",                      NULL, chsrcfunc(pl_java_chsrc)},
*pl_php[]    = {"php",  "composer",                     NULL, chsrcfunc(pl_php_chsrc)},

*pl_cran[]   = {"r",    "cran",                         NULL, chsrcfunc(pl_r_chsrc)},
*pl_julia[]  = {"julia",                                NULL, chsrcfunc(pl_julia_chsrc)},

**pl_packagers[] = {
  pl_ruby,    pl_python,  pl_nodejs,  pl_perl,
  pl_rust,    pl_go,      pl_dotnet,  pl_java,  pl_php,
  pl_cran, pl_julia
},


*os_ubuntu[] = {"ubuntu", NULL,  chsrcfunc(os_ubuntu_chsrc)},

**os_systems[] = {
  os_ubuntu
},


*wr_tex[] = {"tex", "latex", "ctan",   NULL,  NULL},

**wr_softwares[] = {wr_tex};

#undef chsrcfunc



static const char const*
usage[] = {
  "chsrc: Change Source \e[1;35m" Chsrc_Version "\e[0m by \e[4;31mRubyMetric\e[0m\n",

  "维护:  https://gitee.com/RubyMetric/chsrc\n",

  "使用：chsrc <command> [target]",
  "help                  # 打印此帮助，或 -h, --help",
  "list [target]         # 查看可换源软件，或该软件可以换哪些源",
  "cesu <target>         # 对该软件所有源测速",
  "get  <target>         # 查看当前软件的源使用情况",
  "set  <target>         # 换源，自动测速后挑选最快源",
  "set  <target> -1      # 1,2,3的1。换源，不测速，挑选经维护者测速排序的第一源",
  "set  <target> -v      # 换源，并打印换源所执行的具体操作\n"
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


int
print_help ()
{
  for (int i=0; i<Array_Size(usage); i++) {
    puts (usage[i]);
  }
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

  /* chsrc help */
  if (xy_streql(command, "-h") ||
      xy_streql(command, "help") ||
      xy_streql(command, "--help"))
  {
    print_help();
    return 0;
  }

  /* chsrc list */
  else if (xy_streql(command, "list"))
  {
    if (argc < 2) {
      puts("chsrc: 列出所有支持的软件，以及所有可用源");
    } else {
      puts("chsrc: 列出所有可用镜像站");
    }
    return 0;
  }


  /* chsrc cesu */
  else if (xy_streql(command, "cesu"))
  {
    if (argc < 2) {
      xy_error ("chsrc: 请您提供想要测速源的软件名; 使用 chsrc list 查看所有支持的软件");
      return 1;
    }
    puts("chsrc: 测试提供该软件源的镜像站点速度");
    return 0;
  }


  /* chsrc get */
  else if (xy_streql(command, "get"))
  {
    if (argc < 2) {
      xy_error ("chsrc: 请您提供想要查看源的软件名; 使用 chsrc list 查看所有支持的软件");
      return 1;
    }
    puts("chsrc: 查看该软件的换源情况");
    return 0;
  }

  /* chsrc set */
  else if (xy_streql(command, "set"))
  {
    if (argc < 2) {
      xy_error ("chsrc: 请您提供想要设置源的软件名; 使用 chsrc list 查看所有支持的软件");
      return 1;
    }
    puts("chsrc: 开始换源");
    return 0;
  }

  /* 不支持的命令 */
  else
  {
    xy_error ("chsrc: 不支持的命令，请使用 chsrc help 查看使用方式");
    return 1;
  }



  const char* option = NULL;
  const char* cmdarg = NULL;

  if (argc>=2)
  {
    if (argv[2][0]=='-') {
      option = argv[2];
    } else {
      cmdarg = argv[2];
    }
  }

  int matched = 0;

  for (int i=0; i<Array_Size(pl_packagers); i++) {
    const char const** packager = pl_packagers[i];
    int k = 0;
    const char* alias = packager[k];
    while (NULL!=alias) {
      if (xy_streql(target, alias)) {
        // printf("matched: %s\n", alias);
        matched = 1; break;
      }
      k++;
      alias = packager[k];
    }
    if (matched) {
      do {
        k++; alias = packager[k];
      } while (NULL!=alias);
      call_cmd ((void*) packager[k+1], cmdarg);
    }
  }
  goto match_end;

  for (int i=0; i<Array_Size(os_systems); i++) {
    const char const** system = os_systems[i];
    int k = 0;
    const char* alias = system[k];
    while (NULL!=alias) {
      // printf("%s matched: %s\n",target, alias);
      if (xy_streql(target, alias)) {
        // printf("matched: %s\n", alias);
        matched = 1; break;
      }
      k++;
      alias = system[k];
    }
    if (matched) {
      do {
        k++; alias = system[k];
      } while (NULL!=alias);
      call_cmd ((void*) system[k+1], cmdarg);
    }
  }
  goto match_end;

  for (int i=0; i<Array_Size(wr_softwares); i++) {
    const char const** ware = wr_softwares[i];
    int k = 0;
    const char* alias = ware[k];
    while (NULL!=alias) {
      // printf("%s matched: %s\n",target, alias);
      if (xy_streql(target, alias)) {
        // printf("matched: %s\n", alias);
        matched = 1; break;
      }
      k++;
      alias = ware[k];
    }
    if (matched) {
      do {
        k++; alias = ware[k];
      } while (NULL!=alias);
      call_cmd ((void*) ware[k+1], cmdarg);
    }
  }

match_end:
  if (!matched) {
    xy_info("chsrc: 暂不支持的换源类型，请使用chsrc help查看可换源");
  }
  return 0;
}
