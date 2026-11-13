/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_dart, "dart/pub");

void
pl_dart_prepare (void)
{
  chef_prep_this_dish (pl_dart, gsr);

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
  {&UpstreamProvider, "https://pub.dev",                               FeedByPrepare},
  {&FlutterCN,        "https://pub.flutter-io.cn",                     FeedByPrepare},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/dart-pub",           FeedByPrepare},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/dart-pub", FeedByPrepare},
  {&Nju,              "https://mirror.nju.edu.cn/dart-pub",            FeedByPrepare}
  def_sources_end()

  chef_set_rest_smURL_with_postfix (this, "/packages/flutter_vision/versions/1.1.4.tar.gz");
}


void
pl_dart_getsrc (char *option)
{
  chsrc_get_env ("PUB_HOSTED_URL", NULL);
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/dart-pub/
 * @consult https://mirror.sjtu.edu.cn/docs/dart-pub
 * @consult https://mirror.nju.edu.cn/mirrorz-help/dart-pub/?mirror=NJU
 */
void
pl_dart_setsrc (char *option)
{
  chsrc_use_this_source (pl_dart);

  char *url = hp_remove_trailing_slash (source.url);
  chsrc_set_env ("PUB_HOSTED_URL", url);
  free (url);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_dart_resetsrc (char *option)
{
  chsrc_set_env ("PUB_HOSTED_URL", NULL);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
