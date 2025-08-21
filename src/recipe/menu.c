/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : 曾奥然 <ccmywish@qq.com>
 * Contributors   : Mikachu2333 <mikachu.23333@zohomail.com>
 *                |
 * Created On     : <2023-09-01>
 * Major Revision :      4
 * Last Modified  : <2025-08-20>
 * ------------------------------------------------------------*/

#include "lang/rawstr4c.h"

#include "lang/Ruby/Ruby.c"
#include "lang/Python/common.h"
 #include "lang/Python/pip.c"
  #include "lang/Python/Poetry.c"
  #include "lang/Python/PDM.c"
  #include "lang/Python/Rye.c"
  #include "lang/Python/uv.c"
#include "lang/Python/Python.c"

#include "lang/JavaScript/common.h"
  #include "lang/JavaScript/npm.c"
  #include "lang/JavaScript/pnpm.c"
  #include "lang/JavaScript/Yarn.c"
#include "lang/JavaScript/JavaScript.c"
#include "lang/JavaScript/Bun.c"
#include "lang/JavaScript/nvm.c"

#include "lang/Perl.c"
#include "lang/PHP.c"
#include "lang/Lua.c"
#include "lang/Go.c"
#include "lang/Java.c"

#include "lang/Rust/common.h"
  #include "lang/Rust/rustup.c"
  #include "lang/Rust/Cargo.c"

#include "lang/Dart/common.h"
#include "lang/Dart/Pub.c"
#include "lang/Dart/Flutter.c"

#include "lang/Haskell.c"
#include "lang/OCaml.c"
#include "lang/R.c"
#include "lang/Julia.c"
#include "lang/NuGet.c"
#include "lang/Clojure.c"


#include "os/rawstr4c.h"
#include "os/APT/common.h"
// Debian-based
#include "os/APT/Debian.c"
#include "os/APT/Ubuntu.c"
#include "os/APT/Kali-Linux.c"
#include "os/APT/Raspberry-Pi-OS.c"
// Ubuntu-based
#include "os/APT/Linux-Mint.c"
#include "os/APT/Trisquel.c"
#include "os/APT/Linux-Lite.c"
// Debian-based and Ubuntu-based
#include "os/APT/Armbian.c"
// Independent
#include "os/APT/ROS.c"
#include "os/APT/Termux.c"
#include "os/APT/openKylin.c"
#include "os/APT/deepin.c"

#include "os/YUM/common.h"
#include "os/YUM/Fedora-Linux.c"
#include "os/YUM/AlmaLinux.c"
#include "os/YUM/Rocky-Linux.c"
#include "os/YUM/openEuler.c"
#include "os/YUM/Anolis-OS.c"

#include "os/pacman/Arch-Linux.c"
#include "os/pacman/Manjaro-Linux.c"
#include "os/pacman/MSYS2.c"

#include "os/openSUSE.c"
#include "os/Alpine-Linux.c"
#include "os/Gentoo-Linux.c"
#include "os/Void-Linux.c"
#include "os/Solus.c"
#include "os/OpenWrt.c"

#include "os/BSD/FreeBSD.c"
#include "os/BSD/NetBSD.c"
#include "os/BSD/OpenBSD.c"


#include "ware/TeX-Live.c"
#include "ware/Emacs.c"
#include "ware/WinGet.c"
#include "ware/Homebrew/Homebrew.c"
#include "ware/CocoaPods.c"
#include "ware/Nix.c"
#include "ware/Guix.c"
#include "ware/Flatpak.c"
#include "ware/Docker/Docker.c"
#include "ware/Anaconda/Anaconda.c"



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
  {t(wr_winget)},
  {t(wr_homebrew)},
  {t(wr_cocoapods)},
  {t(wr_docker)},
  {t(wr_flatpak)},
  {t(wr_nix)},
  {t(wr_guix)},
  {t(wr_emacs)},
  {t(wr_tex)},
  {t(wr_anaconda)},
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
