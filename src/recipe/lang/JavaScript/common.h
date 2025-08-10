/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 *                |
 * Created On     : <2023-09-09>
 * Major Revision :      1
 * Last Modified  : <2025-07-11>
 * ------------------------------------------------------------*/

#define PL_JS_Group_Speed_URL_Postfix "/@tensorflow/tfjs/-/tfjs-4.22.0.tgz"

static SourceProvider_t pl_js_npm_upstream =
{
  def_upstream, "https://www.npmjs.com/",
  {NotSkip, NA, NA, "https://registry.npmjs.org/@tensorflow/tfjs/-/tfjs-4.22.0.tgz", ACCURATE}
};

static MirrorSite_t NpmMirror =
{
  IS_DedicatedMirrorSite,
  "npmmirror", "npmmirror", "npmmirror (阿里云赞助)", "https://npmmirror.com/",
  // 注意，下面这个是跳转后的地址，不确定未来会不会改变
  {NotSkip, NA, NA, "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.22.0/tfjs-4.22.0.tgz", ACCURATE} // 29MB
};

/**
 * @update 2025-07-11
 * @sync https://github.com/RubyMetric/chsrc/wiki/Node.js-MirrorSite
 * @sync https://github.com/RubyMetric/chsrc/discussions/85
 *
 * @note
 *   Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 */
static Source_t pl_js_group_sources[] =
{
  {&pl_js_npm_upstream,  "https://registry.npmjs.org/", DelegateToUpstream}, /* @note 根据 pnpm 官网，有最后的斜线 */
  {&NpmMirror,    "https://registry.npmmirror.com", DelegateToMirror},

  {&Huawei,       "https://mirrors.huaweicloud.com/repository/npm/",
                  "https://mirrors.huaweicloud.com/repository/npm/" PL_JS_Group_Speed_URL_Postfix},

  {&Tencent,      "https://mirrors.cloud.tencent.com/npm/",
                  "https://mirrors.cloud.tencent.com/npm/" PL_JS_Group_Speed_URL_Postfix},
};
def_sources_n(pl_js_group);


#define PL_Nodejs_Binary_Speed_URL_Postfix "/v23.4.0/node-v23.4.0-linux-x64.tar.xz"

static SourceProvider_t pl_js_binary_release_upstream =
{
  def_upstream, "https://nodejs.org/",
  {NotSkip, NA, NA, "https://nodejs.org/dist/v23.4.0/node-v23.4.0.tar.gz", ACCURATE} // 100MB
};

/**
 * @update 2025-07-11
 * @sync https://github.com/RubyMetric/chsrc/wiki/Node.js-BinaryRelease-MirrorSite
 * @sync https://github.com/RubyMetric/chsrc/discussions/85
 *
 */
static Source_t pl_js_binary_release_sources[] =
{
  {&pl_js_binary_release_upstream,  "https://nodejs.org/dist/", DelegateToUpstream},

  {&NpmMirror,          "https://npmmirror.com/mirrors",
                        "https://registry.npmmirror.com/-/binary/node/v23.4.0/node-v23.4.0.tar.gz"},

  {&Tuna,               "https://mirrors.tuna.tsinghua.edu.cn/nodejs-release/",
                        "https://mirrors.tuna.tsinghua.edu.cn/nodejs-release/" PL_Nodejs_Binary_Speed_URL_Postfix},

  {&Bfsu,               "https://mirrors.bfsu.edu.cn/nodejs-release/",
                        "https://mirrors.bfsu.edu.cn/nodejs-release/" PL_Nodejs_Binary_Speed_URL_Postfix},

  {&Ustc,               "https://mirrors.ustc.edu.cn/node/",
                        "https://mirrors.ustc.edu.cn/node/" PL_Nodejs_Binary_Speed_URL_Postfix},

  {&Huawei,             "https://mirrors.huaweicloud.com/nodejs/",
                        "https://mirrors.huaweicloud.com/nodejs/" PL_Nodejs_Binary_Speed_URL_Postfix},

  {&Tencent,            "https://mirrors.cloud.tencent.com/nodejs-release/",
                        "https://mirrors.cloud.tencent.com/nodejs-release/" PL_Nodejs_Binary_Speed_URL_Postfix}
};
def_sources_n(pl_js_binary_release);
