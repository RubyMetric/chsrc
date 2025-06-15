<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : BUILD.md
 ! Doc Authors   : sanchuanhehe <wyihe5520@gmail.com>
 ! Contributors  :  Aoran Zeng  <ccmywish@qq.com>
 !               |
 ! Created On    : <2025-06-14>
 ! Last Modified : <2025-06-15>
 ! ---------------------------------------------------------- -->

# 构建 DEB package

## 从源代码构建

### 准备

安装构建所需的依赖：

```bash
sudo apt-get update
sudo apt-get install build-essential debhelper devscripts fakeroot
```

### 构建

```bash
git clone https://github.com/RubyMetric/chsrc.git
cd chsrc

# 执行构建命令
./deb.makefile deb-build

# 安装生成的 .deb 文件
sudo dpkg -i ../chsrc_*.deb
```

<br>

### 交叉编译

为不同架构进行构建：

```bash
# For ARM64
CC=aarch64-linux-gnu-gcc dpkg-buildpackage -us -uc -b -aarm64

# For ARMv7 (armhf)
CC=arm-linux-gnueabihf-gcc dpkg-buildpackage -us -uc -b -aarmhf
```

<br>

### 安装与测试

```bash
# 安装生成的包
sudo dpkg -i ../chsrc_*.deb
sudo apt-get install -f  # 修复依赖问题

# 运行测试
./deb-test.sh

# 卸载
sudo apt-get remove chsrc
```

### 清理构建产物

```bash
./deb.makefile deb-clean
```

<br>

## 卸载

```bash
sudo apt-get remove chsrc
```

包括删除配置在内的完全删除：

```bash
sudo apt-get purge chsrc
```

<br>


## 故障排查

### 常见问题

1. **构建失败**: 检查 debian/control 中的依赖是否正确
2. **交叉编译失败**: 确认目标架构的工具链已正确安装
3. **安装测试失败**: 检查 postinst 脚本是否有错误

### 调试构建

```bash
# 启用详细输出
DEB_BUILD_OPTIONS="nocheck" debuild -us -uc -b

# 检查构建日志
less ../chsrc_*.build

# 检查包内容
dpkg --contents ../chsrc_*.deb
# 或
dpkg-deb --contents ../chsrc_*.deb
```
