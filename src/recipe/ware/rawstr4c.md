# rawstr4c input

- prefix = `_rawstr`
- postfix = `:use-language`

- translate = `:escape`
- output = `terminal`


## Homebrew

### Bash config

- keep-prefix = `true`
- keep-postfix = `true`
- name = `config`
- name-literally = `false`

最终输出变量将会成为 `_rawstr4c_config_in_bash`

```bash
# ------ chsrc BLOCK BEGIN for Homebrew ------
export HOMEBREW_BREW_GIT_REMOTE="@1@/git/homebrew/brew.git"
export HOMEBREW_CORE_GIT_REMOTE="@1@/git/homebrew/homebrew-core.git"
# for bottles
export HOMEBREW_API_DOMAIN="@1@/homebrew-bottles/api"
export HOMEBREW_BOTTLE_DOMAIN="@1@/homebrew-bottles"
# ------ chsrc BLOCK ENDIN for Homebrew ------
```

### Fish config

- language = `fish`

```bash
# ------ chsrc BLOCK BEGIN for Homebrew ------
set -x HOMEBREW_BREW_GIT_REMOTE "@1@/git/homebrew/brew.git"
set -x HOMEBREW_CORE_GIT_REMOTE "@1@/git/homebrew/homebrew-core.git"
# for bottles
set -x HOMEBREW_API_DOMAIN    "@1@/homebrew-bottles/api"
set -x HOMEBREW_BOTTLE_DOMAIN "@1@/homebrew-bottles"
# ------ chsrc BLOCK ENDIN for Homebrew ------
```
