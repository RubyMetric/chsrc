# chsrc — Change Source Everywhere

跨平台换源 CLI 工具与框架。使用 **C11**（推荐 C17 或更高版本）编写，从 `src/chsrc-main.c` 单文件编译。支持 Linux、Windows（原生/MSYS2/Cygwin）、macOS、BSD、Android。主程序采用 GPL-3.0-or-later 许可，`lib/xy.h` 采用 MIT 许可。

## 构建与测试

单一入口：`src/chsrc-main.c` 通过 `#include` 引入所有框架和 recipe 文件。无需构建编排工具。

```
# 构建 (just — 推荐用于原生 Windows)
just build            # DEV 模式 → chsrc.exe
just bd               # DEBUG 模式 (-DXY_DEBUG) → chsrc-debug.exe
just br               # RELEASE 模式 (-O2) → chsrc-release.exe

# 构建 (make — Linux/macOS/MSYS2)
make build / bd / br
make STATIC=1 br      # 静态链接

# 调试 / 清理
just debug            # 构建 DEBUG 版并启动调试器 (gdb/lldb)
just clean            # 清理构建产物

# 测试
just test             # xy.h + framework 单元测试
just test-cli         # CLI 集成测试 (Perl)
just fastcheck        # CLI 快速检查 (test/cli.pl fastcheck)
```

DEBUG 模式（`-DXY_DEBUG`）会启用诊断功能：输出 `chsrc_debug*` 日志和 `chef_debug_target()` 调试信息，并在每次 Get/Set/Reset 操作后重跑所有 `_prelude()` 以验证其可重复执行且不会崩溃（见 `src/chsrc-main.c` 中的 `chsrc_perform_all_prelude()`）。

原生 Windows 上 `just br` 会先用 `windres` 编译 `src/resource/chsrc.rc` 生成 `chsrc.res` 再链接。开发与 PR 请使用 `dev` 分支（CI 仅在 PR 目标为 `dev` 时运行测试，见 `doc/01-开发与构建.md`）。

## 架构

### 单文件编译

所有代码通过 `#include` 汇聚到 `src/chsrc-main.c`。整个编译过程不产生中间目标文件，也无额外链接步骤。

### 目录结构

| 路径                           | 职责                                                                                                                          |
| ------------------------------ | ----------------------------------------------------------------------------------------------------------------------------- |
| `src/chsrc-main.c`             | 入口点：`main()`、CLI 解析、输出显示                                                                                          |
| `src/framework/`               | 核心：全局状态、Chef DSL、类型定义、镜像站注册、辅助工具                                                                      |
| `src/framework/struct.h`       | `Target_t`、`Source_t`、`MirrorSite_t`、Chef DSL 宏 — **chsrc 的骨架**                                                        |
| `src/framework/core.c`         | 框架核心：全局状态、C11 版本检查、速度测量（curl）、CLI 辅助                                                                  |
| `src/framework/chef.c`         | Chef DSL 实现，供 recipe 作者使用                                                                                             |
| `src/framework/helper.c`       | `hp_*` 通用辅助函数（URL 判断、字符串处理等），供 recipe 与 framework 使用                                                    |
| `src/framework/mirror.c`       | 通用镜像站定义与测速链接（TUNA、BFSU 等教育网镜像）                                                                           |
| `src/framework/version.h`      | 版本宏（`Chsrc_Version` 等），发布新版本前修改                                                                                |
| `src/recipe/lang/`             | 编程语言 recipe：按语言分子目录（`Ruby/`、`Python/`、`JavaScript/`、`Rust/`、`Dart/`）加平铺文件（Go、Java、Perl、PHP 等）    |
| `src/recipe/os/`               | 操作系统 recipe：按发行版分子目录（`APT/`、`BSD/`、`pacman/`、`YUM/`）加平铺文件（AlmaLinux、Alpine、Fedora 等）              |
| `src/recipe/ware/`             | 软件 recipe：按工具分子目录（`Anaconda/`、`Docker/`、`Homebrew/`）加平铺文件（CocoaPods、Flatpak、Nix 等）                    |
| `src/recipe/*/rawstr4c.h`      | 各 recipe 子目录内使用的原始字符串资源（如 `src/recipe/lang/rawstr4c.h` 供 lang/ 下的 recipe 共享）；文档见 `src/rawstr4c.md` |
| `src/recipe/*/common.h`        | 同一 recipe 子目录内共享的公共头文件（如 `lang/Python/common.h`、`os/APT/common.h`）                                          |
| `src/recipe/menu.c`            | 注册表，将所有 recipe 接入三个分类列表（`pl`、`os`、`wr`）                                                                    |
| `src/recipe/recipe-template.c` | 新 recipe 作者的模板                                                                                                          |
| `lib/xy.h`                     | 独立的 C11 工具库（MIT）：字符串、操作系统检测、日志、数据结构、文件 I/O                                                      |
| `src/resource/`                | Windows 资源文件（`chsrc.rc`、`logo.ico`），RELEASE 构建时由 `windres` 编译                                                   |
| `include/`                     | 空占位目录（仅 `.keep`），配合构建参数 `-Iinclude -Ilib`                                                                      |
| `test/`                        | `xy.c`（xy.h 测试）、`fw.c`（framework 测试）、`cli.pl`（CLI 集成测试，Perl）                                                 |
| `doc/`                         | 开发者和用户文档（中文）                                                                                                      |

