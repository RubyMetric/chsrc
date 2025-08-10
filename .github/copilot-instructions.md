# chsrc Project Rules for AI Assistants

## 项目概述

这是 chsrc 项目，一个用 C 语言编写的跨平台命令行换源工具，帮助用户在不同的镜像之间切换，适用于编程语言、操作系统、其他软件。它的最强大之处在于它是一个框架，能够帮助用户轻松地为不同的目标换源。


## 架构

- **Framework**: 在目录 `src/framework/` 中，包含了核心实现，支持 recipe

  - `chef.c` 里实现了 chef DSL，你可以使用它来确定正确的使用方法
  - `struct.h` 里定义了各种数据结构和宏，这是整个 chsrc 的核心，也是 chef DSL 的核心

- **Recipes**: 在目录 `src/recipe/` 中，包含了针对不同目标的具体实现

  - `lang/` - 编程语言的包管理器 (Python pip, Node.js npm, 等等)
  - `os/`   - 操作系统的包管理器 (Ubuntu apt, Arch pacman, 等等)
  - `ware/` - 软件工具和应用 (Docker, Homebrew, 等等)


## 项目当前状态: chef DSL 迁移

项目正在进行现代化改造，从旧模式迁移到新的 "chef DSL" 模式。

### Recipe 新模式

请阅读 `src/recipe/recipe-template.c`, 每一个 recipe 都应该遵循这个模板。每一个文件都应该定义 prelude，在这个函数里

`chef.c` 里实现了 chef DSL，你可以使用它来确定正确的使用方法。

1. 填充维护者信息
2. recipe 元数据
3. recipe 支持的 feature
4. 初始化源信息 (包括 upstream)


### Recipe 旧模式 (To Be Removed):

- 文件头的注释信息
- `static Source_t target_sources[]` 数组
- `def_sources_n(target)` 宏
- `Feature_t target_feat()` 函数
- `def_target_gsf()` 宏

## Coding Guidelines

### When Modernizing Files:

1. **保持重要的技术注释** - 如对 URLs 的注释，实现备注等等

2. **正确转换元数据**:

   - 提取旧头文件中的作者/贡献者信息
   - 保留创建日期和重要更新日期
   - 保留技术备注和警告
   - 请注意新的 Target_t 结构体

3. **使用 chef DSL**:

   - `def_target()` 在文件最开头
   - `prelude()` 函数包含所有元数据
   - `def_sources_begin/end` 用于源定义

4. **Remove completely**:

   - 旧的文件头部包含许可证/作者信息
   - `Feature_t` 函数删除
   - `static Source_t` 数组删除
   - 旧的宏调用在文件末尾删除

### C Coding Style:

请阅读 `doc/03-为什么拒绝使用代码格式化工具.md`

### Important Project Concepts:

请阅读 `doc/10-Write-A-Recipe-Even-If-You-Dont-Know-C.md`

## Important: 一定要保持注释，因为它记录了重要的维护信息
