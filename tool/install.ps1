# ---------------------------------------------------------------
# File Name     : install.ps1
# File Authors  :    xuan    <wick.dynex@qq.com>
#               |  ChatGPT   <https://chatgpt.com/>
# Contributors  : Aoran Zeng <ccmywish@qq.com>
#               |
# Created On    : <2024-10-26>
# Last Modified : <2024-10-27>
#
#         chsrc installer for Windows
# ---------------------------------------------------------------

# 定义参数
param(
    [switch]
    $h,
    $d = "${HOME}\Downloads",
    $v = "pre"
)

$fileName = "\chsrc.exe"
$default_path = "${HOME}\Downloads"
$binary_name = "chsrc"
$platform = "Windows"

$global:path = ""
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

function Help {
    Write-Host
@"
chsrc-installer: Install chsrc on ${platform}.

Usage: install.sh [options]
Options:
-h            Print this help information
-d <dir>      Specify installation directory, default is $default_path
-v <version>  Specify chsrc version

"@
}

# 执行帮助函数
if ($h) {
    Help
    exit
}

function output_info () {
    Write-Host "[INFO] $args"
}

function output_error () {
    Write-Host "[ERROR] $args"
    exit 1
}


function Set_Install_Dir {
    # 检查目录是否存在
    if (-not (Test-Path -Path $d -PathType Container)) {
        # 如果目录不存在，执行下面的代码块
        try {
            New-Item -Path $d -ItemType Directory -Force | Out-Null
            output_info "Directory created: $d"
            $global:flag = 1
        } catch {
            output_error "Failed to create directory: $_"
        }
    }
    $global:path=$d
    # 输出最终路径
    output_info "Set install dir to: $global:path"
}

function Set_Version {
    $pattern = '^(0\.1\.[4-9]|pre)$'

    if ($v -notmatch $pattern) {
        output_error "Invalid version '$v'. Please provide a valid version (0.1.4 - 0.1.9 or 'pre')."
    }

    # 设置版本号
    $global:version=$v
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
    output_info "My CPU architecture: $global:arch"

    # Set URL
    if ($version -eq "pre") {
        $global:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                            "${global:version}/chsrc-${global:arch}-windows.exe"
    }
    else {
        $global:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                            "v" + "${global:version}/chsrc-${global:arch}-windows.exe"
    }

    output_info "Set downLoad URL: $global:url"
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
        output_info "Downloading $binary_name ($global:arch architecture, $platform platform, version $global:version) to $global:path ..."
        Invoke-WebRequest -OutFile ($global:path + $fileName) -Uri $global:url -ErrorAction Stop
        output_info "🎉 Installation completed, path: $global:path"
    } catch {
        output_error "Unable to download $binary_name. Error: $_"
    }
}


function cleanup {
    if ($flag -eq 1) {
        if (Test-Path -Path $path) {
            Remove-Item -Path $path -Recurse -Force  # 删除路径及其内容
            output_info "Deleted the path: $path"
        }
    }
}


$null = Register-EngineEvent PowerShell.Exiting -Action { cleanup }


# main
Set_Install_Dir
Set_Version
Set_URL
Install
