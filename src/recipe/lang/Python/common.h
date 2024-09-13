/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Nul None  <nul@none.org>
 * Created On     : <2023-09-03>
 * Major Revision :      1
 * Last Modified  : <2024-09-13>
 * ------------------------------------------------------------*/

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
