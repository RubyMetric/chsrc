/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 *               |
 * Created On    : <2024-10-02>
 * Last Modified : <2024-11-22>
 * ------------------------------------------------------------*/

/**
 * @update 2024-10-02
 */
static SourceInfo
pl_rust_rustup_sources[] = {
  {&Upstream,      "https://static.rust-lang.org"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/rustup"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/rust-static"},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/rust-static"},
  {&Zju,           "https://mirrors.zju.edu.cn/rustup"},
  {&Iscas,         "https://mirror.iscas.ac.cn/rustup"},
  {&RsProxyCN,    "https://rsproxy.cn"}
};
def_sources_n(pl_rust_rustup);


/**
 * chsrc get rustup
 */
void
pl_rust_rustup_getsrc (char *option)
{
  char *cmd = "echo RUSTUP_UPDATE_ROOT;"
              "echo RUSTUP_DIST_SERVER;";

  system (cmd);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/rustup/
 *
 * chsrc set rustup
 */
void
pl_rust_rustup_setsrc (char *option)
{
  char *setsrc_type = xy_streql (option, SetsrcType_Reset) ? SetsrcType_Reset : SetsrcType_Auto;

  chsrc_yield_source_and_confirm (pl_rust_rustup);

  char *w1 = xy_strjoin (3, "export RUSTUP_DIST_SERVER=\"", source.url, "\"\n");
  char *w2 = xy_strjoin (3, "export RUSTUP_UPDATE_ROOT=\"", source.url, "/rustup\"\n");

  char *w = xy_2strjoin (w1, w2)

  char *bashrc = "~/.bashrc";
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *zshrc = "~/.zshrc";
  if (xy_file_exist (zshrc))
    {
      chsrc_backup (zshrc);
      chsrc_append_to_file (w, zshrc);
    }

  char *fishrc = "~/.config/fish/config.fish";
  if (xy_file_exist (fishrc))
    {
      char *w1 = xy_strjoin (3, "set -x RUSTUP_DIST_SERVER ", source.url, "\n");
      char *w2 = xy_strjoin (3, "set -x RUSTUP_UPDATE_ROOT ", source.url, "/rustup\n");

      char *w = xy_2strjoin (w1, w2)

      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }

  chsrc_conclude (&source, setsrc_type);
  chsrc_note2 ("请您重启终端使rustup环境变量生效");
}


/**
 * chsrc reset rustup
 */
void
pl_rust_rustup_resetsrc (char *option)
{
  pl_rust_rustup_setsrc (SetsrcType_Reset);
}


/**
 * chsrc ls rustup
 */
FeatInfo
pl_rust_rustup_feat (char *option)
{
  FeatInfo f = {0};

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
