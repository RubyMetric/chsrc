/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_dart, "dart/pub");

void
pl_dart_prelude (void)
{
  chef_prep_this (pl_dart, gsr);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-04-15");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@czyt", "@xrgzs");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://pub.dev",           FeedByPrelude},
  {&FlutterCN,        "https://pub.flutter-io.cn", FeedByPrelude},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/dart-pub", FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/dart-pub", FeedByPrelude},
  {&Nju,              "https://mirror.nju.edu.cn/dart-pub", FeedByPrelude}
  def_sources_end()

  chef_set_all_smURL_with_postfix (this, "/packages/flutter_vision/versions/1.1.4.tar.gz");
}


void
pl_dart_getsrc (char *option)
{
  chsrc_view_env ("PUB_HOSTED_URL", NULL);
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

  char *w = NULL;
  char *cmd = NULL;
  if (xy.on_windows)
    {
      cmd = xy_strcat (3, "setx PUB_HOSTED_URL \"", source.url, "\"");
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      char *zshrc  = xy_zshrc;
      char *bashrc = xy_bashrc;

      chsrc_backup (zshrc);
      w = xy_strcat (3, "export PUB_HOSTED_URL=\"", source.url, "\"\n");

      chsrc_append_to_file (w, zshrc);

      if (xy_file_exist (bashrc))
        {
          chsrc_backup (bashrc);
          chsrc_append_to_file (w, bashrc);
        }
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_dart_resetsrc (char *option)
{
  pl_dart_setsrc (option);
}
