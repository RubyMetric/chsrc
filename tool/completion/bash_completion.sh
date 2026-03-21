# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    : chsrc_completion.sh
# File Authors  : 郭恒 <2085471348@qq.com>
# Contributors  :   Nil Null   <nil@null.org>
#	              |
# Created On    : <2026-03-19>
# Last Modified : <2026-03-21>
#
# ------------------------------------------------------------------------------
# chsrc 的 Bash 自动补全脚本（当前仅在Linux Ubuntu中测试过）
#
# 目标操作系统：Linux
#
# @issue https://github.com/RubyMetric/chsrc/issues/204
# source chsrc_completion.sh   # 当前终端立即生效
# ------------------------------------------------------------------------------

_chsrc() {

    # 初始化部分

    ## 手动划分字符串
    local line="${COMP_LINE:0:$COMP_POINT}"  # COMP_LINE的从0到光标的所有输入，作为一个字符串
    read -ra words <<< "$line"               # 按IFS重新分词，默认分隔符是" \t\n"
    cword=$(( ${#words[@]} - 1 ))            # 下表从0开始，当前词下标

    ### 如果行末是空格，说明需要补充新的词
    if [[ "${line: -1}" == " " ]]; then
        words+=("")
        cword=$(( ${#words[@]} - 1 ))
    fi

    cur="${words[$cword]}"

    ## 定义具体target
    local targets_lang="gem ruby rb rubygem rubygems bundler
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

    local targets_os="ubuntu zorinos
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

    local targets_ware="winget
        brew homebrew
        cocoa cocoapods cocoapod
        docker dockerhub
        flatpak flathub
        nix
        guix
        emacs elpa
        latex ctan tex texlive miktex tlmgr mpm
        conda anaconda"

    local all_targets="$targets_lang $targets_os $targets_ware"
    local options="-dry -scope= -ipv6 -english -en -no-color -h --help"
    local scope_options="project user system"
    local commands="help issue list ls measure cesu get set reset"

    # 逻辑部分

    # 扫描已输入词，跳过选项，依次填入 cmd / second_arg，并计数
    # 找到第一个非选项词作为子命令
    local non_opts=()
    for w in "${words[@]:1:cword-1}"; do
        [[ $w != -* ]] && non_opts+=("$w")
    done
    # 若有non_opts[0]
    local cmd=${non_opts[0]}

    if [[ -z "$cmd" ]]; then
        COMPREPLY=( $(compgen -W "$commands $options" -- "$cur") )
        return 0
    fi

    local second_arg="${non_opts[1]:-}"
    local args_after=${#non_opts[@]}


    # 补全-scope=project这样的选项时，需要手动控制，先补全-scope=再补全后面的
    # 作为一个函数来实现
    _complete_options() {
        if [[ $cur == -scope=* ]]; then
            # cur="-scope=",匹配
            COMPREPLY=( $(compgen -W "$scope_options" -- "${cur#-scope=}") )
        elif [[ $cur == -* ]]; then
            COMPREPLY=( $(compgen -W "$options" -- "$cur") )
            # 补全 -scope 本身时不加尾部空格，让用户可以继续输入=
            if [[ ${COMPREPLY[0]} == "-scope=" ]]; then
                # 对已经补全的候选项做出调整，补全出-scope=后不跟空格，才能匹配scope_options
                compopt -o nospace
            fi
        fi
    }

    # 主要的补全逻辑
    case "$cmd" in
        help|h|issue|i)
            COMPREPLY=( $(compgen -W "help issue" -- $cur))
            ;;
        list|ls|l)
            if [[ $args_after -eq 1 ]]; then
                COMPREPLY=( $(compgen -W "mirror target os lang ware $all_targets " -- $cur) )
            else
                _complete_options
            fi
            ;;
        measure|m|cesu)
            if [[ $args_after -eq 1 && $cur != -* ]]; then
                COMPREPLY=( $(compgen -W "$all_targets" -- "$cur") )
            else
                _complete_options
            fi
            ;;
        get|g)
            if [[ $args_after -eq 1 && $cur != -* ]]; then
                COMPREPLY=( $(compgen -W "$all_targets" -- "$cur") )
            else
                COMPREPLY=( $(compgen -W "$options" -- $cur) )
            fi
            ;;
        reset)
            if [[ $args_after -eq 1 ]]; then
                COMPREPLY=( $(compgen -W "$all_targets" -- $cur) )
            else
                COMPREPLY=( $(compgen -W "$options" -- $cur) )
            fi
            ;;
        set|s)
            if [[ $args_after -eq 1 && $cur != -* ]]; then
                COMPREPLY=( $(compgen -W "$all_targets" -- "$cur") )
            elif [[ $args_after -eq 2 ]]; then
                COMPREPLY=( $(compgen -W "first" -- $cur) )
            else
                _complete_options
            fi
            ;;
        *)
            _complete_options
            ;;

    esac
    return 0
}


# 注册, 作用于当前终端
complete -F _chsrc chsrc
