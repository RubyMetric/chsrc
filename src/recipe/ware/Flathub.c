/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-11>
 * Last Modified : <2025-03-17>
 * ------------------------------------------------------------*/

/**
 * @update 2025-03-17
 * @note 加入官方仓库
 * @note SJTUG分为两个镜像站 思源站和致远站 以解决思源站可能无法访问的问题
 */
static SourceProvider_t wr_flathub_upstream =
{
  "Flathub", "Flathub", "Flathub官方仓库", "https://flathub.org/repo",
  // 没有找到 DaoCloud 合适的下载链接，先随便给一个，以规避 chsrc 自动测速时所有 dockerhub 镜像站都没有测速链接带来的 bug
  {NotSkip, NA, NA, "https://flathub.org/repo/flathub.gpg"}
},

wr_flathub_siyuan =
{
  "sjtu", "SJTUG-siyuan", "上海交通大学思源镜像站Flathub", "https://mirror.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.sjtu.edu.cn/flathub/flathub.gpg"}
},

wr_flathub_zhiyuan =
{
  "sjtu", "SJTUG-zhiyuan", "上海交通大学致远镜像站Flathub", "https://mirrors.sjtug.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sjtug.sjtu.edu.cn/flathub/flathub.gpg"}
};

static Source_t wr_flathub_sources[] =
{
  {&wr_flathub_upstream, "https://flathub.org/repo"},
  {&wr_flathub_siyuan,    "https://mirror.sjtu.edu.cn/flathub"},
  {&wr_flathub_zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/flathub"},
};
def_sources_n(wr_flathub);


/**
 * @consult https://mirrors.sjtug.sjtu.edu.cn/docs/flathub
 */
void
wr_flathub_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_flathub);

  chsrc_note2 ("若出现问题，可先调用以下命令:");
  char *note = xy_strjoin (3,
    "wget ", source.url, "/flathub.gpg\n"
    "flatpak remote-modify --gpg-import=flathub.gpg flathub"
  );
  puts (note);

  char *repo_note = "Flathub 中部分软件由于重分发授权问题，需要从官方服务器下载，无法使用镜像站加速\n"
    "比如 NVIDIA 驱动、JetBrains 系列软件等\n"
    "尝试运行 flatpak remote-modify flathub --url=https://flathub.org/repo";
  puts (repo_note);

  char *cmd = xy_2strjoin ("flatpak remote-modify flathub --url=", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

def_target_s (wr_flathub);
