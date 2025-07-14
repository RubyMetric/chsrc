/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   : yongxiang  <1926885268@qq.com>
 *                | happy game <happygame1024@gmail.com>
 *                |
 * Created On     : <2023-09-03>
 * Major Revision :      1
 * Last Modified  : <2025-07-14>
 * ------------------------------------------------------------*/

 #include "rawstr4c.h"

/**
 * 注意这个前缀是 host/pipi/web/pacakges....
 * 下面有几个镜像站微调了这个路径，我们只要确认能找到 packages 目录就好
 */
#define PL_Python_Group_Speed_URL_Postfix "/packages/56/e4/55aaac2b15af4dad079e5af329a79d961e5206589d0e02b1e8da221472ed/tensorflow-2.18.0-cp312-cp312-manylinux_2_17_aarch64.manylinux2014_aarch64.whl"

static SourceProvider_t pl_python_pypi_upstream =
{
  def_upstream, "https://pypi.org/",
  {NotSkip, NA, NA, "https://files.pythonhosted.org/packages/56/e4/55aaac2b15af4dad079e5af329a79d961e5206589d0e02b1e8da221472ed/tensorflow-2.18.0-cp312-cp312-manylinux_2_17_aarch64.manylinux2014_aarch64.whl", ACCURATE} // 260MB
};

/**
 * @update 2025-07-11
 * @note 不要添加Zju，浙大的PyPI服务在校外访问会自动转向Tuna
 */
static Source_t pl_python_group_sources[] =
{
  {&pl_python_pypi_upstream,   "https://pypi.org/simple", DelegateToUpstream},

  {&MirrorZ,          "https://mirrors.cernet.edu.cn/pypi/web/simple",
                      "https://mirrors.cernet.edu.cn/pypi/web" PL_Python_Group_Speed_URL_Postfix},

  {&Bfsu,             "https://mirrors.bfsu.edu.cn/pypi/web/simple",
                      "https://mirrors.bfsu.edu.cn/pypi/web" PL_Python_Group_Speed_URL_Postfix},

  {&Lzuoss,           "https://mirror.lzu.edu.cn/pypi/web/simple",
                      "https://mirror.lzu.edu.cn/pypi/web"   PL_Python_Group_Speed_URL_Postfix},

  {&Jlu,              "https://mirrors.jlu.edu.cn/pypi/web/simple",
                      "https://mirrors.jlu.edu.cn/pypi/web"  PL_Python_Group_Speed_URL_Postfix},

  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/pypi/web/simple",
                      "https://mirror.sjtu.edu.cn/pypi/web" PL_Python_Group_Speed_URL_Postfix},

  {&Tuna,             "https://pypi.tuna.tsinghua.edu.cn/simple",
                      "https://pypi.tuna.tsinghua.edu.cn" PL_Python_Group_Speed_URL_Postfix},

  {&Ali,              "https://mirrors.aliyun.com/pypi/simple/",
                      "https://mirrors.aliyun.com/pypi" PL_Python_Group_Speed_URL_Postfix},

  {&Nju,              "https://mirror.nju.edu.cn/pypi/web/simple",
                      "https://mirror.nju.edu.cn/pypi/web" PL_Python_Group_Speed_URL_Postfix},

  {&Pku,              "https://mirrors.pku.edu.cn/pypi/web/simple",
                      "https://mirrors.pku.edu.cn/pypi/web" PL_Python_Group_Speed_URL_Postfix},

  {&Tencent,          "https://mirrors.cloud.tencent.com/pypi/simple",
                      "https://mirrors.cloud.tencent.com/pypi" PL_Python_Group_Speed_URL_Postfix},

  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/pypi/simple",
  //                  "https://mirrors.cloud.tencentyun.com/pypi" PL_Python_Group_Speed_URL_Postfix,

  {&Huawei,           "https://mirrors.huaweicloud.com/repository/pypi/simple",
                      "https://mirrors.huaweicloud.com/repository/pypi" PL_Python_Group_Speed_URL_Postfix},

  {&Hust,             "https://mirrors.hust.edu.cn/pypi/web/simple",
                      "https://mirrors.hust.edu.cn/pypi/web" PL_Python_Group_Speed_URL_Postfix},

  /* 不启用原因：24小时更新一次 */
  // {&Netease,       "https://mirrors.163.com/.help/pypi.html", NULL}
};
def_sources_n(pl_python_group);

void
pl_python_check_unofficial_pkger (bool *poetry_exist, bool *pdm_exist, bool *uv_exist)
{
  *poetry_exist = chsrc_check_program ("poetry");
  *pdm_exist = chsrc_check_program ("pdm");
  *uv_exist = chsrc_check_program ("uv");
}


/**
 * @param[out] prog 返回 Python 的可用名，如果不可用，则返回 NULL
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
