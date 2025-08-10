/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static MirrorSite_t NpmMirror =
{
  IS_DedicatedMirrorSite,
  "npmmirror", "npmmirror", "npmmirror (阿里云赞助)", "https://npmmirror.com/",
  {SKIP, NULL, NULL, NULL, ACCURATE}
};

def_target(pl_js_group);

void
pl_js_group_prelude (void)
{
  use_this(pl_js_group);
  chef_allow_gsr(pl_js_group);

  chef_set_created_on   (this, "2023-09-09");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this,0);

  chef_allow_local_mode (this, PartiallyCan,
    "支持 npm, yarn v2, pnpm, 不支持 yarn v1"
    "Support npm, yarn v2, pnpm, not yarn v1");
  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&upstream,  "https://registry.npmjs.org/",    FeedByPrelude}, /* @note 根据 pnpm 官网，有最后的斜线 */
  {&NpmMirror, "https://registry.npmmirror.com", FeedByPrelude},
  {&Huawei,    "https://mirrors.huaweicloud.com/repository/npm/", FeedByPrelude},
  {&Tencent,   "https://mirrors.cloud.tencent.com/npm/", FeedByPrelude}
  def_sources_end()

  // 29MB 大小
  chsrc_set_sources_speed_measure_url_with_postfix (this, "/@tensorflow/tfjs/-/tfjs-4.22.0.tgz");
}



def_target(pl_js_nodejs_binary);

void
pl_js_nodejs_binary_prelude (void)
{
  use_this(pl_js_nodejs_binary);
  chef_allow_gsr(pl_js_group);

  chef_set_created_on   (this, "2023-09-09");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_forbid_local_mode (this);
  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&upstream,  "https://nodejs.org/dist/", FeedByPrelude},
  {&NpmMirror, "https://npmmirror.com/mirrors", FeedByPrelude},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/nodejs-release/",FeedByPrelude},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/nodejs-release/",FeedByPrelude},
  {&Ustc,      "https://mirrors.ustc.edu.cn/node/",FeedByPrelude},
  {&Huawei,    "https://mirrors.huaweicloud.com/nodejs/",FeedByPrelude},
  {&Tencent,   "https://mirrors.cloud.tencent.com/nodejs-release/", FeedByPrelude}
  def_sources_end()

  chsrc_set_sources_speed_measure_url_with_postfix (this, "/v23.4.0/node-v23.4.0-linux-x64.tar.xz");
}
