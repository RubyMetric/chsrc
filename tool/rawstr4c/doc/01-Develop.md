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

**For convenience, when developing, we only use `just` to invoke it.**

> [!IMPORTANT]
> When developing `rawstr4c` and maintaining `chsrc`,
> we must always give a path relative to the root directory of the whole `chsrc` project!
> This is because `just` will switch back to the project root directory by itself.

```bash
# Now we've already cd into the current dir
cd src/recipe
# Still have to use path relative to root!!!
just rawstr4c ./src/recipe/ware
```

```bash
just rawstr4c
```

We can install the distribution, by this way, we don't need `just`.

```bash
zef install .

rawstr4c --help
```

And therefore no such limitations mentioned above!

```bash
cd src/recipe
# No need to to use path relative to root now!!!
rawstr4c .
```

```bash
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
