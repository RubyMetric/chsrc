<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : CI.md
 ! Doc Authors   : sanchuanhehe <wyihe5520@gmail.com>
 ! Contributors  :  曾奥然 <ccmywish@qq.com>
 !               |
 ! Created On    : <2025-06-14>
 ! Last Modified : <2025-06-16>
 ! ---------------------------------------------------------- -->

# deb package CI/CD

本文档说明了 chsrc 项目的 deb 包自动构建和发布流程。

## CI 文件

CI 维护者 [@sanchuanhehe](https://github.com/sanchuanhehe)

- [.github/workflows/pkg-deb.yml](../../.github/workflows/pkg-deb.yml)

<br>

## 支持的架构

当前支持以下架构的 deb 包构建：

- `amd64` (x86_64)

<br>

## CI 构建产物

每次 CI 构建会生成：

1. **deb 包文件**: `chsrc_<version>-1_<arch>.deb`
2. **仓库元数据**: `Packages` 和 `Packages.gz` 文件用于创建 APT 仓库

<br>


## 自动触发

deb 包构建 CI 会在以下情况下自动触发：

1. **Push 事件**：当 push 到 `gh-build` 分支时自动构建，并上传 deb 包 到 `pre` 这个特定的 release 中
2. **Release 事件**: 当创建新的 release 时自动构建，并上传 deb 包到最新的这个 release 中
2. **手动触发**: 可以在 GitHub Actions 页面手动触发构建

<br>

## 手动发布流程

1. 确保所有代码已合并到主分支
2. 更新版本号和 changelog
3. 创建并推送 git tag: `git tag v1.2.3 && git push origin v1.2.3`
4. 在 GitHub 上创建 release
5. CI 将自动构建并上传 deb 包

<br>
