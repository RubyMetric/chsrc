#!/usr/bin/env bash
# ---------------------------------------------------------------
# File Name     : install.sh
# File Authors  :  GnixAij   <gaojiaxing0220@gmail.com>
# Contributors  : Aoran Zeng <ccmywish@qq.com>
#               |
# Created On    : <2024-10-25>
# Last Modified : <2024-10-25>
#
#
#         chsrc Installer for Linux & macOS
#
# ---------------------------------------------------------------

install_dir=""
version="pre"
path_to_executable=""
default_install_path="/usr/local/bin"
binary_name="chsrc"

info() {
  echo "[INFO] $*"
}

error() {
  echo -e "[ERROR] $*" >&2
  exit 1
}

help() {
  echo "chsrc Installer"
  echo
  echo "使用: install.sh [-h] [-d <安装目录>]"
  echo "选项:"
  echo "-h     打印此帮助信息"
  echo "-d     指定安装目录，默认为 /usr/local/bin；如果已安装，则覆盖旧版本"
  echo
}


set_install_path() {
  if [ -n "$install_dir" ]; then
    # 扩展 ~ 符号
    install_dir="${install_dir/#\~/$HOME}"

    # 检查路径是否存在，如果不存在则创建该路径
    if [ ! -d "$install_dir" ]; then
      echo "目录 $install_dir 不存在，正在创建..."
      mkdir -p "$install_dir" || { echo "创建目录失败，请重试"; exit 1; }
    fi
  elif existing_path=$(command -v "$binary_name" 2>/dev/null); then
    info "$binary_name 已安装，更新路径: ${existing_path}"
    install_dir=$(dirname "$existing_path")
  else
    # 检查默认路径
    if [ -d "$default_install_path" ] && [ -w "$default_install_path" ]; then
      install_dir="$default_install_path"
    else
      error "默认下载路径 /usr/local/bin 不可写，请使用 sudo 命令运行脚本；或通过 -d 参数指定其它路径安装"
    fi
  fi
}


install() {
  arch="$(uname -m | tr '[:upper:]' '[:lower:]')"

  case "$arch" in
    x86_64)  arch="x64" ;;
    aarch64|arm64) arch="aarch64" ;;
    riscv64) arch="riscv64" ;;
    armv7*)  arch="armv7" ;;
    *)       error "不支持的架构: ${arch}" ;;
  esac

  platform="$(uname -s | awk '{print tolower($0)}')"

  case "$platform" in
    linux)  platform="linux" ;;
    darwin) platform="macos" ;;
    *)      error "不支持的平台: ${platform}" ;;
  esac

  if [[ ! "$version" =~ ^(pre|0\.1\.([4-9]))$ ]]; then
      # version 不符合条件，报错
      error "不支持的版本: ${version}，版本号必须在 0.1.4 到 0.1.9 之间或为 'pre'"
  fi

  url="https://gitee.com/RubyMetric/chsrc/releases/download/${version}/${binary_name}-${arch}-${platform}"

  path_to_executable="${install_dir}/${binary_name}"

  info "下载 ${binary_name} (${arch} 架构, ${platform} 平台， ${version}版本) 到 ${path_to_executable}"

  # 下载文件并设置权限
  if curl -sL "$url" -o "$path_to_executable"; then
    chmod +x "$path_to_executable"
    info "🎉 安装完成，路径: $path_to_executable"
  else
    error "下载失败，请检查您的网络连接和代理设置: ${url}"
  fi
}


# main
while getopts ":hd:v:" option; do
  case $option in
  h)
    help
    exit 0
    ;;
  d)
    install_dir=${OPTARG}
    ;;
  v)
    version=${OPTARG}
    ;;
  \?)
    echo "无效的命令行选项。使用 -h 查看帮助"
    exit 1
    ;;
  esac
done

set_install_path
install
