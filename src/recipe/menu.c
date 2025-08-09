/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng  <ccmywish@qq.com>
 * Contributors   : Mikachu2333 <mikachu.23333@zohomail.com>
 *                |
 * Created On     : <2023-09-01>
 * Major Revision :      2
 * Last Modified  : <2025-08-09>
 * ------------------------------------------------------------*/

/* Begin Target Matrix */
#define t(a) (const char*)(a)
static const char
*pl_ruby  [] = {"gem",    "ruby",  "rb", "rubygem",  "rubygems", "bundler",  NULL, t(&pl_ruby_target)},

*pl_python[]          = {"python", "pypi",  "py",  NULL, t(&pl_python_group_target)},
  *pl_python_pip[]    = {"pip",                    NULL, t(&pl_python_pip_target)},
  *pl_python_poetry[] = {"poetry",                 NULL, t(&pl_python_poetry_target)},
  *pl_python_pdm[]    = {"pdm",                    NULL, t(&pl_python_pdm_target)},
  *pl_python_rye[]    = {"rye",                    NULL, t(&pl_python_rye_target)},
  *pl_python_uv[]     = {"uv",                     NULL, t(&pl_python_uv_target)},

*pl_nodejs[]        = {"node", "nodejs", NULL, t(&pl_nodejs_group_target)},
*pl_nodejs_bun[]    = {"bun",            NULL, t(&pl_nodejs_bun_target)},
  *pl_nodejs_npm[]  = {"npm",            NULL, t(&pl_nodejs_npm_target)},
  *pl_nodejs_yarn[] = {"yarn",           NULL, t(&pl_nodejs_yarn_target)},
  *pl_nodejs_pnpm[] = {"pnpm",           NULL, t(&pl_nodejs_pnpm_target)},
*pl_nodejs_nvm[]    = {"nvm",            NULL, t(&pl_nodejs_nvm_target)},

*pl_perl  [] = {"perl",  "cpan",                         NULL, t(&pl_perl_target)},
*pl_php   [] = {"php",   "composer",                     NULL, t(&pl_php_target)},
*pl_lua   [] = {"lua",   "luarocks",                     NULL, t(&pl_lua_target)},
*pl_go    [] = {"go",    "golang",  "goproxy",           NULL, t(&pl_go_target)} ,

*pl_rust_cargo[] = {"rust",  "cargo",   "crate",  "crates", NULL, t(&pl_rust_cargo_target)},
*pl_rust_rustup[]= {"rustup",                               NULL, t(&pl_rust_rustup_target)},

*pl_java  [] = {"java",  "maven",   "mvn",    "gradle",  NULL, t(&pl_java_target)},
*pl_clojure[]= {"clojure","clojars","cloj",   "lein",    NULL, t(&pl_clojure_target)},

*pl_dart  [] =        {"dart", "pub",                    NULL, t(&pl_dart_target)},
*pl_dart_flutter [] = {"flutter",                        NULL, t(&pl_dart_flutter_target)},

*pl_nuget[]  = {"nuget", "net",     ".net",   "dotnet",  NULL, t(&pl_nuget_target)},
*pl_haskell[]= {"haskell", "cabal", "stack",  "hackage", NULL, t(&pl_haskell_target)},
*pl_ocaml[]  = {"ocaml", "opam",                         NULL, t(&pl_ocaml_target)},
*pl_r     [] = {"cran",  "r",                            NULL, t(&pl_r_target)},
*pl_julia [] = {"julia",                                 NULL, t(&pl_julia_target)},
**pl_packagers[] =
{
  pl_ruby,
  pl_python,  pl_python_pip, pl_python_poetry, pl_python_pdm, pl_python_rye, pl_python_uv,
  pl_nodejs,  pl_nodejs_bun,
              pl_nodejs_npm, pl_nodejs_pnpm,   pl_nodejs_yarn,
              pl_nodejs_nvm,
  pl_perl,    pl_php,
  pl_lua,
  pl_rust_cargo, pl_rust_rustup,
  pl_go,
  /*pl_nuget,*/
  pl_java,    pl_clojure,
  pl_dart,    pl_dart_flutter,
  pl_ocaml,
  pl_r,       pl_julia,
  pl_haskell,
};

