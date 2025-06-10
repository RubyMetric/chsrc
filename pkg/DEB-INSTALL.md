# DEB Package Installation

## Installing from Release

1. Download the appropriate DEB package from the [releases page](https://github.com/RubyMetric/chsrc/releases)
2. Install using dpkg:
   ```bash
   sudo dpkg -i chsrc_*.deb
   sudo apt-get install -f  # Fix any dependency issues
   ```

## Building from Source

### Prerequisites

Install the required build dependencies:

```bash
sudo apt-get update
sudo apt-get install build-essential debhelper devscripts fakeroot
```

### Building the Package

1. Clone the repository:
   ```bash
   git clone https://github.com/RubyMetric/chsrc.git
   cd chsrc
   ```

2. Build the DEB package:
   ```bash
   make deb-build
   ```

3. Install the generated package:
   ```bash
   sudo dpkg -i ../chsrc_*.deb
   ```

### Cross-compilation

To build for different architectures:

```bash
# For ARM64
CC=aarch64-linux-gnu-gcc dpkg-buildpackage -us -uc -b -aarm64

# For ARMv7 (armhf)
CC=arm-linux-gnueabihf-gcc dpkg-buildpackage -us -uc -b -aarmhf
```

### Cleaning Build Artifacts

```bash
make deb-clean
```

## Package Information

- **Package Name**: chsrc
- **Architecture**: amd64, arm64, armhf
- **Dependencies**: Standard C library
- **Installation Path**: `/usr/bin/chsrc`
- **Manual Page**: `/usr/share/man/man1/chsrc.1`

## Uninstalling

```bash
sudo apt-get remove chsrc
```

Or completely remove including configuration:

```bash
sudo apt-get purge chsrc
```
