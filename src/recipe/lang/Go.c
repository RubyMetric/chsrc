/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static MirrorSite_t GoProxyCN =
{
  "goproxy.cn", "Goproxy.cn", "Goproxy.cn (七牛云)", "https://goproxy.cn/",
  "https://goproxy.cn/github.com/aws/aws-sdk-go/@v/v1.45.2.zip", // 30 MB
  ACCURATE
},

GoProxyIO =
{
  "goproxy.io", "GOPROXY.IO", "GOPROXY.IO", "https://goproxy.io/",
  "https://goproxy.io/github.com/aws/aws-sdk-go/@v/v1.45.2.zip", // 30 MB
  ACCURATE
};

def_target(pl_go);

void
pl_go_prelude ()
{
  use_this(pl_go);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-12");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 2,
    "czyt",     "czyt.go@gmail.com",
    "Rui Yang", "techoc@foxmail.com");

  chef_allow_get();
  chef_allow_set();
  chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);


  def_sources_begin()
  {&upstream,       "https://proxy.golang.org", DelegateToUpstream},
  {&GoProxyCN,      "https://goproxy.cn", DelegateToMirror},
  {&Ali,            "https://mirrors.aliyun.com/goproxy/", DelegateToMirror},

  // 暂时停用华为镜像源, 详见 https://github.com/RubyMetric/chsrc/issues/227
  // {&Huawei,      "https://mirrors.huaweicloud.com/goproxy/", DelegateToMirror},

  {&GoProxyIO,      "https://goproxy.io", DelegateToMirror}
  def_sources_end()
}



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

  chsrc_conclude (&source);
}


void
pl_go_resetsrc (char *option)
{
  pl_go_setsrc (option);
}
