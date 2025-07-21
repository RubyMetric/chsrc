<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : 01-Develop.md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2024-12-27>
 ! Last Modified : <2025-07-21>
 ! ---------------------------------------------------------- -->

# 开发 chsrc

## 依赖与开发环境

请安装好：

  1. `GCC` 或 `Clang`
  2. [just] 或 `make`
  3. `curl`
  4. [rawstr4c] (可选)

**我推荐你使用 VS Code 开发，你可以在一分钟内成功编译、运行和 Debug `chsrc`**

  1. `Ctrl-Shift-B` 直接构建
  2. `F5` 直接开始 Debug

<br>



## 获取代码

**请务必使用 dev 分支开发**

```bash
git clone https://gitee.com/RubyMetric/chsrc.git -b dev
```

关于分支的说明，可参考 [./03-CONTRIBUTING.md](./03-CONTRIBUTING.md)

<br>



## 编译运行

共有四种构建模式：

1. **`DEV mode`**
2. **`DEBUG mode`**
3. **`RELEASE mode`**
4. **`CI-RELEASE mode`**

第四种只在 GitHub Actions 使用，一般只需要前三者；而在开发时你仅需要前两者；如果不需要 Debug，仅仅第一个 **DEV mode** 就够了。

**如果你使用 `just`，可以在 VS Code 中获得更好的体验，按 `Ctrl-Shift-B` 直接使用 DEV mode 构建**

```bash
just (b)      # 在Windows上默认使用 gcc 编译，在macOS上默认使用 clang 编译，在其他系统上默认使用 cc 编译
just CC=clang # 使用 clang 编译
just CC=gcc   # 使用 gcc   编译

# 编译出 debug 版本: chsrc-debug
just bd
# 编译出 release 版本: chsrc-release
just br
```

<br>

`make` 用户:

```bash
make (b)      # 在Windows上默认使用 cc 编译，在macOS上默认使用 clang 编译，在其他系统上默认使用 cc 编译
make CC=clang # 使用 clang 编译
make CC=gcc   # 使用 gcc   编译

# 编译出 debug 版本: chsrc-debug
make bd
# 编译出 release 版本: chsrc-release
make br
```

<br>



## Debug

**如果你使用 `just`，可以在 VS Code 中获得更好的体验，按 F5 即可立即开始 Debug**

```bash
# 重新编译出 ./chsrc-debug，并启动 GDB 调试 (在macOS上启动 LLDB 调试)
$ just debug

# 重新编译出 ./chsrc-debug，并启动 LLDB 调试
$ just DEBUGGER=lldb debug
```

<br>

`make` 用户:

```bash
# 重新编译出 ./chsrc-debug，并启动 GDB 调试 (在macOS上启动 LLDB 调试)
$ make debug

# 重新编译出 ./chsrc-debug，并启动 LLDB 调试
$ make debug DEBUGGER=lldb
```

<br>



## 测试

`just` 用户只需要替换下述 `make` 为 `just`

```bash
make test-xy  # 测试 xy.h
make test-fw  # 测试 framework
make test     # 测试上述两个
make test-cli # 测试命令
make clean
```

<br>



## 提交 PR

关于分支的说明以及如何提交代码，请参考 [./03-CONTRIBUTING.md](./03-CONTRIBUTING.md)

<br>



[rawstr4c]: https://github.com/RubyMetric/rawstr4c
[just]: https://github.com/casey/just
