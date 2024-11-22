/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :    czyt    <czyt.go@gmail.com>
 *                |
 * Created On     : <2023-08-30>
 * Major Revision :      1
 * Last Modified  : <2024-10-31>
 * ------------------------------------------------------------*/

static MirrorSite_t GoProxyCN =
{
  "goproxy.cn", "Goproxy.cn", "Goproxy.cn (七牛云)", "https://goproxy.cn/",
  {NotSkip, NA, NA, "https://goproxy.cn/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"} // 30 MB
},

GoProxyIO =
{
  "goproxy.io", "GOPROXY.IO", "GOPROXY.IO", "https://goproxy.io/",
  {NotSkip, NA, NA, "https://goproxy.io/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"} // 30 MB
};


/**
 * @update 2024-10-31
 * @note 缺少教育网软件源
 */
static Source_t pl_go_sources[] =
{
  {&UpstreamProvider, "https://proxy.golang.org"},
  {&GoProxyCN,        "https://goproxy.cn"},
  {&Ali,              "https://mirrors.aliyun.com/goproxy/"},
  {&Huawei,           "https://mirrors.huaweicloud.com/goproxy/"},
  {&GoProxyIO,        "https://goproxy.io"}
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
  ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
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
