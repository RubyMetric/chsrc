/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2023-2024 Aoran Zeng, Heng Guo
 * -------------------------------------------------------------
 * Project Authors : Aoran Zeng    <ccmywish@qq.com>
 *                 | Heng Guo      <2085471348@qq.com>
 * Contributors    : Aaron Ruan    <aaron212cn@outlook.com>
 *                 | Rui Chen      <rui@chenrui.dev>
 *                 | Shengwei Chen <414685209@qq.com>
 *                 | BlockLune     <blocklune@gmail.com>
 *                 |
 * Created On      : <2023-08-28>
 * Last Modified   : <2024-08-16>
 *
 * chsrc: Change Source —— 全平台通用命令行换源工具
 * ------------------------------------------------------------*/

#define Chsrc_Version       "v0.1.7.2.dev1-2024/08/09"
#define Chsrc_Maintain_URL  "https://github.com/RubyMetric/chsrc"
#define Chsrc_Maintain_URL2 "https://gitee.com/RubyMetric/chsrc"

#include "chsrc.h"

#include "recipe/lang/ruby.c"
#include "recipe/lang/python.c"
#include "recipe/lang/nodejs.c"
#include "recipe/lang/perl.c"
#include "recipe/lang/php.c"
#include "recipe/lang/lua.c"
#include "recipe/lang/go.c"
#include "recipe/lang/java.c"
#include "recipe/lang/rust.c"


void
pl_dotnet_getsrc (char *option)
{
  chsrc_error ("暂时无法查看NuGet源，若有需求，请您提交issue");
}

/**
 * NuGet 换源
 */
void
pl_dotnet_setsrc (char *option)
{
  chsrc_error ("暂时无法为NuGet换源，若有需求，请您提交issue");
}




void
pl_clojure_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (pl_clojure);
  chsrc_confirm_source (&source);

  chsrc_note2 ("抱歉，Clojure换源较复杂，您可手动查阅并换源:");
  puts (source.url);
  chsrc_say_lastly (&source, ChsrcTypeManual);
}


#include "recipe/lang/dart.c"
#include "recipe/lang/haskell.c"
#include "recipe/lang/ocaml.c"
#include "recipe/lang/r.c"
#include "recipe/lang/julia.c"




#include "recipe/os/apt-family/common.h"
#include "recipe/os/apt-family/debian.c"
#include "recipe/os/apt-family/ubuntu.c"
// Debian-based
#include "recipe/os/apt-family/armbian.c"
#include "recipe/os/apt-family/raspberrypi.c"
#include "recipe/os/apt-family/Kali-Linux.c"
// Ubuntu-based
#include "recipe/os/apt-family/Linux-Mint.c"
#include "recipe/os/apt-family/trisquel.c"
#include "recipe/os/apt-family/Linux-Lite.c"
// Independent
#include "recipe/os/apt-family/ros.c"
#include "recipe/os/apt-family/deepin.c"



#include "recipe/os/opensuse.c"


#include "recipe/os/yum-family/common.h"
#include "recipe/os/yum-family/Fedora-Linux.c"
#include "recipe/os/yum-family/AlmaLinux.c"
#include "recipe/os/yum-family/Rocky-Linux.c"
#include "recipe/os/yum-family/openEuler.c"

/**
 * HELP: 未经测试
 */
void
os_msys2_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (os_msys2);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/pacman.d/mirrorlist.mingw32");
  chsrc_backup ("/etc/pacman.d/mirrorlist.mingw64");
  chsrc_backup ("/etc/pacman.d/mirrorlist.msys");

  char *prev = xy_strjoin (3, "请针对你的架构下载安装此目录下的文件:",
                              source.url,
                             "distrib/<架构>/");
  chsrc_note2 (prev);

  char *cmd = xy_strjoin (3, "sed -i \"s#https\?://mirror.msys2.org/#",
                              source.url,
                             "#g\" /etc/pacman.d/mirrorlist* ");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



#include "recipe/os/arch.c"
#include "recipe/os/gentoo.c"


#include "recipe/os/alpine.c"
#include "recipe/os/void.c"
#include "recipe/os/solus.c"



