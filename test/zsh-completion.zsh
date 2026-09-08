#!/usr/bin/env zsh
# -----------------------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Lightweight tests for the completion state machine.  Completion widgets are
# mocked so the test does not touch the user's Zsh setup.
# -----------------------------------------------------------------------------

emulate -L zsh
setopt errexit nounset pipefail extendedglob

typeset test_dir=${0:A:h}
fpath=("${test_dir}/../tool/completion" ${fpath})
autoload -Uz _chsrc

typeset -ga captured

_describe() {
  local array_name=${argv[-1]}
  local -a specs
  specs=("${(@P)array_name}")
  local spec candidate
  for spec in ${specs}; do
    candidate=${spec%%:*}
    if [[ -z ${PREFIX} || ${candidate[1,${#PREFIX}]} == ${PREFIX} ]]; then
      captured+=("${candidate}")
    fi
  done
}

compadd() {
  local after_separator=0 arg
  for arg in ${argv}; do
    if (( after_separator )); then
      if [[ -z ${PREFIX} || ${arg[1,${#PREFIX}]} == ${PREFIX} ]]; then
        captured+=("${arg}")
      fi
    elif [[ ${arg} == -- ]]; then
      after_separator=1
    fi
  done
}

compset() {
  if [[ $1 == -P ]]; then
    PREFIX=${PREFIX#$2}
  fi
}

_message() { return 0 }

capture() {
  captured=()
  words=("${(@)argv}")
  CURRENT=${#words}
  PREFIX=${words[CURRENT]}
  _chsrc
}

assert_has() {
  local expected=$1
  if (( ! ${captured[(Ie)${expected}]} )); then
    print -u2 -- "Expected completion '$expected' in: ${captured[*]}"
    return 1
  fi
}

assert_lacks() {
  local unexpected=$1
  if (( ${captured[(Ie)${unexpected}]} )); then
    print -u2 -- "Unexpected completion '$unexpected' in: ${captured[*]}"
    return 1
  fi
}

capture chsrc ''
assert_has version
assert_has measure
assert_has reset
assert_has -scope=
assert_lacks mea
assert_lacks rest
assert_lacks -local
assert_lacks -scope
assert_lacks -english
assert_lacks -no-colour
assert_lacks -h
assert_lacks -help
assert_lacks --help
assert_lacks -v
assert_lacks -version
assert_lacks --version

capture chsrc -scope=p
assert_has project
assert_lacks default

typeset -A dispatch_expect=(
  help -dry
  h -dry
  -h -dry
  -help -dry
  --help -dry
  issue -dry
  issues -dry
  isue -dry
  i -dry
  version -dry
  ver -dry
  -v -dry
  -version -dry
  --version -dry
  list mirror
  ls mirror
  l mirror
  measure ruby
  mea ruby
  m ruby
  cesu ruby
  ce ruby
  c ruby
  get ruby
  g ruby
  set ruby
  s ruby
  reset ruby
  rest ruby
  r ruby
)

typeset command expected
for command expected in "${(@kv)dispatch_expect}"; do
  capture chsrc "${command}" ''
  assert_has "${expected}"
done

capture chsrc set ''
assert_has omarchy
assert_has uv-pypi-index
assert_has uv-python-build
assert_has -scope=
assert_lacks pypi
assert_lacks nodejs-binary

capture chsrc s -dry ''
assert_has ruby

capture chsrc set -local ''
assert_has ruby

capture chsrc set -english ''
assert_has ruby

capture chsrc set -no-colour ''
assert_has ruby

capture chsrc set -scope project ''
assert_has ruby

capture chsrc set -scope ''
assert_has default
assert_has project
assert_has user
assert_has system
assert_lacks ruby

capture chsrc set -scope=
assert_has default
assert_has project
assert_has user
assert_has system

capture chsrc set -scope=p
assert_has project
assert_lacks default
assert_lacks system

capture chsrc set -scope=x
assert_lacks default
assert_lacks project

capture chsrc set -lo
assert_lacks -local

capture chsrc -en -he
assert_lacks -help
assert_lacks help

capture chsrc list ''
assert_has mirror
assert_has dish
assert_has lang
assert_has ware
assert_lacks mirrors
assert_lacks dishes
assert_lacks language
assert_lacks software
assert_has ruby

capture chsrc set ruby ''
assert_has first
assert_has upstream
assert_lacks -dry

capture chsrc set ruby f
assert_has first
assert_lacks upstream

capture chsrc set ruby first ''
assert_lacks first
assert_lacks upstream

capture chsrc set ruby first u
assert_lacks upstream

capture chsrc set ruby -scope=
assert_lacks default

print 'Zsh completion state-machine tests passed.'
