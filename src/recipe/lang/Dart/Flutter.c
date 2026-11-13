/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_dart_flutter, "flutter");

void
pl_dart_flutter_prepare (void)
{
  chef_prep_this_dish (pl_dart_flutter, gsr);

  chef_set_recipe_created_on   (this, "2023-09-10");
  chef_set_recipe_last_updated (this, "2026-10-28");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 3, "@czyt", "@xrgzs", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_And_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_allow_english (this);
  chef_allow_user_define (this);

  def_sources_begin()
  {&UpstreamProvider, "https://storage.googleapis.com",              FeedByPrepare},
  {&FlutterCN,        "https://storage.flutter-io.cn",               FeedByPrepare},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn",                  FeedByPrepare}, /* 官方文档也没有给后缀，怀疑是否存在问题 */
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/flutter", FeedByPrepare},
  {&Nju,              "https://mirror.nju.edu.cn/flutter",           FeedByPrepare}
  def_sources_end()

  chef_set_rest_smURL_with_postfix (this, "/flutter_infra_release/releases/stable/linux/flutter_linux_v1.0.0-stable.tar.xz");
}


/**
 * chsrc get flutter
 */
void
pl_dart_flutter_getsrc (char *option)
{
  chsrc_get_env ("FLUTTER_STORAGE_BASE_URL", NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/flutter
 * @consult https://mirror.sjtu.edu.cn/docs/flutter_infra
 * @consult https://mirror.nju.edu.cn/mirrorz-help/flutter/?mirror=NJU
 */
void
pl_dart_flutter_setsrc (char *option)
{
  chsrc_use_this_source (pl_dart_flutter);

  char *url = hp_remove_trailing_slash (source.url);
  chsrc_set_env ("FLUTTER_STORAGE_BASE_URL", url);
  free (url);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_dart_flutter_resetsrc (char *option)
{
  chsrc_set_env ("FLUTTER_STORAGE_BASE_URL", NULL);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
