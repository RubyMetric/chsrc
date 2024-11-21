/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :    czyt    <czyt.go@gmail.com>
 *                |
 * Created On     : <2023-09-10>
 * Major Revision :      3
 * Last Modified  : <2024-10-31>
 *
 * Dart Pub 软件仓库
 * ------------------------------------------------------------*/

/**
 * @update 2024-10-31
 */
static SourceInfo
pl_dart_sources[] = {
  {&UpstreamProvider,      "https://pub.dev"},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/dart-pub"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/dart-pub"},
  {&Nju,           "https://mirror.nju.edu.cn/dart-pub"}
};
def_sources_n(pl_dart);


void
pl_dart_getsrc (char *option)
{
  char *cmd = NULL;
  if (xy_on_windows)
    {
      // cmd = "set PUB_HOSTED_URL & set FLUTTER_STORAGE_BASE_URL";
      cmd = "set PUB_HOSTED_URL";
      chsrc_run (cmd, RunOpt_Default);
    }
  else
    {
      cmd = "echo $PUB_HOSTED_URL";
      chsrc_run (cmd, RunOpt_Default);
    }
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

  if (xy_on_windows)
    {
      w = xy_strjoin (3, "$env:PUB_HOSTED_URL = \"", source.url, "\"\n");

      if (xy_file_exist (xy_win_powershell_profile))
        chsrc_append_to_file (w, xy_win_powershell_profile);

      if (xy_file_exist (xy_win_powershellv5_profile))
        chsrc_append_to_file (w, xy_win_powershellv5_profile);
    }
  else
    {
      char *zshrc  = "~/.zshrc";
      char *bashrc = "~/.bashrc";

      chsrc_backup (zshrc);
      w = xy_strjoin (3, "export PUB_HOSTED_URL=\"", source.url, "\"\n");

      chsrc_append_to_file (w, zshrc);

      if (xy_file_exist (bashrc))
        {
          chsrc_backup (bashrc);
          chsrc_append_to_file (w, bashrc);
        }
    }
  chsrc_conclude (&source, SetsrcType_Auto);
}


void
pl_dart_resetsrc (char *option)
{
  pl_dart_setsrc (SetsrcType_Reset);
}


FeatInfo
pl_dart_feat (char *option)
{
  FeatInfo f = {0};

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
