#!/usr/bin/env bash
# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     :  installer.sh
# File Authors  :    GnixAij     <gaojiaxing0220@gmail.com>
#               |     xuan       <wick.dynex@qq.com>
#               |    ChatGPT     <https://chatgpt.com>
#               |   Aoran Zeng   <ccmywish@qq.com>
# Contributors  : GitHub Copilot <https://github.com/copilot>
#               |   DeepSeek     <https://chat.deepseek.com/>
#               |
# Created On    : <2024-10-25>
# Last Modified : <2025-07-12>
#
#         chsrc installer for POSIX (mainly Linux & macOS)
# ---------------------------------------------------------------

binary_name="chsrc"

# 用户指定的安装目录，函数set_install_dir()将填充/校验该变量
userOpt_install_dir=""
# 默认安装目录
default_install_dir_4root="/usr/local/bin"
default_install_dir_4nonroot="$HOME/.local/bin"

# 若用户指定的目录不存在则会创建该目录
tmp_created_install_dir=""

userOpt_version="pre"
userOpt_help=0
userOpt_lang="zh"



info() {
  echo "[INFO] $*"
}

warn() {
  echo "[WARN] $*"
}

error() {
  echo "[ERROR] $*"
}

# 用户自己输入
typeit() {
  echo "[->] $ $*"
}

error_exit() {
  echo -e "[ERROR] $*" >&2
  exit 1
}


is_zh() {
  if [ "$userOpt_lang" = "zh" ]; then
    return 0
  else
    return 1
  fi
}


help() {
  if is_zh; then
    echo "chsrc-installer: 在类Unix操作系统上安装 chsrc"
    echo
    echo "使用: install.sh [选项]"
    echo "选项:"
    echo "-h            打印此帮助信息"
    echo "-d <dir>      指定安装目录，默认为 $default_install_dir_4root 或 $default_install_dir_4nonroot"
    echo "-v <version>  指定 chsrc 版本"
    echo "-l <lang>     指定输出语言，支持 zh 和 en "
    echo
  else
    echo "chsrc-installer: Install chsrc on any Unix-like OS"
    echo
    echo "Usage: install.sh [options]"
    echo "Options:"
    echo "-h            Print this help information"
    echo "-d <dir>      Specify installation directory, default is $default_install_dir_4root OR $default_install_dir_4nonroot"
    echo "-v <version>  Specify chsrc version"
    echo "-l <lang>     Specify output language, supports zh and en"
    echo
  fi
}


let_user_build() {
  if command -v make &>/dev/null; then
    # 检测是否是 GNU make，如果是则用 make，如果不是则用C编译器来编译
    if make --version | grep -q "GNU Make"; then
      typeit "make"
    else
      if is_zh; then
        warn "检测到非 GNU make，本项目的Makefile仅支持GNU make"
      else
        warn "Non-GNU make was detected, the Makefile for this project only supports GNU make"
      fi
      typeit "cc/gcc/clang -Iinclude -Ilib src/chsrc-main.c -o chsrc"
    fi
  else
    if is_zh; then
      warn "未检测到GNU make，需手动执行编译命令："
    else
      warn "GNU make was not detected, so we need to run the compile command manually:"
    fi
    typeit "cc/gcc/clang -Iinclude -Ilib src/chsrc-main.c -o chsrc"
  fi
}

let_user_use_bootstrap() {
  info "请参考: https://github.com/RubyMetric/chsrc/tree/main/bootstrap 查找是否存在已有的 boostrapper"
}


let_user_compile() {
  source_zip_url="https://gitee.com/RubyMetric/chsrc/repository/archive/main.zip"
  echo ""

  if command -v git &>/dev/null; then
    typeit "git clone https://gitee.com/RubyMetric/chsrc.git"
    typeit "cd chsrc"
    let_user_build
    exit 1
  elif command -v curl &>/dev/null; then
    typeit "curl -LO $source_zip_url"
  elif command -v wget &>/dev/null; then
    typeit "wget $source_zip_url"
  else
    info "Please download the source code: $source_zip_url"
  fi

  typeit "unzip ./main.zip "
  typeit "cd ./chsrc-main"
  let_user_build
  exit 1;
}