### 核心抽象

- **`Target_t`** — 换源目标。字段：别名（`aliases`）、函数指针（`getfn`/`setfn`/`resetfn`/`preludefn`）、`inited` 标志、源列表（`sources`/`sources_n`）、特性（`can_english`、`can_user_define`）、作用域（`scope_caps[]`、`default_scope`）、`note`、维护信息（`created_on`/`last_updated`）、贡献者列表（`chefs`/`sauciers`）。
- **`Scope_t`** — 作用域：`ProjectScope`/`UserScope`/`SystemScope`，外加特殊的 `ImplementationDefinedScope`（由 recipe 按实际情况决定；多数 recipe 的 `default_scope` 设为它）。
- **`SourceProvider_t` / `MirrorSite_t`** — 镜像站或上游源提供者。类型：`IS_GeneralMirrorSite`（通用镜像站）、`IS_DedicatedMirrorSite`（专用镜像站）、`IS_UpstreamProvider`（上游默认源）、`IS_UserDefinedProvider`（用户自定义源）。
- **Chef DSL** — 宏与函数的混合 API，供 recipe 作者使用：
  - 宏：`def_target()`、`chef_prep_this()`、`chef_allow_gsr()/s()/sr()/gs()/NOOP()`、`def_sources_begin()/end()`、`chsrc_use_this_source()`。
  - 函数：贡献者 `chef_set_chefs()`/`chef_set_sauciers()`；作用域 `chef_set_scope_cap()`/`chef_set_default_scope()`；换源与测速链接 `chef_set_repoURL()`、`chef_set_smURL()`、`chef_set_smURL_with_postfix()`、`chef_set_smURL_with_func()`、`chef_set_provider_smURL()`；特性 `chef_allow_english()`/`chef_deny_english()`、`chef_allow_user_define()`/`chef_deny_user_define()`；其他 `chef_set_note()`、`chef_use_other_target_sources()`、`chef_verify_contributor()`。
- **xy.h** — 跨平台运行时：`xy_on_windows`、字符串操作（`xy_str_gsub`、`xy_2strcat`）、`XySeq_t`（链表）、`XyMap_t`（哈希表）、命令执行、文件 I/O。内存约定：`return caller-free` 表示调用方必须释放返回值。

### 执行流程

1. `main()` → `chsrc_init_framework()` → `chsrc_init_menu()`（填充 target 列表），并调用 `chsrc_register_contributors()` 把所有贡献者登记到代码中
2. 解析 CLI → 在 `pl`/`os`/`wr` 三个菜单中搜索匹配的 target → 调用 `preludefn()` → 按 `TargetOp`（Get/Set/Reset/Measure/List_Config）分派到 `getfn`/`setfn`/`resetfn` 等
3. 执行 `set` 时：通过系统 `curl` 自动测量镜像站速度（精准/粗略测速，见 `doc/11-如何设置换源链接与测速链接.md`），选择最快的，recipe 的 `_setsrc()` 执行换源

### Recipe 模式

每个 target 由 `.c` 文件实现（一个文件可包含多个 target），包含：

- `_prelude()` — **必需。** 通过 Chef DSL 初始化元数据和源列表。
- `_setsrc()` — **必需。** 执行换源操作。必须调用 `chsrc_use_this_source (target)` 注入选中的源。
- `_getsrc()` / `_resetsrc()` — 可选。

新 recipe 还必须在 `src/recipe/menu.c` 中注册（`#include` 该文件，然后用 `add()` 将其加入对应的 `pl`/`os`/`wr` 列表）；未注册的 recipe 文件不会出现在任何菜单中。

