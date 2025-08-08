<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 !               | happy game <happygame1024@gmail.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-08-08>
 ! ---------------------------------------------------------- -->

# rawstr4c input

- prefix = `RAWSTR_wr_dockerhub`
- output = `:global-variable-only-header`
- translate = `:hex`
- no-postfix = `true`



## auto insert content

- name = `insert_content`

```json
{
  "registry-mirrors": ["@1@"]
}
```



## jq command1

检查配置文件中是否已存在即将要换的源

- name = `check_cmd`

```sh
jq '.["registry-mirrors"] | index("@1@")' @2@
```



## jq command2

插入新的源到配置文件中

- name = `insert_cmd`

```sh
jq '.["registry-mirrors"] |= ["@1@"] + .' @2@.bak >  @2@
```

注释: `|=` 为赋值, `+ .` 表示把原数组加过来



## sed command

没有 `jq` 时，用 `sed` 换源，写入到配置文件中

```sh
sed -z -i 's/"registry-mirrors":[^]]*]/"registry-mirrors":["@1@"]/' @2@
```

注释: `[^]]*` 即不是 `]` 的所有字符, 后面再跟一个 `]` 由于没有前面的 `[`，因此可被解析为普通字符 `]`，没有特殊含义
