# rawstr4c input

该文件尽可能测试 `rawstr4c` 的多个功能，以下三个变量的输出，将分别为:

1. `RAWSTR_wr_homebrew_read_config_cmd`
2. `RAWSTR_wr_homebrew_config_in_bash`
3. `RAWSTR_wr_homebrew_config_in_fish`

若不同，则生成有问题。

- prefix = `RAWSTR_wr_homebrew`
- postfix = `:use-language`

- translate = `:escape`
- output = `:global-variable-only-header`

## Homebrew

### read user env

下面的命令对于 `Bash`、`Zsh` 和 `Fish` 都适用

- name = `RAWSTR_wr_homebrew_read_config_cmd`
- name-literally = `true`

测试一下 name-literally 功能

```bash
echo HOMEBREW_API_DOMAIN=$HOMEBREW_API_DOMAIN;
echo HOMEBREW_BOTTLE_DOMAIN=$HOMEBREW_BOTTLE_DOMAIN;
echo HOMEBREW_BREW_GIT_REMOTE=$HOMEBREW_BREW_GIT_REMOTE;
echo HOMEBREW_CORE_GIT_REMOTE=$HOMEBREW_CORE_GIT_REMOTE;
```

### Bash config

- keep-prefix = `true`
- keep-postfix = `false`
- name = `config_in_bash`

测试一下 keep-postfix 功能

```bash
# ------ chsrc BLOCK BEGIN for Homebrew ------
export HOMEBREW_BREW_GIT_REMOTE="@1@/git/homebrew/brew.git"
export HOMEBREW_CORE_GIT_REMOTE="@1@/git/homebrew/homebrew-core.git"
# For Bottles
export HOMEBREW_API_DOMAIN="@1@/homebrew-bottles/api"
export HOMEBREW_BOTTLE_DOMAIN="@1@/homebrew-bottles"
# ------ chsrc BLOCK ENDIN for Homebrew ------
```

### Fish config

- name = `config`
- language = `fish`

`Fish` 没有高亮，所以我们这里的 code block 用 `bash` 指示 ，我们刚好测试一下 language 功能

```bash
# ------ chsrc BLOCK BEGIN for Homebrew ------
set -x HOMEBREW_BREW_GIT_REMOTE "@1@/git/homebrew/brew.git"
set -x HOMEBREW_CORE_GIT_REMOTE "@1@/git/homebrew/homebrew-core.git"
# For Bottles
set -x HOMEBREW_API_DOMAIN    "@1@/homebrew-bottles/api"
set -x HOMEBREW_BOTTLE_DOMAIN "@1@/homebrew-bottles"
# ------ chsrc BLOCK ENDIN for Homebrew ------
```
