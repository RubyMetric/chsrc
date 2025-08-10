/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

// Size: 20MB
#define PL_Rustup_Speed_URL_Suffix

def_target(pl_rust_rustup, "rustup");

void
pl_rust_rustup_prelude (void)
{
  use_this(pl_rust_rustup);
  chef_allow_gsr(pl_rust_rustup);

  chef_set_created_on   (this, "2024-10-02");
  chef_set_last_updated (this, "2025-08-07");
  chef_set_sources_last_updated (this, "2025-08-07");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 2,
    "Yangmoooo", "yangmoooo@outlook.com",
    "Mikachu2333", "mikachu.23333@zohomail.com");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english (this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://static.rust-lang.org",         FeedByPrelude},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/rustup", FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/rustup", FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/rust-static", FeedByPrelude},
  {&Nju,              "https://mirror.nju.edu.cn/rustup", FeedByPrelude},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/rust-static", FeedByPrelude},
  {&Zju,              "https://mirrors.zju.edu.cn/rustup", FeedByPrelude},
  {&Iscas,            "https://mirror.iscas.ac.cn/rustup", FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/rustup", FeedByPrelude},
  {&RsProxyCN,        "https://rsproxy.cn",                FeedByPrelude}
  def_sources_end()

  // 20MB大小
  chef_set_sources_speed_measure_url_with_postfix (this, "/dist/2025-06-26/cargo-1.88.0-x86_64-unknown-illumos.tar.gz");
}



void
pl_rust_rustup_getsrc (char *option)
{
  chsrc_view_env ("RUSTUP_UPDATE_ROOT", "RUSTUP_DIST_SERVER", NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/rustup/
 */
void
pl_rust_rustup_setsrc (char *option)
{
  use_this_source(pl_rust_rustup);

#ifdef XY_On_Windows

  char *cmd1 = xy_strjoin (3, "setx RUSTUP_DIST_SERVER \"", source.url, "\"");
  char *cmd2 = xy_strjoin (3, "setx RUSTUP_UPDATE_ROOT \"", source.url, "/rustup\"");

  char *cmd = xy_strjoin (3, cmd1, " & ", cmd2);
  chsrc_run (cmd, RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line);

#else

  char *w1 = xy_strjoin (3, "export RUSTUP_DIST_SERVER=\"", source.url, "\"\n");
  char *w2 = xy_strjoin (3, "export RUSTUP_UPDATE_ROOT=\"", source.url, "/rustup\"\n");

  char *w = xy_2strjoin (w1, w2);

  char *bashrc = xy_bashrc;
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *zshrc = xy_zshrc;
  if (xy_file_exist (zshrc))
    {
      chsrc_backup (zshrc);
      chsrc_append_to_file (w, zshrc);
    }

  char *fishrc = xy_fishrc;
  if (xy_file_exist (fishrc))
    {
      char *w1 = xy_strjoin (3, "set -x RUSTUP_DIST_SERVER ", source.url, "\n");
      char *w2 = xy_strjoin (3, "set -x RUSTUP_UPDATE_ROOT ", source.url, "/rustup\n");

      char *w = xy_2strjoin (w1, w2);

      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }
#endif

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_alert2 ("请重启终端使rustup环境变量生效");
}



void
pl_rust_rustup_resetsrc (char *option)
{
  pl_rust_rustup_setsrc (option);
}
