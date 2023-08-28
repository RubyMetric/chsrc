#include <stdio.h>
#include "helper.h"

#define Chsrc_Version "v0.1.0"


/**
 * Python换源
 *
 * 参考：https://mirrors.tuna.tsinghua.edu.cn/help/pypi/
 */
void
pl_chsrc_python (char* source_name)
{
  char* source_url = NULL;
  if (0==strcmp("tuna", source_name)) {
    puts("chsrc: Selected source provider: Tuna");
    source_url = "https://pypi.tuna.tsinghua.edu.cn/simple";
  }

  char* cmd = xy_strjoin("pip config set global.index-url ", source_url);
  system(cmd);
  free(cmd);
}


/**
 * Ruby换源
 *
 * 参考：https://gitee.com/RubyKids/rbenv-cn
 */
void
pl_chsrc_ruby (char* source_name)
{
  char* source_url = NULL;

  if (0==strcmp("rubychina", source_name)) {
    puts("chsrc: Selected source provider: Ruby China");
    source_url = "https://gems.ruby-china.com";
  }
  else if (0==strcmp("ali", source_name)) {
    puts("chsrc: Selected source provider: Alibaba OPSX");
    source_url = "https://mirrors.aliyun.com/rubygems/";
  }
  else if (0==strcmp("tencent", source_name)) {
    puts("chsrc: Selected source provider: Tencent");
    source_url = "http://mirrors.tencent.com/rubygems/";
  }
  else if (0==strcmp("tuna", source_name)) {
    puts("chsrc: Selected source provider: Tuna");
    source_url = "https://mirrors.tuna.tsinghua.edu.cn/rubygems/";
  }

  puts("chsrc: Change source for 'gem'");
  system("gem source -r https://rubygems.org/");


  char* cmd = xy_strjoin("gem source -a ", source_url);
  system(cmd);
  free(cmd);

  cmd = xy_strjoin("bundle config 'mirror.https://rubygems.org' ", source_url);
  puts("chsrc: Change source for 'bundler'");
  system(cmd);
  free(cmd);
}

#define arg(func) (const char const*)func
static const char const
*pl_ruby[]   = {"gem",  "ruby",    "rb",       NULL,  arg(pl_chsrc_ruby)},
*pl_python[] = {"pip",  "python",  "py",       NULL},
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
// Java暂时需要直接指定包管理器
// pl_java
**pl_packagers[] = {
  pl_ruby, pl_python, pl_nodejs, pl_perl,  pl_php,    pl_cran,
  pl_rust, pl_go,     pl_dotnet, pl_maven, pl_gradle, pl_julia
};
#undef arg

static const char const*
usage[] = {
  "chsrc: Change Source " Chsrc_Version " by RubyMetric\n",
  "Usage:\n"
  "  chsrc <target> [source]\n",

  "Options:\n"
  "  -h                     Print this help\n",

  "Supported:\n"
};


void
call_cmd (void* cmdptr, char* arg)
{
  void (*cmd_func)(char*) = cmdptr;
  cmd_func(arg);
}


int
print_help()
{
  for (int i=0; i<ARRAY_SIZE(usage); i++) {
    puts(usage[i]);
  }
}


int
main(int argc, char const *argv[])
{
  if (1==argc) {
    print_help(); return 0;
  }

  int matched = 0;

  for (int i=0; ARRAY_SIZE(pl_packagers); i++) {
    const char const** packager = pl_packagers[i];
    int k = 0;
    const char* alias = packager[k];
    while (NULL!=alias) {
      if (0==strcmp(argv[1], alias)) {
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
      call_cmd ((void*) packager[k+1], "tuna");
    }
  }

  return 0;
}
