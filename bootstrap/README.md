<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : (chsrc-bootstrap Introduction.md)
 ! Doc Authors   : 曾奥然 <ccmywish@qq.com>
 ! Contributors  : Nul None <nul@none.org>
 !               |
 ! Created On    : <2025-07-12>
 ! Last Modified : <2025-07-22>
 ! ---------------------------------------------------------- -->

# Bootstrap

```ruby
begin
  download_prebuilt_chsrc_binary_for_my_platform
rescue NoReadyMadeBinary => e
  bootstrap! e.my_platform
end
```

<br>



## 预编译产生的死锁问题

我们支持预编译的操作系统目前只有 `Windows`, `Linux`, `macOS`，支持的架构请参考[项目 README](../README.md)

某些操作系统如 BSD 家族，甚至是更小众的操作系统, 或者上述操作系统的某些架构，无法享受直接下载二进制立即使用的便利性，这导致用户需要自己编译 `chsrc`.

然而用户自己编译则面临着一个问题: **需要提前安装项目依赖**

1. 把代码 `git clone` 下来 或 下载 `zip` 包进行解压缩
2. C语言编译器
3. `GNU make` 或 `just` (这二者非强制，但是有了更好)

可是如果用户还没有换源，他/她又如何获得上述这些程序呢？**这是一把死锁，导致用户最终回到手动换源的原始农耕时代。**

<br>



## `chsrc-bootstrap` to the Rescue

`chsrc-bootstrap` 是一组使用原生脚本语言的脚本，每个脚本称为 `bootstrapper`，用来完成两件事情:

1. 帮助用户进行最基本的换源，让用户能够立即开始使用该系统安装其他软件

2. 用户自行决定是否需要安装 `chsrc`

    - 若不需要: 流程直接结束，用户已可以完成基本日常工作

    - 若需要: 帮助用户安装好所需的最少依赖，并编译安装 `chsrc`

注意，最基本的换源，即不测速，由维护者暂时决定先切换到某一个具体的源，先让系统跑起来。

### 支持的语言

上述提到的原生脚本语言只能是这些:

1. Windows: `PowerShell`, `CMD`

2. Linux: `Bash`, `sh`

    备选: `Perl`

3. macOS: `Zsh`, `Bash`, `sh`

    备选: `Ruby`, `Perl`

4. BSD: `sh`

    备选: 待议

5. 其他平台: 一切该平台支持的脚本语言，无需额外安装

<br>



## `chsrc` 与 `chsrc-bootstrap`

`chsrc` 需要实现 `chsrc-bootstrap` 中支持的平台。这是因为，`chsrc-bootstrap` 只换到了某一个具体的源，只是临时性的，后续用户可能还想要换到其他源，此时 `chsrc` 就可以接管。

而事实上，`chsrc-bootstrap` 可以完成一些操作，使得系统已经满足一些前置条件，这样的话 `chsrc` 可以在这些前置条件存在时接着换源，从而大幅减轻 `recipe` 的实现难度。

<br>



## 帮助

1. [FreeBSD 的换源](https://github.com/RubyMetric/chsrc/issues/11) 从项目一开始就是我们的目标之一，然而由于其:

    - 换源所需的步骤太多
    - 面临着特有的鸡蛋困境
    - BSD 工具和 GNU 工具总是有不兼容之处
    - 在C语言中实现困难重重

    因此我们一直没有很好的办法去实现

2. 任何你所在的平台，都可以添加到此
