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
 * Last Modified   : <2024-08-15>
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



void
pl_dart_getsrc (char *option)
{
  char *cmd = NULL;
  if (xy_on_windows)
    {
      cmd = "set PUB_HOSTED_URL & set FLUTTER_STORAGE_BASE_URL";
      chsrc_run (cmd, RunOpt_Default);
    }
  else
    {
      cmd = "echo $PUB_HOSTED_URL; echo $FLUTTER_STORAGE_BASE_URL";
      chsrc_run (cmd, RunOpt_Default);
    }
}

/**
 * Dart pub 换源，参考：https://mirrors.tuna.tsinghua.edu.cn/help/dart-pub/
 */
void
pl_dart_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (pl_dart);
  chsrc_confirm_source (&source);

  char *towrite = NULL;

  char *pub = xy_2strjoin(source.url, "dart-pub");
  char *flutter = xy_2strjoin(source.url, "flutter");

  if (xy_on_windows)
    {
      if (xy_file_exist (xy_win_powershell_profile))
        {
          towrite = xy_strjoin (4, "$env:PUB_HOSTED_URL = \"", pub, "\"");
          chsrc_append_to_file (towrite, xy_win_powershell_profile);

          towrite = xy_strjoin (4, "$env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\"");
          chsrc_append_to_file (towrite, xy_win_powershell_profile);
        }

      if (xy_file_exist (xy_win_powershellv5_profile))
        {
          towrite = xy_strjoin (4, "$env:PUB_HOSTED_URL = \"", pub, "\"");
          chsrc_append_to_file (towrite, xy_win_powershellv5_profile);

          towrite = xy_strjoin (4, "$env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\"");
          chsrc_append_to_file (towrite, xy_win_powershellv5_profile);
        }
    }
  else
    {
      towrite = xy_strjoin (3, "export PUB_HOSTED_URL=\"", pub, "\"");
      chsrc_append_to_file (towrite, "~/.bashrc >> ~/.zshrc");

      towrite = xy_strjoin (3, "export FLUTTER_STORAGE_BASE_URL=\"", flutter, "\"");
      chsrc_append_to_file (towrite, "~/.bashrc >> ~/.zshrc");
    }
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



void
pl_haskell_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (pl_haskell);
  chsrc_confirm_source (&source);

  char *file = xy_strjoin (3, "repository mirror\n"
                              "  url: ", source.url,
                            "\n  secure: True");

  char *config = NULL;
  if (xy_on_windows)
    {
      config = xy_uniform_path ("~/AppData/Roaming/cabal/config");
    }
  else
    {
      config = "~/.cabal/config";
    }

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  puts (file); puts ("");

  config = xy_uniform_path ("~/.stack/config.yaml");
  file = xy_strjoin (3, "package-indices:\n"
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

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  puts (file);
  chsrc_say_lastly (&source, ChsrcTypeManual);
}


#include "recipe/lang/ocaml.c"
#include "recipe/lang/r.c"



/**
 * Julia的换源可以通过两种方式
 * 1. 写入 startup.jl
 * 2. 使用环境变量
 *
 * 我们采用第一种
 */
void
pl_julia_getsrc (char *option)
{
  chsrc_view_file ("~/.julia/config/startup.jl");
}

/**
 * Julia 换源，参考：
 * 1. https://help.mirrors.cernet.edu.cn/julia/
 * 2. https://docs.julialang.org/en/v1/manual/command-line-interface/#Startup-file
 */
void
pl_julia_setsrc (char *option)
{
  SourceInfo source;
  chsrc_yield_source (pl_julia);
  chsrc_confirm_source (&source);

  const char *towrite = xy_strjoin (3, "ENV[\"JULIA_PKG_SERVER\"] = \"", source.url, "\"");

  chsrc_append_to_file (towrite, "~/.julia/config/startup.jl");
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}







#define ETC_APT_SOURCELIST "/etc/apt/sources.list"
#define ETC_OSRELEASE      "/etc/os-release"

#define Debian_debian       1
#define Debian_deriv_ubuntu 2