/**
 * 似乎会弹出GUI，待确定
 */
void
os_manjaro_setsrc (char *option)
{
  chsrc_ensure_root ();
  char *cmd = "pacman-mirrors -i -c China -m rank";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (NULL, ChsrcTypeAuto);
}



#include "recipe/os/openwrt.c"



/**
 * 参考: https://mirrors.hust.edu.cn/docs/anolis
 */
void
os_anolis_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_anolis);
  chsrc_confirm_source (&source);

  char *cmd = xy_strjoin (3, "sed -i.bak -E 's|https?://(mirrors\\.openanolis\\.cn/anolis)|", source.url, "|g' /etc/yum.repos.d/*.repo");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("dnf makecache", RunOpt_Default);
  chsrc_run ("dnf update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}


void
os_openkylin_getsrc (char *option)
{
  chsrc_view_file (OS_Apt_SourceList);
}

void
os_openkylin_setsrc (char *option)
{
  chsrc_ensure_root();

  SourceInfo source;
  chsrc_yield_source (os_openkylin);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_Apt_SourceList);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/openkylin/?@", source.url, "@g'" OS_Apt_SourceList);
  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}


#include "recipe/os/freebsd.c"
#include "recipe/os/netbsd.c"
#include "recipe/os/openbsd.c"





void
wr_tex_check_cmd (bool *tlmgr_exist, bool *mpm_exist)
{
  *tlmgr_exist = chsrc_check_program ("tlmgr");
  *mpm_exist = chsrc_check_program ("mpm");

  if (!*tlmgr_exist && !*mpm_exist)
    {
      chsrc_error ("未找到 tlmgr 或 mpm 命令，请检查是否存在（其一）");
      exit(1);
    }
}

void
wr_tex_getsrc (char *option)
{
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd (&tlmgr_exist, &mpm_exist);

  if (tlmgr_exist)
    {
      chsrc_run("tlmgr option repository", RunOpt_Default);
    }
  if (mpm_exist)
    {
      chsrc_run("mpm --get-repository", RunOpt_Default);
    }
}

/**
 * 参考 https://help.mirrors.cernet.edu.cn/CTAN/
 */