TargetRegisterInfo_t
chsrc_programming_languages_registry[] =
{
  {&pl_ruby_target,  NULL,  "gem ruby rb rubygem rubygems bundler"},
  {&pl_python_group_target, NULL,  "python pypi py"},
  {&pl_python_pip_target, NULL,  "pip"},
  {&pl_python_poetry_target, NULL,  "poetry"},
  {&pl_python_pdm_target, NULL,  "pdm"},
  {&pl_python_rye_target, NULL,  "rye"},
  {&pl_python_uv_target, NULL,  "uv"},
  {&pl_nodejs_group_target, NULL,  "node nodejs"},
  {&pl_nodejs_bun_target, NULL,  "bun"},
  {&pl_nodejs_npm_target, NULL,  "npm"},
  {&pl_nodejs_yarn_target, NULL,  "yarn"},
  {&pl_nodejs_pnpm_target, NULL,  "pnpm"},
  {&pl_nodejs_nvm_target, NULL,  "nvm"},
  {&pl_perl_target, NULL,  "perl cpan"},
  {&pl_php_target, NULL,  "php composer"},
  {&pl_lua_target, NULL,  "lua luarocks"},
  {&pl_go_target, NULL,  "go golang goproxy"},
  {&pl_rust_cargo_target, NULL,  "rust cargo crate crates"},
  {&pl_rust_rustup_target, NULL,  "rustup"},
  {&pl_java_target, NULL,  "java maven mvn gradle"},
  {&pl_clojure_target, NULL,  "clojure clojars cloj lein"},
  {&pl_dart_target, NULL,  "dart pub"},
  {&pl_dart_flutter_target, NULL,  "flutter"},
  {&pl_nuget_target, NULL,  "nuget net .net dotnet"},
  {&pl_haskell_target, NULL,  "haskell cabal stack hackage"},
  {&pl_ocaml_target, NULL,  "ocaml opam"},
  {&pl_r_target, NULL,  "cran r"},
  {&pl_julia_target, NULL,  "julia"},
};

static const char
/* Zorin OS 完全使用 Ubuntu 的换源方法，二者兼容 */
*os_ubuntu     [] = {"ubuntu",   "zorinos",  NULL,  t(&os_ubuntu_target)},
*os_linuxmint  [] = {"linuxmint",             NULL,  t(&os_linuxmint_target)},
*os_debian     [] = {"debian",               NULL,  t(&os_debian_target)},
*os_fedora     [] = {"fedora",               NULL,  t(&os_fedora_target)},
*os_opensuse   [] = {"opensuse", "opensuse-leap", "opensuse-tumbleweed", NULL, t(&os_opensuse_target)},
*os_kali       [] = {"kali",                 NULL,  t(&os_kali_target)},
*os_msys2      [] = {"msys2",   "msys",      NULL,  t(&os_msys2_target)},
*os_arch       [] = {"arch",    "archlinux", NULL,  t(&os_arch_target)},
*os_archlinuxcn[] = {"archlinuxcn", "archcn",NULL,  t(&os_archlinuxcn_target)},
*os_manjaro    [] = {"manjaro",              NULL,  t(&os_manjaro_target)},
*os_gentoo     [] = {"gentoo",               NULL,  t(&os_gentoo_target)},
*os_rockylinux [] = {"rocky",  "rockylinux", NULL,  t(&os_rockylinux_target)},
*os_almalinux  [] = {"alma",   "almalinux",  NULL,  t(&os_almalinux_target)},
*os_alpine     [] = {"alpine",               NULL,  t(&os_alpine_target)},
*os_voidlinux  [] = {"voidlinux",            NULL,  t(&os_voidlinux_target)},
*os_solus      [] = {"solus",                NULL,  t(&os_solus_target)},
*os_trisquel   [] = {"trisquel",             NULL,  t(&os_trisquel_target)},
*os_linuxlite  [] = {"linuxlite",             NULL,  t(&os_linuxlite_target)},
*os_ros        [] = {"ros",    "ros2",       NULL,  t(&os_ros_target)},

*os_raspberrypi[] = {"raspi",  "raspberrypi",NULL,  t(&os_raspberrypi_target)},
*os_armbian    [] = {"armbian",              NULL,  t(&os_armbian_target)},
*os_openwrt    [] = {"openwrt", "opkg", "LEDE", NULL, t(&os_openwrt_target)},
*os_termux     [] = {"termux", NULL,                t(&os_termux_target)},

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
  os_trisquel, os_linuxlite, os_raspberrypi,  os_armbian,   os_openwrt, os_termux,
  os_deepin,   os_openkylin, os_ros,

  os_fedora,  os_rockylinux, os_almalinux,  os_openeuler, os_anolis,

  os_opensuse,

  os_arch,    os_archlinuxcn, os_manjaro, os_gentoo,

  os_alpine,   os_voidlinux,  os_solus,

  os_msys2,
  os_freebsd,  os_netbsd,    os_openbsd,
};