/**
 * 当不存在该文件时，我们只能拼凑一个假的出来，但该函数目前只适用于 Ubuntu 和 Debian
 * 因为其它的 Debian 变体可能不使用 ETC_APT_SOURCELIST，也可能并不适用 `VERSION_CODENAME`
 *
 * @return 文件是否存在
 */
bool
ensure_apt_sourcelist (int debian_type)
{
  bool exist = chsrc_check_file (ETC_APT_SOURCELIST);

  if (exist)
    {
      return true;
    }
  else
    {
      chsrc_note2 ("将生成新的源配置文件");
    }

  // 反向引用需要escape一下
  char *codename = xy_run ("sed -nr 's/VERSION_CODENAME=(.*)/\\1/p' " ETC_OSRELEASE, 0, NULL);
  codename = xy_str_delete_suffix (codename, "\n");

  char *version_id = xy_run ("sed -nr 's/VERSION_ID=(.*)/\\1/p' " ETC_OSRELEASE, 0, NULL);
  version_id = xy_str_delete_suffix (codename, "\n");
  double version = atof (version_id);

  char *makeup = NULL;

  if (debian_type == Debian_deriv_ubuntu)
    {
      makeup = xy_strjoin (9,
               "# Generated by chsrc " Chsrc_Version "\n\n"
               "deb " Chsrc_Maintain_URL "/ubuntu ", codename, " main restricted universe multiverse\n"
               "deb " Chsrc_Maintain_URL "/ubuntu ", codename, "-updates main restricted universe multiverse\n"
               "deb " Chsrc_Maintain_URL "/ubuntu ", codename, "-backports main restricted universe multiverse\n"
               "deb " Chsrc_Maintain_URL "/ubuntu ", codename, "-security main restricted universe multiverse\n");
    }
  else
    {
      if (version >= 12)
        {
          // https://wiki.debian.org/SourcesList
          // https://mirrors.tuna.tsinghua.edu.cn/help/debian/
          // 从 Debian 12 开始，开始有一项 non-free-firmware
          makeup = xy_strjoin (9,
               "# Generated by chsrc " Chsrc_Version "\n\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, " main contrib non-free non-free-firmware\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, "-updates main contrib non-free non-free-firmware\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, "-backports main contrib non-free non-free-firmware\n"
               "deb " Chsrc_Maintain_URL "/debian-security ", codename, "-security main contrib non-free non-free-firmware\n");
               // 上述 debian-security 这种写法是和 Debian 10不同的，所以我们只能支持 Debian 11+
        }
      else if (version >= 11)
        {
          makeup = xy_strjoin (9,
               "# Generated by chsrc " Chsrc_Version "(" Chsrc_Maintain_URL ")\n\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, " main contrib non-free\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, "-updates main contrib non-free\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, "-backports main contrib non-free\n"
               "deb " Chsrc_Maintain_URL "/debian-security ", codename, "-security main contrib non-free\n");
        }
      else if (version >= 10)
        {
          makeup = xy_strjoin (9,
               "# Generated by chsrc " Chsrc_Version "(" Chsrc_Maintain_URL ")\n\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, " main contrib non-free\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, "-updates main contrib non-free\n"
               "deb " Chsrc_Maintain_URL "/debian ", codename, "-backports main contrib non-free\n"
               "deb " Chsrc_Maintain_URL "/debian-security ", codename, "/updates main contrib non-free\n");
               // 上述 debian-security 这种写法是和 Debian 11 不同的
        }
      else
        {
          chsrc_error ("您的Debian版本过低(<10)，暂不支持换源");
          exit (Exit_Unsupported);
        }
    }

  FILE *f = fopen (ETC_APT_SOURCELIST, "w");
  fwrite (makeup, strlen (makeup), 1, f);
  fclose (f);
  return false;
}



/**
 * @note 从 Debian 12 开始，Debain 的软件源配置文件变更为 DEB822 格式，
 *       路径为:  /etc/apt/sources.list.d/debian.sources"
 *
 * @note 从 Ubuntu 24.04 开始，Ubuntu 的软件源配置文件变更为 DEB822 格式，
 *       路径为:  /etc/apt/sources.list.d/ubuntu.sources
 */
