/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#include "rawstr4c.h"

def_target(pl_python_group, "python/pypi/py");

/**
 * @note 测速链接的这个前缀是 ${host}/pipi/web/pacakges/56/e4....
 * 下面有几个镜像站微调了这个路径，我们只要确认能找到 packages 目录就好
 *
 * @note 2025-09-29 更新了测试的 pkg 链接，换用了一个 40M 的文件
 *
 * @warning 2025-09-29 Sjtug 需要特殊处理
 */
static char *
pl_python_speed_url_constructor (const char *url, const char *user_data)
{
  char *str = xy_str_delete_suffix (url, "/simple");
  str = xy_2strcat (str, "/packages/fa/80/eb88edc2e2b11cd2dd2e56f1c80b5784d11d6e6b7f04a1145df64df40065/opencv_python-4.12.0.88-cp37-abi3-win_amd64.whl");
  if (strstr (url, "mirror.sjtu.edu.cn"))
    // e.g. https://mirror.sjtu.edu.cn/pypi-packages/fa/80/eb88edc2e2b11cd2dd2e56f1c80b5784d11d6e6b7f04a1145df64df40065/opencv_python-4.12.0.88-cp37-abi3-win_amd64.whl
    str = xy_str_gsub (str, "pypi-packages/packages", "pypi-packages");// 针对 Sjtug

  return str;
}


void
pl_python_group_prelude (void)
{
  chef_prep_this (pl_python_group, gsr);

  chef_set_recipe_created_on   (this, "2023-09-03");
  chef_set_recipe_last_updated (this, "2026-03-31");
  chef_set_sources_last_updated (this, "2025-09-30");

  chef_set_chef (this, "@happy-game");
  // 组换源的 leader target 应把所有 follower target 的贡献者都记录过来
  chef_set_cooks (this, 2, "@ccmywish", "@happy-game");
  chef_set_sauciers (this, 3, "@xyx1926885268", "@Kattos", "@Mikachu2333");

  /* 部分包管理器支持 ProjectScope，但是为了让流程执行下去，我们这里都写 ScopeCap_Able_And_Implemented */
  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_allow_english(this);
  chef_allow_user_define(this);


  def_sources_begin()
  {&UpstreamProvider, "https://pypi.org/simple",                       FeedByPrelude},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/pypi/web/simple", FeedByPrelude},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/pypi/web/simple",   FeedByPrelude},
  // 不要添加Zju，浙大的PyPI服务在校外访问会自动转向Tuna
  {&Lzuoss,           "https://mirror.lzu.edu.cn/pypi/web/simple",     FeedByPrelude},
  // 2025-09-29 此源已停用
  // @ref https://mirrors.jlu.edu.cn/_news/#2025-04-06-pypi-repo-down
  // {&Jlu,              "https://mirrors.jlu.edu.cn/pypi/web/simple", FeedByPrelude},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/pypi-packages",      FeedByPrelude},
  {&Tuna,             "https://pypi.tuna.tsinghua.edu.cn/simple",      FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/pypi/simple",        FeedByPrelude},
  {&Nju,              "https://mirror.nju.edu.cn/pypi/web/simple",     FeedByPrelude},
  {&Pku,              "https://mirrors.pku.edu.cn/pypi/web/simple",    FeedByPrelude},
  {&Tencent,          "https://mirrors.cloud.tencent.com/pypi/simple", FeedByPrelude},

  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/pypi/simple",FeedByPrelude}
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/pypi/simple",FeedByPrelude},
  {&Hust,             "https://mirrors.hust.edu.cn/pypi/web/simple",FeedByPrelude}

  /* 不启用原因：24小时更新一次 */
  // {&Netease,       "https://mirrors.163.com/.help/pypi.html", NULL}
  def_sources_end()

  chef_set_rest_smURL_with_func (this, pl_python_speed_url_constructor, NULL);
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

  bool python3_is_winstore_placeholder = false;

  const int winstore_python_status_code = 9009;

  const char *winstore_placeholder_cn = "是微软商店的占位符，并非真正可用的 Python";
  const char *winstore_placeholder_en = "a placeholder of Microsoft Store, not the real Python";

  if (py_exist)
    {
      *prog_name = "python3";

      // https://github.com/RubyMetric/chsrc/issues/327
      if (xy.on_windows)
        {
          int status = xy_run_get_status ("python3 --version");
          if (status == winstore_python_status_code)
            {
              python3_is_winstore_placeholder = true;
              *prog_name = NULL;
              // https://github.com/RubyMetric/chsrc/issues/351
              // 仅警告，不要直接退出，因为用户环境中还可能存在真正的 `python` 命令
              chsrc_warn2 (CHINESE ? xy_2strcat ("用户环境中的 `python3` 命令，", winstore_placeholder_cn)
                                   : xy_2strcat ("`python3` in your environment is ", winstore_placeholder_en));
            }
        }
    }

  if (!py_exist || python3_is_winstore_placeholder)
    {
      /**
       * 不要直接:
       *
       *   $ python
       *
       * 这样调用 `python` 自己，而是使用 `python --version`，或者其他方式
       * 因为直接执行 `python` 会使 Windows 弹出Microsoft Store
       */
      py_exist = chsrc_check_program ("python");

      if (py_exist)
        {
          if (xy.on_windows)
            {
              int status = xy_run_get_status ("python --version");
              if (status == winstore_python_status_code)
                {
                  chsrc_warn2 (CHINESE ? xy_2strcat ("用户环境中的 `python` 命令，也", winstore_placeholder_cn)
                                       : xy_2strcat ("`python` in your environment is also ", winstore_placeholder_en));
                  chsrc_error (CHINESE ? "请安装真正的 Python 后重试！"
                                       : "Please install the real Python and try again!");
                  exit (Exit_UserCause);
                }
            }
          *prog_name = "python";
        }
      else
        {
          chsrc_error ("未找到 Python 相关命令，请检查是否存在");
          exit (Exit_UserCause);
        }
    }
}