void
wr_tex_setsrc (char *option)
{
  bool tlmgr_exist, mpm_exist;
  wr_tex_check_cmd (&tlmgr_exist, &mpm_exist);

  SourceInfo source;
  chsrc_yield_source (wr_tex);
  chsrc_confirm_source (&source);

  char *cmd = NULL;

  if (tlmgr_exist)
    {
      cmd = xy_2strjoin ("tlmgr option repository ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (mpm_exist)
    {
      char *miktex_url = xy_2strjoin (xy_str_delete_suffix (source.url, "texlive/tlnet"), "win32/miktex/tm/packages/");
      cmd = xy_2strjoin ("mpm --set-repository=", miktex_url);
      chsrc_run (cmd, RunOpt_Default);
    }

  chsrc_say_lastly (&source, ChsrcTypeUntested);
}


#include "recipe/ware/emacs.c"


void
wr_winget_getsrc (char *option)
{
  chsrc_run ("winget source list", RunOpt_Default);
}

/* 参考：https://mirrors.ustc.edu.cn/help/winget-source.html */
void
wr_winget_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_winget);
  chsrc_confirm_source (&source);

  chsrc_run ("winget source remove winget", RunOpt_Default);
  chsrc_run (xy_2strjoin ("winget source add winget ", source.url), RunOpt_Default);

  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

void
wr_winget_resetsrc (char *option)
{
  chsrc_run ("winget source reset winget", RunOpt_Default);
  chsrc_say_lastly (NULL, ChsrcTypeAuto);
}



void
wr_brew_getsrc (char *option)
{
  char *cmd = "echo HOMEBREW_API_DOMAIN=$HOMEBREW_API_DOMAIN;"
              "echo HOMEBREW_BOTTLE_DOMAIN=$HOMEBREW_BOTTLE_DOMAIN;"
              "echo HOMEBREW_BREW_GIT_REMOTE=$HOMEBREW_BREW_GIT_REMOTE;"
              "echo HOMEBREW_CORE_GIT_REMOTE=$HOMEBREW_CORE_GIT_REMOTE;";
  system (cmd);
}

/**
 * 参考自: https://mirrors.tuna.tsinghua.edu.cn/help/homebrew/
 *
 * 自brew 4.0.0 (2023 年 2 月 16日) 起，
 * HOMEBREW_INSTALL_FROM_API 会成为默认行为，无需设置。大部分用户无需再克隆 homebrew-core 仓库，故无需设置 HOMEBREW_CORE_GIT_REMOTE 环境变量；
 * 但是为了以防万一，我们还是为用户设置该环境变量
 */
void
wr_brew_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_brew);
  chsrc_confirm_source (&source);

  char *splitter        = "\n\n# Generated by chsrc " Chsrc_Version;
  char *api_domain      = xy_strjoin (3, "export HOMEBREW_API_DOMAIN=\"",      xy_2strjoin (source.url, "homebrew-bottles/api"),  "\"");
  char *bottle_domain   = xy_strjoin (3, "export HOMEBREW_BOTTLE_DOMAIN=\"",   xy_2strjoin (source.url, "homebrew-bottles"),      "\"");
  char *brew_git_remote = xy_strjoin (3, "export HOMEBREW_BREW_GIT_REMOTE=\"", xy_2strjoin (source.url, "git/homebrew/brew.git"), "\"");
  char *core_git_remote = xy_strjoin (3, "export HOMEBREW_CORE_GIT_REMOTE=\"", xy_2strjoin (source.url, "git/homebrew/homebrew-core.git"), "\"");

  char *zshrc = "~/.zshrc";
  chsrc_backup (zshrc);
  chsrc_append_to_file (splitter,        zshrc);
  chsrc_append_to_file (api_domain,      zshrc);
  chsrc_append_to_file (bottle_domain,   zshrc);
  chsrc_append_to_file (brew_git_remote, zshrc);
  chsrc_append_to_file (core_git_remote, zshrc);

  char *bashrc = "~/.bashrc";
  if (xy_file_exist (bashrc))
    {
      chsrc_backup ("~/.bashrc");
      chsrc_append_to_file (splitter,        bashrc);
      chsrc_append_to_file (api_domain,      bashrc);
      chsrc_append_to_file (bottle_domain,   bashrc);
      chsrc_append_to_file (brew_git_remote, bashrc);
      chsrc_append_to_file (core_git_remote, bashrc);
    }

  chsrc_say_lastly (&source, ChsrcTypeAuto);
  chsrc_note2 ("请您重启终端使Homebrew环境变量生效");
}



/**
 * 参考：https://mirrors.tuna.tsinghua.edu.cn/help/CocoaPods/
 */
void
wr_cocoapods_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_cocoapods);
  chsrc_confirm_source (&source);

  chsrc_note2 ("请手动执行以下命令:");

  say ("cd ~/.cocoapods/repos");
  say ("pod repo remove master");
  char *git_cmd = xy_strjoin (3, "git clone ", source.url, " master");
  say (git_cmd);
  say ("");

  chsrc_note2 ("最后进入项目工程目录，在Podfile中第一行加入:");
  char *source_str = xy_strjoin (3, "source '", source.url, "'");
  say (source_str);

  chsrc_say_lastly (&source, ChsrcTypeManual);
}



/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/guix
 */
void
wr_guix_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_guix);
  chsrc_confirm_source (&source);

  char *file =  xy_strjoin (3, "(list (channel\n"
                               "       (inherit (car %default-channels))\n"
                               "       (url \"", source.url, "\")))");

  chsrc_note2 ("为防止扰乱配置文件，请您手动写入以下内容到 ~/.config/guix/channels.scm 文件中");
  puts (file);
  chsrc_say_lastly (&source, ChsrcTypeManual);
}



void
wr_nix_check_cmd ()
{
  chsrc_ensure_program ("nix-channel");
}

/**
 * 参考:
 *  1. https://mirrors.bfsu.edu.cn/help/nix-channels/
 *  2. https://gitee.com/RubyMetric/chsrc/issues/I83894
 */