#define ETC_APT_DEB822_Debian_Sources "/etc/apt/sources.list.d/debian.sources"
#define ETC_APT_DEB822_Ubuntu_Sources "/etc/apt/sources.list.d/ubuntu.sources"

void
os_ubuntu_getsrc (char *option)
{
  if (chsrc_check_file (ETC_APT_DEB822_Ubuntu_Sources))
    {
      chsrc_view_file (ETC_APT_DEB822_Ubuntu_Sources);
      return;
    }

  if (chsrc_check_file (ETC_APT_SOURCELIST))
    {
      chsrc_view_file (ETC_APT_SOURCELIST);
      return;
    }

  chsrc_error2 ("缺少源配置文件！但仍可直接通过 chsrc set ubuntu 来添加使用新的源");
  return;
}


/**
 * 此函数基本和 os_ubuntu_setsrc() 一致
 */
void
os_ubuntu_setsrc_for_deb822 (char *option)
{
  SourceInfo source;
  chsrc_yield_source (os_ubuntu);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_DEB822_Ubuntu_Sources);

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (strncmp (arch, "x86_64", 6)==0)
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " ETC_APT_DEB822_Ubuntu_Sources);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " ETC_APT_DEB822_Ubuntu_Sources);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}

/**
 * @note 不同架构下换源不一样
 */
void
os_ubuntu_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (ETC_APT_DEB822_Ubuntu_Sources))
    {
      chsrc_note2 ("将基于新格式换源");
      os_ubuntu_setsrc_for_deb822 (option);
      return;
    }

  bool sourcelist_exist = ensure_apt_sourcelist (Debian_deriv_ubuntu);

  SourceInfo source;
  chsrc_yield_source (os_ubuntu);
  chsrc_confirm_source (&source);

  // 不存在的时候，用的是我们生成的无效文件，不要备份
  if (sourcelist_exist)
    {
      chsrc_backup (ETC_APT_SOURCELIST);
    }

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (0==strncmp (arch, "x86_64", 6))
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " ETC_APT_SOURCELIST);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " ETC_APT_SOURCELIST);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



void
os_mint_getsrc (char *option)
{
  chsrc_view_file ("/etc/apt/sources.list.d/official-package-repositories.list");
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/linuxmint/
 */
void
os_mint_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_mint);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/apt/sources.list.d/official-package-repositories.list");

  char* cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url,
                            "@g' /etc/apt/sources.list.d/official-package-repositories.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
  chsrc_warn2 ("完成后请不要再使用 mintsources（自带的图形化软件源设置工具）进行任何操作，因为在操作后，无论是否有按“确定”，mintsources 均会覆写我们刚才换源的内容");
}



void
os_debian_getsrc (char *option)
{
  if (chsrc_check_file (ETC_APT_DEB822_Debian_Sources))
    {
      chsrc_view_file (ETC_APT_DEB822_Debian_Sources);
      return;
    }

  if (chsrc_check_file (ETC_APT_SOURCELIST))
    {
      chsrc_view_file (ETC_APT_SOURCELIST);
      return;
    }

  chsrc_error2 ("缺少源配置文件！但仍可直接通过 chsrc set debian 来添加使用新的源");
  return;
}

