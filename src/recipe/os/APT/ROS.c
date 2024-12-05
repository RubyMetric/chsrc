/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 *               |  zouri <guoshuaisun@outlook.com>
 * Created On    : <2023-09-03>
 * Last Modified : <2024-12-05>
 * ------------------------------------------------------------*/

/**
 * @update 2024-12-05
 */
static Source_t os_ros_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Ali,              "https://mirrors.aliyun.com"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn"},
  {&Ustc,             "https://mirrors.ustc.edu.cn"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,          "https://mirrors.tencent.com"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com"},
  {&Huawei,           "https://mirrors.huaweicloud.com"},
  {&Netease,          "https://mirrors.163.com"},
  {&Sohu,             "https://mirrors.sohu.com"}
};
def_sources_n(os_ros);

/**
 * 参考：
 * https://mirrors.tuna.tsinghua.edu.cn/help/ros/
 */
void
os_ros_setsrc (char *option)
{
  chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_ros);

  chsrc_backup (OS_ROS_SourceList);

  char *cmd  = NULL;
  cmd = xy_strjoin(3, "sed -E -i \'s@https?://.*/ros/ubuntu/?@", source.url, "/ros/ubuntu@g\' " OS_ROS_SourceList);
  chsrc_run(cmd, RunOpt_Default);

  cmd = "apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Untested;
  chsrc_conclude (&source);
}

def_target_s(os_ros);
