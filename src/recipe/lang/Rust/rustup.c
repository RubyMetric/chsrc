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
  {&Sjtug,         "https://mirror.sjtu.edu.cn/rust-static"},
  {&Zju,           "https://mirrors.zju.edu.cn/rustup"},
  {&Iscas,         "https://mirror.iscas.ac.cn/rustup"}
};
def_sources_n(pl_rust_rustup);


/**
 * chsrc get rustup
 */
void
pl_rust_rustup_getsrc (char *option)
{
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

  chsrc_conclude (&source, setsrc_type);
}


/**
 * chsrc reset rustup
 */
void
pl_rust_rustup_resetsrc (char *option)
{
  // pl_rust_rustup_setsrc (SetsrcType_Reset);
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

  fi.stcan_locally = CanSemi;
  fi.locally = "具体说明是否支持项目级换源...";
  fi.can_english = false;
  fi.can_user_define = false;

  fi.note = "备注说明...";
  return fi;
}


def_target_gsrf(pl_rust_rustup);
