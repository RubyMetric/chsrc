/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2023-2024 Aoran Zeng, Heng Guo
 * -------------------------------------------------------------
 * Project Authors : Aoran Zeng    <ccmywish@qq.com>
 *                 |  Heng Guo     <2085471348@qq.com>
 * Contributors    : Aaron Ruan    <aaron212cn@outlook.com>
 *                 |  Rui Chen     <rui@chenrui.dev>
 *                 | Shengwei Chen <414685209@qq.com>
 *                 |  BlockLune    <blocklune@gmail.com>
 *                 |
 * Created On      : <2023-08-28>
 * Last Modified   : <2024-08-18>
 *
 * chsrc: Change Source —— 全平台通用命令行换源工具
 * ------------------------------------------------------------*/

#define Chsrc_Version       "v0.1.7.3-2024/08/18"
#define Chsrc_Maintain_URL  "https://github.com/RubyMetric/chsrc"
#define Chsrc_Maintain_URL2 "https://gitee.com/RubyMetric/chsrc"

#include "chsrc.h"

#include "recipe/lang/Ruby.c"
#include "recipe/lang/Python.c"
#include "recipe/lang/Node.js.c"
#include "recipe/lang/Perl.c"
#include "recipe/lang/PHP.c"
#include "recipe/lang/Lua.c"
#include "recipe/lang/Go.c"
#include "recipe/lang/Java.c"
#include "recipe/lang/Rust.c"
#include "recipe/lang/Dart.c"
#include "recipe/lang/Haskell.c"
#include "recipe/lang/OCaml.c"
#include "recipe/lang/R.c"
#include "recipe/lang/Julia.c"
#include "recipe/lang/NuGet.c"
#include "recipe/lang/Clojure.c"



#include "recipe/os/APT/common.h"
// Debian-based
#include "recipe/os/APT/Debian.c"
#include "recipe/os/APT/Ubuntu.c"
#include "recipe/os/APT/Kali-Linux.c"
#include "recipe/os/APT/Raspberry-Pi-OS.c"
// Ubuntu-based
#include "recipe/os/APT/Linux-Mint.c"
#include "recipe/os/APT/Trisquel.c"
#include "recipe/os/APT/Linux-Lite.c"
// Debian-based and Ubuntu-based
#include "recipe/os/APT/Armbian.c"
// Independent
#include "recipe/os/APT/ROS.c"
#include "recipe/os/APT/openKylin.c"
#include "recipe/os/APT/deepin.c"

#include "recipe/os/YUM/common.h"
#include "recipe/os/YUM/Fedora-Linux.c"
#include "recipe/os/YUM/AlmaLinux.c"
#include "recipe/os/YUM/Rocky-Linux.c"
#include "recipe/os/YUM/openEuler.c"
#include "recipe/os/YUM/Anolis-OS.c"

#include "recipe/os/pacman/Arch-Linux.c"
#include "recipe/os/pacman/Manjaro-Linux.c"
#include "recipe/os/pacman/MSYS2.c"

#include "recipe/os/openSUSE.c"
#include "recipe/os/Alpine-Linux.c"
#include "recipe/os/Gentoo-Linux.c"
#include "recipe/os/Void-Linux.c"
#include "recipe/os/Solus.c"
#include "recipe/os/OpenWrt.c"

#include "recipe/os/BSD/FreeBSD.c"
#include "recipe/os/BSD/NetBSD.c"
#include "recipe/os/BSD/OpenBSD.c"



#include "recipe/ware/TeX-Live.c"
#include "recipe/ware/Emacs.c"
#include "recipe/ware/WinGet.c"
#include "recipe/ware/Homebrew.c"
#include "recipe/ware/CocoaPods.c"
#include "recipe/ware/Nix.c"
#include "recipe/ware/Guix.c"
#include "recipe/ware/Flathub.c"
#include "recipe/ware/Docker-Hub.c"
#include "recipe/ware/Anaconda.c"

#include "recipe/catalog.c"


