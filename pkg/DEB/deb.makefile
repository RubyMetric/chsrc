#!/usr/bin/make -f
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    : deb.makefile
# File Authors  : sanchuanhehe <wyihe5520@gmail.com>
# Contributors  :  Aoran Zeng  <ccmywish@qq.com>
#								|
# Created On    : <2025-06-14>
# Last Modified : <2025-06-15>
#
# DEB package targets
#
# @issue https://github.com/RubyMetric/chsrc/pull/206
# 这些本是 debhelper 兼容性版本 (debian/compat) 为 9 时所需要定义的 targets
# 然而现在已经为版本 13 (debian/compat已移除)，不再需要这些 targets，仅出于
# 实用目的保留。由于以上原因，也不要修改这些 targets 的名称。
# --------------------------------------------------------------

all: deb-build

deb-prepare:
	@echo "Starting: Prepare for building DEB package"
	@echo "Finished: Prepare for building DEB package"

deb-build: deb-prepare
	@echo "Starting: Build DEB package"
	@debuild -us -uc -b
	@echo "Finished: Build DEB package"

deb-clean:
	@echo "Starting: Clean DEB build artifacts"
	-@rm -rf debian/chsrc/
	-@rm -f ../chsrc_*.deb ../chsrc-dbgsym_*.ddeb ../chsrc_*.changes ../chsrc_*.buildinfo ../chsrc_*.build
	@echo "Finished: Clean DEB build artifacts"

.PHONY: deb-prepare deb-build deb-clean
