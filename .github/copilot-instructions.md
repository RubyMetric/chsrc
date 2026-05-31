# chsrc — Change Source Everywhere

Cross-platform CLI tool and framework for switching software repository mirrors. Written in **C11** (recommend C17+), single-file compilation from `src/chsrc-main.c`. Targets Linux, Windows (native/MSYS2/Cygwin), macOS, BSD, Android. Licensed GPL-3.0-or-later (main) and MIT (`lib/xy.h`).

## Build & Test

Single entry point: `src/chsrc-main.c` `#include`s all framework and recipe files. No build orchestration needed.

```
# Build (just — recommended for native Windows)
just build            # DEV mode → chsrc.exe
just bd               # DEBUG mode (-DXY_DEBUG) → chsrc-debug.exe
just br               # RELEASE mode (-O2) → chsrc-release.exe

# Build (make — Linux/macOS/MSYS2)
make build / bd / br
make STATIC=1 br      # static linking

# Test
just test             # xy.h + framework unit tests
just test-cli         # CLI integration tests (Perl)
```

DEBUG mode (`-DXY_DEBUG`) enables runtime validation — all `_prelude()` functions are called after every recipe operation to verify integrity.

## Architecture

### Single-file compilation

Everything funnels through `src/chsrc-main.c` via `#include`. No object files, no link step beyond the initial compilation.

### Directory layout

| Path                           | Role                                                                                            |
| ------------------------------ | ----------------------------------------------------------------------------------------------- |
| `src/chsrc-main.c`             | Entry point: `main()`, CLI parsing, display                                                     |
| `src/framework/`               | Core: global state, Chef DSL, type definitions, mirror registry, helpers                        |
| `src/framework/struct.h`       | `Target_t`, `Source_t`, `MirrorSite_t`, Chef DSL macros — **the backbone of chsrc**             |
| `src/framework/chef.c`         | Chef DSL implementation for recipe authors                                                      |
| `src/recipe/lang/`             | Programming language recipes (Ruby, Python, Rust, Go, JS, …)                                    |
| `src/recipe/os/`               | OS recipes (APT/Debian, YUM/Fedora, pacman/Arch, BSD, …)                                        |
| `src/recipe/ware/`             | Software recipes (Homebrew, Docker, Flatpak, …)                                                 |
| `src/recipe/menu.c`            | Registry wiring all recipes into three category lists (`pl`, `os`, `wr`)                        |
| `src/recipe/recipe-template.c` | Template for new recipe authors                                                                 |
| `lib/xy.h`                     | Standalone C11 utility library (MIT): strings, OS detection, logging, data structures, file I/O |
| `test/`                        | `xy.c` (xy.h tests), `fw.c` (framework tests), `cli.pl` (CLI integration)                       |
| `doc/`                         | Developer and user documentation (Chinese)                                                      |

### Core abstractions

- **`Target_t`** — A change-source target. Fields: aliases, function pointers (`getfn`, `setfn`, `resetfn`, `preludefn`), source list, scope capabilities, contributor metadata.
- **`SourceProvider_t` / `MirrorSite_t`** — Mirror or upstream provider. Types: `IS_GeneralMirrorSite`, `IS_DedicatedMirrorSite`, `IS_UpstreamProvider`, `IS_UserDefinedProvider`.
- **Chef DSL** — Macro-based API: `chef_prep_this()`, `chef_set_chef/cooks/sauciers()`, `chef_set_scope_cap()`, `def_sources_begin()/end()`, `chef_set_smURL()`, `chef_set_smURL_with_postfix()`, etc.
- **xy.h** — Cross-platform runtime: `xy_on_windows`, string ops (`xy_str_gsub`, `xy_2strcat`), `XySeq_t` (linked list), `XyMap_t` (hash map), command execution, file I/O. Memory convention: `return caller-free` means caller must free.

### Execution flow

1. `main()` → `chsrc_init_framework()` → `chsrc_init_menu()` (populate targets)
2. Parse CLI → search menus for matching target → call `preludefn()` → dispatch to `getfn`/`setfn`/`resetfn`
3. For `set`: auto-measure mirror speeds via `curl`, select fastest, recipe's `_setsrc()` performs the change

### Recipe pattern

Each target is a `.c` file with:

- `_prelude()` — **Required.** Initializes metadata and sources via Chef DSL.
- `_setsrc()` — **Required.** Performs the source change. Must call `chsrc_use_this_source(target)` to inject the selected source.
- `_getsrc()` / `_resetsrc()` — Optional.

Reference: `src/recipe/recipe-template.c`, `doc/10-如何编写recipe.md`, `doc/11-如何设置换源链接与测速链接.md`.

### Naming conventions

- `pl*` = programming **l**anguage → `src/recipe/lang/`
- `os*` = **o**perating **s**ystem → `src/recipe/os/`
- `wr*` = soft**w**a**r**e → `src/recipe/ware/`
- Type names: `PascalCase_t`
- Space between function name and `()` in definitions and calls

## Code Style

- Based on GNU style with project-specific refinements (see `doc/03-为什么拒绝使用代码格式化工具.md`).
- **No code formatters.** Deliberate manual formatting (alignment, preprocessor indentation, etc.).
- **Preserve existing comments** — they record important maintenance metadata.
- Convention over Configuration. NO UFO principle: the tool produces no config/data files in user directories.
- Contributors are registered in code via `chef_register_contributor()` in `src/chsrc-main.c`, not just in git history.

## Memory Management Rules

**Recipes (`src/recipe/**`):**
Do NOT use `free()`in recipe code. The project's single-invocation CLI model means the OS reclaims memory on exit. Adding`free()`to recipes creates noise without benefit. Code review should not flag missing`free()` in recipe files.

**Framework (`lib/xy.h`, `src/framework/**`):**
Strictly check all memory allocations for leaks. Functions marked `return caller-free`in xy.h require callers to free the returned pointer. Every`xy_malloc0`, `xy_strdup`, `xy_str_gsub`, `xy_strcat`, `xy_2strcat`, `xy_file_read` result must be freed or returned to a caller that takes ownership.

## Code Review Checklist

When reviewing changes, check:

1. **Pointers, bounds & memory** — NULL dereferences, buffer overflows, use-after-free.
2. **Logic flaws & structural design** — Incorrect assumptions, fragile coupling, missing edge cases.
3. **Race conditions** — The tool is single-threaded; flag any introduced concurrency.
4. **Deadlocks** — Similarly, flag any introduced locking.
5. **Permission issues** — File/directory access, privilege requirements.
6. **Deprecated functions or standards** — Must target C11 (prefer C17). No POSIX-only APIs without `#ifdef` guards.
7. **Comment-code mismatch** — Comments must accurately describe the code they document.
8. **Redundant code** — Dead code, unreachable branches, duplicate logic.
9. **Inadequate error handling** — Silent failures, missing NULL checks on fallible operations.
10. **Undefined behavior** — Integer overflow, out-of-bounds access, use of uninitialized memory, strict aliasing violations.
11. **Compatibility** — Must compile on Linux (GCC/Clang), macOS (Clang), Windows (MinGW). No C11-incompatible constructs.
