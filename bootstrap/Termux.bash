#!/usr/bin/env bash
# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : Termux.bash
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  : Nul None <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-22>
#
# Termux:
#
#   Bootstrap Termux: https://github.com/RubyMetric/chsrc/issues/173
#
#   @consult https://help.mirrors.cernet.edu.cn/termux/
#
#   我们默认采用校园网联合镜像站提供的源
# ---------------------------------------------------------------

bs_echo() {
  echo "[chsrc-bootstrap] $*"
}

if command -v termux-change-repo &>/dev/null; then
  termux-change-repo
else

  # $PREFIX 的值为:  /data/data/com.termux/files/usr
  # 用户主目录 ~ 为: /data/data/com.termux/files/home

  # 必要的
  sed -i 's@^\(deb.*stable main\)$@#\1\ndeb https://mirrors.cernet.edu.cn/termux/apt/termux-main stable main@' $PREFIX/etc/apt/sources.list
  apt update

  # x11-repo
  sed -i 's@^\(deb.*x11 main\)$@#\1\ndeb https://mirrors.cernet.edu.cn/termux/apt/termux-x11 x11 main @' $PREFIX/etc/apt/sources.list.d/x11.list

  # root-repo
  sed -i 's@^\(deb.*root main\)$@#\1\ndeb https://mirrors.cernet.edu.cn/termux/apt/termux-root root main @' $PREFIX/etc/apt/sources.list.d/root.list
fi

# 立即更新测试换源状态
apt update && apt upgrade

bs_echo "基础换源已完成"

read -p "是否需要安装 chsrc ? (y/n): " need_install_chsrc

if [[ $need_install_chsrc == "y" || $need_install_chsrc == "Y" ]]; then
  bs_echo "正在安装依赖项..."
  apt-get install -y gcc make git
  git clone https://gitee.com/RubyMetric/chsrc.git --depth 1
  bs_echo "依赖安装完成!"

  bs_echo "正在开始编译和安装"
  cd chsrc
  make build-in-release-mode
  make install
  bs_echo "chsrc 安装完成!"
fi