void
os_debian_setsrc_for_deb822 (char *option)
{
  SourceInfo source;
  chsrc_yield_source (os_debian);
  chsrc_confirm_source (&source);

  chsrc_note2 ("如果遇到无法拉取 HTTPS 源的情况，我们会使用 HTTP 源并需要您运行:");
  puts ("apt install apt-transport-https ca-certificates");

  chsrc_backup (ETC_APT_DEB822_Debian_Sources);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/debian/?@", source.url, "@g' " ETC_APT_DEB822_Debian_Sources);
  chsrc_run (cmd, RunOpt_Default);

  // debian-security 源和其他源不一样
  cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/debian-security/?@", source.url, "-security@g' " ETC_APT_DEB822_Debian_Sources);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}


/**
 * Debian 10 Buster 以上版本默认支持 HTTPS 源。如果遇到无法拉取 HTTPS 源的情况，请先使用 HTTP 源并安装
 * apt install apt-transport-https ca-certificates
 */
void
os_debian_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (ETC_APT_DEB822_Debian_Sources))
    {
      chsrc_note2 ("将基于新格式换源");
      os_debian_setsrc_for_deb822 (option);
      return;
    }


  // Docker环境下，Debian镜像可能不存在该文件
  bool sourcelist_exist = ensure_apt_sourcelist (Debian_debian);

  SourceInfo source;
  chsrc_yield_source (os_debian);
  chsrc_confirm_source (&source);

  chsrc_note2 ("如果遇到无法拉取 HTTPS 源的情况，我们会使用 HTTP 源并需要您运行:");
  puts ("apt install apt-transport-https ca-certificates");

  // 不存在的时候，用的是我们生成的无效文件，不要备份
  if (sourcelist_exist)
    {
      chsrc_backup (ETC_APT_SOURCELIST);
    }

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/debian/?@", source.url, "@g\' " ETC_APT_SOURCELIST);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



void
os_raspberrypi_getsrc (char *option)
{
  chsrc_view_file ("/etc/apt/sources.list.d/raspi.list");
}

void
os_raspberrypi_setsrc (char *option)
{
  // chsrc_ensure_root(); // HELP: 不确定是否需要

  SourceInfo source;
  chsrc_yield_source (os_raspberrypi);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/apt/sources.list.d/raspi.list");

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url,
                            "@g' /etc/apt/sources.list.d/raspi.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}


#define OS_Armbian_SOURCELIST "/etc/apt/sources.list.d/armbian.list"
void
os_armbian_getsrc (char *option)
{
  if (chsrc_check_file (OS_Armbian_SOURCELIST))
    {
      chsrc_view_file (OS_Armbian_SOURCELIST);
      return;
    }

  chsrc_error2 ("缺少源配置文件！路径：" OS_Armbian_SOURCELIST);
}

/**
 * 参考: https://mirrors.tuna.tsinghua.edu.cn/help/armbian
 */
void
os_armbian_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_armbian);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_Armbian_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?[^ ]*armbian/?[^ ]*@", source.url,
                             "@g' " OS_Armbian_SOURCELIST);

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}
#undef OS_Armbian_SOURCELIST



void
os_deepin_getsrc(char *option)
{
  chsrc_view_file (ETC_APT_SOURCELIST);
}

/**
 * HELP: 未经测试
 */
void
os_deepin_setsrc (char *option)
{
  chsrc_ensure_root();

  SourceInfo source;
  chsrc_yield_source (os_deepin);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/deepin/?@",
                              source.url,
                              "@g\' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



/**
 * @note fedora 29 及以下版本暂不支持
 */
void
os_fedora_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_fedora);
  chsrc_confirm_source (&source);

  chsrc_note2 ("Fedora 29 及以下版本暂不支持");

  chsrc_backup ("/etc/yum.repos.d/fedora.repo");
  chsrc_backup ("/etc/yum.repos.d/fedora-updates.repo");

  char* cmd = xy_strjoin (9, "sed -e 's|^metalink=|#metalink=|g' ",
         "-e 's|^#baseurl=http://download.example/pub/fedora/linux/|baseurl=",
         source.url,
         "|g' ",
         "-i.bak ",
         "/etc/yum.repos.d/fedora.repo ",
         "/etc/yum.repos.d/fedora-modular.repo ",
         "/etc/yum.repos.d/fedora-updates.repo ",
         "/etc/yum.repos.d/fedora-updates-modular.repo");

  chsrc_run (cmd, RunOpt_Default);

  chsrc_log2 ("已替换文件 /etc/yum.repos.d/fedora.repo");
  chsrc_log2 ("已新增文件 /etc/yum.repos.d/fedora-modular.repo");
  chsrc_log2 ("已替换文件 /etc/yum.repos.d/fedora-updates.repo");
  chsrc_log2 ("已新增文件 /etc/yum.repos.d/fedora-updates-modular.repo");

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



/**
 * HELP: 未经测试
 */
