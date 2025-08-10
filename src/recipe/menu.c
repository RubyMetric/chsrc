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
  {t(pl_ruby),           "gem ruby rb rubygem rubygems bundler"},
  {t(pl_python_group),   "python pypi py"},
  {t(pl_python_pip),     "pip"},
  {t(pl_python_poetry),  "poetry"},
  {t(pl_python_pdm),     "pdm"},
  {t(pl_python_rye),     "rye"},
  {t(pl_python_uv),      "uv"},
  {t(pl_js_group),       "node nodejs"},
  {t(pl_js_bun),         "bun"},
  {t(pl_js_npm),         "npm"},
  {t(pl_js_yarn),        "yarn"},
  {t(pl_js_pnpm),        "pnpm"},
  {t(pl_js_nvm),         "nvm"},
  {t(pl_perl),           "perl cpan"},
  {t(pl_php),            "php composer"},
  {t(pl_lua),            "lua luarocks"},
  {t(pl_go),             "go golang goproxy"},
  {t(pl_rust_cargo),     "rust cargo crate crates"},
  {t(pl_rust_rustup),    "rustup"},
  {t(pl_java),           "java maven mvn gradle"},
  {t(pl_clojure),        "clojure clojars cloj lein"},
  {t(pl_dart),           "dart pub"},
  {t(pl_dart_flutter),   "flutter"},
  {t(pl_nuget),          "nuget net .net dotnet"},
  {t(pl_haskell),        "haskell cabal stack hackage"},
  {t(pl_ocaml),          "ocaml opam"},
  {t(pl_r),              "cran r"},
  {t(pl_julia),          "julia"},
};



TargetRegisterInfo_t
chsrc_os_menu[] =
{
  /* Zorin OS 完全使用 Ubuntu 的换源方法，二者兼容 */
  {t(os_ubuntu),     "ubuntu zorinos"},
  {t(os_linuxmint),  "linuxmint"},
  {t(os_debian),     "debian"},
  {t(os_fedora),     "fedora"},
  {t(os_opensuse),   "opensuse opensuse-leap opensuse-tumbleweed"},
  {t(os_kali),       "kali"},
  {t(os_msys2),      "msys2 msys"},
  {t(os_arch),       "arch archlinux"},
  {t(os_archlinuxcn),"archlinuxcn archcn"},
  {t(os_manjaro),    "manjaro"},
  {t(os_gentoo),     "gentoo"},
  {t(os_rockylinux), "rocky rockylinux"},
  {t(os_almalinux),  "alma almalinux"},
  {t(os_alpine),     "alpine"},
  {t(os_voidlinux),  "voidlinux"},
  {t(os_solus),      "solus"},
  {t(os_trisquel),   "trisquel"},
  {t(os_linuxlite),  "linuxlite"},
  {t(os_ros),        "ros ros2"},
  {t(os_raspberrypi),"raspi raspberrypi"},
  {t(os_armbian),    "armbian"},
  {t(os_openwrt),    "openwrt opkg LEDE"},
  {t(os_termux),     "termux"},
  {t(os_openkylin),  "openkylin"},
  {t(os_openeuler),  "openeuler"},
  {t(os_anolis),     "openanolis anolis"},
  {t(os_deepin),     "deepin"},
  {t(os_freebsd),    "freebsd"},
  {t(os_netbsd),     "netbsd"},
  {t(os_openbsd),    "openbsd"},
};


TargetRegisterInfo_t
chsrc_wr_menu[] =
{
  {t(wr_winget),    "winget"},
  {t(wr_homebrew),  "brew homebrew"},
  {t(wr_cocoapods), "cocoa cocoapods cocoapod"},
  {t(wr_dockerhub), "dockerhub docker"},
  {t(wr_flathub),   "flathub flatpak"},
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
