/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-30>
 * Last Modified : <2024-08-09>
 * ------------------------------------------------------------*/

static MirrorSite
GoProxyCN = {"goproxy.cn",   "Goproxy.cn",   "Goproxy.cn (七牛云)",   "https://goproxy.cn/",
             "https://goproxy.cn/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"},  // 30 MB

GoProxyIO = {"goproxy.io",   "GOPROXY.IO",   "GOPROXY.IO",         "https://goproxy.io/",
             "https://goproxy.io/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"};   // 30 MB


/**
 * @time 2024-04-18 更新
 * @note 缺少教育网软件源
 */
static SourceInfo
pl_go_sources[] = {
  {&Upstream,       "https://proxy.golang.org,direct"},
  {&GoProxyCN,     "https://goproxy.cn"},
  {&Ali,           "https://mirrors.aliyun.com/goproxy/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/goproxy/"},
  {&GoProxyIO,     "https://goproxy.io"}
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
 * Go换源，参考：https://goproxy.cn/
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
  chsrc_conclude (&source, SetsrcType_Auto);
}

def_target(pl_go);
