/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : 曾奥然 <ccmywish@qq.com>
 * Contributors   : Mikachu2333 <mikachu.23333@zohomail.com>
 *                |
 * Created On     : <2023-09-01>
 * Major Revision :      5
 * Last Modified  : <2025-08-22>
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


void
chsrc_init_menu ()
{
#define add(t) xy_seq_push(ProgStore.pl, &pl_##t##_target); (&pl_##t##_target)->preludefn = pl_##t##_prelude
  add (ruby);
  add (python_group);
  add (python_pip);
  add (python_poetry);
  add (python_pdm);
  add (python_rye);
  add (python_uv);
  add (js_group);
  add (js_bun);
  add (js_npm);
  add (js_yarn);
  add (js_pnpm);
  add (js_nvm);
  add (perl);
  add (php);
  add (lua);
  add (go);
  add (rust_cargo);
  add (rust_rustup);
  add (java);
  add (clojure);
  add (dart);
  add (dart_flutter);
  add (nuget);
  add (haskell);
  add (ocaml);
  add (r);
  add (julia);
#undef add

#define add(t) xy_seq_push(ProgStore.os, &os_##t##_target); (&os_##t##_target)->preludefn = os_##t##_prelude
  add (ubuntu);
  add (linuxmint);
  add (debian);
  add (fedora);
  add (opensuse);
  add (kali);
  add (msys2);
  add (arch);
  add (archlinuxcn);
  add (manjaro);
  add (gentoo);
  add (rockylinux);
  add (almalinux);
  add (alpine);
  add (voidlinux);
  add (solus);
  add (trisquel);
  add (linuxlite);
  add (ros);
  add (raspberrypi);
  add (armbian);
  add (openwrt);
  add (termux);
  add (openkylin);
  add (openeuler);
  add (anolis);
  add (deepin);
  add (freebsd);
  add (netbsd);
  add (openbsd);
#undef add

#define add(t) xy_seq_push(ProgStore.wr, &wr_##t##_target); (&wr_##t##_target)->preludefn = wr_##t##_prelude
  add (winget);
  add (homebrew);
  add (cocoapods);
  add (docker);
  add (flatpak);
  add (nix);
  add (guix);
  add (emacs);
  add (tex);
  add (anaconda);
#undef add

  // Internal Target 不会由用户使用，需要特殊对待
  pl_js_nodejs_binary_target.preludefn = pl_js_nodejs_binary_prelude;
}



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
