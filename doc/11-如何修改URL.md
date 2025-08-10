<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : 11-如何修改URL.md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2025-08-11>
 ! Last Modified : <2025-08-11>
 ! ---------------------------------------------------------- -->

# 新增/修改换源 URL

在 `_prelude()` 函数中，在 `def_sources_begin()` 的位置的**第二列**，新增/修改你想要替换的URL。

**注: 以下代码仅起解释作用，JavaScript换源的真正实现可能不同**

```c
def_sources_begin()
{&UpstreamProvider, "https://registry.npmjs.org/",                     FeedByPrelude},
{&NpmMirror,        "https://registry.npmmirror.com",                  FeedByPrelude},
{&Huawei,           "https://mirrors.huaweicloud.com/repository/npm/", FeedByPrelude},
{&Tencent,          "https://mirrors.cloud.tencent.com/npm/",          FeedByPrelude},
{&某新镜像站,       "某镜像站提供的换源URL"，                          FeedByPrelude}
def_sources_end()
```

<br>


# 提供精准测速 URL

在 `_prelude()` 函数中，在 `def_sources_begin()` 的位置的**第三列**，新增/修改你想要替换的URL。

1. 在下面代码的第一行中，我们直接给了一个链接，**这就是精准测速 URL**
2. 在下面代码的第二行和第三行中，我们都设置了一个宏 `DelegateToMirror`，这意味着我们没有直接提供精准测速 URL，而是让 `chsrc` 去测试其对应镜像站定义的测速 URL

    - `NpmMirror` 是专用镜像站，所以其测速链接被设置为了 `ACCURATE`，即精准测速
    - `Huawei` 是通用镜像站，所以其测速链接被设置为了 `ROUGH `，即模糊测速

3. 在下面代码的第四行中，我们设置了一个宏 `FeedByPrelude`，这意味着我们将在 `_prelude()` 函数的后续动态计算 URL

**注: 以下代码仅起解释作用，JavaScript换源的真正实现可能不同**

```c
def_sources_begin()
{&UpstreamProvider, "https://registry.npmjs.org/",                     "https://registry.npmjs.org/BigFile.tar.gz"},
{&NpmMirror,        "https://registry.npmmirror.com",                  DelegateToMirror},
{&Huawei,           "https://mirrors.huaweicloud.com/repository/npm/", DelegateToMirror},
{&Tencent,          "https://mirrors.cloud.tencent.com/npm/",          FeedByPrelude},
def_sources_end()

// 把所有上述源的测速 URL 设置为 "换源 URL" + postfix
chsrc_set_sources_speed_measure_url_with_postfix (this, "/dir/BigFile.tar.gz");
// 基于 "换源 URL" 做更自定义的操作
chsrc_set_sources_speed_measure_url_with_func (this, func, data);

// 调整某一个镜像站(Provider)的测速 URL
chsrc_set_provider_speed_measure_url (&Tencent, "https://mirrors.cloud.tencent.com/npm/BigFile.tar.gz")
// 调整某一个镜像站(Provider)的测速精度
chsrc_set_provider_speed_measure_accuracy (&UpstreamProvider, ROUGH);;
```