static const char *
Chsrc_Usage[] = {
  "维护: " Chsrc_Maintain_URL "\n",

  "使用: chsrc <command> [options] [target] [mirror]",
  "help                      打印此帮助，或 h, -h, --help",
  "issue                     查看相关issue\n",
  "list (或 ls, 或 l)        列出可用镜像源，和可换源目标",
  "list mirror/target        列出可用镜像源，或可换源目标",
  "list os/lang/ware         列出可换源的操作系统/编程语言/软件\n",

  "measure <target>          对该目标所有源测速",
  "cesu    <target>          \n",

  "list <target>             查看该目标可用源与支持功能",
  "get  <target>             查看该目标当前源的使用情况\n",

  "set  <target>             换源，自动测速后挑选最快源",
  "set  <target>  first      换源，使用维护团队测速第一的源",
  "set  <target> <mirror>    换源，指定使用某镜像站 (通过list <target>查看)",
  "set  <target> https://url 换源，用户自定义源URL",
  "reset <target>            重置，使用上游默认使用的源\n",

  "选项:",
  "-dry                      Dry Run，模拟换源过程，命令仅打印并不运行",
  "-ipv6                     使用IPv6测速",
  "-local                    仅对本项目而非全局换源 (通过ls <target>查看支持情况)"
};



void
cli_print_available_mirrors ()
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
cli_print_supported_targets_ (const char ***array, size_t size)
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
      br();
    }
  br();
}

void
cli_print_supported_targets ()
{
  chsrc_info ("支持对以下目标换源 (同一行表示这几个命令兼容)"); puts("");
  puts (xy_str_to_blue ("编程语言开发"));
  puts ("-------------------------");
  cli_print_supported_targets_ (pl_packagers, xy_arylen(pl_packagers));
  puts (xy_str_to_blue ("操作系统"));
  puts ("-------------------------");
  cli_print_supported_targets_ (os_systems,   xy_arylen(os_systems));
  puts (xy_str_to_blue ("软件"));
  puts ("-------------------------");
  cli_print_supported_targets_ (wr_softwares, xy_arylen(wr_softwares));
}

void
cli_print_supported_pl ()
{
  chsrc_info ("支持对以下编程语言生态换源 (同一行表示这几个命令兼容)");
  cli_print_supported_targets_ (pl_packagers,   xy_arylen(pl_packagers));
}

void
cli_print_supported_os ()
{
  chsrc_info ("支持对以下操作系统换源 (同一行表示这几个命令兼容)");
  cli_print_supported_targets_ (os_systems,   xy_arylen(os_systems));
}

void
cli_print_supported_wr ()
{
  chsrc_info ("支持对以下软件换源 (同一行表示这几个命令兼容)");
  cli_print_supported_targets_ (wr_softwares,   xy_arylen(wr_softwares));
}



/**
 * 用于 chsrc list <target>
 */
