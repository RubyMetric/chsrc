# ---------------------------------------------------------------
# File Name     : install.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
#               |   ChatGPT  <https://chatgpt.com/>
# Created On    : <2024-10-26>
# Last Modified : <2024-10-26>
#
#
#         chsrc installer for Windows
#
# ---------------------------------------------------------------

# [Environment]::Is64BitProcess # True

$osarch = [System.Runtime.InteropServices.RuntimeInformation]::OSArchitecture

$dlarch = ""

switch ($osarch) {

  "Arm64" {
    $dlarch = "arm64"
    Write-Error "Unsupported platform Arm64";
  }

  "Arm"   {
    $dlarch = "arm"
    Write-Error "Unsupported platform Arm";
  }

  'X64' { $dlarch = "x64" }
  'X86' { $dlarch = "x86" }
}

$url = "https://gitee.com/RubyMetric/chsrc/releases/download/pre/chsrc-${dlarch}-windows.exe"

Write-Output "[INFO] URL: $url"
curl.exe -L $url -o chsrc.exe
Write-Output "🎉 Installation completed, path: ./chsrc.exe"
