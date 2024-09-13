/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nil Null  <nil@null.org>
 * Created On     : <2023-09-01>
 * Major Revision :      1
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

/* Begin Target Matrix */
#define t(a) (const char*)(a)
static const char
*pl_ruby  [] = {"gem",    "ruby",  "rb", "rubygem",  "rubygems", "bundler",  NULL, t(&pl_ruby_target)},
*pl_python[] = {"python", "pypi",  "py",                                     NULL, t(&pl_python_target)},
  *pl_python_pip[]    = {"pip",     NULL, t(&pl_python_pip_target)},
  *pl_python_poetry[] = {"poetry",  NULL, t(&pl_python_poetry_target)},
  *pl_python_pdm[]    = {"pdm",     NULL, t(&pl_python_pdm_target)},

*pl_nodejs[] = {"node", "nodejs", NULL, t(&pl_nodejs_target)},
  *pl_nodejs_npm[]  = {"npm",  NULL, t(&pl_nodejs_npm_target)},
  *pl_nodejs_yarn[] = {"yarn", NULL, t(&pl_nodejs_yarn_target)},
  *pl_nodejs_pnpm[] = {"pnpm", NULL, t(&pl_nodejs_pnpm_target)},

*pl_perl  [] = {"perl",  "cpan",                         NULL,  t(&pl_perl_target)},
*pl_php   [] = {"php",   "composer",                     NULL,  t(&pl_php_target)},
*pl_lua   [] = {"lua",   "luarocks",                     NULL,  t(&pl_lua_target)},
*pl_go    [] = {"go",    "golang",  "goproxy",           NULL,  t(&pl_go_target)} ,
*pl_rust  [] = {"rust",  "cargo",   "crate",  "crates",  NULL,  t(&pl_rust_target)},
*pl_java  [] = {"java",  "maven",   "mvn",    "gradle",  NULL,  t(&pl_java_target)},
*pl_clojure[]= {"clojure","clojars","cloj",   "lein",   "leiningen",  NULL, t(&pl_clojure_target)},
*pl_dart  [] = {"dart",  "pub",     "flutter",           NULL,  t(&pl_dart_target)},
*pl_nuget[]  = {"nuget", "net",     ".net",   "dotnet",  NULL,  t(&pl_nuget_target)},
*pl_haskell[]= {"haskell", "cabal", "stack",  "hackage", NULL,  t(&pl_haskell_target)},
*pl_ocaml[]  = {"ocaml", "opam",                         NULL,  t(&pl_ocaml_target)},
*pl_r     [] = {"cran",  "r",                            NULL,  t(&pl_r_target)},
*pl_julia [] = {"julia",                                 NULL,  t(&pl_julia_target)},
**pl_packagers[] =
{
  pl_ruby,
  pl_python,  pl_python_pip, pl_python_poetry, pl_python_pdm,
  pl_nodejs,  pl_nodejs_npm, pl_nodejs_pnpm,   pl_nodejs_yarn,
  pl_perl,    pl_php,
  pl_lua,
  pl_rust,    pl_go,
  /*pl_nuget,*/
  pl_java,    pl_clojure,
  pl_dart,
  pl_ocaml,
  pl_r,       pl_julia,
  pl_haskell,
};


static const char
*os_ubuntu     [] = {"ubuntu",               NULL,  t(&os_ubuntu_target)},
*os_linuxmint  [] = {"linuxmint", "mint",    NULL,  t(&os_linuxmint_target)},
*os_debian     [] = {"debian",               NULL,  t(&os_debian_target)},
*os_fedora     [] = {"fedora",               NULL,  t(&os_fedora_target)},
*os_opensuse   [] = {"opensuse",   NULL,            t(&os_opensuse_target)},
*os_kali       [] = {"kali",                 NULL,  t(&os_kali_target)},
*os_msys2      [] = {"msys2",   "msys",      NULL,  t(&os_msys2_target)},
*os_arch       [] = {"arch",    "archlinux", NULL,  t(&os_arch_target)},
*os_archlinuxcn[] = {"archlinuxcn", "archcn",NULL,  t(&os_archlinuxcn_target)},
*os_manjaro    [] = {"manjaro",              NULL,  t(&os_manjaro_target)},
*os_gentoo     [] = {"gentoo",               NULL,  t(&os_gentoo_target)},
*os_rockylinux [] = {"rocky",  "rockylinux", NULL,  t(&os_rockylinux_target)},
*os_almalinux  [] = {"alma",   "almalinux",  NULL,  t(&os_almalinux_target)},
*os_alpine     [] = {"alpine",               NULL,  t(&os_alpine_target)},
*os_void       [] = {"void",   "voidlinux",  NULL,  t(&os_void_target)},
*os_solus      [] = {"solus",                NULL,  t(&os_solus_target)},
*os_trisquel   [] = {"trisquel",             NULL,  t(&os_trisquel_target)},
*os_linuxlite  [] = {"lite",   "linuxlite",  NULL,  t(&os_linuxlite_target)},
*os_ros        [] = {"ros",    "ros2",       NULL,  t(&os_ros_target)},

