/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-08-09>
 * ------------------------------------------------------------*/

static MirrorSite_t NugetOrg =
{
  IS_UpstreamProvider,
  "nuget.org", "NuGet Org", "Nuget Organization", "https://www.nuget.org/",
  {SKIP, ToFill, ToFill, NULL, ROUGH}
};

/**
 * @update 2024-04-18
 * @note {
 *   暂时未实现该换源功能，可参照
 *   https://mirrors.huaweicloud.com/mirrorDetail/5ebf85de07b41baf6d0882ab?mirrorName=nuget&catalog=language
 * }
 */
static Source_t pl_nuget_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&NugetOrg,         "https://www.nuget.org/api/v2/"},
  {&Huawei,           "https://mirrors.huaweicloud.com/repository/nuget/v3"}
};
def_sources_n(pl_nuget);

void
pl_nuget_getsrc (char *option)
{
  chsrc_error ("暂时无法查看NuGet源，若有需求，请提交issue");
}

/**
 * NuGet 换源
 */
void
pl_nuget_setsrc (char *option)
{
  chsrc_error ("暂时无法为NuGet换源，若有需求，请提交issue");
}


def_target_s (pl_nuget);
