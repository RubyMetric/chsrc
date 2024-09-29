/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-06-08>
 * Last Modified : <2024-08-16>
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
 * 2024-07-24 更新
 *
 * @note USTC 与 SJTUG 于 2024-06-06 停止支持 DockerHub
 * @note NJU 于 2024-06-07 停止支持 DockerHub
 *
 * 参考：https://gist.github.com/y0ngb1n/7e8f16af3242c7815e7ca2f0833d3ea6
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


void
wr_dockerhub_getsrc (char *option)
{
  if (xy_on_linux || xy_on_bsd)
    {
      chsrc_view_file ("/etc/docker/daemon.json");
    }
  else
    {
      chsrc_note2 ("请打开Docker Desktop设置");
      chsrc_note2 ("选择“Docker Engine”选项卡，在该选项卡中找到“registry-mirrors”一栏查看");
    }
}

/**
 * 参考：
 *  1. https://mirrors.ustc.edu.cn/help/dockerhub.html
 *  2. https://www.cnblogs.com/yuzhihui/p/17461781.html
 */
void
wr_dockerhub_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_dockerhub);

  if (xy_on_linux || xy_on_bsd)
    {
      char *to_add = xy_strjoin (3, "{\n"
                                    "  \"registry-mirrors\": [\"", source.url, "\"]\n"
                                    "}");
      chsrc_note2 ("请向 /etc/docker/daemon.json 中添加下述内容:");
      puts (to_add);
      if (xy_on_linux)
        {
          chsrc_note2 ("然后请运行:");
          puts ("sudo systemctl restart docker");
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


def_target(wr_dockerhub);
