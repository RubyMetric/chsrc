/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static MirrorSite_t NpmMirror =
{
  IS_DedicatedMirrorSite,
  "npmmirror", "npmmirror", "npmmirror (阿里云赞助)", "https://npmmirror.com/",
  {SKIP, NULL, NULL, NULL, ACCURATE}
};

def_sources_only_dish(pl_nodejs_binary, "nodejs-binary");

void
pl_nodejs_binary_prepare (void)
{
  chef_prep_this_sources_only_dish (pl_nodejs_binary);

  chef_set_recipe_created_on   (this, "2023-09-09");
  chef_set_recipe_last_updated (this, "2026-08-12");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,  "https://nodejs.org/dist/", FeedByPrepare},
  {&NpmMirror, "https://npmmirror.com/mirrors", FeedByPrepare},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/nodejs-release/",FeedByPrepare},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/nodejs-release/",FeedByPrepare},
  {&Ustc,      "https://mirrors.ustc.edu.cn/node/",FeedByPrepare},
  {&Huawei,    "https://mirrors.huaweicloud.com/nodejs/",FeedByPrepare},
  {&Tencent,   "https://mirrors.tencent.com/nodejs-release/", FeedByPrepare}
  def_sources_end()

  chef_set_rest_smURL_with_postfix (this, "/v23.4.0/node-v23.4.0-linux-x64.tar.xz");
}
