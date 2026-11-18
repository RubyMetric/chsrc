# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# Shell File    : completion.sh
# File Authors  : 郭恒 <2085471348@qq.com>
# Contributors  : Nil Null <nil@null.org>
#	              |
# Created On    : <2026-03-19>
# Last Modified : <2026-03-21>
#
# ----------
# chsrc 的 Shell 自动补全脚本（当前仅在 Ubuntu 中测试过）
#   1. Bash 已测试
#   2. Zsh 暂未测试
#
# @issue https://github.com/RubyMetric/chsrc/issues/204
# source completion.sh   # 当前终端立即生效
# ---------------------------------------------------------------

_chsrc() {

    # 初始化部分

    local cur="${COMP_WORDS[COMP_CWORD]}"
    COMPREPLY=()

    ## 定义具体dish
    local dishes_lang="gem ruby rb rubygem rubygems bundler
        python pypi py
        pip
        poetry
        pdm
        rye
        uv
        js javascript node nodejs
        bun
        npm
        yarn
        pnpm
        nvm
        perl cpan
        php composer
        lua luarocks
        go golang goproxy
        rust cargo crate crates
        rustup
        java maven mvn maven-daemon mvnd gradle
        clojure clojars cloj lein
        dart pub
        flutter
        nuget net .net dotnet
        haskell cabal stack hackage
        ocaml opam
        r cran
        julia"

    local dishes_os="ubuntu zorinos
        linuxmint
        debian
        fedora
        opensuse opensuse-leap opensuse-tumbleweed
        kali
        msys2 msys
        arch archlinux
        archlinuxcn archcn
        manjaro
        gentoo
        rocky rockylinux
        alma almalinux
        alpine
        voidlinux
        solus
        trisquel
        linuxlite
        ros ros2
        raspi raspberrypi
        armbian
        openwrt opkg LEDE
        termux
        openkylin
        openeuler
        anolis openanolis
        deepin
        freebsd
        netbsd
        openbsd"

    local dishes_ware="winget
        brew homebrew
        cocoa cocoapods cocoapod
        docker dockerhub
        flatpak flathub
        nix
        guix
        emacs elpa
        latex ctan tex texlive miktex tlmgr mpm
        conda anaconda"

    local all_dishes="$dishes_lang $dishes_os $dishes_ware"
    local options="--dry --scope --ipv6 --english --English --no-color --version --help -h"
    local scope_options="default project user system"
    local commands="help issue issues isue i list ls l measure mea m cesu ce c get g set s reset rest r"

    local non_opts=() word need_scope=0
    local index
    for ((index=1; index<COMP_CWORD; index++)); do
        word="${COMP_WORDS[index]}"
        [[ $word == -- ]] && return 0
        if ((need_scope)); then
            # Bash 默认把 '=' 作为单独的补全词。
            [[ $word != = ]] && need_scope=0
            continue
        fi
        case "$word" in
            --scope) need_scope=1 ;;
            --help|-h|--version) return 0 ;;
            -*) ;;
            *)
                [[ ${#non_opts[@]} -eq 0 && $word == help ]] && return 0
                non_opts+=("$word")
                ;;
        esac
    done

    if ((need_scope)); then
        [[ $cur == = ]] && cur=""
        COMPREPLY=( $(compgen -W "$scope_options" -- "$cur") ) || true
        return 0
    fi
    if [[ $cur == --scope=* ]]; then
        COMPREPLY=( $(compgen -W "--scope=default --scope=project --scope=user --scope=system" -- "$cur") ) || true
        return 0
    fi
    if [[ $cur == -* ]]; then
        COMPREPLY=( $(compgen -W "$options" -- "$cur") ) || true
        return 0
    fi

    local cmd="${non_opts[0]:-}"
    local args_after=${#non_opts[@]}
    if [[ -z $cmd ]]; then
        COMPREPLY=( $(compgen -W "$commands $options" -- "$cur") ) || true
        return 0
    fi

    case "$cmd" in
        list|ls|l)
            if [[ $args_after -eq 1 ]]; then
                COMPREPLY=( $(compgen -W "mirrors mirror dishes dish dishs os lang pl language ware software $all_dishes" -- "$cur") ) || true
            fi
            ;;
        measure|mea|m|cesu|ce|c|get|g|reset|rest|r)
            if [[ $args_after -eq 1 ]]; then
                COMPREPLY=( $(compgen -W "$all_dishes" -- "$cur") ) || true
            fi
            ;;
        set|s)
            if [[ $args_after -eq 1 ]]; then
                COMPREPLY=( $(compgen -W "$all_dishes" -- "$cur") ) || true
            elif [[ $args_after -eq 2 ]]; then
                COMPREPLY=( $(compgen -W "first" -- "$cur") ) || true
            fi
            ;;
    esac
    return 0
}


# 注册, 作用于当前终端
complete -F _chsrc chsrc
