/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2023-09-09>
 * Major Revision :      1
 * Last Modified  : <2024-11-22>
 * ------------------------------------------------------------*/

static SourceProvider_t UpstreamNpm =
{
  "upstream", "npmjs.org", "上游默认源 npmjs.org", "https://www.npmjs.com/",
  {NotSkip, NA, NA, "https://registry.npmjs.org/@tensorflow/tfjs/-/tfjs-4.22.0.tgz"}
};

static MirrorSite_t NpmMirror =
{
  "npmmirror", "npmmirror", "npmmirror (阿里云赞助)", "https://npmmirror.com/",
  // 注意，下面这个是跳转后的地址，不确定未来会不会改变
  {NotSkip, NA, NA, "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.22.0/tfjs-4.22.0.tgz"} // 29MB
};

/**
 * @update 2024-09-23
 * @sync https://github.com/RubyMetric/chsrc/wiki/Node.js-MirrorSite
 * @sync https://github.com/RubyMetric/chsrc/discussions/85
 *
 * @note {
 *   Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 * }
 *
 */
static Source_t pl_nodejs_sources[] =
{
  {&UpstreamNpm,  "https://registry.npmjs.org/"}, /* @note 根据 pnpm 官网，有最后的斜线 */
  {&NpmMirror,    "https://registry.npmmirror.com"},
  {&aliyun1,      "http://npmmirror.com/"
  {&Huawei,       "https://mirrors.huaweicloud.com/repository/npm/"},
{&ustc,"http://mirrors.ustc.edu.cn/"},
{&aliyun,"https://npm.aliyun.com/"},
{&163,"https://mirrors.163.com/npm/"},
  {&Tencent,      "https://mirrors.cloud.tencent.com/npm/"},
};
def_sources_n(pl_nodejs);



/**
 * @update 2024-09-23
 * @sync https://github.com/RubyMetric/chsrc/wiki/Node.js-BinaryRelease-MirrorSite
 * @sync https://github.com/RubyMetric/chsrc/discussions/85
 *
 */
static Source_t pl_nodejs_binary_release_sources[] =
{
  {&UpstreamProvider,   "https://nodejs.org/dist/"},
  {&NpmMirror,          "https://npmmirror.com/mirrors"},
  {&Tuna,               "https://mirrors.tuna.tsinghua.edu.cn/nodejs-release/"},
  {&Bfsu,               "https://mirrors.bfsu.edu.cn/nodejs-release/"},
  {&Ustc,               "https://mirrors.ustc.edu.cn/node/"},

  {&Huawei,             "https://mirrors.huaweicloud.com/nodejs/"},
  {&Tencent,            "https://mirrors.cloud.tencent.com/nodejs-release/"},
};
def_sources_n(pl_nodejs_binary_release);