get_arch() {
  echo $(uname -m | tr '[:upper:]' '[:lower:]')
}

get_platform() {
  uname -s | tr '[:upper:]' '[:lower:]'
}

# Linux   -> GNU/Linux
# Android -> Android
get_os() {
  uname -o | tr '[:upper:]' '[:lower:]'
}


set_arch() {
  arch=$(get_arch)

  case "$arch" in
    x86_64)  arch="x64" ;;
    aarch64|arm64) arch="aarch64" ;;
    riscv64) arch="riscv64" ;;
    armv7*)  arch="armv7" ;;
    *)
      if is_zh; then
        warn "抱歉, 暂无预编译二进制文件供你所在的平台架构: ${arch} 使用。请使用 chsrc-bootstrap 或 自行编译："
      else
        warn "Sorry, No precompiled binaries for your arch: ${arch}. Please compile it on your own:"
      fi
      let_user_use_bootstrap
      let_user_compile
      exit 1
      ;;
  esac
}


set_platform() {
  platform=$(get_platform)

  case "$platform" in
    linux)
      platform="linux"
      whatos=$(get_os)
      if [ "$whatos" = "android" ]; then
        if is_zh; then
          info "使用 chsrc-bootstrap: Termux"
        else
          info "Use chsrc-bootstrap: Termux"
        fi
          wget -O - https://gitee.com/RubyMetric/chsrc/raw/main/bootstrap/Termux.bash | bash
        exit 1
      fi
      ;;
    darwin) platform="macos" ;;
    bsd|dragonfly)
      platform="bsd"
      if is_zh; then
        info "抱歉, 暂无预编译二进制文件供BSD使用。请使用 chsrc-bootstrap 或 自行编译："
      else
        info "Sorry, No precompiled binaries for BSD! Please compile it on your own:"
      fi
      let_user_use_bootstrap
      let_user_compile
      exit 1
      ;;
    *)
      if is_zh; then
        error_exit "抱歉，暂无预编译二进制文件供你所在的平台: ${platform} 使用。请使用 chsrc-bootstrap 或 自行编译："
      else
        error_exit "Sorry, No precompiled binaries for your platform: ${platform}. Please compile it on your own:"
      fi
      let_user_use_bootstrap
      let_user_compile
      exit 1
      ;;
  esac
}


set_binary_version() {
  if [[ ! "$userOpt_version" =~ ^(pre|0\.([1-9])\.([0-9]))$ ]]; then
      # version 不符合条件，报错
      if is_zh; then
        error_exit "不支持的版本: ${userOpt_version}，版本号必须为 0.x.y (>=0.1.4) 或 'pre'"
      else
        error_exit "Unsupported version: ${userOpt_version}. Version number must be 0.x.y (>=0.1.4) or 'pre'"
      fi
  fi

  binary_version="${userOpt_version}"

  # version 版本不是 'pre'，添加'v'前缀
  if [[ "$userOpt_version" =~ ^(0\.([1-9])\.([0-9]))$ ]]; then
    binary_version="v${userOpt_version}"
  fi
}


