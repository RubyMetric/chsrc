/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :    Zhao    <1792582687@qq.com>
 *               |
 * Created On    : <2023-08-30>
 * Last Modified : <2024-12-18>
 * ------------------------------------------------------------*/

static SourceProvider_t os_ubuntu_upstream =
{
  def_upstream, "http://archive.ubuntu.com/",
  // https://github.com/RubyMetric/chsrc/issues/121
  {NotSkip, NA, NA, "http://archive.ubuntu.com/ubuntu/dists/noble/Contents-amd64.gz"}
};

/**
 * @update 2024-11-21
 */
static Source_t os_ubuntu_sources[] =
{
  {&os_ubuntu_upstream, "http://archive.ubuntu.com/ubuntu/"}, // 不支持https
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/ubuntu/"},
  {&Ali,              "https://mirrors.aliyun.com/ubuntu"},
  {&Volcengine,       "https://mirrors.volces.com/ubuntu"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/ubuntu"},
  {&Ustc,             "https://mirrors.ustc.edu.cn/ubuntu"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/ubuntu"},
  {&Tencent,          "https://mirrors.tencent.com/ubuntu"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/ubuntu"},
  {&Huawei,           "https://mirrors.huaweicloud.com/ubuntu"},
  {&Netease,          "https://mirrors.163.com/ubuntu"},
  {&Sohu,             "https://mirrors.sohu.com/ubuntu"}
};
def_sources_n(os_ubuntu);


void
os_ubuntu_getsrc (char *option)
{
  if (chsrc_check_file (OS_Ubuntu_SourceList_DEB822))
    {
      chsrc_view_file (OS_Ubuntu_SourceList_DEB822);
      return;
    }

  if (chsrc_check_file (OS_Apt_SourceList))
    {
      chsrc_view_file (OS_Apt_SourceList);
      return;
    }

  char *msg = CliOpt_InEnglish ? "Source config file missing! However, you can still run `chsrc set ubuntu` to add and use new sources"
                               : "缺少源配置文件！但仍可直接通过 chsrc set ubuntu 来添加使用新的源";
  chsrc_error2 (msg);
  return;
}


/**
 * 此函数基本和 os_ubuntu_setsrc() 一致
 */
void
os_ubuntu_setsrc_for_deb822 (char *option)
{
  chsrc_yield_source_and_confirm (os_ubuntu);

  chsrc_backup (OS_Ubuntu_SourceList_DEB822);

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (strncmp (arch, "x86_64", 6)==0)
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Ubuntu_SourceList_DEB822);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Ubuntu_SourceList_DEB822);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Auto;
  chsrc_conclude (&source);
}

/**
 * @note 不同架构下换源不一样
 */
void
os_ubuntu_setsrc (char *option)
{
  chsrc_ensure_root ();

  if (chsrc_check_file (OS_Ubuntu_SourceList_DEB822))
    {
      char *msg = CliOpt_InEnglish ? "Will change source based on new format"
                                   : "将基于新格式换源";
      chsrc_note2 (msg);
      os_ubuntu_setsrc_for_deb822 (option);
      return;
    }

  bool sourcelist_exist = ensure_apt_sourcelist (OS_Is_Ubuntu);

  chsrc_yield_source_and_confirm (os_ubuntu);

  // 不存在的时候，用的是我们生成的无效文件，不要备份
  if (sourcelist_exist)
    {
      chsrc_backup (OS_Apt_SourceList);
    }

  char *arch = chsrc_get_cpuarch ();
  char *cmd  = NULL;
  if (0==strncmp (arch, "x86_64", 6))
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu/?@", source.url, "@g\' " OS_Apt_SourceList);
    }
  else
    {
      cmd = xy_strjoin (3, "sed -E -i \'s@https?://.*/ubuntu-ports/?@", source.url, "-ports@g\' " OS_Apt_SourceList);
    }

  chsrc_run (cmd, RunOpt_Default);
  chsrc_run ("apt update", RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ChgType_Auto;
  chsrc_conclude (&source);
}


void
os_ubuntu_resetsrc (char *option)
{
  os_ubuntu_setsrc (option);
}


Feature_t
os_ubuntu_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(os_ubuntu);
