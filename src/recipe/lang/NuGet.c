/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_nuget);

void
pl_nuget_prelude ()
{
  use_this(pl_nuget);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2024-04-18");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_get();
  chef_allow_set();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note ("暂时未实现该换源功能，可参照 https://mirrors.huaweicloud.com/mirrorDetail/5ebf85de07b41baf6d0882ab?mirrorName=nuget&catalog=language", NULL);

  def_sources_begin()
  {&upstream,         "https://www.nuget.org/api/v3/", DelegateToUpstream},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/nuget/v3", DelegateToMirror}
  def_sources_end()
}


void
pl_nuget_getsrc (char *option)
{
  chsrc_error ("暂时无法查看NuGet源，若有需求，请提交issue");
}


void
pl_nuget_setsrc (char *option)
{
  chsrc_error ("暂时无法为NuGet换源，若有需求，请提交issue");
}
