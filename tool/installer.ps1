# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : installer.ps1
# File Authors  :  xuan   <wick.dynex@qq.com>
#               | ChatGPT <https://chatgpt.com/>
#               | 曾奥然  <ccmywish@qq.com>
# Contributors  : GitHub Copilot <https://github.com/copilot>
#               |
# Created On    : <2024-10-26>
# Last Modified : <2026-01-06>
#
#         chsrc installer for Windows
# ---------------------------------------------------------------

param(
    [switch]
    $Help,
    $Directory = $null,
    $Version = "pre"
)


$script:binary_name = "chsrc"
$script:platform = "Windows"

$script:install_dir = ""
$script:arch = ""
$script:target_version = ""
$script:url = ""
$script:create_dir_flag = $false


function Help {
    Write-Host
@"
chsrc-installer: Install chsrc on ${script:platform}.

Usage: install.ps1 [options]
Options:
-h            Print this help information
-d <dir>      Specify installation directory, default is current directory
-v <version>  Specify chsrc version

"@
}

# 检查当前操作系统是否为 macOS 或 Linux
if ($IsMacOS -or $IsLinux) {
    Write-Host @"
Sorry,

This installer is only available for ${script:platform}.
If you're looking for installation instructions for your operating system,
please visit the following link:

https://github.com/RubyMetric/chsrc
"@
    exit
}

if ($Help) {
    Help
    exit
}

function Output_Info () {
    Write-Host "[INFO] $args"
}

function Output_Error () {
    Write-Host "[ERROR] $args"
    exit 1
}


# https://github.com/RubyMetric/chsrc/issues/332
function Get_System_Downloads_Dir {
    # 尝试从注册表获取实际的 Downloads 文件夹位置
    try {
        $shellFoldersKey = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders"
        $downloadsGuid = "{374DE290-123F-4565-9164-39C4925E467B}"
        $downloadsPath = (Get-ItemProperty -Path $shellFoldersKey -Name $downloadsGuid -ErrorAction Stop).$downloadsGuid

        # 展开环境变量 (例如 %USERPROFILE% -> C:\Users\xxx)
        $downloadsPath = [System.Environment]::ExpandEnvironmentVariables($downloadsPath)

        if (Test-Path -Path $downloadsPath -PathType Container) {
            return $downloadsPath
        }
    } catch {
        # 如果注册表读取失败，不输出错误信息，继续使用后备方案
    }

    # 后备方案：返回 null，稍后使用当前目录
    return $null
}

function Set_Install_Dir {
    # 如果用户未指定目录，则自动检测
    if ($null -eq $Directory) {
        # 尝试获取实际的 Downloads 目录
        $detectedDownloads = Get_System_Downloads_Dir
        if ($null -ne $detectedDownloads) {
            $Directory = $detectedDownloads
            Output_Info "Detected Downloads directory: $Directory"
        } else {
            # 使用当前目录作为默认值
            $Directory = $PWD.Path
            Output_Info "Using current directory: $Directory"
        }
    }

    # 检查目录是否存在
    if (-not (Test-Path -Path $Directory -PathType Container)) {
        # 如果目录不存在，执行下面的代码块
        try {
            New-Item -Path $Directory -ItemType Directory -Force | Out-Null
            Output_Info "Directory created: $Directory"
            $script:create_dir_flag = $true
        } catch {
            Output_Error "Failed to create directory: $_"
        }
    }
    $script:install_dir=$Directory
    # 输出最终路径
    Output_Info "Set install dir to: $script:install_dir"
}

function Set_Version {
    $pattern = '^(0\.[1-9]\.[0-9]|pre)$'

    if ($Version -notmatch $pattern) {
        Output_Error "Invalid version '$Version'. Please provide a valid version: 0.x.y (>=0.1.4) or 'pre'"
    }

    # 设置版本号
    $script:target_version=$Version
    Output_Info "Set chsrc version: $script:target_version"
}

function Set_URL {
    # 获取 CPU 型号
    $cpuArchitecture = Get-WmiObject Win32_Processor `
                        | Select-Object -First 1 -ExpandProperty Architecture

    switch ($cpuArchitecture) {
        0 { $script:arch = 'x86' }
        9 {
            # 如果是 64 位操作系统，选择 x64 安装包，否则选择 x86
            if ([Environment]::Is64BitOperatingSystem) {
                $script:arch = "x64"
            }
            else {
                $script:arch = "x86"
            }
        }
        default {
            Output_Error "Unsupported architecture '$cpuArchitecture'. Only x86 or x64 architectures are supported."
        }
    }
    Output_Info "Get my CPU architecture: $script:arch"

    # Set URL
    if ($script:target_version -eq "pre") {
        $script:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                            "${script:target_version}/chsrc-${script:arch}-windows.exe"
    }
    else {
        $script:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                            "v" + "${script:target_version}/chsrc-${script:arch}-windows.exe"
    }

    Output_Info "Set download URL: $script:url"
}

function Install {
    try {
        # 设置安全协议为 TLS 1.2
        [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

        # 检查 URL 是否可访问
        $response = Invoke-WebRequest -Uri $script:url -Method Head -ErrorAction Stop

        if ($response.StatusCode -ne 200) {
            Output_Error "Unable to access $script:url. Status code: $($response.StatusCode)"
        }
    }
    catch {
        Write-Host "Unable to download ${script:binary_name}. Please check your internet connection."
        exit 1
    }

    try {
        $outfile = "\${script:binary_name}.exe"
        Output_Info "Downloading $script:binary_name (architecture: $script:arch, platform: $script:platform, version: $script:target_version) to $script:install_dir"
        Invoke-WebRequest -OutFile ($script:install_dir + $outfile) -Uri $script:url -ErrorAction Stop
        # 🎉 这个符号会变成 ??? 不要添加
        Output_Info "Installation completed, destination:" ($script:install_dir + $outfile)
    } catch {
        Output_Error "Unable to download $script:binary_name. Error: $_"
    }
}


function Cleanup {
    if ($script:create_dir_flag -eq $true) {
        if (Test-Path -Path $script:install_dir) {
            Remove-Item -Path $script:install_dir -Recurse -Force  # 删除路径及其内容
            Output_Info "Deleted the directory: $script:install_dir"
        }
    }
}


$null = Register-EngineEvent PowerShell.Exiting -Action { Cleanup }


# main
Set_Install_Dir
Set_Version
Set_URL
Install
