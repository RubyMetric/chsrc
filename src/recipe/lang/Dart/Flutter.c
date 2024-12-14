/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :    czyt    <czyt.go@gmail.com>
 *                |
 * Created On     : <2023-09-10>
 * Major Revision :      2
 * Last Modified  : <2024-11-22>
 *
 * 2024-09-14: 不得不将Dart和Flutter拆分为两个Target，
 *             因为3家教育网镜像站给出的 Dart 和 Flutter 换源URL模式都不一样
 * ------------------------------------------------------------*/

/**
 * @update 2024-10-31
 */
static Source_t pl_dart_flutter_sources[] =
{
  {&UpstreamProvider, "https://storage.googleapis.com"},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn"}, // 没有后缀，怀疑是否存在问题
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/git/flutter"}, // URL 带 git
  {&Nju,              "https://mirror.nju.edu.cn/flutter"}
};
def_sources_n(pl_dart_flutter);


/**
 * chsrc get flutter
 */
void
pl_dart_flutter_getsrc (char *option)
{
  char *cmd = NULL;
  if (xy_on_windows)
    {
      // cmd = "set PUB_HOSTED_URL & set FLUTTER_STORAGE_BASE_URL";
      cmd = "set FLUTTER_STORAGE_BASE_URL";
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
  else
    {
      cmd = "echo $FLUTTER_STORAGE_BASE_URL";
      chsrc_run (cmd, RunOpt_No_Last_New_Line);
    }
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/flutter
 * @consult https://mirror.sjtu.edu.cn/docs/flutter_infra
 * @consult https://mirror.nju.edu.cn/mirrorz-help/flutter/?mirror=NJU
 */
void
pl_dart_flutter_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_dart_flutter);

  char *w = NULL;

  if (xy_on_windows)
    {
      w = xy_strjoin (3, "$env:FLUTTER_STORAGE_BASE_URL = \"", source.url, "\"\n");

      if (xy_file_exist (xy_win_powershell_profile))
        chsrc_append_to_file (w, xy_win_powershell_profile);

      if (xy_file_exist (xy_win_powershellv5_profile))
        chsrc_append_to_file (w, xy_win_powershellv5_profile);
    }
  else
    {
      char *zshrc  = xy_zshrc;
      char *bashrc = xy_bashrc;

      chsrc_backup (zshrc);
      w = xy_strjoin (3, "export FLUTTER_STORAGE_BASE_URL=\"", source.url, "\"\n");
      chsrc_append_to_file (w, zshrc);

      if (xy_file_exist (bashrc))
        {
          chsrc_backup (bashrc);
          chsrc_append_to_file (w, bashrc);
        }
    }

  ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
  chsrc_conclude (&source);
}


void
pl_dart_flutter_resetsrc (char *option)
{
  pl_dart_flutter_setsrc (option);
}


/**
 * chsrc ls flutter
 */
Feature_t
pl_dart_flutter_feat (char *option)
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

def_target_gsrf(pl_dart_flutter);
