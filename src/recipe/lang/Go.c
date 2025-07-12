/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :    czyt    <czyt.go@gmail.com>
*                 |  Rui Yang   <techoc@foxmail.com>
 *                |
 * Created On     : <2023-08-30>
 * Major Revision :      1
 * Last Modified  : <2025-07-12>
 * ------------------------------------------------------------*/

static SourceProvider_t pl_go_upstream =
{
  def_upstream, "https://pkg.go.dev/",
  def_need_measure_info
};

static MirrorSite_t GoProxyCN =
{
  IS_DedicatedMirrorSite,
  "goproxy.cn", "Goproxy.cn", "Goproxy.cn (七牛云)", "https://goproxy.cn/",
  {NotSkip, NA, NA, "https://goproxy.cn/github.com/aws/aws-sdk-go/@v/v1.45.2.zip", ACCURATE} // 30 MB
},

GoProxyIO =
{
  IS_DedicatedMirrorSite,
  "goproxy.io", "GOPROXY.IO", "GOPROXY.IO", "https://goproxy.io/",
  {NotSkip, NA, NA, "https://goproxy.io/github.com/aws/aws-sdk-go/@v/v1.45.2.zip", ACCURATE} // 30 MB
};


/**
 * @update 2024-12-18
 * @note 缺少教育网软件源
 */
static Source_t pl_go_sources[] =
{
  {&pl_go_upstream, "https://proxy.golang.org", NULL},
  {&GoProxyCN,        "https://goproxy.cn", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/goproxy/", NULL},
  // 注释华为镜像源 详见 https://github.com/RubyMetric/chsrc/issues/227
  // {&Huawei,           "https://mirrors.huaweicloud.com/goproxy/", NULL},
  {&GoProxyIO,        "https://goproxy.io", DelegateToMirror}
};
def_sources_n(pl_go);



void
pl_go_check_cmd ()
{
  char *check_cmd = xy_str_to_quietcmd ("go version");
  bool exist = query_program_exist (check_cmd, "go", Noisy_When_Exist|Noisy_When_NonExist);

  if (!exist)
    {
      chsrc_error ("未找到 go 相关命令，请检查是否存在");
      exit (Exit_UserCause);
    }
}


void
pl_go_getsrc (char *option)
{
  pl_go_check_cmd ();
  chsrc_run ("go env GOPROXY", RunOpt_Default);
}


/**
 * chsrc set go
 *
 * @consult https://goproxy.cn/
 */
void
pl_go_setsrc (char *option)
{
  pl_go_check_cmd ();

  chsrc_yield_source_and_confirm (pl_go);

  char *cmd = "go env -w GO111MODULE=on";
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strjoin (3, "go env -w GOPROXY=", source.url, ",direct");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

/**
 * chsrc reset go
 */
void
pl_go_resetsrc (char *option)
{
  pl_go_setsrc (option);
}


/**
 * chsrc ls go
 */
Feature_t
pl_go_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = false;

  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsrf(pl_go);
