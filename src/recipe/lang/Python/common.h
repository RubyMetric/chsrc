/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

def_target(pl_python_group, "python/pypi/py");

/**
 * @note 测速链接的这个前缀是 ${host}/pipi/web/pacakges/56/e4....
 * 下面有几个镜像站微调了这个路径，我们只要确认能找到 packages 目录就好
 */
static char *
pl_python_speed_url_constructor (const char *url, const char *user_data)
{
  char *str = xy_str_delete_suffix (url, "/simple");
  str = xy_2strcat (str, "/packages/56/e4/55aaac2b15af4dad079e5af329a79d961e5206589d0e02b1e8da221472ed/tensorflow-2.18.0-cp312-cp312-manylinux_2_17_aarch64.manylinux2014_aarch64.whl");
  return str;
}


void
pl_python_group_prelude (void)
{
  chef_prep_this (pl_python_group, gsr);

  chef_set_created_on   (this, "2023-09-03");
  chef_set_last_updated (this, "2025-07-14");
  chef_set_sources_last_updated (this, "2025-07-11");

  chef_set_chef (this, "@happy-game");
  // 组换源的 leader target 应把所有 follower target 的贡献者都记录过来
  chef_set_cooks (this, 2, "@ccmywish", "@happy-game");
  chef_set_sauciers (this, 2, "@xyx1926885268", "@Kattos");

  chef_allow_local_mode (this, PartiallyCan, "部分包管理器支持项目级换源", "Some package managers support project-level source changing");
  chef_allow_english(this);
  chef_allow_user_define(this);


  def_sources_begin()
  {&UpstreamProvider, "https://pypi.org/simple",                     FeedByPrelude},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/pypi/web/simple",FeedByPrelude},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/pypi/web/simple", FeedByPrelude},
  /* 不要添加Zju，浙大的PyPI服务在校外访问会自动转向Tuna */
  {&Lzuoss,           "https://mirror.lzu.edu.cn/pypi/web/simple",FeedByPrelude},
  {&Jlu,              "https://mirrors.jlu.edu.cn/pypi/web/simple",FeedByPrelude},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/pypi/web/simple",FeedByPrelude},
  {&Tuna,             "https://pypi.tuna.tsinghua.edu.cn/simple", FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/pypi/simple/", FeedByPrelude},
  {&Nju,              "https://mirror.nju.edu.cn/pypi/web/simple",FeedByPrelude},
  {&Pku,              "https://mirrors.pku.edu.cn/pypi/web/simple",FeedByPrelude},
  {&Tencent,          "https://mirrors.cloud.tencent.com/pypi/simple",FeedByPrelude},

  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/pypi/simple",FeedByPrelude}
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/pypi/simple",FeedByPrelude},
  {&Hust,             "https://mirrors.hust.edu.cn/pypi/web/simple",FeedByPrelude}

  /* 不启用原因：24小时更新一次 */
  // {&Netease,       "https://mirrors.163.com/.help/pypi.html", NULL}
  def_sources_end()

  chef_set_sources_speed_measure_url_with_func (this, pl_python_speed_url_constructor, NULL);
}

void
pl_python_check_unofficial_pkger (bool *poetry_exist, bool *pdm_exist, bool *uv_exist)
{
  *poetry_exist = chsrc_check_program ("poetry");
  *pdm_exist = chsrc_check_program ("pdm");
  *uv_exist = chsrc_check_program ("uv");
}


/**
 * @param[out] prog_name 返回 Python 的可用名，如果不可用，则返回 NULL
 */
void
pl_python_get_py_program_name (char **prog_name)
{
  *prog_name = NULL;

  bool py_exist = false;

  /**
   * @issue https://gitee.com/RubyMetric/chsrc/issues/I9VZL2
   *
   * 由于Python2和Python3的历史，目前（2024-06）许多python命令实际上仍然是python2
   * 因此我们首先测试 python3
   */
  py_exist = chsrc_check_program ("python3");

  if (py_exist) *prog_name = "python3";
  else
    {
      /* 不要调用 python 自己，而是使用 python --version，或者其他方式
         因为直接执行 python 会使Windows弹出Microsoft Store */
      py_exist = chsrc_check_program ("python");

      if (py_exist) *prog_name = "python";
      else
        {
          chsrc_error ("未找到 Python 相关命令，请检查是否存在");
          exit (Exit_UserCause);
        }
    }
}
