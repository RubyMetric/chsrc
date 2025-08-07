# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : download-pre-on-GitHub.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Created On    : <2023-09-21>
# Last Modified : <2025-08-07>
#
# Download all files from the `pre` release on GitHub:
#   https://github.com/RubyMetric/chsrc/releases/tag/pre
# ---------------------------------------------------------------

$destination = "~\Desktop\chsrc-pre-on-GitHub"

Write-Output "=> Mkdir $destination"
mkdir -Force $destination | Out-Null

$destination = (Resolve-Path $destination).Path

$names = @(
    'chsrc-x64-windows.exe'
    'chsrc-x86-windows.exe'

    'chsrc-aarch64-macos'
    'chsrc-x64-macos'

    'chsrc-x64-linux'
    'chsrc-aarch64-linux'
    'chsrc-riscv64-linux'
    'chsrc-armv7-linux'
    'chsrc_latest-1_amd64.deb'

    'chsrc-arm64-android'
)

# Like https://github.com/RubyMetric/chsrc/releases/download/latest/chsrc-x64-windows.exe
$url_prefix = "https://github.com/RubyMetric/chsrc/releases/download/pre/"

Write-Output "=> Starting downloads..."

$names | ForEach-Object -Parallel {
    $name = $_
    $url = $using:url_prefix + $name
    $dest = $using:destination

    Write-Output "  - Downloading $name"
    # -s 阻止输出，避免并行输出混乱
    # 此处必须使用绝对路径
    curl -s -LO $url --output-dir $dest
    Write-Output "  √ Completed $name"
} -ThrottleLimit 5  # 限制同时下载5个文件

Write-Output "=> All downloads completed!"

Write-Output "=> Downloaded to $destination"
