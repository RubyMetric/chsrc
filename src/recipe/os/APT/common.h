/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   : happy game <happygame1024@gmail.com>
 *                |
 * Created On     : <2024-06-14>
 * Major Revision :      3
 * Last Modified  : <2025-07-14>
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

#define OS_Apt_SourceList   "/etc/apt/sources.list"
#define OS_Apt_SourceList_D "/etc/apt/sources.list.d/"

/**
 * @note 从 Debian 12 (bookworm) 开始，Debain 的软件源配置文件变更为 DEB822 格式，
 *       路径为:  /etc/apt/sources.list.d/debian.sources"
 *
 * @note 从 Ubuntu 24.04 开始，Ubuntu 的软件源配置文件变更为 DEB822 格式，
 *       路径为:  /etc/apt/sources.list.d/ubuntu.sources
 */
#define OS_Debian_SourceList_DEB822 "/etc/apt/sources.list.d/debian.sources"
#define OS_Ubuntu_SourceList_DEB822 "/etc/apt/sources.list.d/ubuntu.sources"

#define OS_Debian_old_SourceList OS_Apt_SourceList
#define OS_Ubuntu_old_SourceList OS_Apt_SourceList


#define ETC_OS_RELEASE    "/etc/os-release"

#define OS_Is_Debian_Literally  1
#define OS_Is_Ubuntu            2

// independent
#define OS_ROS_SourceList         OS_Apt_SourceList_D "ros-latest.list"

#define OS_Termux_SourceList      "/data/data/com.termux/files/usr" OS_Apt_SourceList


// Ubuntu based
#define OS_LinuxMint_SourceList   OS_Apt_SourceList_D "official-package-repositories.list"

// Debian based
#define OS_Armbian_SourceList     OS_Apt_SourceList_D "armbian.list"
#define OS_RaspberryPi_SourceList OS_Apt_SourceList_D "raspi.list"


/**
 * 当不存在该文件时，我们只能拼凑一个假的出来，但该函数目前只适用于 Ubuntu 和 Debian
 * 因为其它的 Debian 变体可能不使用 OS_Apt_SourceList，也可能并不适用 `VERSION_CODENAME`
 *
 * @return 文件是否存在
 */
bool
ensure_debian_or_ubuntu_old_sourcelist (int debian_type)
{
  bool exist = chsrc_check_file (OS_Apt_SourceList);

  if (exist)
    {
      return true;
    }
  else
    {
      char *msg = ENGLISH ? "Will generate a new source list file"
                          : "将生成新的源配置文件";
      chsrc_note2 (msg);
    }

  /* 反向引用需要escape */
  char *codename = xy_run ("sed -nr 's/VERSION_CODENAME=(.*)/\\1/p' " ETC_OS_RELEASE, 0);

  char *version_id = xy_run ("sed -nr 's/VERSION_ID=\"(.*)\"/\\1/p' " ETC_OS_RELEASE, 0);

  double version = atof (version_id);

  char *makeup = NULL;

  if (debian_type == OS_Is_Ubuntu)
    {
      makeup = xy_str_gsub (RAWSTR_os_apt_based_ubuntu_old_source_list, "@v@", Chsrc_Version);
      makeup = xy_str_gsub (makeup, "@1@", Chsrc_Maintain_URL);
      makeup = xy_str_gsub (makeup, "@2@", codename);
    }
  else
    {
      if (version >= 12)  /* bookworm */
        {
          makeup = xy_str_gsub (RAWSTR_os_apt_based_debian_12_source_list, "@v@", Chsrc_Version);
          makeup = xy_str_gsub (makeup, "@1@", Chsrc_Maintain_URL);
          makeup = xy_str_gsub (makeup, "@2@", codename);
        }
      else if (version >= 11) /* bullseye */
        {
          makeup = xy_str_gsub (RAWSTR_os_apt_based_debian_11_source_list, "@v@", Chsrc_Version);
          makeup = xy_str_gsub (makeup, "@1@", Chsrc_Maintain_URL);
          makeup = xy_str_gsub (makeup, "@2@", codename);
        }
      else if (version >= 10) /* buster */
        {
          makeup = xy_str_gsub (RAWSTR_os_apt_based_debian_10_source_list, "@v@", Chsrc_Version);
          makeup = xy_str_gsub (makeup, "@1@", Chsrc_Maintain_URL);
          makeup = xy_str_gsub (makeup, "@2@", codename);
        }
      else
        {
          char *msg = ENGLISH ? "Your Debian version is too low (<10) for chsrc to support"
                              : "你的 Debian 版本过低 (<10)，暂不支持换源";
          chsrc_error (msg);
          exit (Exit_Unsupported);
        }
    }

  FILE *f = fopen (OS_Apt_SourceList, "w");
  fwrite (makeup, strlen (makeup), 1, f);
  fclose (f);
  return false;
}
