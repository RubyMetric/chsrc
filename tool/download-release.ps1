# ---------------------------------------------------------------
# File          : download-release.ps1
# Authors       : Aoran Zeng <ccmywish@qq.com>
# Created on    : <2023-09-21>
# Last modified : <2024-05-24>
#
# download:
#
#   Download from GitHub Releases
# ----------
# Changelog:
#
# ~> v0.1.0
# <2024-05-24> Rename to download-release
# <2023-09-21> Create file
# ---------------------------------------------------------------

$destination = "~\Desktop\chsrc-GitHub-release"

Write-Output "=> Mkdir $destination"
mkdir -Force $destination | Out-Null
Set-Location $destination

$names = @(
  'chsrc-x64-windows.exe'
  'chsrc-x86-windows.exe'
  'chsrc-x64-macos'
  'chsrc-x64-linux'
  'chsrc-aarch64-linux'
  'chsrc-riscv64-linux'
  'chsrc-armv7-linux'
)

# Like https://github.com/RubyMetric/chsrc/releases/download/preview/chsrc-x64-windows.exe
$url_prefix = "https://github.com/RubyMetric/chsrc/releases/download/preview/"

foreach ($name in $names) {
  curl -LO "${url_prefix}${name}"
}

Set-Location -
Write-Output "=> Downloaded to $destination"
