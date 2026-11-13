# chsrc — Change Source Everywhere

跨平台换源 CLI 工具与框架。使用 **C11**（推荐 C17 或更高版本）编写，从 `src/chsrc-main.c` 单文件编译（`#include` 汇聚所有框架与 recipe 文件，无中间目标文件）。支持 Linux、Windows（原生/MSYS2/Cygwin）、macOS、BSD、Android。主程序 GPL-3.0-or-later 许可，`lib/xy.h` 为 MIT 许可。

**本项目在 `doc/` 中维护了详细的中文文档，遇到构建、recipe 编写、代码风格等问题时，请优先阅读对应文档，而不要凭空猜测：**

| 主题                             | 文档                                          |
| -------------------------------- | --------------------------------------------- |
| 开发环境、构建（just/make）、Debug、测试 | `doc/01-开发与构建.md`                        |
| 分支策略、提交与贡献流程          | `doc/02-提交与贡献.md`                        |
| 为什么不用代码格式化工具、风格哲学 | `doc/03-为什么拒绝使用代码格式化工具.md`      |
| 如何编写新 recipe（核心概念、步骤、准则） | `doc/10-如何编写recipe.md`                    |
| 如何设置换源链接与测速链接        | `doc/11-如何设置换源链接与测速链接.md`        |
| 协作者与维护者                    | `doc/50-协作者与维护者.md`                    |

## 快速命令

```
just build / bd / br     # DEV / DEBUG / RELEASE 模式构建（推荐原生 Windows）
make build / bd / br     # 同上（Linux/macOS/MSYS2）
just test / test-cli / fastcheck
```

开发与 PR 请使用 `dev` 分支。详细依赖、调试方式见 `doc/01-开发与构建.md`。

## 架构速览

- `src/chsrc-main.c` — 入口：`main()`、CLI 解析、输出显示
- `src/framework/` — 核心骨架：`struct.h`（`Dish_t`/`Source_t`/`MirrorSite_t`/Chef DSL 宏定义）、`base.c`/`OS.c`/`source.c`/`waiter.c`（全局状态、系统与文件操作、测速与命令分派）、`env.c`（跨平台环境变量幂等持久化读写：`chsrc_set_env`/`chsrc_set_env_block`/`chsrc_get_env`）、`chef.c`（Chef DSL 实现）、`helper.c`（`hp_*` 辅助函数）、`mirror.c`（通用镜像站）
- `src/recipe/{lang,os,ware}/` — 按类别（`pl`/`os`/`wr`）组织的 recipe；新 recipe 须在 `src/recipe/menu.c` 中注册（`#include` + `add()`）才会出现在菜单中
- `lib/xy.h` — 独立 C11 工具库（MIT）：字符串、系统检测、日志、数据结构、文件 I/O。约定 `return caller-free` 表示调用方必须释放返回值
- `src/framework/file-update.c` — 通用字节读取与 POSIX 文件更新，在 `OS.c` 之后引入；接收旧快照和目标内容，负责幂等判断、暂存与回滚，不依赖 env 或具体配置格式。接口与配置归属方案见 `doc/12-配置文件更新.md`。
- 执行流程：`main()` → `chsrc_init_framework()` → 解析 CLI → 在 `pl`/`os`/`wr` 菜单中匹配 dish → `preparefn()` → 按操作分派 `getfn`/`setfn`/`resetfn`

概念（`Dish_t`/`Scope_t`/`SourceProvider_t`/`MirrorSite_t`/Chef DSL）与编写步骤详见 `doc/10-如何编写recipe.md`；换源/测速链接设置详见 `doc/11-如何设置换源链接与测速链接.md`。

## 代码风格与命名

- **不使用代码格式化工具**，采用手动排版对齐（原因见 `doc/03-为什么拒绝使用代码格式化工具.md`）
- 保留已有注释；函数名与 `()` 之间留空格，如 `foo ()` 而非 `foo()`
- `pl*`/`os*`/`wr*` 前缀分别对应 `lang`/`os`/`ware` 目录；类型名用 `PascalCase_t`
- Convention over Configuration，NO UFO 原则（不向用户目录写配置/数据文件）

## 内存管理规则

除非不得不，不要调用 `free()`（单次 CLI 运行，进程退出即回收）。仅当指针来自 xy.h 的 caller-free 函数、且生命周期止于函数内部时才释放。避免改动已有 recipe。

## 代码审查清单

检查：指针/边界/内存安全（NULL 解引用、溢出、use-after-free）、逻辑缺陷、竞态条件（工具单线程，标记任何新引入的并发）、死锁、权限问题、C11 标准合规（POSIX 专有 API 需 `#ifdef` 守卫）、注释与代码一致性、冗余代码、错误处理是否充分、未定义行为、跨平台兼容性（Linux/macOS/Windows MinGW）。

改动后至少本地运行 `just test`（或 `make test`）与 `just test-cli`（或 `make test-cli`）；修改 `*.{c,h}` 后运行 `just bd && ./chsrc-debug get <受影响dish>` 确认不崩溃（lefthook 提交时会自动执行同样检查，见 `doc/01-开发与构建.md`）。