*os_raspberrypi[] = {"raspi",  "raspberrypi",NULL,  t(&os_raspberrypi_target)},
*os_armbian    [] = {"armbian",              NULL,  t(&os_armbian_target)},
*os_openwrt    [] = {"openwrt", "opkg", "LEDE", NULL, t(&os_openwrt_target)},

*os_openkylin  [] = {"openkylin",  NULL,            t(&os_openkylin_target)},
*os_openeuler  [] = {"openeuler",  NULL,            t(&os_openeuler_target)},
*os_anolis     [] = {"openanolis", "anolis", NULL,  t(&os_anolis_target)},
*os_deepin     [] = {"deepin",               NULL,  t(&os_deepin_target)},

*os_freebsd    [] = {"freebsd",              NULL,  t(&os_freebsd_target)},
*os_netbsd     [] = {"netbsd",               NULL,  t(&os_netbsd_target)},
*os_openbsd    [] = {"openbsd",              NULL,  t(&os_openbsd_target)},

**os_systems[] =
{
  os_debian,  os_ubuntu,  os_linuxmint,  os_kali,
  os_trisquel, os_linuxlite, os_raspberrypi,  os_armbian,   os_openwrt,
  os_deepin,   os_openkylin, os_ros,

  os_fedora,  os_rockylinux, os_almalinux,  os_openeuler, os_anolis,

  os_opensuse,

  os_arch,    os_archlinuxcn, os_manjaro, os_gentoo,

  os_alpine,   os_void,      os_solus,

  os_msys2,
  os_freebsd,  os_netbsd,    os_openbsd,
};


static const char
*wr_winget  [] = {"winget",  NULL,                   t(&wr_winget_target)},
*wr_homebrew[] = {"brew",       "homebrew",   NULL,  t(&wr_homebrew_target)},
*wr_cocoapods[] = {"cocoa",     "cocoapods","pod", "cocoapod",  NULL,  t(&wr_cocoapods_target)},
*wr_dockerhub [] ={"dockerhub", "docker",  NULL,  t(&wr_dockerhub_target)},
*wr_flathub [] = {"flathub",    "flatpak", NULL,  t(&wr_flathub_target)},
*wr_nix     [] = {"nix",     NULL,                t(&wr_nix_target)},
*wr_guix    [] = {"guix",    NULL,                t(&wr_guix_target)},
*wr_emacs   [] = {"emacs", "elpa",         NULL,  t(&wr_emacs_target)},
*wr_tex     [] = {"latex", "ctan", "tex", "texlive", "miktex", "tlmgr", "mpm", NULL, t(&wr_tex_target)},
*wr_anaconda[] = {"conda", "anaconda",     NULL,  t(&wr_anaconda_target)},
**wr_softwares[] =
{
  wr_winget, wr_homebrew, wr_cocoapods, wr_dockerhub, wr_flathub, wr_nix, wr_guix, wr_emacs, wr_tex, wr_anaconda
};
#undef t



static MirrorSite*
available_mirrors[] = {
  &MirrorZ, &Tuna, &Sjtug_Zhiyuan, &Zju, &Lzuoss, &Jlu, &Bfsu, &Pku, &Bjtu, &Sustech, &Ustc, &Hust, &Nju, // &Cqu,
  &Ali,  &Tencent, &Huawei, &Volcengine,  &Netease, &Sohu, &Api7, &Fit2Cloud,
  &RubyChina, &EmacsChina, &NpmMirror, &GoProxyCN, &GoProxyIO,
  // 暂不支持 &NugetOrg
  // 不要列出 &Upstream 和 &UserDdefine
};

/* End Target Matrix */
