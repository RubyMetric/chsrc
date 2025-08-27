/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

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

def_target(pl_go, "go/golang/goproxy");

void
pl_go_prelude ()
{
  chef_prep_this (pl_go, gsr);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-12");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@czyt", "@techoc");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);


  def_sources_begin()
  {&UpstreamProvider, "https://proxy.golang.org", DelegateToUpstream},
  {&GoProxyCN,        "https://goproxy.cn", DelegateToMirror},
  {&Ali,              "https://mirrors.aliyun.com/goproxy/", DelegateToMirror},

  // 暂时停用华为镜像源, 详见 https://github.com/RubyMetric/chsrc/issues/227
  // {&Huawei,        "https://mirrors.huaweicloud.com/goproxy/", DelegateToMirror},

  {&GoProxyIO,        "https://goproxy.io", DelegateToMirror}
  def_sources_end()
}



void
pl_go_check_cmd ()
{
  char *check_cmd = xy_quiet_cmd ("go version");
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
 * @consult https://goproxy.cn/
 */
void
pl_go_setsrc (char *option)
{
  pl_go_check_cmd ();

  chsrc_use_this_source (pl_go);

  char *cmd = "go env -w GO111MODULE=on";
  chsrc_run (cmd, RunOpt_Default);

  cmd = xy_strcat (3, "go env -w GOPROXY=", source.url, ",direct");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_conclude (&source);
}


void
pl_go_resetsrc (char *option)
{
  pl_go_setsrc (option);
}