#
# 1. 若用户指定了安装目录，则安装至那里
#
#    若安装目录不存在，该脚本会为用户创建
#
# 2. 若用户没有指定安装目录:
#
#    2.1 若 chsrc 已存在，自动安装到该位置
#    2.2 若 chsrc 不存在，安装到两个预定义的默认安装位置
#
set_install_dir() {
  if [ -n "$userOpt_install_dir" ]; then
    # 扩展 ~ 符号
    userOpt_install_dir="${userOpt_install_dir/#\~/$HOME}"

    # 检查目录是否存在，如果不存在则创建该目录
    if [ ! -d "$userOpt_install_dir" ]; then

      if is_zh; then
        echo "目录 $userOpt_install_dir 不存在，正在创建..."
      else
        echo "Directory $userOpt_install_dir does not exist. Creating..."
      fi

      if ! mkdir -p "$userOpt_install_dir"; then
        if is_zh; then
          echo "创建目录失败，请重试"
        else
          echo "Failed to create directory, please try again"
        fi
        exit 1
      fi

      tmp_created_install_dir="$userOpt_install_dir"  # 记录临时安装目录
    fi

  elif existing_path=$(command -v "$binary_name" 2>/dev/null); then

    if is_zh; then
      info "$binary_name 已安装，更新路径: ${existing_path}"
    else
      info "$binary_name is already installed, updating path: ${existing_path}"
    fi

    userOpt_install_dir=$(dirname "$existing_path")

    if [ ! -w "$userOpt_install_dir" ]; then
      if is_zh; then
        error_exit "目录 $userOpt_install_dir 不可写，请使用 sudo 命令运行脚本；或通过 -d 参数指定其它目录安装"
      else
        error_exit "Directory $userOpt_install_dir is not writable. Please run the script with sudo; or specify another dir using the -d option."
      fi
    fi

  else
    # 检查默认安装目录
    if [ -d "$default_install_dir_4root" ] && [ -w "$default_install_dir_4root" ]; then
      userOpt_install_dir="$default_install_dir_4root"
    elif [ -d "$default_install_dir_4nonroot" ] && [ -w "$default_install_dir_4nonroot" ]; then
      userOpt_install_dir="$default_install_dir_4nonroot"
    else
      if is_zh; then
        error_exit "默认安装目录 $default_install_dir_4root 和 $default_install_dir_4nonroot 不可写，请使用 sudo 命令运行脚本；或通过 -d 参数指定其它目录安装"
      else
        error_exit "Default install dir $default_install_dir_4root and $default_install_dir_4nonroot is not writable. Please run the script with sudo; or specify another dir using the -d option."
      fi
    fi
  fi
}

#
# @param $1: 下载链接
# @param $2: 保存路径(带文件名)
#
download() {
  if command -v curl &>/dev/null; then
    curl -sL "$1" -o "$2"
    return $? # 只能单独 return
  fi

  if command -v wget &>/dev/null; then
    # follow 是wget默认行为
    wget -qO "$2" "$1"
    return $?
  fi

  if is_zh; then
    error_exit "缺乏必要的下载工具(curl或wget)，无法下载文件"
  else
    error_exit "Missing necessary download tools (curl or wget) to download the file!"
  fi
}


install() {

  set_binary_version

  set_arch

  set_platform

  set_install_dir

  url="https://gitee.com/RubyMetric/chsrc/releases/download/${binary_version}/${binary_name}-${arch}-${platform}"

  path_to_executable="${userOpt_install_dir}/${binary_name}"

  if is_zh; then
    info "下载 ${binary_name} (架构: ${arch}, 平台: ${platform}, 版本: ${binary_version}) 到 ${path_to_executable}"
  else
    info "Downloading ${binary_name} (arch: ${arch}, platform: ${platform}, version: ${binary_version}) to ${path_to_executable}"
  fi

  if download $url "$path_to_executable"; then
    chmod +x "$path_to_executable"

    if is_zh; then
      info "🎉 安装完成，路径: $path_to_executable"
    else
      info "🎉 Installation completed, path: $path_to_executable"
    fi
  else
    if is_zh; then
      error_exit "下载失败，请检查网络连接和代理设置: ${url}"
    else
      error_exit "Download failed, please check your network connection and proxy settings: ${url}"
    fi
  fi
}


cleanup() {
  if [ -n "$tmp_created_install_dir" ] && [ -d "$tmp_created_install_dir" ]; then

    if is_zh; then
      echo "清理创建的目录: $tmp_created_install_dir"
    else
      echo "Cleaning up created directory: $tmp_created_install_dir"
    fi
    rm -rf "$tmp_created_install_dir"
  fi
}

trap cleanup EXIT


# main
while getopts ":hd:v:l:" option; do
  case $option in
  h)
    userOpt_help=1
    ;;
  d)
    userOpt_install_dir=${OPTARG}
    ;;
  v)
    userOpt_version=${OPTARG}
    ;;
  l)
    userOpt_lang=${OPTARG}
    ;;
  \?)
    echo "无效的命令行选项，请使用 -h 查看帮助"
    exit 1
    ;;
  esac
done

if [[ "$userOpt_lang" != "zh" && "$userOpt_lang" != "en" ]]; then
  error_exit "无效的语言选项: $userOpt_lang，支持的选项为 zh 和 en"
fi

if [ "$userOpt_help" -eq 1 ]; then
  help
  exit 0;
fi

install
