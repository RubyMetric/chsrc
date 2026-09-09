#!/usr/bin/env zsh
# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# Test File     : zsh-completion-integration.zsh
# Test Authors  : @swim233
# Contributors  : Nil Null <nil@null.org>
#               |
# Created On    : <2026-09-08>
# Last Modified : <2026-09-09>
#
# 在隔离的子 Zsh 中测试真实的 ZLE 补全匹配与插入行为。
# 子进程使用临时 ZDOTDIR 和 compinit -D，不读取或修改用户及系统配置。
#
#    $ zsh -f test/zsh-completion-integration.zsh
# ---------------------------------------------------------------

emulate -L zsh
setopt errexit nounset pipefail extendedglob

zmodload zsh/zpty
zmodload zsh/zselect

typeset test_dir=${0:A:h}
typeset completion_dir=${test_dir:h}/tool/completion
typeset temp_parent=${TMPDIR:-/tmp}
typeset temp_zdotdir
temp_zdotdir=$(mktemp -d "${temp_parent}/chsrc-zsh-completion.XXXXXXXX")
typeset child_started=0

cleanup() {
  if (( child_started )); then
    zpty -d chsrc_completion_shell 2>/dev/null || true
  fi
  if [[ -n ${temp_zdotdir:-} && -d ${temp_zdotdir} &&
        ${temp_zdotdir:t} == chsrc-zsh-completion.* ]]; then
    command rm -r -- "${temp_zdotdir}"
  fi
}
trap cleanup EXIT HUP INT TERM

{
  print -r -- "fpath=(${(q)completion_dir} \${fpath})"
  print -r -- 'autoload -Uz compinit'
  print -r -- 'compinit -D'
  print -r -- "PS1='CHSRC_TEST> '"
  print -r -- "RPROMPT=''"
  print -r -- "bindkey '^I' expand-or-complete"
  print -r -- 'setopt autolist beep'
} > "${temp_zdotdir}/.zshrc"

# 使用 TERM=dumb，避免运行测试的终端影响捕获到的 ZLE 输出。
# 功能较丰富的终端会给自动插入的后缀增加 ANSI 属性，即使补全行为正确，
# 也会导致下面的纯文本断言失败。
zpty -b chsrc_completion_shell env TERM=dumb ZDOTDIR="${temp_zdotdir}" zsh -d
child_started=1

wait_for_output() {
  local needle=$1 chunk=''
  integer attempts=300
  REPLY=''
  while (( attempts-- > 0 )); do
    while zpty -rt chsrc_completion_shell chunk; do
      REPLY+="${chunk}"
      chunk=''
    done
    [[ ${REPLY} == *"${needle}"* ]] && return 0
    zselect -t 1 || true
  done
  print -u2 -r -- "Timed out waiting for ${(qqq)needle}; output was ${(qqq)REPLY}"
  return 1
}

clear_input() {
  zpty -wn chsrc_completion_shell $'\C-u'
}

collect_pending_output() {
  local chunk=''
  zselect -t 5 || true
  while zpty -rt chsrc_completion_shell chunk; do
    REPLY+="${chunk}"
    chunk=''
  done
}

complete_and_expect() {
  local input=$1 expected=$2
  zpty -wn chsrc_completion_shell "${input}"$'\t'
  wait_for_output "${expected}"
  clear_input
}

complete_and_expect 'chsrc set omar' 'chsrc set omarchy '
complete_and_expect 'chsrc s -scope=p' 'chsrc s -scope=project '
complete_and_expect 'chsrc set -scope p' 'chsrc set -scope project '
complete_and_expect 'chsrc -scope=p' 'chsrc -scope=project '
complete_and_expect 'chsrc set ruby fi' 'chsrc set ruby first '
complete_and_expect 'chsrc -en -no-' 'chsrc -en -no-color '

# 镜像占据最后一个位置参数；在后续空参数处按 Tab 不应再次列出
# first/upstream。
zpty -wn chsrc_completion_shell $'chsrc set ruby first \t'
wait_for_output $'\a'
collect_pending_output
if [[ ${REPLY} == *upstream* ]]; then
  print -u2 -r -- "Completion offered a second mirror: ${(qqq)REPLY}"
  exit 1
fi
clear_input

# 在同一个无效位置输入非空内容，也不应被唯一扩展为 upstream。
# 即使候选项样式隐藏了空前缀菜单，该断言仍可捕获重复镜像补全。
zpty -wn chsrc_completion_shell $'chsrc set ruby first u\t'
wait_for_output $'\a'
collect_pending_output
if [[ ${REPLY} == *upstream* ]]; then
  print -u2 -r -- "Completion inserted a second mirror: ${(qqq)REPLY}"
  exit 1
fi
clear_input

print 'Real Zsh completion insertion tests passed.'
