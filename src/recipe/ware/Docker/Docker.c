/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

static MirrorSite_t DaoCloud =
{
  IS_DedicatedMirrorSite,
  "daocloud", "DaoCloud","上海道客网络科技有限公司", "https://www.daocloud.io/",
  // 没有找到 DaoCloud 合适的下载链接，先随便给一个，以规避 chsrc 自动测速时所有 Docker Hub 镜像站都没有测速链接带来的 Bug
  {NotSkip, NA, NA, "https://qiniu-download-public.daocloud.io/DaoCloud_Enterprise/dce5/offline-community-v0.18.0-amd64.tar", ACCURATE}
},

Fit2Cloud =
{
  IS_DedicatedMirrorSite,
  "fit2cloud", "FIT2CLOUD", "杭州飞致云信息科技有限公司", "https://www.fit2cloud.com/",
  {SKIP, ToFill, ToFill, NULL, ROUGH}
};

def_target(wr_docker, "docker/dockerhub");

void
wr_docker_prelude ()
{
  chef_prep_this (wr_docker, gs);

  chef_set_created_on   (this, "2024-06-08");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_chef (this, "@happy-game");
  chef_set_cooks (this, 2, "@happy-game", "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://hub.docker.com/", DelegateToUpstream},
  {&DaoCloud,         "https://docker.m.daocloud.io", DelegateToMirror},
  {&Fit2Cloud,        "https://docker.1panel.live", DelegateToMirror}
  def_sources_end()
}

#define WR_Docker_ConfigFile "/etc/docker/daemon.json"

void
wr_docker_getsrc (char *option)
{
  if (xy.on_linux || xy.on_bsd)
    {
      chsrc_view_file (WR_Docker_ConfigFile);
    }
  else
    {
      chsrc_note2 ("请打开 Docker Desktop 设置");
      chsrc_note2 ("选择 'Docker Engine' 选项卡，在该选项卡中找到 'registry-mirrors' 一栏查看");
    }
}

/**
 * @consult
 *  1. https://mirrors.ustc.edu.cn/help/dockerhub.html
 *  2. https://www.cnblogs.com/yuzhihui/p/17461781.html
 */
void
wr_docker_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_use_this_source (wr_docker);

  if (xy.on_linux || xy.on_bsd)
    {
      char *to_add = xy_str_gsub (RAWSTR_wr_docker_insert_content, "@1@", source.url);

      if (chsrc_check_file (WR_Docker_ConfigFile))
        {
          chsrc_note2 ("已找到Docker配置文件，将自动换源");
          chsrc_backup (WR_Docker_ConfigFile);

          if (chsrc_check_program_quietly ("jq"))
            {
              /* 检查是否已经存在 source.url */
              char *cmd = xy_str_gsub (RAWSTR_wr_docker_check_cmd, "@1@", source.url);
                    cmd = xy_str_gsub (cmd, "@2@", WR_Docker_ConfigFile);

              char *result = xy_run (cmd, 0);
              if (result && !xy_streql (result, "null"))
                {
                  chsrc_alert2 ("已存在源，无需重复添加");
                }
              else
                {
                  cmd = xy_str_gsub (RAWSTR_wr_docker_insert_cmd, "@1@", source.url);
                  cmd = xy_str_gsub (cmd, "@2@", WR_Docker_ConfigFile);
                  chsrc_run (cmd, RunOpt_Default);
                  chsrc_succ2 ("源已添加");
                }
            }
          else
            {
              chsrc_alert2 ("未找到 jq 命令, 将使用 sed 换源");
              char *cmd = xy_str_gsub (RAWSTR_wr_docker_sed_command, "@1@", source.url);
                    cmd = xy_str_gsub (cmd, "@2@", WR_Docker_ConfigFile);
              chsrc_run (cmd, RunOpt_Default);
            }
        }
      else
        {
          /* 不存在 /etc/docker/daemon.json 时可以直接写入文件 */
          chsrc_alert2 ("未找到Docker配置文件, 将自动创建");
          chsrc_ensure_dir ("/etc/docker");
          chsrc_run ( xy_2strcat ("touch ", WR_Docker_ConfigFile), RunOpt_Default);

          chsrc_append_to_file (to_add, WR_Docker_ConfigFile);
        }

      if (xy.on_linux)
        {
          /* 由于 systemctl restart docker 会导致所有容器停止，所以不自动重启 */
          chsrc_alert2 ("请自行运行: sudo systemctl restart docker");
          chsrc_alert2 ("该命令会重启所有容器, 请在合适的时机执行");
        }
      else
        {
          chsrc_alert2 ("然后请手动重启 docker 服务");
        }
    }
  else
    {
      chsrc_note2 ("请打开 Docker Desktop 设置");
      chsrc_note2 ("选择 'Docker Engine' 选项卡，在该选项卡中找到 'registry-mirrors' 一栏，添加镜像地址:");
      println (source.url);
    }

  chsrc_determine_chgtype (ChgType_SemiAuto);
  chsrc_conclude (&source);
}