void
os_opensuse_setsrc (char *option)
{
  chsrc_ensure_root (); // HELP: 不知道是否需要确保root权限

  SourceInfo source;
  chsrc_yield_source (os_opensuse);
  chsrc_confirm_source (&source);

  char *source_nselect = "zypper mr -da";
  chsrc_run (source_nselect, RunOpt_Default);

  char *cmd1 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/repo/oss/' mirror-oss");
  char *cmd2 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/repo/non-oss/' mirror-non-oss");
  char *cmd3 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/oss/' mirror-update");
  char *cmd4 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/non-oss/' mirror-update-non-oss");
  char *cmd5 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/sle/' mirror-sle-update");
  char *cmd6 = xy_strjoin (3,
    "zypper ar -cfg '",
    source.url,
    "/opensuse/distribution/leap/$releasever/backports/' mirror-backports-update");

  chsrc_run (cmd1, RunOpt_Default);
  chsrc_run (cmd2, RunOpt_Default);
  chsrc_run (cmd3, RunOpt_Default);
  chsrc_run (cmd4, RunOpt_Default);

  chsrc_note2 ("leap 15.3用户还需要添加sle和backports源");
  chsrc_note2 ("另外请确保系统在更新后仅启用了六个软件源，可以使用 zypper lr 检查软件源状态");
  chsrc_note2 ("并使用 zypper mr -d 禁用多余的软件源");

  chsrc_run (cmd5, RunOpt_Default);
  chsrc_run (cmd6, RunOpt_Default);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



void
os_kali_getsrc (char *option)
{
  chsrc_view_file (ETC_APT_SOURCELIST);
}

/**
 * HELP: 未经测试
 */
void
os_kali_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_kali);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/kali/?@",
                              source.url,
                             "@g\' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



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



#define OS_Pacman_MirrorList "/etc/pacman.d/mirrorlist"
void
os_arch_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_MirrorList);
}

/**
 * 参考:
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/archlinux/
 * 2. https://mirrors.tuna.tsinghua.edu.cn/help/archlinuxarm/
 */
void
os_arch_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_arch);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_Pacman_MirrorList);

  bool  is_x86 = false;
  char *to_write = NULL;
  char *arch = chsrc_get_cpuarch ();

  if (strncmp(arch, "x86_64", 6)==0)
    {
      is_x86 = true;
      to_write = xy_strjoin (3, "Server = ", source.url, "/$repo/os/$arch");
    }
  else
    {
      is_x86 = false;
      to_write = xy_strjoin (3, "Server = ", source.url, "arm/$arch/$repo");
    }

  // 越前面的优先级越高
  chsrc_prepend_to_file (to_write, OS_Pacman_MirrorList);

  if (is_x86)
    {
      chsrc_run ("pacman -Syyu", RunOpt_No_Last_New_Line);
    }
  else
    {
      chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);
    }
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}


void
os_archlinuxcn_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_MirrorList);
}

/**
 * 参考 https://mirrors.tuna.tsinghua.edu.cn/help/archlinuxcn/
 */
void
os_archlinuxcn_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_archlinuxcn);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_Pacman_MirrorList);

  bool  arch_flag = false;
  char *arch = chsrc_get_cpuarch ();

  char *towrite = xy_strjoin (3, "[archlinuxcn]\nServer = ", source.url, "$arch");
  // 越前面的优先级越高
  chsrc_prepend_to_file (towrite, OS_Pacman_MirrorList);

  chsrc_run ("pacman-key --lsign-key \"farseerfc@archlinux.org\"", RunOpt_Dont_Abort_On_Failure);
  chsrc_run ("pacman -Sy archlinuxcn-keyring", RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}
#undef OS_Pacman_MirrorList

/**
 * HELP: 未经测试
 */
void
os_gentoo_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_gentoo);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/portage/repos.conf/gentoo.conf");

  char *cmd = xy_strjoin (3, "sed -i \"s#rsync://.*/gentoo-portage#rsync://",
                             source.url,
                            "gentoo-portage#g");
  chsrc_run (cmd, RunOpt_Default);

  char *towrite = xy_strjoin (3, "GENTOO_MIRRORS=\"https://", source.url, "gentoo\"");

  chsrc_append_to_file (towrite, "/etc/portage/make.conf");
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



