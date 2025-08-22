/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static MirrorSite_t Api7 =
{
  IS_DedicatedMirrorSite,
  "api7", "api7.ai", "深圳支流科技有限公司", "https://www.apiseven.com/",
  {SKIP, ToFill, ToFill, NULL, ROUGH}
};

def_target(pl_lua, "lua/luarocks");

void
pl_lua_prelude ()
{
  chef_prep_this (pl_lua, gs);

  chef_set_created_on   (this, "2023-09-27");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-07-14");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider,  NULL, DelegateToUpstream},
  {&Api7,             "https://luarocks.cn", DelegateToMirror}
  def_sources_end()
}


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
  use_this_source (pl_lua);

  char *config = xy_strcat (3, "rocks_servers = {\n"
                                "  \"", source.url, "\"\n"
                                "}");

  chsrc_note2 ("请手动修改 ~/.luarocks/config.lua 文件 (用于下载):");
  println (config);

  char *upload_config = xy_strcat (3, "key = \"<Your API Key>\"\n"
                                      "server = \"", source.url, "\"");

  chsrc_note2 ("请手动修改 ~/.luarocks/upload_config.lua 文件 (用于上传):");
  println (upload_config);

  chsrc_conclude (&source);
}
