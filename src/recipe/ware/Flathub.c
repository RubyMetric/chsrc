/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Jialin Lyu <jialinlvcn@aliyun.com>
 *               |
 * Created On    : <2023-09-11>
 * Last Modified : <2025-05-27>
 * ------------------------------------------------------------*/

/**
 * @update 2025-05-27
 */
static SourceProvider_t wr_flathub_upstream =
{
  def_upstream, "https://flathub.org/repo",
  {NotSkip, NA, NA, "https://flathub.org/repo/flathub.gpg", ROUGH}
},

/**
 * @note 下述上海交大两个镜像站都可使用，但实际使用时出现过无法访问的情况 (GitHub-#178)，
 *       所以额外定义两个镜像站以更改测速链接为一个很小的文件: flathub.gpg
 *       若无速度，则证明无法访问。
 *       注意，这会使得测速的效果严重失真。
 */

wr_flathub_siyuan =
{
  "sjtu-sy", "SJTUG-siyuan", "上海交通大学思源镜像站Flathub", "https://mirror.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.sjtu.edu.cn/flathub/flathub.gpg"}
},

wr_flathub_zhiyuan =
{
  "sjtu-zy", "SJTUG-zhiyuan", "上海交通大学致远镜像站Flathub", "https://mirrors.sjtug.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sjtug.sjtu.edu.cn/flathub/flathub.gpg"}
};

static Source_t wr_flathub_sources[] =
{
  {&wr_flathub_upstream,  "https://flathub.org/repo"},
  {&wr_flathub_siyuan,    "https://mirror.sjtu.edu.cn/flathub"},
  {&wr_flathub_zhiyuan,   "https://mirrors.sjtug.sjtu.edu.cn/flathub"},
};
def_sources_n(wr_flathub);


/**
 * chsrc get flathub
 */
void
wr_flathub_getsrc (char *option)
{
  chsrc_run ("flatpak remotes", RunOpt_Default);
  chsrc_run ("flatpak remote-info flathub", RunOpt_Default);
}


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
  say (note);

  char *repo_note = "Flathub 中部分软件由于重分发授权问题，需要从官方服务器下载，无法使用镜像站加速\n"
    "比如 NVIDIA 驱动、JetBrains 系列软件等\n"
    "尝试运行 flatpak remote-modify flathub --url=https://flathub.org/repo";
  say (repo_note);

  char *cmd = xy_2strjoin ("flatpak remote-modify flathub --url=", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * chsrc reset flathub
 */
void
wr_flathub_resetsrc (char *option)
{
  wr_flathub_setsrc (option);
}


/**
 * chsrc ls flathub
 */
Feature_t
wr_flathub_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = false;
  f.can_user_define = true;

  f.note = "对Flathub目标进行测速的文件非常小，测速效果严重失真，若您知道可供测速的URL，欢迎参与贡献: chsrc issue";

  return f;
}

def_target_gsrf (wr_flathub);