/**
 * 参考: https://help.mirrors.cernet.edu.cn/rocky/
 */
void
os_rocky_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_rocky);
  chsrc_confirm_source (&source);

  char *cmd = xy_strjoin (3,
            "sed -e 's|^mirrorlist=|#mirrorlist=|g' "
            "-e 's|^#baseurl=http://dl.rockylinux.org/$contentdir|baseurl=", source.url, "|g' "
            "-i.bak /etc/yum.repos.d/rocky-extras.repo /etc/yum.repos.d/rocky.repo"
            );
  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}


/**
 * 参考: https://developer.aliyun.com/mirror/almalinux
 */
void
os_alma_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_alma);
  chsrc_confirm_source (&source);

  char *cmd = xy_strjoin (3,
    "sed -e 's|^mirrorlist=|#mirrorlist=|g' -e 's|^#\\s*baseurl=https://repo.almalinux.org/almalinux|baseurl=", source.url, "|g'  -i.bak  /etc/yum.repos.d/almalinux*.repo");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



void
os_alpine_getsrc (char *option)
{
  chsrc_view_file ("/etc/apk/repositories");
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/alpine/
 */
void
os_alpine_setsrc (char *option)
{
  // chsrc_ensure_root(); // HELP: 不确定是否需要root

  SourceInfo source;
  chsrc_yield_source (os_alpine);
  chsrc_confirm_source (&source);

  char* cmd = xy_strjoin (3,
            "sed -i 's#https\\?://dl-cdn.alpinelinux.org/alpine#", source.url, "#g' /etc/apk/repositories"
            );
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apk update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



void
os_void_getsrc (char *option)
{
  char* cmd = "xbps-query -L";
  chsrc_run (cmd, RunOpt_No_Last_New_Line);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/voidlinux/
 */
void
os_void_setsrc (char *option)
{
  // chsrc_ensure_root(); // HELP: 不确定是否需要root

  SourceInfo source;
  chsrc_yield_source (os_void);
  chsrc_confirm_source (&source);

  chsrc_ensure_dir ("/etc/xbps.d");
  char *cmd = "cp /usr/share/xbps.d/*-repository-*.conf /etc/xbps.d/";
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3,
            "sed -i 's|https://repo-default.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3,
            "sed -i 's|https://alpha.de.repo.voidlinux.org|", source.url, "|g' /etc/xbps.d/*-repository-*.conf"
            );

  chsrc_note2 ("若报错可尝试使用以下命令:");
  puts (cmd);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



/**
 * 参考: https://help.mirrors.cernet.edu.cn/solus/
 */
void
os_solus_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_solus);
  chsrc_confirm_source (&source);

  char *cmd = xy_2strjoin ("eopkg add-repo Solus ", source.url);
  chsrc_run (cmd, RunOpt_Default);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



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



void
os_trisquel_getsrc (char *option)
{
  chsrc_view_file (ETC_APT_SOURCELIST);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/trisquel/
 */
void
os_trisquel_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_trisquel);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/trisquel/?@", source.url, "@g' /etc/apt/sources.list");

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



void
os_linuxlite_getsrc (char *option)
{
  chsrc_view_file (ETC_APT_SOURCELIST);
}

/**
 * 参考: https://help.mirrors.cernet.edu.cn/linuxliteos/
 */
void
os_linuxlite_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_linuxlite);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/.*/?@", source.url, "@g' /etc/apt/sources.list");

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}



#include "./recipe/os/openwrt.c"



/**
 * HELP: 未经测试
 */
void
os_openeuler_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_openeuler);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/yum.repos.d/openEuler.repo");

  char *towrite = xy_strjoin (3, "s#http://repo.openeuler.org#",
                              source.url,
                              "#\'< /etc/yum.repos.d/openEuler.repo.bak");

  chsrc_overwrite_file (towrite, "/etc/yum.repos.d/openEuler.repo");

  chsrc_run ("dnf makecache", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeAuto);
}


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
  chsrc_view_file (ETC_APT_SOURCELIST);
}

