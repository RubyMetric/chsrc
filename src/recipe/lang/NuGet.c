/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_nuget, "nuget/net/.net/dotnet");

void
pl_nuget_prelude ()
{
  chef_prep_this (pl_nuget, gs);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-04-18");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://www.nuget.org/api/v3/", DelegateToUpstream},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/nuget/v3", DelegateToMirror}
  def_sources_end()
}


void
pl_nuget_getsrc (char *option)
{
  chsrc_error ("暂时无法查看NuGet源，若有需求，请提交issue");
}


/**
 * 暂时未实现该换源功能，可参照 https://mirrors.huaweicloud.com/mirrorDetail/5ebf85de07b41baf6d0882ab?mirrorName=nuget&catalog=language
 */
void
pl_nuget_setsrc (char *option)
{
  chsrc_error ("暂时无法为NuGet换源，若有需求，请提交issue");
}
