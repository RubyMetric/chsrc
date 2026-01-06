# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : installer.ps1
# File Authors  :    xuan    <wick.dynex@qq.com>
#               |  ChatGPT   <https://chatgpt.com/>
# Contributors  : Aoran Zeng <ccmywish@qq.com>
#               |
# Created On    : <2024-10-26>
# Last Modified : <2026-01-06>
#
#         chsrc installer for Windows
# ---------------------------------------------------------------

# 定义参数
param(
    [switch]
    $Help,
    $Directory = $null,
    $Version = "pre"
)


$binary_name = "chsrc"
$platform = "Windows"

$global:install_dir = ""
$global:arch = ""
$global:version = ""
$global:url = ""
$global:flag = 0


$installInstructions = @"
Hey friend

This installer is only available for ${platform}.
If you're looking for installation instructions for your operating system,
please visit the following link:
"@

# 检查当前操作系统是否为 macOS 或 Linux
if ($IsMacOS -or $IsLinux) {
    Write-Host @"
$installInstructions

https://github.com/RubyMetric/chsrc
"@
    exit
}

function help {
    Write-Host
@"
chsrc-installer: Install chsrc on ${platform}.

Usage: install.ps1 [options]
Options:
-h            Print this help information
-d <dir>      Specify installation directory, default is current directory
-v <version>  Specify chsrc version

"@
}

# 执行帮助函数
if ($Help) {
    help
    exit
}

function output_info () {
    Write-Host "[INFO] $args"
}

function output_error () {
    Write-Host "[ERROR] $args"
    exit 1
}


function Get_Downloads_Dir {
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
        $detectedDownloads = Get_Downloads_Dir
        if ($null -ne $detectedDownloads) {
            $Directory = $detectedDownloads
            output_info "Detected Downloads directory: $Directory"
        } else {
            # 使用当前目录作为默认值
            $Directory = $PWD.Path
            output_info "Using current directory: $Directory"
        }
    }
    
    # 检查目录是否存在
    if (-not (Test-Path -Path $Directory -PathType Container)) {
        # 如果目录不存在，执行下面的代码块
        try {
            New-Item -Path $Directory -ItemType Directory -Force | Out-Null
            output_info "Directory created: $Directory"
            $global:flag = 1
        } catch {
            output_error "Failed to create directory: $_"
        }
    }
    $global:install_dir=$Directory
    # 输出最终路径
    output_info "Set install dir to: $global:install_dir"
}

function Set_Version {
    $pattern = '^(0\.[1-9]\.[0-9]|pre)$'

    if ($Version -notmatch $pattern) {
        output_error "Invalid version '$Version'. Please provide a valid version: 0.x.y (>=0.1.4) or 'pre'"
    }

    # 设置版本号
    $global:version=$Version
    output_info "Set chsrc version: $global:version"
}

function Set_URL {
    # 获取 CPU 型号
    $cpuArchitecture = Get-WmiObject Win32_Processor `
                        | Select-Object -First 1 -ExpandProperty Architecture

    switch ($cpuArchitecture) {
        0 { $global:arch = 'x86' }
        9 {
            # 如果是 64 位操作系统，选择 x64 安装包，否则选择 x86
            if ([Environment]::Is64BitOperatingSystem) {
                $global:arch = "x64"
            }
            else {
                $global:arch = "x86"
            }
        }
        default {
            output_error "Unsupported architecture '$cpuArchitecture'. Only x86 or x64 architectures are supported."
        }
    }
    output_info "Get my CPU architecture: $global:arch"

    # Set URL
    if ($version -eq "pre") {
        $global:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                            "${global:version}/chsrc-${global:arch}-windows.exe"
    }
    else {
        $global:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                            "v" + "${global:version}/chsrc-${global:arch}-windows.exe"
    }

    output_info "Set download URL: $global:url"
}

function Install {
    try {
        # 设置安全协议为 TLS 1.2
        [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

        # 检查 URL 是否可访问
        $response = Invoke-WebRequest -Uri $global:url -Method Head -ErrorAction Stop

        if ($response.StatusCode -ne 200) {
            output_error "Unable to access $global:url. Status code: $($response.StatusCode)"
        }
    }
    catch {
        Write-Host "Unable to download ${binary_name}. Please check your internet connection."
        exit 1
    }

    try {
        $outfile = "\${binary_name}.exe"
        output_info "Downloading $binary_name (architecture: $global:arch, platform: $platform, version: $global:version) to $global:install_dir"
        Invoke-WebRequest -OutFile ($global:install_dir + $outfile) -Uri $global:url -ErrorAction Stop
        # 🎉 这个符号会变成 ??? 不要添加
        output_info "Installation completed, destination:" ($global:install_dir + $outfile)
    } catch {
        output_error "Unable to download $binary_name. Error: $_"
    }
}


function cleanup {
    if ($flag -eq 1) {
        if (Test-Path -Path $global:install_dir) {
            Remove-Item -Path $global:install_dir -Recurse -Force  # 删除路径及其内容
            output_info "Deleted the directory: $global:install_dir"
        }
    }
}


$null = Register-EngineEvent PowerShell.Exiting -Action { cleanup }


# main
Set_Install_Dir
Set_Version
Set_URL
Install