void
wr_nix_setsrc (char *option)
{
  wr_nix_check_cmd ();

  SourceInfo source;
  chsrc_yield_source (wr_nix);
  chsrc_confirm_source (&source);

  char *cmd = xy_strjoin (3, "nix-channel --add ", source.url, "nixpkgs-unstable nixpkgs");
  chsrc_run (cmd, RunOpt_Default);

  char *towrite = xy_strjoin (3, "substituters = ", source.url, "store https://cache.nixos.org/");
  chsrc_append_to_file (towrite , "~/.config/nix/nix.conf");

  chsrc_run ("nix-channel --update", RunOpt_Default);

  chsrc_note2 ("若您使用的是NixOS，请确认您的系统版本<version>（如22.11），并手动运行:");
  cmd = xy_strjoin (3, "nix-channel --add ", source.url, "nixpkgs-<version> nixpkgs");
  puts (cmd);

  cmd = xy_strjoin (3, "nix.settings.substituters = [ \"", source.url, "store\" ];");
  chsrc_note2 ("若您使用的是NixOS，请额外添加下述内容至 configuration.nix 中");
  puts (cmd);

  chsrc_say_lastly (&source, ChsrcTypeSemiAuto);
}


#include "recipe/ware/docker.c"


/**
 * 参考: https://mirrors.sjtug.sjtu.edu.cn/docs/flathub
 */
