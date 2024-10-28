/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : happy game <happygame1024@gmail.com>
 *               | Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Nil Null <nil@null.org>
 *               |
 * Created On    : <2024-06-08>
 * Last Modified : <2024-10-28>
 * ------------------------------------------------------------*/

static MirrorSite
// 没有找到 DaoCloud 合适的下载链接，先随便给一个，以规避 chsrc 自动测速时所有 dockerhub 镜像站都没有测速链接带来的 bug
DaoCloud = {"daocloud", "DaoCloud","上海道客网络科技有限公司", "https://www.daocloud.io/",
            "https://qiniu-download-public.daocloud.io/DaoCloud_Enterprise/dce5/offline-community-v0.18.0-amd64.tar"},

Fit2Cloud = {"fit2cloud", "FIT2CLOUD", "杭州飞致云信息科技有限公司", "https://www.fit2cloud.com/", NULL},

Huecker = {"huecker",  "(Russia) Huecker", "俄罗斯 Huecker.io",   "https://huecker.io/",
           // 同 DaoCloud，没有合适的下载链接，先随便给一个，以避免 bug
           "https://huecker.io/en/use.html"};

/**
 * @update 2024-07-24
 *
 * @note USTC 与 SJTUG 于 2024-06-06 停止支持 DockerHub
 * @note NJU 于 2024-06-07 停止支持 DockerHub
 *
 * @consult https://gist.github.com/y0ngb1n/7e8f16af3242c7815e7ca2f0833d3ea6
 */

static SourceInfo
wr_dockerhub_sources[] = {
  {&Upstream,       NULL},
  // {&Ustc,       "https://docker.mirrors.ustc.edu.cn/"},

  // https://github.com/DaoCloud/public-image-mirror
  {&DaoCloud,      "https://docker.m.daocloud.io"},
  {&Fit2Cloud,     "https://docker.1panel.live"},

  // 暂时加入，未来若国内镜像恢复，将删除
  // https://huecker.io/en/use.html#unbanned
  {&Huecker,       "https://huecker.io"}
};

def_sources_n(wr_dockerhub);

#define WARE_DockerHub_SourceConfig "/etc/docker/daemon.json"

void
wr_dockerhub_getsrc (char *option)
{
  if (xy_on_linux || xy_on_bsd)
    {
      chsrc_view_file (WARE_DockerHub_SourceConfig);
    }
  else
    {
      chsrc_note2 ("请打开Docker Desktop设置");
      chsrc_note2 ("选择“Docker Engine”选项卡，在该选项卡中找到“registry-mirrors”一栏查看");
    }
}

/**
 * @consult
 *  1. https://mirrors.ustc.edu.cn/help/dockerhub.html
 *  2. https://www.cnblogs.com/yuzhihui/p/17461781.html
 */
void
wr_dockerhub_setsrc (char *option)
{
  chsrc_ensure_root ();
  chsrc_yield_source_and_confirm (wr_dockerhub);

  if (xy_on_linux || xy_on_bsd)
    {
      char *to_add = xy_strjoin (3, "{\n"
                                    "  \"registry-mirrors\": [\"", source.url, "\"]\n"
                                    "}");
      if (chsrc_check_file (WARE_DockerHub_SourceConfig))
        {
          chsrc_note2 ("已找到Docker配置文件，将自动换源");
          chsrc_backup (WARE_DockerHub_SourceConfig);
          if (chsrc_check_program_quietly ("jq"))
            {
              // 检查是否已经存在 source.url
              char *cmd = xy_strjoin (4, "jq '.[\"registry-mirrors\"] | index(\"",
                                         source.url,
                                         "\")' ",
                                         WARE_DockerHub_SourceConfig);
              char *ret = xy_run(cmd, 0, NULL);
              if (ret && !xy_streql(ret, "null\n"))
                {
                  chsrc_note2 ("已存在源，无需重复添加");
                }
              else
                {
                  cmd = xy_strjoin (6, "jq '.[\"registry-mirrors\"] |= [\"",
                                       source.url,
                                       "\"] + .' ",
                                       xy_2strjoin(WARE_DockerHub_SourceConfig, ".bak"),
                                       " > ",
                                       WARE_DockerHub_SourceConfig);
                  chsrc_run (cmd, RunOpt_Default);
                  chsrc_note2 ("源已添加");
                }
            }
          else
            {
              chsrc_note2("未找到 jq 命令, 将使用 sed 换源");
              char *cmd = xy_strjoin (5, "sed ",
                                       "-z -i 's|\"registry-mirrors\":[^]]*]|\"registry-mirrors\":[\"",
                                       source.url,
                                       "\"]|' ",
                                       WARE_DockerHub_SourceConfig);
              chsrc_run (cmd, RunOpt_Default);
            }
        }
      else
        {
          // 不存在 /etc/docker/daemon.json 时可以直接写入文件
          chsrc_note2 ("未找到Docker配置文件, 将自动创建");
          chsrc_ensure_dir ("/etc/docker");
          chsrc_run ( xy_2strjoin("touch ", WARE_DockerHub_SourceConfig), RunOpt_Default);

          chsrc_append_to_file (to_add, WARE_DockerHub_SourceConfig);
        }
      // chsrc_note2 ("请向 /etc/docker/daemon.json 中添加下述内容:");
      // puts (to_add);
      if (xy_on_linux)
        {
          // 由于 systemctl restart docker 会导致所有容器停止，所以不自动重启
          chsrc_note2 ("请自行运行: sudo systemctl restart docker");
          chsrc_note2 ("该命令会重启所有容器, 请在合适的时机执行");
        }
      else
        {
          chsrc_note2 ("然后请手动重启 docker 服务");
        }
    }
  else
    {
      chsrc_note2 ("请打开Docker Desktop设置");
      chsrc_note2 ("选择“Docker Engine”选项卡，在该选项卡中找到“registry-mirrors”一栏，添加镜像地址:");
      puts (source.url);
    }
  chsrc_conclude (&source, SetsrcType_Manual);
}


FeatInfo
wr_dockerhub_feat (char *option)
{
  FeatInfo f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.locally = NULL;
  f.can_english = false;

  f.can_user_define = true;

  f.note = NULL;
  return f;
}


def_target_gsf(wr_dockerhub);
