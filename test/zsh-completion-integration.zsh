#!/usr/bin/env zsh
# -----------------------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Exercise real ZLE completion matching and insertion in an isolated child Zsh.
# The child uses a temporary ZDOTDIR and compinit -D, so neither user nor system
# completion configuration is read or written.
# -----------------------------------------------------------------------------

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

# Keep the captured ZLE transcript independent of the terminal running the
# tests.  Rich terminals add ANSI attributes around the automatically inserted
# suffix, which breaks the plain-text assertions below even when completion is
# correct.
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

# A completed mirror occupies the final positional slot.  Pressing Tab in the
# next empty slot must not list first/upstream again.
zpty -wn chsrc_completion_shell $'chsrc set ruby first \t'
wait_for_output $'\a'
collect_pending_output
if [[ ${REPLY} == *upstream* ]]; then
  print -u2 -r -- "Completion offered a second mirror: ${(qqq)REPLY}"
  exit 1
fi
clear_input

# A non-empty token in the same invalid slot must not uniquely expand to
# upstream either.  This catches the case even when candidate listing styles
# suppress the menu for an empty prefix.
zpty -wn chsrc_completion_shell $'chsrc set ruby first u\t'
wait_for_output $'\a'
collect_pending_output
if [[ ${REPLY} == *upstream* ]]; then
  print -u2 -r -- "Completion inserted a second mirror: ${(qqq)REPLY}"
  exit 1
fi
clear_input

print 'Real Zsh completion insertion tests passed.'
