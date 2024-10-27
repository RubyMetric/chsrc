# ---------------------------------------------------------------
# File Name     : install.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
#               |   ChatGPT  <https://chatgpt.com/>
# Created On    : <2024-10-26>
# Last Modified : <2024-10-27>
#
#
#         chsrc installer for Windows
#
# ---------------------------------------------------------------
# 定义参数
param(
    [switch]
    $h,
    $d = "${Home}\.chsrc\bin", 
    $v = "pre"
)
$fileName = "\chsrc.exe"
$default_path = "${Home}\.chsrc\bin"  
$binary_name = "chsrc"
$platform = "Windows"

$global:path = ""                            
$global:arch = ""                            
$global:version = ""                         
$global:url = ""                             
$global:flag = 0

# 安装说明的多行字符串
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
    exit  # 退出脚本
}

function Help {
    Write-Host 
@"
chsrc-installer: Install chsrc on ${platform}.

Usage: install.sh [options]
Options:
    -h              Print this help information.
    -d <directory>  Specify installation directory, default is $default_path.
    -v <version>    Specify chsrc version.

"@
}

# 执行帮助函数
if ($h) {
    Help
    exit
}

function Get_Path {
    # 检查目录是否存在
    if (-not (Test-Path -Path $d -PathType Container)) {
        # 如果目录不存在，执行下面的代码块
        try {
            New-Item -Path $d -ItemType Directory -Force | Out-Null
            Write-Host "Directory created: $d"
            $global:flag = 1
        } catch {
            # 捕获异常并输出错误信息
            Write-Host "Failed to create directory: $_"
            exit 1
        }
    }
    $global:path=$d
    # 输出最终路径
    Write-Output "The path is set to: $global:path"
}

function Get_Version {
    # 定义有效的版本
    $pattern = '^(0\.1\.[4-9]|pre)$'

    # 检查版本号是否符合
    if ($v -notmatch $pattern) {
        # 输出错误信息并结束程序
        Write-Host "Error: Invalid version '$v'."
        Write-Host "Please provide a valid version (0.1.4 - 0.1.9 or 'pre')."
        exit 1
    }

    # 设置版本号
    $global:version=$v
    Write-Host "Version: $global:version"
}

function Get_Url {
    # 获取 CPU 型号
    $cpuArchitecture = Get-WmiObject Win32_Processor `
                        | Select-Object -First 1 -ExpandProperty Architecture

    # 将 CPU 型号转换为 x64 或 x86
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
            Write-Host "Error: Unsupported architecture '$cpuArchitecture'."
            Write-Host "Only x86 or x64 architectures are supported."
            exit 1
        }
    }
    Write-Host "CPU Architecture: $global:arch"

    # Set URL
    $global:url =  "https://gitee.com/RubyMetric/chsrc/releases/download/" + `
                "v" + "${global:version}/chsrc-${global:arch}-windows.exe"

    Write-Host "DownLoad URL: $global:url."
}

function DownLoad {
    try {
        # 设置安全协议为 TLS 1.2
        [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
        
        # 检查 URL 是否可访问
        $response = Invoke-WebRequest -Uri $global:url -Method Head -ErrorAction Stop
        
        # 检查状态码是否为 200
        if ($response.StatusCode -ne 200) {
            Write-Host "Error: Unable to access $global:url. Status code: $($response.StatusCode)"
            exit 1  # 状态码不为 200，退出
        }
    }
    catch {
        Write-Host "Unable to download ${binary_name}. Please check your internet connection."
        exit 1  # 下载失败，输出错误信息并退出
    }

    # 执行下载
    try {
        Invoke-WebRequest -OutFile ($global:path + $fileName) -Uri $global:url -ErrorAction Stop
        Write-Host "Downloading $binary_name ($global:arch architecture, $platform platform, version $global:version) to $global:path"
        Write-Host "🎉 Installation completed, path: $global:path"
    } catch {
        Write-Host "Error: Unable to download $binary_name. Error: $_"
        exit 1  # 下载失败，输出错误信息并退出
    }
}

# 定义清理函数
function Cleanup {
    if ($flag -eq 1) {
        if (Test-Path -Path $path) {
            Remove-Item -Path $path -Recurse -Force  # 删除路径及其内容
            Write-Host "Deleted the path: $path"
        }
    }
}

# 注册退出事件
$null = Register-EngineEvent PowerShell.Exiting -Action { Cleanup }

# 下载chsrc

Get_Path 
Get_Version
Get_Url
DownLoad
