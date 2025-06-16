# ---------------------------------------------------------------
# File Name     : download-pre-on-GitHub.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Created On    : <2023-09-21>
# Last Modified : <2025-06-16>
#
# Download all files from the `pre` release on GitHub:
#   https://github.com/RubyMetric/chsrc/releases/tag/pre
# ---------------------------------------------------------------

$destination = "~\Desktop\chsrc-pre-onGitHub"

Write-Output "=> Mkdir $destination"
mkdir -Force $destination | Out-Null
Set-Location $destination

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
)

# Like https://github.com/RubyMetric/chsrc/releases/download/latest/chsrc-x64-windows.exe
$url_prefix = "https://github.com/RubyMetric/chsrc/releases/download/pre/"

foreach ($name in $names) {
  curl -LO "${url_prefix}${name}"
}

Set-Location -
Write-Output "=> Downloaded to $destination"
