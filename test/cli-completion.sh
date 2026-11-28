#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
source tool/completion/bash_completion.sh

check_completion() {
    local expected=$1
    shift
    COMP_WORDS=(chsrc "$@")
    COMP_CWORD=$((${#COMP_WORDS[@]} - 1))
    _chsrc
    if [[ $expected == EMPTY ]]; then
        [[ ${#COMPREPLY[@]} -eq 0 ]] || { printf 'Unexpected completion: %s\n' "${COMPREPLY[*]}"; exit 1; }
    else
        [[ " ${COMPREPLY[*]} " == *" $expected "* ]] || { printf 'Missing completion %s: %s\n' "$expected" "${COMPREPLY[*]}"; exit 1; }
    fi
}

check_completion project --scope pr
check_completion project --scope = pr
check_completion --scope=project --scope=pr
check_completion npm --scope project set np
check_completion npm --scope=project set np
check_completion npm --scope = project set np
check_completion first set --scope user npm fi
check_completion --dry set npm first --d
check_completion --English -E
check_completion EMPTY set npm -- --d
check_completion EMPTY set npm -- --scope pr
check_completion EMPTY --help --d
check_completion EMPTY --version --d
check_completion EMPTY help --d
check_completion EMPTY -dry
check_completion EMPTY --dry-run
check_completion EMPTY -en
printf 'Bash completion tests passed\n'
