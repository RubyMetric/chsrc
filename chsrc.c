/* --------------------------------------------------------------
* File          : chsrc.c
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-28>
* Last modified : <2023-08-30>
*
* chsrc:
*
*   Change Source —— 换源命令行工具
* -------------------------------------------------------------*/

#include "chsrc.h"

#define Chsrc_Version "v0.1.0"


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

  for (int i=0;i<sizeof(pl_ruby_sources);i++) {
    // 循环测速
  }

  const char* source_name = pl_ruby_sources[selected].mirror->name;
  const char* source_abbr = pl_ruby_sources[selected].mirror->abbr;
  const char* source_url  = pl_ruby_sources[selected].url;

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
  int selected = 0;
  for (int i=0;i<sizeof(pl_ruby_sources);i++) {
    // 循环测速
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

  cmd = xy_2strjoin("bundle config 'mirror.https://rubygems.org' ", source_url);
  xy_info("chsrc: 为 bundler 命令换源");
  system(cmd);
  free(cmd);

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

  xy_info ("chsrc: 为'ubuntu'命令换源");
  xy_success (xy_2strjoin("chsrc: 感谢镜像提供方：", source_name));
}


#define chsrcfunc(func) (const char const*)func
static const char const
*pl_ruby[]   = {"gem",  "ruby",    "rb",       NULL,          chsrcfunc(pl_ruby_chsrc)  },
*pl_python[] = {"pip",  "python",  "py",      "pypi",   NULL, chsrcfunc(pl_python_chsrc)},
*pl_nodejs[] = {"npm",  "node",    "nodejs",  "js",     NULL},
*pl_perl[]   = {"perl", "cpan",     NULL},
*pl_php[]    = {"php",  "composer", NULL},
*pl_cran[]   = {"r",    "cran",     NULL},
*pl_rust[]   = {"rust", "cargo",   "crate",   "crates",  NULL},
*pl_go[]     = {"go",   "golang",  "goproxy",  NULL},
*pl_dotnet[] = {"nuget","net",     "dotnet",  ".net",    NULL},
*pl_maven[]  = {"maven", NULL},
*pl_gradle[] = {"gradel",NULL},
*pl_julia[]  = {"julia", NULL},

*os_ubuntu[] = {"ubuntu", NULL,  chsrcfunc(os_ubuntu_chsrc)},
// Java暂时需要直接指定包管理器
// pl_java
**pl_packagers[] = {
  pl_ruby, pl_python, pl_nodejs, pl_perl,  pl_php,    pl_cran,
  pl_rust, pl_go,     pl_dotnet, pl_maven, pl_gradle, pl_julia
},
**os_packagers[] = {
  os_ubuntu
};

#undef chsrcfunc

static const char const*
usage[] = {
  "chsrc: Change Source " Chsrc_Version " by RubyMetric\n",

  "维护:  https://gitee.com/RubyMetric/chsrc\n",

  "使用：\n"
  "  chsrc  <要换源的对象>  [所换源名称]\n",

  "选项：\n"
  "  -h, --help           打印该帮助\n",

  "支持：\n"
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
    xy_info(usage[i]);
  }
}


int
main (int argc, char const *argv[])
{
  xy_useutf8();

  // 未提供参数时
  if (argc<=1) {
    print_help(); return 0;
  }

  // 第一个参数
  const char* target = NULL;
  if (0==strcmp("-h",argv[1]) || 0==strcmp("--help",argv[1])) {
    print_help(); return 0;
  } else {
    target = argv[1];
  }

  const char* option = NULL;
  const char* cmdarg = NULL;
  // 第二个参数
  if (argc>=3)
  {
    // printf ("argc = %d\n", argc);
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
      if (0==strcmp(target, alias)) {
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
  for (int i=0; i<Array_Size(os_packagers); i++) {
    const char const** packager = os_packagers[i];
    int k = 0;
    const char* alias = packager[k];
    while (NULL!=alias) {
      // printf("%s matched: %s\n",target, alias);
      if (0==strcmp(target, alias)) {
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

  if (!matched) {
    xy_info("chsrc: 暂不支持的换源类型，请使用-h查看可换源");
  }
  return 0;
}
