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

TargetRegisterInfo_t
chsrc_pl_menu[] =
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



TargetRegisterInfo_t
chsrc_os_menu[] =
{
  /* Zorin OS 完全使用 Ubuntu 的换源方法，二者兼容 */
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


TargetRegisterInfo_t
chsrc_wr_menu[] =
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
