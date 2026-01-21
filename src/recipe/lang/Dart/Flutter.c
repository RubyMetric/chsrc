/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_dart_flutter, "flutter");

void
pl_dart_flutter_prelude (void)
{
  chef_prep_this (pl_dart_flutter, gsr);

  chef_set_created_on   (this, "2023-09-10");
  chef_set_last_updated (this, "2025-07-11");
  chef_set_sources_last_updated (this, "2025-04-15");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@czyt", "@xrgzs");

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://storage.googleapis.com", FeedByPrelude},
  {&FlutterCN,        "https://storage.flutter-io.cn",  FeedByPrelude},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn",     FeedByPrelude }, /* 官方文档也没有给后缀，怀疑是否存在问题 */
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/flutter", FeedByPrelude},
  {&Nju,              "https://mirror.nju.edu.cn/flutter", FeedByPrelude}
  def_sources_end()

  chef_set_rest_smURL_with_postfix (this, "/flutter_infra_release/releases/stable/linux/flutter_linux_v1.0.0-stable.tar.xz");
}


/**
 * chsrc get flutter
 */
void
pl_dart_flutter_getsrc (char *option)
{
  chsrc_view_env ("FLUTTER_STORAGE_BASE_URL", NULL);
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

  char *w = NULL;
  char *cmd = NULL;
  if (xy.on_windows)
    {
      cmd = xy_strcat (3, "setx FLUTTER_STORAGE_BASE_URL \"", source.url, "\"");
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      char *zshrc  = xy_zshrc;
      char *bashrc = xy_bashrc;

      chsrc_backup (zshrc);
      w = xy_strcat (3, "export FLUTTER_STORAGE_BASE_URL=\"", source.url, "\"\n");
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
pl_dart_flutter_resetsrc (char *option)
{
  pl_dart_flutter_setsrc (option);
}
