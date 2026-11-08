/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

def_sources_only_dish(pl_pypi, "pypi");

/**
 * @note 测速链接的这个前缀是 ${host}/pipi/web/pacakges/56/e4....
 * 下面有几个镜像站微调了这个路径，我们只要确认能找到 packages 目录就好
 *
 * @note 2025-09-29 更新了测试的 pkg 链接，换用了一个 40M 的文件
 *
 * @warning 2026-08-01 Sjtug 的索引地址为 /pypi/web/simple，
 * 实际文件仍位于 /pypi-packages，需要特殊处理
 */
static char *
pl_pypi_smURL_constructor (const char *url, const char *user_data)
{
  char *str = xy_str_delete_suffix (url, "/simple");
  str = xy_2strcat (str, "/packages/fa/80/eb88edc2e2b11cd2dd2e56f1c80b5784d11d6e6b7f04a1145df64df40065/opencv_python-4.12.0.88-cp37-abi3-win_amd64.whl");
  if (strstr (url, "mirror.sjtu.edu.cn"))
    // e.g. https://mirror.sjtu.edu.cn/pypi-packages/fa/80/eb88edc2e2b11cd2dd2e56f1c80b5784d11d6e6b7f04a1145df64df40065/opencv_python-4.12.0.88-cp37-abi3-win_amd64.whl
    str = xy_str_gsub (str, "pypi/web/packages", "pypi-packages");// 针对 Sjtug

  return str;
}


void
pl_pypi_prepare (void)
{
  chef_prep_this_sources_only_dish (pl_pypi);

  chef_set_recipe_created_on   (this, "2023-09-03");
  chef_set_recipe_last_updated (this, "2026-08-01");

  chef_set_chefs (this, 2, "@ccmywish", "@happy-game");
  chef_set_sauciers (this, 4, "@ReachForStar", "@Kattos", "@Mikachu2333", "@Yangmoooo");

  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://pypi.org/simple",                       FeedByPrepare},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/pypi/web/simple", FeedByPrepare},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/pypi/web/simple",   FeedByPrepare},
  // 不要添加Zju，浙大的PyPI服务在校外访问会自动转向Tuna
  {&Lzuoss,           "https://mirrors.lzu.edu.cn/pypi/web/simple",     FeedByPrepare},
  // 2025-09-29 此源已停用
  // @ref https://mirrors.jlu.edu.cn/_news/#2025-04-06-pypi-repo-down
  // {&Jlu,              "https://mirrors.jlu.edu.cn/pypi/web/simple", FeedByPrepare},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/pypi/web/simple",    FeedByPrepare},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/pypi/web/simple",   FeedByPrepare},
  {&Ustc,             "https://mirrors.ustc.edu.cn/pypi/simple",       FeedByPrepare},
  {&Ali,              "https://mirrors.aliyun.com/pypi/simple",        FeedByPrepare},
  {&Nju,              "https://mirror.nju.edu.cn/pypi/web/simple",     FeedByPrepare},
  {&Pku,              "https://mirrors.pku.edu.cn/pypi/web/simple",    FeedByPrepare},
  {&Tencent,          "https://mirrors.tencent.com/pypi/simple",       FeedByPrepare},

  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/pypi/simple", FeedByPrepare}
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/pypi/simple", FeedByPrepare},
  {&Hust,             "https://mirrors.hust.edu.cn/pypi/web/simple",   FeedByPrepare}

  /* 不启用原因：24小时更新一次 */
  // {&Netease,       "https://mirrors.163.com/.help/pypi.html", NULL}
  def_sources_end()

  chef_set_rest_smURL_with_func (this, pl_pypi_smURL_constructor, NULL);
}
