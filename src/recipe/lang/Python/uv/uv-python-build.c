/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

/**
 * 控制 uv 如何下载 Python 本身
 *
 * @consult https://docs.astral.sh/uv/reference/settings/#python-install-mirror
 *
 * 针对上游仓库 python-build-standalone 的镜像。
 */

def_dish(pl_uv_python_build, "uv-python-build");

// CNB (Cloud Native Build, 腾讯) 托管的 python-build-standalone releases 镜像
static MirrorSite_t CnbUvPython =
{
  IS_DedicatedMirrorSite,
  "cnb", "CNB", "Cloud Native Build (腾讯)", "https://cnb.cool/",
  {NotSkip, NA, NA, "https://cnb.cool/astral-sh/python-build-standalone/-/releases/download/20260728/cpython-3.15.0b4+20260728-x86_64_v4-unknown-linux-gnu-freethreaded-install_only.tar.gz", ACCURATE}
};

void
pl_uv_python_build_prepare (void)
{
  chef_prep_this_dish (pl_uv_python_build, gsr);

  chef_set_recipe_created_on   (this, "2026-08-02");
  chef_set_recipe_last_updated (this, "2026-08-13");

  chef_set_chefs (this, 2, "@Mikachu2333", "@ccmywish");
  chef_set_sauciers (this, 0);

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_But_Not_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_deny_english (this);
  chef_deny_user_define (this);
  this->can_user_define_explain =
    CHINESE ? "python-install-mirror 不支持手动指定，chsrc 会自动选择"
            : "python-install-mirror cannot be specified manually; chsrc selects it automatically";

  def_sources_begin ()
  {&UpstreamProvider, "https://github.com/astral-sh/python-build-standalone/releases/download",       DelegateToUpstream},
  {&Nju,              "https://mirrors.nju.edu.cn/github-release/astral-sh/python-build-standalone",  FeedByPrepare},
  {&Ustc,             "https://mirrors.ustc.edu.cn/github-release/astral-sh/python-build-standalone", FeedByPrepare},
  {&Lzuoss,           "https://mirror.lzu.edu.cn/github-release/astral-sh/python-build-standalone",   FeedByPrepare},
  {&CnbUvPython,      "https://cnb.cool/astral-sh/python-build-standalone/-/releases/download",       FeedByPrepare}
  def_sources_end ()

#define GH_SM_POSTFIX  "/20260728/cpython-3.14.6+20260728-i686-pc-windows-msvc-install_only_stripped.tar.gz"
  chef_set_smURL_with_postfix (this, &Nju,         GH_SM_POSTFIX);
  chef_set_smURL_with_postfix (this, &Lzuoss,      GH_SM_POSTFIX);
  chef_set_smURL_with_postfix (this, &CnbUvPython, GH_SM_POSTFIX);
#undef GH_SM_POSTFIX

  // 2026-5-31: USTC 仅保留 Latest, 只能用 SHA256SUMS 粗略测速
  chef_set_smURL_with_postfix (this, &Ustc, "/LatestRelease/SHA256SUMS");

  // 中科大仅保留 Latest 且文件内含动态版本号, 使用模糊测速
  chef_set_provider_sm_accuracy (&Ustc, ROUGH);
}


void
pl_uv_python_build_getsrc (char *option)
{
  char *uv_config = pl_uv_find_uv_config (false);

  if (!uv_config || !chsrc_check_file (uv_config))
    {
      if (!uv_config)
        chsrc_error2 ("无法获取 uv 配置文件路径");
      else
        chsrc_error2 ("未找到 uv 配置文件");
      return;
    }

  // uv.toml 与 pyproject.toml 均使用同一套受限 TOML 读取逻辑。
  char *content = xy_file_read (xy_normalize_path (uv_config));
  if (!content)
    {
      chsrc_error2 ("无法读取 uv 配置文件");
      return;
    }

  bool pyproject = xy_str_end_with (uv_config, PL_uv_PyprojectConfigFile);
  const char *parent_table = pyproject ? "[tool.uv]" : NULL;

  char *mirror = pl_uv_toml_get_value_in_table (content, "python-install-mirror", parent_table);
  if (mirror)
    {
      println (mirror);
    }
  else
    {
      chsrc_note2 ("uv 中未配置 python-install-mirror，显示默认上游源：");
      Source_t default_source = chsrc_yield_source (&pl_uv_python_build_dish, "upstream");
      println (default_source.url);
    }
}


void
pl_uv_python_build_setsrc (char *option)
{
  char *uv_config = pl_uv_find_uv_config (true);
  if (!uv_config)
    {
      chsrc_error2 ("无法获取 uv 配置文件路径");
      return;
    }

  char *content = xy_file_read (xy_normalize_path (uv_config));
  if (!content)
    {
      chsrc_error2 ("无法读取 uv 配置文件");
      return;
    }


  chsrc_use_this_source (pl_uv_python_build);

  bool pyproject = xy_str_end_with (uv_config, PL_uv_PyprojectConfigFile);
  const char *parent_table = pyproject ? "[tool.uv]" : NULL;

  char *updated = pl_uv_toml_replace_key_value (content, "python-install-mirror", source.url, parent_table);

  chsrc_backup (uv_config);
  chsrc_overwrite_file (updated, uv_config);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}



void
pl_uv_python_build_resetsrc (char *option)
{
  pl_uv_python_build_setsrc (option);
}
