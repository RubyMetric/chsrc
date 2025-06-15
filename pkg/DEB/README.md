# DEB Package CI/CD 构建

本文档说明了 chsrc 项目的 DEB 包自动构建和发布流程。

## 自动触发

DEB 包构建 CI 会在以下情况下自动触发：

1. **Release 事件**: 当创建新的 release 时自动构建并上传 DEB 包到 release assets
2. **手动触发**: 可以在 GitHub Actions 页面手动触发构建

## 支持的架构

当前支持以下架构的 DEB 包构建：

- `amd64` (x86_64)

## 构建产物

每次构建会生成：

1. **DEB 包文件**: `chsrc_<version>-1_<arch>.deb`
2. **仓库元数据**: `Packages` 和 `Packages.gz` 文件用于创建 APT 仓库

## 本地测试

### 构建 DEB 包

```bash
# 准备构建环境
sudo apt-get install build-essential debhelper devscripts fakeroot

# 构建包
make deb-build

# 清理构建产物
make deb-clean
```

### 测试安装

```bash
# 安装生成的包
sudo dpkg -i ../chsrc_*.deb
sudo apt-get install -f  # 修复依赖问题

# 运行测试
bash ./pkg/DEB/deb-test.sh

# 卸载
sudo apt-get remove chsrc
```

## 文件结构

```
debian/
├── changelog      # 版本更新日志
├── compat         # debhelper 兼容性版本
├── control        # 包控制信息和依赖
├── copyright      # 版权信息
├── postinst       # 安装后脚本
├── prerm          # 卸载前脚本
└── rules          # 构建规则
```

## 手动发布流程

1. 确保所有代码已合并到主分支
2. 更新版本号和 changelog
3. 创建并推送 git tag: `git tag v1.2.3 && git push origin v1.2.3`
4. 在 GitHub 上创建 release
5. CI 将自动构建并上传 DEB 包

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
dpkg-deb --contents chsrc_*.deb
```

## 相关文件

- `.github/workflows/pkg-deb.yml` - CI 工作流配置
- `debian/` - Debian 包配置目录
- `pkg/DEB/deb-test.sh` - DEB 包功能测试脚本
- `pkg/DEB/INSTALL.md` - 用户安装指南
