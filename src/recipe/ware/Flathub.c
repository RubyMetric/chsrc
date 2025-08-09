/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

define_target(wr_flathub);

void
wr_flathub_prelude ()
{
  use_this(wr_flathub);

  chef_set_recipe_created_on_date    (this, "2023-09-11");
  chef_set_recipe_last_updated_date  (this, "2025-08-09");


  chef_add_recipe_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_current_chef (this, NULL, NULL);
  chef_set_current_sous_chefs (this, 0);
  chef_add_contributors (this, 1,
    "Jialin Lyu", "jialinlvcn@aliyun.com");


  chef_set_sources_last_updated_date (this, "2025-05-27");
  def_upstream_provider("https://flathub.org/repo");

  def_sources_begin()
  {&upstream,         "https://flathub.org/repo",                  DelegateToUpstream},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/flathub",        DelegateToMirror},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/flathub", DelegateToMirror},
  def_sources_end()


  chef_debug_target(this);

  /**
   * @note 下述上海交大两个镜像站都可使用，但实际使用时出现过无法访问的情况 (GitHub-#178)，
   *       所以额外定义两个镜像站以更改测速链接为一个很小的文件: flathub.gpg
   *       若无速度，则证明无法访问。
   *       注意，这会使得测速的效果严重失真。
   */
  chsrc_set_provider_speed_measure_url (&upstream, "https://flathub.org/repo/flathub.gpg");
  /* upstream 默认是 ACCURATE 的，但是我们给了一个超小的文件，测速效果严重失真，所以改为 ROUGH */
  chsrc_set_provider_speed_measure_accuracy (&upstream, ROUGH);

  chsrc_set_provider_speed_measure_url (&Sjtug_Siyuan,  "https://mirror.sjtu.edu.cn/flathub/flathub.gpg");
  chsrc_set_provider_speed_measure_url (&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/flathub/flathub.gpg");
  /* 由于实在找不到其他可测文件，所以这也只能是 ROUGH */
  chsrc_set_provider_speed_measure_accuracy (&Sjtug_Siyuan, ROUGH);
  chsrc_set_provider_speed_measure_accuracy (&Sjtug_Zhiyuan, ROUGH);
}


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
  // chsrc_yield_source_and_confirm (wr_flathub);
  Source_t source;

  chsrc_alert2 ("若出现问题，可先调用以下命令:");
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

  f.note = "对Flathub目标进行测速的文件非常小，测速效果严重失真，若你知道可供测速的URL，欢迎参与贡献: chsrc issue";

  return f;
}

// def_target_gsrf (wr_flathub);
