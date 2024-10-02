/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-10-02>
 * Last Modified : <2024-10-02>
 * ------------------------------------------------------------*/

/**
 * @update 2024-10-02
 */
static SourceInfo
pl_rust_rustup_sources[] = {
  {&Upstream,      NULL},
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

  char *dist_server = xy_strjoin (3, "export RUSTUP_DIST_SERVER=\"", source.url, "\"");
  char *update_root = xy_strjoin (3, "export RUSTUP_UPDATE_ROOT=\"", source.url, "/rustup\"");

  char *bashrc = "~/.bashrc";
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (dist_server, bashrc);
      chsrc_append_to_file (update_root, bashrc);
    }

  char *zshrc = "~/.zshrc";
  if (xy_file_exist (zshrc))
    {
      chsrc_backup (zshrc);
      chsrc_append_to_file (dist_server, zshrc);
      chsrc_append_to_file (update_root, zshrc);
    }

  char *fishrc = "~/.config/fish/config.fish";
  if (xy_file_exist (fishrc))
    {
      char *dist_server = xy_2strjoin ("set -x  RUSTUP_DIST_SERVER ", source.url);
      char *update_root = xy_2strjoin ("set -x  RUSTUP_UPDATE_ROOT ", xy_2strjoin (source.url, "/rustup"));

      chsrc_backup (fishrc);
      chsrc_append_to_file (dist_server, fishrc);
      chsrc_append_to_file (update_root, fishrc);
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
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = false;

  fi.stcan_locally = CanNot;
  fi.locally = "";
  fi.can_english = false;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}


def_target_gsrf(pl_rust_rustup);
