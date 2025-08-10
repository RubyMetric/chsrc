# chsrc Project Rules for AI Assistants

## 项目概述

这是 chsrc 项目，一个用 C 语言编写的跨平台命令行换源工具，帮助用户在不同的镜像之间切换，适用于编程语言、操作系统、其他软件。它的最强大之处在于它是一个框架，能够帮助用户轻松地为不同的目标换源。


## 架构

- **Framework**: 在目录 `src/framework/` 中，包含了核心实现，支持 recipe

  - `struct.h` 里定义了各种数据结构和宏，这是整个 chsrc 的核心，也是 chef DSL 的核心
  - `chef.c` 里实现了 chef DSL，你可以使用它来确定正确的使用方法

- **Recipes**: 在目录 `src/recipe/` 中，包含了针对不同目标的具体实现

  - `lang/` - 编程语言 (Ruby, JavaScript 等等)
  - `os/`   - 操作系统 (Ubuntu, Arch Linux 等等)
  - `ware/` - 软件工具和应用 (Docker, Homebrew 等等)


## Coding Guidelines

### C Coding Style:

请阅读 `doc/03-为什么拒绝使用代码格式化工具.md`

### Important Project Concepts:

请阅读 `doc/10-如何编写recipe.md`

## Important: 一定要保持注释，因为它记录了重要的维护信息
