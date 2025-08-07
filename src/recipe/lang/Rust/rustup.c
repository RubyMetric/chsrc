/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Yangmoooo  <yangmoooo@outlook.com>
 *               | Mikachu2333 <mikachu.23333@zohomail.com>
 *               |
 * Created On    : <2024-10-02>
 * Last Modified : <2025-08-07>
 * ------------------------------------------------------------*/

// Size: 20MB
#define Rustup_Speed_URL_Suffix "/dist/2025-06-26/cargo-1.88.0-x86_64-unknown-illumos.tar.gz"

static SourceProvider_t pl_rust_binary_upstream =
{
  def_upstream, "https://www.rust-lang.org/",
  def_need_measure_info
};

/**
 * @update 2025-08-07
 */
static Source_t pl_rust_rustup_sources[] = {
    {&pl_rust_binary_upstream, "https://static.rust-lang.org", "https://static.rust-lang.org" Rustup_Speed_URL_Suffix},
    {&MirrorZ, "https://mirrors.cernet.edu.cn/rustup", NULL},
    {&Tuna, "https://mirrors.tuna.tsinghua.edu.cn/rustup", "https://mirrors.tuna.tsinghua.edu.cn/rustup" Rustup_Speed_URL_Suffix},
    {&Ustc, "https://mirrors.ustc.edu.cn/rust-static", "https://mirrors.ustc.edu.cn/rust-static" Rustup_Speed_URL_Suffix},
    {&Nju, "https://mirror.nju.edu.cn/rustup", "https://mirror.nju.edu.cn/rustup" Rustup_Speed_URL_Suffix},
    {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/rust-static", "https://mirror.sjtu.edu.cn/rust-static" Rustup_Speed_URL_Suffix},
    {&Zju, "https://mirrors.zju.edu.cn/rustup", "https://mirrors.zju.edu.cn/rustup" Rustup_Speed_URL_Suffix},
    {&Iscas, "https://mirror.iscas.ac.cn/rustup", "https://mirror.iscas.ac.cn/rustup" Rustup_Speed_URL_Suffix},
    {&Ali, "https://mirrors.aliyun.com/rustup", "https://mirrors.aliyun.com/rustup" Rustup_Speed_URL_Suffix},
    {&RsProxyCN, "https://rsproxy.cn", "https://rsproxy.cn" Rustup_Speed_URL_Suffix}};
def_sources_n(pl_rust_rustup);


/**
 * chsrc get rustup
 */
void
pl_rust_rustup_getsrc (char *option)
{
  chsrc_view_env ("RUSTUP_UPDATE_ROOT", "RUSTUP_DIST_SERVER", NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/rustup/
 *
 * chsrc set rustup
 */
void
pl_rust_rustup_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_rust_rustup);

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


/**
 * chsrc reset rustup
 */
void
pl_rust_rustup_resetsrc (char *option)
{
  pl_rust_rustup_setsrc (option);
}


/**
 * chsrc ls rustup
 */
Feature_t
pl_rust_rustup_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = "";
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}


def_target_gsrf(pl_rust_rustup);
