/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  :  Heng Guo  <2085471348@qq.com>
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 *               |    zouri   <guoshuaisun@outlook.com>
 *               |
 * Created On    : <2023-09-03>
 * Last Modified : <2025-07-21>
 * ------------------------------------------------------------*/

/**
 * @update 2024-04-18
 */
static Source_t os_ros_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},

  {&Ali,              "https://mirrors.aliyun.com",  DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn", DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com", DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com", DelegateToMirror},
  {&Huawei,           "https://mirrors.huaweicloud.com", DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com", DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com", DelegateToMirror}
};
def_sources_n(os_ros);


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

  chsrc_yield_source_and_confirm (os_ros);

  chsrc_backup (OS_ROS_SourceList);

  char *cmd  = NULL;
  cmd = xy_strjoin(3, "sed -E -i \'s@https?://.*/ros/ubuntu/?@", source.url, "/ros/ubuntu@g\' " OS_ROS_SourceList);
  chsrc_run(cmd, RunOpt_Default);

  cmd = "apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


Feature_t
os_ros_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = false;

  f.note = "该换源方案中，URL存在拼凑，因此不能手动使用某URL来换源";
  return f;
}

def_target_gsf(os_ros);
