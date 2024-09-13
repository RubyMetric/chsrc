/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2023-09-09>
 * Major Revision :      1
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

static MirrorSite
NpmMirror = {"npmmirror",    "npmmirror",    "npmmirror (阿里云赞助)",    "https://npmmirror.com/",
              // 注意，这个是跳转后的地址，不确定未来会不会改变
              "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.10.0/tfjs-4.10.0.tgz"}; // 29MB


/**
 * @time 2024-04-18 更新
 * @note {
 *   Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *   腾讯软件源的npm源一直未证实是否可用
 * }
 */
static SourceInfo
pl_nodejs_sources[] = {
  {&Upstream,      "https://registry.npmjs.org/"}, // @note 根据 pnpm 官网，有最后的斜线
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs);