TargetRegisterInfo_t
chsrc_operating_systems_registry[] =
{
  {&os_ubuntu_target,  NULL,  "ubuntu zorinos"},
  {&os_linuxmint_target, NULL,  "linuxmint"},
  {&os_debian_target,  NULL,  "debian"},
  {&os_fedora_target,  NULL,  "fedora"},
  {&os_opensuse_target, NULL,  "opensuse opensuse-leap opensuse-tumbleweed"},
  {&os_kali_target,    NULL,  "kali"},
  {&os_msys2_target,   NULL,  "msys2 msys"},
  {&os_arch_target,    NULL,  "arch archlinux"},
  {&os_archlinuxcn_target, NULL,  "archlinuxcn archcn"},
  {&os_manjaro_target, NULL,  "manjaro"},
  {&os_gentoo_target,  NULL,  "gentoo"},
  {&os_rockylinux_target, NULL,  "rocky rockylinux"},
  {&os_almalinux_target, NULL,  "alma almalinux"},
  {&os_alpine_target,  NULL,  "alpine"},
  {&os_voidlinux_target, NULL,  "voidlinux"},
  {&os_solus_target,  NULL,  "solus"},
  {&os_trisquel_target, NULL,  "trisquel"},
  {&os_linuxlite_target, NULL,  "linuxlite"},
  {&os_ros_target,    NULL,  "ros ros2"},
  {&os_raspberrypi_target, NULL,  "raspi raspberrypi"},
  {&os_armbian_target, NULL,  "armbian"},
  {&os_openwrt_target, NULL,  "openwrt opkg LEDE"},
  {&os_termux_target, NULL,  "termux"},
  {&os_openkylin_target, NULL,  "openkylin"},
  {&os_openeuler_target, NULL,  "openeuler"},
  {&os_anolis_target, NULL,  "openanolis anolis"},
  {&os_deepin_target, NULL,  "deepin"},
  {&os_freebsd_target, NULL,  "freebsd"},
  {&os_netbsd_target, NULL,  "netbsd"},
  {&os_openbsd_target, NULL,  "openbsd"},
};

static const char
*wr_winget  [] = {"winget",  NULL,                   t(&wr_winget_target)},
*wr_homebrew[] = {"brew",       "homebrew",   NULL,  t(&wr_homebrew_target)},
*wr_cocoapods[] = {"cocoa",     "cocoapods","pod", "cocoapod",  NULL,  t(&wr_cocoapods_target)},
*wr_dockerhub [] ={"dockerhub", "docker",  NULL,  t(&wr_dockerhub_target)},
*wr_flathub [] = {"flathub",    "flatpak", NULL,  t(&wr_flathub_target), t(wr_flathub_prelude)},
*wr_nix     [] = {"nix",     NULL,                t(&wr_nix_target)},
*wr_guix    [] = {"guix",    NULL,                t(&wr_guix_target)},
*wr_emacs   [] = {"emacs", "elpa",         NULL,  t(&wr_emacs_target)},
*wr_tex     [] = {"latex", "ctan", "tex", "texlive", "miktex", "tlmgr", "mpm", NULL, t(&wr_tex_target)},
*wr_anaconda[] = {"conda", "anaconda",     NULL,  t(&wr_anaconda_target)},
**wr_softwares[] =
{
  wr_winget, wr_homebrew, wr_cocoapods, wr_dockerhub, wr_flathub, wr_nix, wr_guix, wr_emacs, wr_tex, wr_anaconda
};


TargetRegisterInfo_t
chsrc_softwares_registry[] =
{
  {&wr_winget_target,    NULL,  "winget"},
  {&wr_homebrew_target,  NULL,  "brew homebrew"},
  {&wr_cocoapods_target, NULL,  "cocoa cocoapods cocoapod"},
  {&wr_dockerhub_target, NULL,  "dockerhub docker"},
  {&wr_flathub_target,   wr_flathub_prelude,  "flathub flatpak"},
  {&wr_nix_target,       NULL,  "nix"},
  {&wr_guix_target,      NULL,  "guix"},
  {&wr_emacs_target,     NULL,  "emacs elpa"},
  {&wr_tex_target,       NULL,  "latex ctan tex texlive miktex tlmgr mpm"},
  {&wr_anaconda_target,  NULL,  "conda anaconda"},
};


#undef t


/**
 * @sync https://github.com/RubyMetric/chsrc/wiki
 */
static MirrorSite_t*
available_mirrors[] = {

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

/* End Target Matrix */
