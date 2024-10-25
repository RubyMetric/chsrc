# ---------------------------------------------------------------
# File          : download-release.sh
# Authors       : GnixAij <gaojiaxing0220@gmail.com>
# Created on    : <2024-10-25>
# Last modified : <2024-10-25>
#
# download:
#
#   Download from GitHub Releases
# ---------------------------------------------------------------

install_dir=""
path_to_executable=""
default_install_path="/usr/local/bin"
binary_name="chsrc"

info() {
    echo "[INFO] $*"
}

error() {
    echo -e "[ERROR] $*" >&2
    exit 1
}

help() {
    # 显示帮助
    echo "chsrc 安装脚本"
    echo
    echo "用法: install.sh [-h] [-d <安装目录>]"
    echo "选项:"
    echo "-h     打印此帮助信息。"
    echo "-d     指定安装目录。默认为 /usr/local/bin；如果已安装，则覆盖旧版本。"
    echo
}

# 解析命令行选项
while getopts ":hd:" option; do
    case $option in
    h)
        help
        exit 0
        ;;
    d)
        install_dir=${OPTARG}
        ;;
    v)
        version=${OPTARG}
        ;;
    \?)
        echo "无效的命令行选项。使用 -h 查看帮助。"
        exit 1
        ;;
    esac
done

# 设置安装路径
set_install_path() {
    if [ -n "$install_dir" ]; then
        # 扩展 ~ 符号
        install_dir="${install_dir/#\~/$HOME}"
    elif existing_path=$(command -v "$binary_name" 2>/dev/null); then
        info "$binary_name 已安装，更新路径: ${existing_path}"
        install_dir=$(dirname "$existing_path")
    else
        # 检查默认路径
        if [ -d "$default_install_path" ] && [ -w "$default_install_path" ]; then
            install_dir="$default_install_path"
        else
            error "默认下载路径/usr/local/bin 不可写，请使用 sudo 命令运行脚本；或通过-v参数指定其它路径安装"
        fi
    fi
}

# 下载并安装函数
install() {
    arch="$(uname -m | tr '[:upper:]' '[:lower:]')"
    case "$arch" in
    x86_64) arch="x64" ;;
    aarch64) arch="aarch64" ;;
    riscv64) arch="riscv64" ;;
    armv7*) arch="armv7" ;;
    *) error "不支持的架构: ${arch}" ;;
    esac

    platform="$(uname -s | awk '{print tolower($0)}')"
    case "$platform" in
    linux) platform="linux" ;;
    darwin) platform="macos" ;;
    *) error "不支持的平台: ${platform}" ;;
    esac

    url="https://gitee.com/RubyMetric/chsrc/releases/download/pre/${binary_name}-${arch}-${platform}"

    path_to_executable="${install_dir}/${binary_name}"

    info "下载 ${binary_name} (${arch} 架构, ${platform} 平台) 到 ${path_to_executable}"

    # 下载文件并设置权限
    if curl -sL "$url" -o "$path_to_executable"; then
        chmod +x "$path_to_executable"
        info "🎉 安装完成，路径: $path_to_executable"
    else
        error "下载失败，请检查您的网络连接和代理设置: ${url}"
    fi
}

set_install_path
install