void
wr_flathub_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_flathub);
  chsrc_confirm_source (&source);

  chsrc_note2 ("若出现问题，可先调用以下命令:");
  char *note = xy_strjoin (3,
    "wget ", source.url, "/flathub.gpg\n"
    "flatpak remote-modify --gpg-import=flathub.gpg flathub"
  );
  puts (note);

  char *cmd = xy_2strjoin ("flatpak remote-modify flathub --url=", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



void
wr_anaconda_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (wr_anaconda);
  chsrc_confirm_source (&source);

  char *main  = xy_2strjoin (source.url, "pkgs/main");
  char *r     = xy_2strjoin (source.url, "pkgs/r");
  char *msys2 = xy_2strjoin (source.url, "pkgs/msys2");
  char *cloud = xy_2strjoin (source.url, "cloud");

  char *file = xy_strjoin (22,
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
  char *config = xy_2strjoin (xy_os_home, "/.condarc");

  if (xy_on_windows)
    {
      bool exist = chsrc_check_program ("conda");
      if (!exist)
        {
          chsrc_error ("未找到 conda 命令，请检查是否存在");
          exit (Exit_UserCause);
        }
      chsrc_run ("conda config --set show_channel_urls yes", RunOpt_Default);
    }

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  puts (file);

  chsrc_note2 ("然后运行 conda clean -i 清除索引缓存，保证用的是镜像站提供的索引");
  chsrc_say_lastly (&source, ChsrcTypeSemiAuto);
}


#include "recipe/catalog.c"


static const char *
Chsrc_Usage[] = {
  "维护: " Chsrc_Maintain_URL "\n",

  "使用: chsrc <command> [options] [target] [mirror]",
  "help                      打印此帮助，或 h, -h, --help",
  "issue                     查看相关issue",
  "list (或 ls, 或 l)        列出可用镜像源，和可换源软件",
  "list mirror/target        列出可用镜像源，或可换源软件",
  "list os/lang/ware         列出可换源的操作系统/编程语言/软件",
  "list <target>             查看该软件可以使用哪些源",
  "cesu <target>             对该软件所有源测速",
  "get  <target>             查看当前软件的源使用情况",
  "set  <target>             换源，自动测速后挑选最快源",
  "set  <target>  first      换源，使用维护团队测速第一的源",
  "set  <target> <mirror>    换源，指定使用某镜像站 (通过list命令查看)",
  "set  <target> https://abc 换源，用户自定义源URL",
  "reset <target>            重置，使用上游默认使用的源\n",

  "选项:",
  "-dry                      Dry Run，模拟换源过程，命令仅打印并不运行",
  "-ipv6                     使用IPv6测速",
  "-local                    仅对某项目而非全局换源 (通过issue命令查看支持情况)"
};



void
print_available_mirrors ()
{
  chsrc_info ("支持以下镜像站");
  chsrc_info ("下方 code 列，可用于指定使用某镜像站，请使用 chsrc set <target> <code>\n");
  printf (" %-13s%-35s%-38s", "code", "镜像站简写", "镜像站URL"); puts ("镜像站名称");
  puts   ("-------      -----------       -------------------------------------     ---------------------");
  // puts   ("-------------------------------------------------------------------------------------------------");
  for (int i = 0; i < xy_arylen (available_mirrors); i++)
    {
      MirrorSite* mir = available_mirrors[i];
      printf ("%-14s%-18s%-41s ", mir->code, mir->abbr, mir->site); puts (mir->name);
    }
}


void
print_supported_targets_ (const char ***array, size_t size)
{
  for (int i=0; i<size; i++)
    {
      const char **target = array[i];
      const char *alias = target[0];
      for (int k=1; alias!=NULL; k++)
        {
          printf ("%s\t", alias);
          alias = target[k];
        }
      puts ("");
    }
  puts ("");
}

void
print_supported_targets ()
{
  chsrc_info ("支持对以下目标换源 (同一行表示这几个命令兼容)"); puts("");
  puts (xy_str_to_blue ("编程语言开发"));
  puts ("-------------------------");
  print_supported_targets_ (pl_packagers, xy_arylen(pl_packagers));
  puts (xy_str_to_blue ("操作系统"));
  puts ("-------------------------");
  print_supported_targets_ (os_systems,   xy_arylen(os_systems));
  puts (xy_str_to_blue ("软件"));
  puts ("-------------------------");
  print_supported_targets_ (wr_softwares, xy_arylen(wr_softwares));
}

void
print_supported_pl ()
{
  chsrc_info ("支持对以下编程语言生态换源 (同一行表示这几个命令兼容)");
  print_supported_targets_ (pl_packagers,   xy_arylen(pl_packagers));
}

void
print_supported_os ()
{
  chsrc_info ("支持对以下操作系统换源 (同一行表示这几个命令兼容)");
  print_supported_targets_ (os_systems,   xy_arylen(os_systems));
}

void
print_supported_wr ()
{
  chsrc_info ("支持对以下软件换源 (同一行表示这几个命令兼容)");
  print_supported_targets_ (wr_softwares,   xy_arylen(wr_softwares));
}



/**
 * 用于 chsrc list <target>
 */
void
print_supported_sources_for_target (SourceInfo sources[], size_t size)
{
  for (int i=0;i<size;i++)
    {
      SourceInfo src = sources[i];
      const MirrorSite *mir = src.mirror;
      if (NULL == src.url)
        {
          src.url = "Please help to add the upstream url!";
        }
      printf ("%-14s%-18s%-50s ", mir->code, mir->abbr, src.url);
      puts (mir->name);
    }
}


void
cli_print_version ()
{
  puts (xy_strjoin (3, "chsrc: Change Source (GPL-3.0) ",
                      xy_str_to_magenta (Chsrc_Version), " by RubyMetric"));
}


void
cli_print_help ()
{
  puts ("");
  for (int i=0; i<xy_arylen (Chsrc_Usage); i++)
    {
      puts (Chsrc_Usage[i]);
    }
}

void
cli_print_issues ()
{
  puts (
  "chsrc issues: Gitee和GitHub两边同时接受issue\n\n"
  "- https://gitee.com/RubyMetric/chsrc/issues\n"
  "- https://github.com/RubyMetric/chsrc/issues\n\n"

  "目前支持reset命令的源:  https://gitee.com/RubyMetric/chsrc/issues/I9V6EV\n"
  "目前支持-local选项的源: https://gitee.com/RubyMetric/chsrc/issues/I9V5I0\n"
  "已收录的镜像站:         https://gitee.com/RubyMetric/chsrc/wikis\n"
  );

  if (chsrc_check_program ("gh"))
    {
      char *cmd = xy_str_to_quietcmd ("gh browse --repo RubyMetric/chsrc");
      system (cmd);
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
iterate_targets_ (const char ***array, size_t size, const char *input, const char ***target_info)
{
  int matched = 0;

  const char **target = NULL;
  int k = 0;
  const char *alias = NULL;

  for (int i=0; i<size; i++)
    {
      target = array[i];
      alias = target[k];
      while (NULL!=alias)
        {
          if (xy_streql(input, alias))
            {
              matched = 1; break;
            }
          k++;
          alias = target[k];
        }
      if (!matched) k = 0;
      if (matched) break;
    }

  if (!matched)
    {
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

typedef enum {
  TargetOp_Get_Source = 1,
  TargetOp_Set_Source,
  TargetOp_Reset_Source,
  TargetOp_Cesu_Source,
  TargetOp_List_Source
} TargetOp;

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
get_target (const char *input, TargetOp code, char *option)
{
  const char **target_tmp = NULL;

           bool matched = iterate_targets(pl_packagers, input, &target_tmp);
  if (!matched) matched = iterate_targets(os_systems,   input, &target_tmp);
  if (!matched) matched = iterate_targets(wr_softwares, input, &target_tmp);

  if (!matched) {
    return false;
  }

  TargetInfo *target = (TargetInfo*) *target_tmp;

  if (TargetOp_Set_Source==code)
    {
      if (target->setfn) target->setfn(option);
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现set功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_Reset_Source==code)
    {
      if (target->resetfn) target->resetfn(option);
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现reset功能，邀您帮助: chsrc issue"));
      // puts ("将重置并恢复上游默认使用的源");
    }
  else if (TargetOp_Get_Source==code)
    {
      if (target->getfn) target->getfn("");
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现get功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_List_Source==code)
    {
      chsrc_info (xy_strjoin (3, "对 ", input, " 支持以下镜像站"));
      chsrc_info (xy_strjoin (3, "下方 code 列，可用于指定使用某源，请使用 chsrc set ", input, " <code>\n"));
      printf (" %-14s%-35s%-43s ", "code", "镜像站简写", "换源URL"); puts ("镜像站名称");
      puts   ("---------    --------------    -----------------------------------------------    ---------------------");
      print_supported_sources_for_target (target->sources, target->sources_n);
    }
  else if (TargetOp_Cesu_Source==code)
    {
      auto_select_ (target->sources, target->sources_n, input-3);
      return true;
    }
  return true;
}


int
main (int argc, char const *argv[])
{
  xy_useutf8 (); argc -= 1;

  if (argc==0)
    {
      cli_print_version ();
      cli_print_help ();
      return 0;
    }

  const char *command = argv[1];

  // chsrc set target mirror
  //        1    2      3
  int cli_arg_Target_pos = 2;
  int cli_arg_Mirror_pos = cli_arg_Target_pos + 1;
  const char *target = NULL;


  // chsrc set -ipv6 target mirror
  //        1    2     3      4
  // argc = 4
  for (int i=2; i<=argc ;i++)
    {
      if (xy_str_start_with (argv[i], "-"))
        {
          if (xy_streql (argv[i], "-ipv6"))
            {
              CliOpt_IPv6 = true;
            }
          else if (xy_streql (argv[i], "-local"))
            {
              CliOpt_Locally = true;
            }
          else if (xy_streql (argv[i], "-en") || xy_streql (argv[i], "-english"))
            {
              CliOpt_InEnglish = true;
            }
          else if (xy_streql (argv[i], "-dry"))
            {
              CliOpt_DryRun = true;
              chsrc_log (to_boldyellow ("**开启Dry Run模式，模拟换源过程(跳过测速)，命令仅打印并不运行**\n"));
            }
          else
            {
              chsrc_error (xy_2strjoin ("未识别的命令行选项 ", argv[i])); return 1;
            }
          cli_arg_Target_pos++;
          cli_arg_Mirror_pos++;
        }
    }


  bool matched = false;

  /* chsrc help */
  if (xy_streql (command, "h")  ||
      xy_streql (command, "-h") ||
      xy_streql (command, "help") ||
      xy_streql (command, "--help"))
    {
      cli_print_version ();
      cli_print_help ();
      return 0;
    }

  /* chsrc -v */
  else if (xy_streql (command, "-v")       ||
           xy_streql (command, "-version") ||
           xy_streql (command, "--version")||
           xy_streql (command, "ver") ||
           xy_streql (command, "version"))
    {
      cli_print_version ();
      return 0;
    }

  /* chsrc list */
  else if (xy_streql (command, "list") ||
           xy_streql (command, "l")    ||
           xy_streql (command, "ls"))
    {
      if (argc < cli_arg_Target_pos)
        {
          print_available_mirrors ();
          puts ("");
          print_supported_targets ();
        }
      else
        {
          target = argv[cli_arg_Target_pos];
          if (xy_streql (target, "mirrors") || xy_streql (target, "mirror"))
            {
              print_available_mirrors (); return 0;
            }
          else if (xy_streql (target, "targets") || xy_streql (target, "target"))
            {
              print_supported_targets (); return 0;
            }
          else if (xy_streql (target, "os"))
            {
              print_supported_os (); return 0;
            }
          else if (xy_streql (target, "lang") || xy_streql (target, "pl") || xy_streql (target, "language"))
            {
              print_supported_pl(); return 0;
            }
          else if (xy_streql (target, "ware") || xy_streql (target, "software"))
            {
              print_supported_wr (); return 0;
            }

          matched = get_target (target, TargetOp_List_Source, NULL);
          if (!matched) goto not_matched;
        }
      return 0;
  }


  /* chsrc cesu */
  else if (xy_streql (command, "cesu") ||
           xy_streql (command, "ce")   ||
           xy_streql (command, "c"))
    {
      if (argc < cli_arg_Target_pos)
        {
          chsrc_error ("请您提供想要测速源的软件名; 使用 chsrc list targets 查看所有支持的软件");
          return 1;
        }
      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Cesu_Source, NULL);
      if (!matched) goto not_matched;
      return 0;
    }


  /* chsrc get */
  else if (xy_streql (command, "get") ||
           xy_streql (command, "g"))
    {
      if (argc < cli_arg_Target_pos)
        {
          chsrc_error ("请您提供想要查看源的软件名; 使用 chsrc list targets 查看所有支持的软件");
          return 1;
        }
      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Get_Source, NULL);
      if (!matched) goto not_matched;
      return 0;
    }

  /* chsrc set */
  else if (xy_streql (command, "set") ||
           xy_streql (command, "s"))
    {
      if (argc < cli_arg_Target_pos)
        {
          chsrc_error ("请您提供想要设置源的软件名; 使用 chsrc list targets 查看所有支持的软件");
          return 1;
        }

      target = argv[cli_arg_Target_pos];
      char *mirrorCode_or_url = NULL;
      if (argc >= cli_arg_Mirror_pos)
        {
          mirrorCode_or_url = xy_strdup (argv[cli_arg_Mirror_pos]);
        }

      matched = get_target (target, TargetOp_Set_Source, mirrorCode_or_url);
      if (!matched) goto not_matched;
      return 0;
    }

  /* chsrc reset */
  else if (xy_streql (command, "reset") ||
           xy_streql (command, "rest") ||
           xy_streql (command, "r"))
    {
      if (argc < cli_arg_Target_pos)
        {
          chsrc_error ("请您提供想要重置源的软件名; 使用 chsrc list targets 查看所有支持的软件");
          return 1;
        }

      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Reset_Source, NULL);
      if (!matched) goto not_matched;
      return 0;
    }

    /* chsrc issue */
  else if (xy_streql (command, "issue") ||
           xy_streql (command, "issues") ||
           xy_streql (command, "isue") ||
           xy_streql (command, "i"))
    {
      cli_print_issues ();
      return 0;
    }

  /* 不支持的命令 */
  else
    {
      chsrc_error (xy_strjoin (3, "不支持的命令 ", command, ". 请使用 chsrc help 查看使用方式"));
      return 1;
    }

not_matched:
  if (!matched)
    {
      chsrc_error ("暂不支持的换源目标，请使用 chsrc list targets 查看可换源软件");
      return 1;
    }
}
