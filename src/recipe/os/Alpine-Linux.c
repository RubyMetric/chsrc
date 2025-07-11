/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : Yangmoooo <yangmoooo@outlook.com>
 *               |
 * Created On    : <2023-09-24>
 * Last Modified : <2024-12-18>
 * ------------------------------------------------------------*/

static SourceProvider_t os_alpine_upstream =
{
  def_upstream, "http://dl-cdn.alpinelinux.org/alpine",
  {NotSkip, NA, NA, "https://dl-cdn.alpinelinux.org/alpine/latest-stable/releases/x86_64/alpine-standard-3.21.0-x86_64.iso", ACCURATE}
};


/**
 * @update 2024-09-14
 */
static Source_t os_alpine_sources[] =
{
  {&os_alpine_upstream, "http://dl-cdn.alpinelinux.org/alpine"},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/alpine"},
  {&Sjtug_Zhiyuan,    "https://mirrors.sjtug.sjtu.edu.cn/alpine"},
  {&Sustech,          "https://mirrors.sustech.edu.cn/alpine"},
  {&Zju,              "https://mirrors.zju.edu.cn/alpine"},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/alpine"},
  {&Ali,              "https://mirrors.aliyun.com/alpine"},
  {&Tencent,          "https://mirrors.cloud.tencent.com/alpine"},
  // {&Tencent_Intra, "https://mirrors.cloud.tencentyun.com/alpine"},
  {&Huawei,           "https://mirrors.huaweicloud.com/alpine"}
};
def_sources_n(os_alpine);


void
os_alpine_getsrc (char *option)
{
  chsrc_view_file ("/etc/apk/repositories");
}

/**
 * @consult https://help.mirrors.cernet.edu.cn/alpine/
 */
void
os_alpine_setsrc (char *option)
{
  // chsrc_ensure_root(); // HELP: 不确定是否需要root

  chsrc_yield_source_and_confirm (os_alpine);

  char* cmd = xy_strjoin (3,
            "sed -i 's#https\\?://dl-cdn.alpinelinux.org/alpine#", source.url, "#g' /etc/apk/repositories"
            );
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("apk update", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}

def_target(os_alpine);
