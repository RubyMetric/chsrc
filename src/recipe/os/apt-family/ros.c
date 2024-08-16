/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 * Created On    : <2023-09-03>
 * Last Modified : <2024-08-16>
 * ------------------------------------------------------------*/

/**
 * @time 2024-04-18 更新
 * @note 未经测试是否有效
 */
static SourceInfo
os_ros_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn"},
  {&Ustc,           "https://mirrors.ustc.edu.cn"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,        "https://mirrors.tencent.com"},
  {&Huawei,         "https://mirrors.huaweicloud.com"},
  {&Netease,        "https://mirrors.163.com"},
  {&Sohu,           "https://mirrors.sohu.com"}
};
def_sources_n(os_ros);

/**
 * 参考：
 * https://mirrors.tuna.tsinghua.edu.cn/help/ros/
 */
void
os_ros_setsrc (char *option)
{
  chsrc_ensure_root (OS_ROS_SourceList);

  SourceInfo source;
  chsrc_yield_source (os_ros);
  chsrc_confirm_source (&source);

  chsrc_backup (OS_ROS_SourceList);

  char *cmd  = NULL;
  cmd = xy_strjoin(3, "sed -E -i \'s@https?://.*/ros/ubuntu/?@", source.url, "@/ros/ubuntug\' " OS_ROS_SourceList);
  chsrc_run(cmd, RunOpt_Default);

  cmd = "apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

def_target_s(os_ros);