参考：`src/recipe/recipe-template.c`、`doc/10-如何编写recipe.md`、`doc/11-如何设置换源链接与测速链接.md`。

### 开发辅助与质量保障

- **Lefthook git hooks**（`lefthook.yml`）— 提交前自动验证：
  - 修改 `*.{c,h}`（不含 `test/`）：`just build-in-debug-mode` + `./chsrc-debug get pip`（触发所有 `_prelude()` 检查）
  - 修改 `lib/xy.h` 或 `test/xy.c`：`just test-xy`
  - 修改 `src/framework/*`、`src/chsrc-main.c` 或 `test/fw.c`：`just test-fw`
- **CI**（`.github/workflows/`）— 多平台构建：Linux（x64/AArch64/ARMv7/riscv64）、macOS、Windows（x64/Arm64）；PR 时 Ubuntu 运行 `make` + `make test`、Windows 运行 `just` + `just test`（`PR-test.yml`，仅 PR 目标分支为 `dev` 时触发）
- **参考文档**：`doc/README.md`、`doc/01-开发与构建.md`、`doc/02-提交与贡献.md`、`doc/50-协作者与维护者.md`

### 命名约定

- `pl*` = programming **l**anguage（编程语言）→ `src/recipe/lang/`
- `os*` = **o**perating **s**ystem（操作系统）→ `src/recipe/os/`
- `wr*` = soft**w**a**r**e（软件）→ `src/recipe/ware/`
- 类型名：`PascalCase_t`
- 函数名与 `()` 之间必须有空格 — 代码应写 `foo ()` 而非 `foo()`。（本文档中的 API 提及如 `_prelude()` 仅为简写。）

## 代码风格

- 基于 GNU 风格并做了项目特化调整（见 `doc/03-为什么拒绝使用代码格式化工具.md`）。
- **不使用代码格式化工具。** 采用刻意的手动排版（对齐、预处理器缩进等）。
- **保留已有注释** — 它们记录了重要的维护元数据。
- 约定优于配置。NO UFO 原则：工具不会在用户目录下产生任何配置/数据文件。
- 贡献者通过 `src/chsrc-main.c` 中的 `chef_register_contributor()` 注册到代码中，而非仅记录在 git 历史里。

## 内存管理规则

**Recipe 层（`src/recipe/**`）：**
除非不得不，否则不要调用 `free()`。单次调用的 CLI 模型意味着操作系统在退出时回收内存，释放短生命周期的临时对象收益过低，且易导致二次释放等额外问题。仅当指针由 xy.h 的 caller-free 函数堆分配、且生命周期止于 recipe 函数内部时才 `free()`（如 `Cargo.c`释放`cargo_config_file` 路径）。避免改动已有 recipe —— 保持工作代码不变。

**Framework 层（`lib/xy.h`、`src/framework/**`）：**
严格检查所有内存分配是否泄漏。xy.h 中标记为 `return caller-free` 的函数要求调用方释放返回值。`xy_malloc0`、`xy_strdup`、`xy_str_gsub`、`xy_strcat`、`xy_2strcat`、`xy_file_read` 的每个返回结果都必须被释放或移交所有权。

## 代码审查清单

进行代码审查时，检查以下各项：

1. **指针、边界与内存** — NULL 解引用、缓冲区溢出、use-after-free。
2. **逻辑缺陷与结构设计** — 错误假设、脆弱的耦合、缺失的边界情况。
3. **竞态条件** — 工具为单线程；标记任何引入的并发。
4. **死锁** — 同上，标记任何引入的锁。
5. **权限问题** — 文件/目录访问权限、特权要求。
6. **标准合规** — 目标标准为 C11（推荐 C17 或更高版本，见开头说明）。POSIX 专有 API 必须有 `#ifdef` 守卫。
7. **注释与代码不匹配** — 注释必须准确描述其所文档化的代码。
8. **冗余代码** — 死代码、不可达分支、重复逻辑。
9. **不充分的错误处理** — 静默失败、对可能失败的操作缺少 NULL 检查。
10. **未定义行为** — 整数溢出、越界访问、使用未初始化内存、严格别名违反。
11. **兼容性** — 必须能在 Linux（GCC/Clang）、macOS（Clang）、Windows（MinGW）上编译。不得使用 C11 不兼容的构造。
12. **构建与测试验证** — 改动后至少本地运行 `just test`（或 `make test`）与 `just test-cli`（或 `make test-cli`）；修改 `*.{c,h}` 后运行 `just bd && ./chsrc-debug get <受影响target>` 确认不崩溃（lefthook 提交时会自动执行同样检查）。
