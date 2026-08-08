/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : @ccmywish
 * Contributors   : @Mikachu2333
 *                | @BingChunMoLi
 *                |
 * Created On     : <2023-09-01>
 * Major Revision :      6
 * Last Modified  : <2026-08-02>
 * ------------------------------------------------------------*/

#include "lang/rawstr4c.h"

#include "lang/Ruby/Ruby.c"
#include "lang/Python/common.h"
 #include "lang/Python/pip.c"
  #include "lang/Python/Poetry.c"
  #include "lang/Python/PDM.c"
  #include "lang/Python/Rye.c"
  #include "lang/Python/uv/uv.c"
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
/* @sync https://github.com/RubyMetric/chsrc/wiki */
#define add(t) xy_seq_push(ProgStore.mirror_sites, &t)

  /* ------------------------ 教育网(通用镜像站) ------------------------ */
  add(MirrorZ);
  add(Tuna);
  add(Sjtug_Zhiyuan); add(Sjtug_Siyuan);
  add(Bfsu);
  add(Ustc);
  add(Zju);
  add(Jlu);
  add(Lzuoss);
  add(Pku);
  add(Bjtu);
  add(Sustech);
  add(Nju);
  add(Xjtu);
  add(Hust);   add(Iscas); add(Hit); add(Scau);
  add(NJTech); add(Nyist); add(Sdu); add(Qlu);
  add(Cqupt);  add(Cqu);   add(Neosoft);

  /* ------------------------ 商业公司(通用镜像站) ------------------------ */
  add(Ali);     // add(Ali_ECS_VPC); add(Ali_ECS_classic);
  add(Tencent); // add(Tencent_Intra);
  add(Huawei); add(HuaweiCDN);
  add(Volcengine);
  add(Netease);
  add(Sohu);

  add(Api7); add(Fit2Cloud); add(DaoCloud);


  /* ------------------------ 专用镜像站 ------------------------ */
  add(RubyChina); // Ruby China 镜像站实际上已于2026年7月21日停用，但由于其是持续数十年的由社区维护驱动的镜像站，我们保留表示致敬
  add(EmacsChina);
  add(NpmMirror); add(GoProxyIO); add(GoProxyCN); add(RsProxyCN); add(FlutterCN);
  /* 暂不支持 &NugetOrg */


  /* ------------------------ chsrc 内部实现 ------------------------ */
  /* 不要列出 UpstreamProvider 和 UserDefinedProvider */
#undef add



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
