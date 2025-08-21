/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_ros, "ros/ros2");

void
os_ros_prelude ()
{
  use_this(os_ros);
  chef_allow_gs(os_ros);

  chef_set_created_on   (this, "2023-09-03");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-04-18");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@G_I_Y");
  chef_set_contributors (this, 2, "@ccmywish", "@zouri");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, "该换源方案中，URL存在拼凑，因此不能手动使用某URL来换源", "In this switching method, URLs are constructed, so manual URL specification is not supported");

  def_sources_begin()
  {&UpstreamProvider, "http://packages.ros.org",     DelegateToUpstream},
  {&Ali,              "https://mirrors.aliyun.com",  DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn", DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com", DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com", DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com", DelegateToMirror}
  /* 不启用原因：过慢 */
  // {&Netease,       "https://mirrors.163.com", DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,          "https://mirrors.sohu.com", DelegateToMirror}
  def_sources_end()
}


void
os_ros_getsrc (char *option)
{
  chsrc_view_file (OS_ROS_SourceList);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/ros/
 */
void
os_ros_setsrc (char *option)
{
  chsrc_ensure_root ();

  use_this_source(os_ros);

  chsrc_backup (OS_ROS_SourceList);

  char *cmd  = NULL;
  cmd = xy_strcat(3, "sed -E -i \'s@https?://.*/ros/ubuntu/?@", source.url, "/ros/ubuntu@g\' " OS_ROS_SourceList);
  chsrc_run(cmd, RunOpt_Default);

  cmd = "apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}