void
cli_print_target_available_sources (SourceInfo sources[], size_t size)
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
cli_print_target_features (FeatInfo f, const char *input_target_name)
{
  say (to_boldpurple("\nFeatures:\n"));

  char *get_msg = xy_2strjoin (" Get: 查看当前源状态 | chsrc get ", input_target_name);
  if (f.can_get) printf (" %s%s\n", to_boldgreen(YesMark), to_purple(get_msg));
  else printf (" %s%s\n", to_boldred(NoMark), get_msg);puts("");

  char *reset_msg = xy_2strjoin (" Reset: 重置回默认源 | chsrc reset ", input_target_name);
  if (f.can_reset) printf (" %s%s\n", to_boldgreen(YesMark), to_purple(reset_msg));
  else printf (" %s%s\n", to_boldred(NoMark), reset_msg);puts("");

  char *user_define_msg = xy_strjoin (4, " UserDefine: 用户自定义换源URL | chsrc set ", input_target_name, " https://user-define-url.org/", input_target_name);
  if (f.can_user_define) printf (" %s%s\n", to_boldgreen(YesMark), to_purple(user_define_msg));
  else printf (" %s%s\n", to_boldred(NoMark), user_define_msg);puts("");


  char *locally_msg = xy_2strjoin (" Locally: 仅对本项目换源 | chsrc set -local ", input_target_name);

  switch (f.stcan_locally)
  {
  case CanNotFully:
    printf (" %s%s\n", to_boldred(NoMark), locally_msg);puts("");
    break;
  case CanFully:
    printf (" %s%s\n", to_boldgreen(YesMark), to_purple(locally_msg));puts("");
    break;
  case CanSemi:
    printf (" %s%s\n\n   %s\n", to_boldgreen(SemiYesMark), to_purple(locally_msg), f.locally);puts("");
    break;
  default:
    xy_unreach;
  }

  char *english_msg = xy_2strjoin (" English: 英文输出 | chsrc set -en ", input_target_name);
  // if (f.can_english) printf (" %s%s\n", to_boldgreen(YesMark), to_purple(english_msg));
  // else printf (" %s%s\n", to_boldred(NoMark), english_msg);


  if (f.note)
    {
      printf ("%s%s\n", to_boldyellow ("备注: "), to_boldyellow (f.note));
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
  br();
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
  "- https://github.com/RubyMetric/chsrc/issues\n"
  );

  /*
  if (chsrc_check_program ("gh"))
    {
      char *cmd = xy_str_to_quietcmd ("gh browse --repo RubyMetric/chsrc");
      system (cmd);
    }
  */
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
  TargetOp_Measure_Source,
  TargetOp_List_Config
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

  if (!matched) return false;

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
    }
  else if (TargetOp_Get_Source==code)
    {
      if (target->getfn) target->getfn("");
      else chsrc_error (xy_strjoin (3, "暂未对 ", input, " 实现get功能，邀您帮助: chsrc issue"));
    }
  else if (TargetOp_List_Config==code)
    {
      say (to_boldblue(xy_strjoin (3, "指定使用某源，请使用 chsrc set ", input, " <code>\n")));
      say (to_boldgreen("Available Sources: \n"));
      // chsrc_info (xy_strjoin (3, "下方 code 列，可用于指定使用某源，请使用 chsrc set ", input, " <code>\n"));
      printf (" %-14s%-35s%-43s ", "code", "镜像站简写", "换源URL"); puts ("镜像站名称");
      puts   ("---------    --------------    -----------------------------------------------    ---------------------");
      cli_print_target_available_sources (target->sources, target->sources_n);
      if (target->featfn)
        {
          FeatInfo fi = target->featfn("");
          cli_print_target_features (fi, input);
        }
    }
  else if (TargetOp_Measure_Source==code)
    {
      auto_select_ (target->sources, target->sources_n, input);
      return true;
    }
  return true;
}


int
main (int argc, char const *argv[])
{
  xy_useutf8 (); argc -= 1;

  if (0==argc)
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
  for (int i=2; i<=argc; i++)
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
          cli_print_available_mirrors ();
          br();
          cli_print_supported_targets ();
        }
      else
        {
          target = argv[cli_arg_Target_pos];
          if (xy_streql (target, "mirrors") || xy_streql (target, "mirror"))
            {
              cli_print_available_mirrors (); return 0;
            }
          else if (xy_streql (target, "targets") || xy_streql (target, "target"))
            {
              cli_print_supported_targets (); return 0;
            }
          else if (xy_streql (target, "os"))
            {
              cli_print_supported_os (); return 0;
            }
          else if (xy_streql (target, "lang") || xy_streql (target, "pl") || xy_streql (target, "language"))
            {
              cli_print_supported_pl(); return 0;
            }
          else if (xy_streql (target, "ware") || xy_streql (target, "software"))
            {
              cli_print_supported_wr (); return 0;
            }

          matched = get_target (target, TargetOp_List_Config, NULL);
          if (!matched) goto not_matched;
        }
      return 0;
  }


  /* chsrc measure */
  else if (xy_streql (command, "measure") ||
           xy_streql (command, "mea")  ||
           xy_streql (command, "m")    ||
           xy_streql (command, "cesu") ||
           xy_streql (command, "ce")   ||
           xy_streql (command, "c"))
    {
      if (argc < cli_arg_Target_pos)
        {
          chsrc_error ("请您提供想要测速源的软件名; 使用 chsrc list targets 查看所有支持的软件");
          return 1;
        }
      target = argv[cli_arg_Target_pos];
      matched = get_target (target, TargetOp_Measure_Source, NULL);
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
