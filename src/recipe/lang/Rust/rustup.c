/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Aoran Zeng     <ccmywish@qq.com>
 * Contributors  :   Yangmoooo     <yangmoooo@outlook.com>
 *               |  Mikachu2333    <mikachu.23333@zohomail.com>
 *               |
 * Created On    : <2024-10-02>
 * Last Modified : <2025-06-17>
 * ------------------------------------------------------------*/

static SourceProvider_t pl_rust_binary_upstream =
{
  def_upstream, "https://www.rust-lang.org/",
  def_need_measure_info
};

/**
 * @update 2025-06-17
 */
static Source_t pl_rust_rustup_sources[] = {
    {&pl_rust_binary_upstream, "https://static.rust-lang.org"},
    {&MirrorZ, "https://mirrors.cernet.edu.cn/rustup"},
    {&Tuna, "https://mirrors.tuna.tsinghua.edu.cn/rustup"},
    {&Ustc, "https://mirrors.ustc.edu.cn/rust-static"},
    {&Nju, "https://mirror.nju.edu.cn/rustup"},
    {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/rust-static"},
    {&Zju, "https://mirrors.zju.edu.cn/rustup"},
    {&Iscas, "https://mirror.iscas.ac.cn/rustup"},
    {&Ali, "https://mirrors.aliyun.com/rustup"},
    {&RsProxyCN, "https://rsproxy.cn"}};
def_sources_n(pl_rust_rustup);


/**
 * chsrc get rustup
 */
void
pl_rust_rustup_getsrc (char *option)
{
  char *cmd = "echo $RUSTUP_UPDATE_ROOT; "
              "echo $RUSTUP_DIST_SERVER;";

  chsrc_run (cmd, RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line);
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

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_note2 ("请您重启终端使rustup环境变量生效");
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
