# chsrc Project Rules for AI Assistants

## Project Overview

这是 chsrc 项目，一个用 C 语言编写的跨平台命令行换源工具，帮助用户在不同的镜像之间切换，适用于编程语言、操作系统、其他软件。它的最强大之处在于它是一个框架，能够帮助用户轻松地为不同的目标换源。


## 架构

- **Framework**: 在目录 `src/framework/` 中，包含了核心实现，支持 recipe

  - `chef.c` 里实现了 chef DSL，你可以使用它来确定正确的使用方法
  - `struct.h` 里定义了各种数据结构和宏，这是整个 chsrc 的核心，也是 chef DSL 的核心

- **Recipes**: 在目录 `src/recipe/` 中，包含了针对不同目标的具体实现

  - `lang/` - 编程语言的包管理器 (Python pip, Node.js npm, 等等)
  - `os/`   - 操作系统的包管理器 (Ubuntu apt, Arch pacman, 等等)
  - `ware/` - 软件工具和应用 (Docker, Homebrew, 等等)


## Current State: chef DSL Migration

The project is currently undergoing a major modernization to the "chef DSL" pattern:

### Recipe 新模式

请阅读 `src/recipe/recipe-template.c`, 每一个 recipe 都应该遵循这个模板。每一个文件都应该定义 prelude，在这个函数里

`chef.c` 里实现了 chef DSL，你可以使用它来确定正确的使用方法。

1. 填充维护者信息
2. recipe 元数据
3. recipe 支持的 feature
4. 初始化源信息 (包括 upstream)


### Recipe 旧模式 (To Be Removed):

- File header comments with author/contributor info
- `static Source_t target_sources[]` arrays
- `def_sources_n(target)` macros
- `Feature_t target_feat()` functions
- `def_target_gsf()` style macros

## Coding Guidelines

### When Modernizing Files:
1. **Always preserve important technical comments** - comments about URLs, implementation notes, etc.
2. **Convert metadata properly**:
   - Extract author/contributor info from old headers
   - Preserve creation dates and important update dates
   - Keep technical notes and warnings
3. **Use proper DSL structure**:
   - `def_target()` at top
   - `prelude()` function with all metadata
   - `def_sources_begin/end` for source definitions
4. **Remove completely**:
   - Old file headers with license/author blocks
   - `Feature_t` function definitions
   - `static Source_t` arrays
   - Old macro calls at file end

### C Coding Style:

请阅读 `doc/03-为什么拒绝使用代码格式化工具.md`

### Important Project Concepts:

请阅读 `doc/02-Write-A-Recipe-Even-If-You-Dont-Know-C.md`

## Important: 一定要保持注释，因为它记录了重要的维护信息
