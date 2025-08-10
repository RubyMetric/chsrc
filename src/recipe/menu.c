/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng  <ccmywish@qq.com>
 * Contributors   : Mikachu2333 <mikachu.23333@zohomail.com>
 *                |
 * Created On     : <2023-09-01>
 * Major Revision :      3
 * Last Modified  : <2025-08-10>
 * ------------------------------------------------------------*/

#define t(o) &o##_target, &o##_prelude

TargetRegisterInfo_t
chsrc_pl_menu[] =
{
  {t(pl_ruby)},
  {t(pl_python_group)},
  {t(pl_python_pip)},
  {t(pl_python_poetry)},
  {t(pl_python_pdm)},
  {t(pl_python_rye)},
  {t(pl_python_uv)},
  {t(pl_js_group)},
  {t(pl_js_bun)},
  {t(pl_js_npm)},
  {t(pl_js_yarn)},
  {t(pl_js_pnpm)},
  {t(pl_js_nvm)},
  {t(pl_perl)},
  {t(pl_php)},
  {t(pl_lua)},
  {t(pl_go)},
  {t(pl_rust_cargo)},
  {t(pl_rust_rustup)},
  {t(pl_java)},
  {t(pl_clojure)},
  {t(pl_dart)},
  {t(pl_dart_flutter)},
  {t(pl_nuget)},
  {t(pl_haskell)},
  {t(pl_ocaml)},
  {t(pl_r)},
  {t(pl_julia)},
};



TargetRegisterInfo_t
chsrc_os_menu[] =
{
  {t(os_ubuntu)},
  {t(os_linuxmint)},
  {t(os_debian)},
  {t(os_fedora)},
  {t(os_opensuse)},
  {t(os_kali)},
  {t(os_msys2)},
  {t(os_arch)},
  {t(os_archlinuxcn)},
  {t(os_manjaro)},
  {t(os_gentoo)},
  {t(os_rockylinux)},
  {t(os_almalinux)},
  {t(os_alpine)},
  {t(os_voidlinux)},
  {t(os_solus)},
  {t(os_trisquel)},
  {t(os_linuxlite)},
  {t(os_ros)},
  {t(os_raspberrypi)},
  {t(os_armbian)},
  {t(os_openwrt)},
  {t(os_termux)},
  {t(os_openkylin)},
  {t(os_openeuler)},
  {t(os_anolis)},
  {t(os_deepin)},
  {t(os_freebsd)},
  {t(os_netbsd)},
  {t(os_openbsd)},
};


TargetRegisterInfo_t
chsrc_wr_menu[] =
{
  {t(wr_winget),    "winget"},
  {t(wr_homebrew),  "brew homebrew"},
  {t(wr_cocoapods), "cocoa cocoapods cocoapod"},
  {t(wr_docker),    "docker dockerhub"},
  {t(wr_flatpak),   "flatpak flathub"},
  {t(wr_nix),       "nix"},
  {t(wr_guix),      "guix"},
  {t(wr_emacs),     "emacs elpa"},
  {t(wr_tex),       "latex ctan tex texlive miktex tlmgr mpm"},
  {t(wr_anaconda),  "conda anaconda"},
};
#undef t



/**
 * @sync https://github.com/RubyMetric/chsrc/wiki
 */
static MirrorSite_t*
chsrc_available_mirrors[] = {

  /* ------------------------ 教育网(通用镜像站) ------------------------ */
  &MirrorZ, &Tuna, &Sjtug_Zhiyuan, &Sjtug_Siyuan, &Bfsu, &Ustc, &Zju, &Jlu, &Lzuoss, &Pku, &Bjtu, &Sustech, &Nju, &Xjtu,

  &Hust,   &Iscas, &Hit, &Scau,
  &NJTech, &Nyist, &Sdu, &Qlu,
  &Cqupt,  &Cqu,   &Neosoft,


  /* ------------------------ 商业公司(通用镜像站) ------------------------ */
  &Ali,     // &Ali_ECS_VPC, &Ali_ECS_classic,
  &Tencent, // &Tencent_Intra,
  &Huawei,
  &Volcengine,
  &Netease,
  &Sohu,

  &Api7, &Fit2Cloud, &DaoCloud,


  /* ------------------------ 专用镜像站 ------------------------ */
  &RubyChina, &EmacsChina, &NpmMirror, &GoProxyIO, &GoProxyCN, &RsProxyCN, &FlutterCN,
  /* 暂不支持 &NugetOrg */


  /* ------------------------ chsrc 内部实现 ------------------------ */
  /* 不要列出 &UpstreamProvider 和 &UserDefinedProvider */
};
