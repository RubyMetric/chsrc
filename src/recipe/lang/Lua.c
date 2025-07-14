/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-27>
 * Last Modified : <2025-07-14>
 * ------------------------------------------------------------*/

static MirrorSite_t Api7 =
{
  IS_DedicatedMirrorSite,
  "api7", "api7.ai", "深圳支流科技有限公司", "https://www.apiseven.com/",
  {SKIP, ToFill, ToFill, NULL, ROUGH}
};


/**
 * @update 2025-07-14
 * @note 目前只有一个源
 */
static Source_t pl_lua_sources[] =
{
  {&UpstreamProvider, NULL, NULL},
  {&Api7,             "https://luarocks.cn", NULL},
};
def_sources_n(pl_lua);


void
pl_lua_getsrc (char *option)
{
  chsrc_view_file ("~/.luarocks/config.lua");
  chsrc_view_file ("~/.luarocks/upload_config.lua");
}

/**
 * @consult https://luarocks.cn/
 */
void
pl_lua_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_lua);

  char *config = xy_strjoin (3, "rocks_servers = {\n"
                                "  \"", source.url, "\"\n"
                                "}");

  chsrc_note2 ("请手动修改 ~/.luarocks/config.lua 文件 (用于下载):");
  println (config);

  char *upload_config = xy_strjoin (3, "key = \"<Your API Key>\"\n"
                                      "server = \"", source.url, "\"");

  chsrc_note2 ("请手动修改 ~/.luarocks/upload_config.lua 文件 (用于上传):");
  println (upload_config);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


Feature_t
pl_lua_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NA;
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

def_target_gsf(pl_lua);
