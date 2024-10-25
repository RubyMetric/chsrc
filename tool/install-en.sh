# to avoid the operate system not supporting Chinese output, create an english version
install_dir=""
version="pre"
path_to_executable=""
default_install_path="/usr/local/bin"
binary_name="chsrc"
temp_install_dir=""

# display related info
info() {
  echo "[INFO] $*"
}

# display error message
error() {
  echo -e "[ERROR] $*" >&2
  exit 1
}

# display help information for the chsrc installer,
# including usage instructions and available options.
help() {
  echo "chsrc Installer"
  echo
  echo "Usage: install.sh [-h] [-d <install_directory>] [-v <version>]"
  echo "Options:"
  echo "-h     Display this help information"
  echo "-d     Specify the installation directory, defaults to /usr/local/bin; overwrites old versions if already installed"
  echo "-v     Specify the version number, range is from 0.1.4 to 0.1.9 or 'pre'"
  echo
}

# set install path, if use -d specify, check whether it exists.
set_install_path() {
  if [ -n "$install_dir" ]; then
    # Expand ~ symbol
    install_dir="${install_dir/#\~/$HOME}"

    # Check if the path exists; if not, create it
    if [ ! -d "$install_dir" ]; then
      echo "Directory $install_dir does not exist. Creating..."
      mkdir -p "$install_dir" || { echo "Failed to create directory, please retry"; exit 1; }
      temp_install_dir="$install_dir"  # Record temporary installation directory
    fi
  elif existing_path=$(command -v "$binary_name" 2>/dev/null); then
    info "$binary_name is already installed, updating path: ${existing_path}"
    install_dir=$(dirname "$existing_path")
  else
    # Check default path
    if [ -d "$default_install_path" ] && [ -w "$default_install_path" ]; then
      install_dir="$default_install_path"
    else
      error "Default download path /usr/local/bin is not writable. Please run the script with sudo; or specify another path using the -d option."
    fi
  fi
}

# install specifying arch, os, version chsrc binary file from Gitee Repository
install() {
  arch="$(uname -m | tr '[:upper:]' '[:lower:]')"

  case "$arch" in
    x86_64)  arch="x64" ;;
    aarch64|arm64) arch="aarch64" ;;
    riscv64) arch="riscv64" ;;
    armv7*)  arch="armv7" ;;
    *)       error "Unsupported architecture: ${arch}" ;;
  esac

  platform="$(uname -s | awk '{print tolower($0)}')"

  case "$platform" in
    linux)  platform="linux" ;;
    darwin) platform="macos" ;;
    *)      error "Unsupported platform: ${platform}" ;;
  esac

  if [[ ! "$version" =~ ^(pre|0\.1\.([4-9]))$ ]]; then
      # Version does not meet the criteria, report error
      error "Unsupported version: ${version}. Version number must be between 0.1.4 and 0.1.9 or 'pre'"
  fi

  # generate download url
  url="https://gitee.com/RubyMetric/chsrc/releases/download/${version}/${binary_name}-${arch}-${platform}"

  path_to_executable="${install_dir}/${binary_name}"

  info "Downloading ${binary_name} (${arch} architecture, ${platform} platform, version ${version}) to ${path_to_executable}"

  if curl -sL "$url" -o "$path_to_executable"; then
    chmod +x "$path_to_executable"
    info "🎉 Installation completed, path: $path_to_executable"
  else
    error "Download failed, please check your network connection and proxy settings: ${url}"
  fi
}

# Cleanup function
cleanup() {
  if [ -n "$temp_install_dir" ] && [ -d "$temp_install_dir" ]; then
    echo "Cleaning up created directory: $temp_install_dir"
    rm -rf "$temp_install_dir"
  fi
}

# Set trap to catch exit signals
trap cleanup EXIT

# Parser commandline argvs
while getopts ":hd:v:" option; do
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
    echo "Invalid command line option. Use -h for help"
    exit 1
    ;;
  esac
done

set_install_path
install