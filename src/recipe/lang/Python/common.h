/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2023-09-03>
 * Major Revision :      1
 * Last Modified  : <2024-11-22>
 * ------------------------------------------------------------*/

static SourceProvider_t UpstreamPython =
{
  "upstream", "PyPI", "上游默认源 PyPI", "https://pypi.org/",
  {NotSkip, NA, NA, "https://files.pythonhosted.org/packages/56/e4/55aaac2b15af4dad079e5af329a79d961e5206589d0e02b1e8da221472ed/tensorflow-2.18.0-cp312-cp312-manylinux_2_17_aarch64.manylinux2014_aarch64.whl"} // 260MB
};

/**
 * @update 2024-09-14
 * @note 不要添加Zju，浙大的pypi在校外访问会自动转向Tuna
 */
static Source_t
pl_python_sources[] = {
  {&UpstreamPython,   "https://pypi.org/simple"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/pypi/web/simple"},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/pypi/web/simple"},
  {&Jlu,              "https://mirrors.jlu.edu.cn/pypi/web/simple"},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/pypi/web/simple"},
  {&Tuna,             "https://pypi.tuna.tsinghua.edu.cn/simple"},
  {&Ali,              "https://mirrors.aliyun.com/pypi/simple/"},
  {&Tencent,          "https://mirrors.cloud.tencent.com/pypi/simple"},
  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/pypi/simple"},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/pypi/simple"},
  {&Hust,             "https://mirrors.hust.edu.cn/pypi/web/simple"}
  // {&Netease,       "https://mirrors.163.com/.help/pypi.html"} // 不用，24小时更新一次
};
def_sources_n(pl_python);

void
pl_python_check_unofficial_pkger (bool *poetry_exist, bool *pdm_exist)
{
  *poetry_exist = chsrc_check_program ("poetry");
  *pdm_exist = chsrc_check_program ("pdm");
}


/**
 * @param[out] prog 返回 Python 的可用名，如果不可用，则返回 NULL
 */
void
pl_python_get_py_program_name (char **prog_name)
{
  *prog_name = NULL;

  bool py_exist = false;

  // 由于Python2和Python3的历史，目前（2024-06）许多python命令实际上仍然是python2
  // https://gitee.com/RubyMetric/chsrc/issues/I9VZL2
  // 因此我们首先测试 python3
  py_exist = chsrc_check_program ("python3");

  if (py_exist) *prog_name = "python3";
  else
    {
      // 不要调用 python 自己，而是使用 python --version，避免Windows弹出Microsoft Store
      py_exist = chsrc_check_program ("python");

      if (py_exist) *prog_name = "python";
      else
        {
          chsrc_error ("未找到 Python 相关命令，请检查是否存在");
          exit (Exit_UserCause);
        }
    }
}
