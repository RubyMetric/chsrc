#!/usr/bin/env bash
# ---------------------------------------------------------------
# File Name     : install.sh
# File Authors  :  GnixAij          <gaojiaxing0220@gmail.com>
#               |   xuan            <wick.dynex@qq.com>
#               | ChatGPT 4o mini   <https://chatgpt.com>
#               |
# Contributors  : Aoran Zeng <ccmywish@qq.com>
#               |
# Created On    : <2024-10-25>
# Last Modified : <2024-10-26>
#
#
#         chsrc installer for Linux & macOS
#
# ---------------------------------------------------------------

install_dir=""
version="pre"
path_to_executable=""
default_install_path="/usr/local/bin"
noroot_default_install_path="$HOME/.local/bin"
binary_name="chsrc"
temp_install_dir=""  # 用于存储临时安装目录
helpflag=0
lang="zh"

# 输出相关信息
info() {
  echo "[INFO] $*"
}

# 输出错误到stdout和stderr
error() {
  echo -e "[ERROR] $*" >&2
  exit 1
}

# 显示 chsrc 安装程序的帮助信息，包括使用说明和可用选项
help() {
  if [ "$lang" = "zh" ]; then
    echo "chsrc-installer: 在任何类Unix操作系统上安装 chsrc"
    echo
    echo "使用: install.sh [选项]"
    echo "选项:"
    echo "  -h               打印此帮助信息"
    echo "  -d <destination> 指定安装目录，默认为 /usr/local/bin；若已安装，则覆盖旧版本"
    echo "  -v <version>     指定 chsrc 版本"
    echo "  -l <language>    指定脚本语言，支持 zh 和 en "
    echo
  else
    echo "chsrc-installer: Install chsrc on any Unix-like OS"
    echo
    echo "Usage: install.sh [options]"
    echo "Options:"
    echo "  -h               Print this help information"
    echo "  -d <destination> Specify installation directory, default is /usr/local/bin; will overwrite if already installed"
    echo "  -v <version>     Specify chsrc version"
    echo "  -l <language>    Specify script language, supports zh and en"
    echo
  fi
}


set_install_path() {
  if [ -n "$install_dir" ]; then
    # 扩展 ~ 符号
    install_dir="${install_dir/#\~/$HOME}"

    # 检查路径是否存在，如果不存在则创建该路径
    if [ ! -d "$install_dir" ]; then
      # 多种语言输出
      if [ "$lang" = "zh" ]; then
        echo "目录 $install_dir 不存在，正在创建..."
      else
        echo "Directory $install_dir does not exist. Creating..."
      fi
      # 多语言输出
      if ! mkdir -p "$install_dir"; then
        if [ "$lang" = "zh" ]; then
          echo "创建目录失败，请重试"
        else
          echo "Failed to create directory, please try again"
        fi
        exit 1
      fi

      temp_install_dir="$install_dir"  # 记录临时安装目录
    fi

  elif existing_path=$(command -v "$binary_name" 2>/dev/null); then

    if [ "$lan" = "zh" ]; then
      info "$binary_name 已安装，更新路径: ${existing_path}"
    else
      info "$binary_name is already installed, updating path: ${existing_path}"
    fi

    install_dir=$(dirname "$existing_path")
  else
    # 检查默认路径
    if [ -d "$default_install_path" ] && [ -w "$default_install_path" ]; then
      install_dir="$default_install_path"
    elif [ -d "$noroot_default_install_path" ] && [ -w "$noroot_default_install_path" ]; then
    elif [ -d "$noroot_default_install_path" ] && [ -w "$noroot_default_install_path" ]; then
      install_dir="$noroot_default_install_path"
    else
      if [ "$lan" = "zh" ]; then
        error "默认下载路径 /usr/local/bin 不可写，请使用 sudo 命令运行脚本；或通过 -d 参数指定其它路径安装"
      else
        error "Default download path /usr/local/bin is not writable. Please run the script with sudo; or specify another path using the -d option."
      fi
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
    *)
      if [ "$lang" = "zh" ]; then
        error "不支持的架构: ${arch}"
      else
        error "Unsupported architecture: ${arch}"
      fi
      ;;
  esac

  platform="$(uname -s | awk '{print tolower($0)}')"

  case "$platform" in
    linux)  platform="linux" ;;
    darwin) platform="macos" ;;
    *)
      if [ "$lang" = "zh" ]; then
        error "不支持的平台: ${platform}"
      else
        error "Unsupported platform: ${platform}"
      fi
      ;;
  esac

  if [[ ! "$version" =~ ^(pre|0\.1\.([4-9]))$ ]]; then
      # version 不符合条件，报错
      if [ "$lang" = "zh" ]; then
        error "不支持的版本: ${version}，版本号必须在 0.1.4 到 0.1.9 之间或为 'pre'"
      else
        error "Unsupported version: ${version}. Version number must be between 0.1.4 and 0.1.9 or 'pre'"
      fi
  fi

  url="https://gitee.com/RubyMetric/chsrc/releases/download/${version}/${binary_name}-${arch}-${platform}"

  path_to_executable="${install_dir}/${binary_name}"

  if [ "$lang" = "zh" ]; then
    info "下载 ${binary_name} (${arch} 架构, ${platform} 平台， ${version}版本) 到 ${path_to_executable}"
  else
    info "Downloading ${binary_name} (${arch} architecture, ${platform} platform, version ${version}) to ${path_to_executable}"
  fi

  if curl -sL "$url" -o "$path_to_executable"; then
    chmod +x "$path_to_executable"

    if [ "$lang" = "zh" ]; then
      info "🎉 安装完成，版本： $version，路径: $path_to_executable"
    else
      info "🎉 Installation completed, path: $path_to_executable"
    fi

  else
    if [ "$lang" = "zh" ]; then
      error "下载失败，请检查您的网络连接和代理设置: ${url}"
    else
      error "Download failed, please check your network connection and proxy settings: ${url}"
    fi

  fi
}

# 清理函数
cleanup() {
  if [ -n "$temp_install_dir" ] && [ -d "$temp_install_dir" ]; then

    if [ "$lang" = "zh" ]; then
      echo "清理创建的目录: $temp_install_dir"
    else
      echo "Cleaning up created directory: $temp_install_dir"
    fi
    rm -rf "$temp_install_dir"
  fi
}

# 设置 trap 以捕获退出信号
trap cleanup EXIT


# 从命令行读取 安装路径与版本号
while getopts ":hd:v:l:" option; do
  case $option in
  h)
    helpflag=1
    ;;
  d)
    install_dir=${OPTARG}
    ;;
  v)
    version=${OPTARG}
    ;;
  l)
    lang=${OPTARG}
    ;;
  \?)
    echo "无效的命令行选项，请使用 -h 查看帮助"
    exit 1
    ;;
  esac
done

# 判断语言的类型，不符合直接退出
if [[ "$lang" != "zh" && "$lang" != "en" ]]; then
  error "无效的语言选项: $lang，支持的选项为 zh 和 en"
fi

if [ "$helpflag" -eq 1 ]; then
  help
  exit 0;
fi

set_install_path
install
