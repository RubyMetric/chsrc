<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : 01-Develop.md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2025-07-21>
 ! Last Modified : <2025-07-21>
 ! ---------------------------------------------------------- -->

# Develop `rawstr4c`

## Dependencies and Dev environment

Please install these first:

  1. [rakudo]
  2. [just]

<br>



## Get code

**Please make sure to use the dev branch for development**

```bash
git clone https://gitee.com/RubyMetric/chsrc.git -b dev
```

<br>



## Run

```bash
just rawstr4c
```

```bash
zef install .

rawstr4c

zef uninstall rawstr4c
```

<br>



## Debug

```bash
just rawstr4c --debug
# Note: there must be an = between option value and option
just rawstr4c --debug=parser
just rawstr4c --debug=generator
```

<br>



## Test

Run test scripts

```bash
cd test

raku ./xx-file.rakutest
```

Test executable file

```bash
just rawstr4c
```

<br>



[rakudo]: https://rakudo.org/
[just]: https://github.com/casey/just