void
os_openkylin_setsrc (char *option)
{
  chsrc_ensure_root();

  SourceInfo source;
  chsrc_yield_source (os_openkylin);
  chsrc_confirm_source (&source);

  chsrc_backup (ETC_APT_SOURCELIST);

  char *cmd = xy_strjoin (3, "sed -E -i 's@https?://.*/openkylin/?@", source.url, "@g'" ETC_APT_SOURCELIST);
  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



/**
 * 参考:
 *  1. https://book.bsdcn.org/di-3-zhang-ruan-jian-yuan-ji-bao-guan-li-qi/di-3.2-jie-freebsd-huan-yuan-fang-shi.html
 *  2. https://help.mirrors.cernet.edu.cn/FreeBSD-ports/
 *
 * 据 @ykla,
 *   FreeBSD 有五类源：pkg、ports、port、portsnap、update，其中 portsnap 在 FreeBSD 14 已经被移除了
 */
void
os_freebsd_setsrc (char *option)
{
  // 据 @ykla，FreeBSD不自带sudo，但是我们依然要保证是root权限
  chsrc_ensure_root ();

  int index = use_specific_mirror_or_auto_select (option, os_freebsd);

  SourceInfo source = os_freebsd_sources[index];
  chsrc_confirm_source (&source);

  chsrc_log2 ("1. 添加 freebsd-pkg 源 (二进制安装包)");
  chsrc_ensure_dir ("/usr/local/etc/pkg/repos");

  char *conf = xy_strjoin (3, "/usr/local/etc/pkg/repos/", source.mirror->code, ".conf");

  char *pkg_content = xy_strjoin (4,
                      source.mirror->code, ": { \n"
                      "  url: \"http://", source.url, "/freebsd-pkg/${ABI}/latest\",\n"
                      "}\n"
                      "FreeBSD: { enabled: no }"
                    );

  chsrc_overwrite_file (pkg_content, conf);
  chsrc_note2 (
    xy_strjoin (3, "若要使用季度分支，请在", conf ,"中将latest改为quarterly"));

  chsrc_note2 ("若要使用HTTPS源，请先安装securtiy/ca_root_ns，并将'http'改成'https'，最后使用'pkg update -f'刷新缓存即可\n");
  puts ("");

  chsrc_log2 ("2. 修改 freebsd-ports 源");
  // @ccmywish: [2023-09-27] 据 @ykla , NJU的freebsd-ports源没有设置 Git，
  //                         但是我认为由于使用Git还是要比非Git方便许多，我们尽可能坚持使用Git
  //                         而 gitup 又要额外修改它自己的配置，比较麻烦
  bool git_exist = query_program_exist (xy_str_to_quietcmd ("git version"), "git");
  if (git_exist)
    {
      if (xy_streql("nju",source.mirror->code))
        {
          source = os_freebsd_sources[index-1]; // 使用NJU的前一个源，即USTC源
        }
      char *git_cmd = xy_strjoin (3, "git clone --depth 1 https://", source.url, "/freebsd-ports/ports.git /usr/ports");
      chsrc_run (git_cmd, RunOpt_Default);
      source = os_freebsd_sources[index]; // 恢复至选中的源
      chsrc_note2 ("下次更新请使用 git -C /usr/ports pull 而非使用 gitup");
    }
  else
    {
      char *fetch  = xy_strjoin (3, "fetch https://", source.url, "/freebsd-ports/ports.tar.gz");  // 70多MB
      char *unzip  = "tar -zxvf ports.tar.gz -C /usr/ports";
      char *delete = "rm ports.tar.gz";
      chsrc_run (fetch, RunOpt_Default);
      chsrc_run (unzip, RunOpt_Default);
      chsrc_run (delete, RunOpt_Default);
      chsrc_log2 ("下次更新请重新下载内容至 /usr/ports");
    }


  chsrc_log2 ("3. 指定 port 源");
  // https://help.mirrors.cernet.edu.cn/FreeBSD-ports/
  chsrc_backup ("/etc/make.conf");

  char *ports = xy_strjoin (3, "MASTER_SITE_OVERRIDE?=http://", source.url, "/freebsd-ports/distfiles/${DIST_SUBDIR}/");
  chsrc_append_to_file (ports, "/etc/make.conf");


  /* 不再换 portsnap */
  /*
    chsrc_backup ("/etc/portsnap.conf");

    char *portsnap =xy_strjoin(3,"s@(.*)SERVERNAME=[\\.|a-z|A-Z]*@\\1SERVERNAME=", source.url,
                                "@g < /etc/portsnap.conf.bak");

    chsrc_overwrite_file (portsnap, "/etc/portsnap.conf");

    chsrc_log2 ("portsnap sources changed");
    chsrc_log2 ("获取portsnap更新使用此命令: 'portsnap fetch extract'");
  */


  // HELP: 暂时没有源提供
  chsrc_note2 ("4. 抱歉，目前境内无 freebsd-update 源，若存在请报告issue，谢谢");
  /*
    chsrc_log2 ("3. 修改 freebsd-update 源");

    char *update_cp = "cp /etc/freebsd-update.conf /etc/freebsd-update.conf.bak";
    chsrc_run (update_cp, RunOpt_Default);

    char *update =xy_strjoin (3,"s@(.*)SERVERNAME [\\.|a-z|A-Z]*@\\1SERVERNAME ",
                                 source.url,
                                "@g < /etc/freebsd-update.conf.bak");

    chsrc_overwrite_file (update, "/etc/freebsd-update.conf");
  */

  chsrc_say_lastly (&source, ChsrcTypeSemiAuto);
}



void
os_netbsd_getsrc (char *option)
{
  chsrc_view_file ("/usr/pkg/etc/pkgin/repositories.conf");
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
os_netbsd_setsrc (char *option)
{
  chsrc_ensure_root (); // HELP: 不知道是否需要确保root权限

  SourceInfo source;
  chsrc_yield_source (os_netbsd);
  chsrc_confirm_source (&source);

  chsrc_backup ("/usr/pkg/etc/pkgin/repositories.conf");

  char *arch = chsrc_get_cpuarch ();
  char *vercmd  = "cat /etc/os-release | grep \"VERSION=\" | grep -Po \"[8-9].[0-9]+\"";
  char *version = xy_run (vercmd, 0, NULL);

  char *url = xy_strjoin (5, source.url, arch, "/", version, "/All");
  chsrc_overwrite_file (url, "/usr/pkg/etc/pkgin/repositories.conf");

  chsrc_say_lastly (&source, ChsrcTypeUntested);
}



void
os_openbsd_getsrc (char *option)
{
  chsrc_view_file ("/etc/installurl");
}

/**
 * 参考：
 * 1. https://mirrors.tuna.tsinghua.edu.cn/help/openbsd/
 * 2. https://book.bsdcn.org/di-26-zhang-openbsd/di-26.2-jie-pei-zhi.html
 */
void
os_openbsd_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_openbsd);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/installurl");
  chsrc_overwrite_file (source.url, "/etc/installurl");

  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

/**
 * 参考：
 * https://mirrors.tuna.tsinghua.edu.cn/help/ros/
 */
void
os_ros_setsrc (char *option)
{
  chsrc_ensure_root ();

  SourceInfo source;
  chsrc_yield_source (os_ros);
  chsrc_confirm_source (&source);

  chsrc_backup ("/etc/apt/sources.list.d/ros-latest.list");

  char *cmd  = NULL;
  cmd = xy_strjoin(3, "sed -E -i \'s@https?://.*/ros/ubuntu/?@", source.url, "@/ros/ubuntug\' /etc/apt/sources.list");
  chsrc_run(cmd, RunOpt_Default);

  cmd = "apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}






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
  printf ("%-14s%-30s%-41s ", "code", "服务商简写", "服务商URL"); puts ("服务商名称");
  puts   ("-------------------------------------------------------------------------------------------------");
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
      printf ("%-14s%-35s%-45s ", "code", "服务商简写", "服务源URL"); puts ("服务商名称");
      puts   ("--------------------------------------------------------------------------------------------------------");
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
