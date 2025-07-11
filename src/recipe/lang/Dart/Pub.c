/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng  <ccmywish@qq.com>
 * Contributors   :    czyt     <czyt.go@gmail.com>
 *                | MadDogOwner <xiaoran@xrgzs.top>
 *                |
 * Created On     : <2023-09-10>
 * Major Revision :      3
 * Last Modified  : <2025-07-11>
 *
 * Dart Pub 软件仓库
 * ------------------------------------------------------------*/

#define PL_Dart_Pub_Speed_URL_Postfix "/packages/flutter_vision/versions/1.1.4.tar.gz

/**
 * @update 2025-04-15
 */
 static SourceProvider_t pl_dart_upstream =
{
  def_upstream, "https://pub.dev",
  {NotSkip, NA, NA, "https://pub.dev/packages/flutter_vision/versions/1.1.4.tar.gz", ACCURATE} // 37.05 MB
};

static Source_t pl_dart_sources[] =
{
  {&pl_dart_upstream, "https://pub.dev", DelegateToUpstream},

  {&FlutterCN,        "https://pub.flutter-io.cn",
                      "https://pub.flutter-io.cn" PL_Dart_Pub_Speed_URL_Postfix},

  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/dart-pub",
                      "https://mirror.sjtu.edu.cn/dart-pub" PL_Dart_Pub_Speed_URL_Postfix},

  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/dart-pub",
                      "https://mirrors.tuna.tsinghua.edu.cn/dart-pub" PL_Dart_Pub_Speed_URL_Postfix},

  {&Nju,              "https://mirror.nju.edu.cn/dart-pub",
                      "https://mirror.nju.edu.cn/dart-pub" PL_Dart_Pub_Speed_URL_Postfix}
};
def_sources_n(pl_dart);


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
  chsrc_yield_source_and_confirm (pl_dart);

  char *w = NULL;
  char *cmd = NULL;
  if (xy_on_windows)
    {
      cmd = xy_strjoin (3, "setx PUB_HOSTED_URL \"", source.url, "\"");
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      char *zshrc  = xy_zshrc;
      char *bashrc = xy_bashrc;

      chsrc_backup (zshrc);
      w = xy_strjoin (3, "export PUB_HOSTED_URL=\"", source.url, "\"\n");

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


Feature_t
pl_dart_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = true;

  f.can_user_define = true;

  f.note = "该换源通过写入环境变量实现，若多次换源，请手动清理profile文件";
  return f;
}

def_target_gsrf(pl_dart);
