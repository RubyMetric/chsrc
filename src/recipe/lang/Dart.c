/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-08-28>
 * Revision      :      2
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-10 更新
 * @note {
 *   我们这里挑选的必须也支持 Flutter
 *   我们将会在setsrc函数中生成
 *     1. https://mirrors.tuna.tsinghua.edu.cn/dart-pub
 *     2. https://mirrors.tuna.tsinghua.edu.cn/flutter
 * }
 */
static SourceInfo
pl_dart_sources[] = {
  {&Upstream,       NULL},
  // {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/"}, // 不确定SJTUG的flutter镜像给的对不对
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/"},
  {&Nju,           "https://mirror.nju.edu.cn/"}
};
def_sources_n(pl_dart);


void
pl_dart_getsrc (char *option)
{
  char *cmd = NULL;
  if (xy_on_windows)
    {
      cmd = "set PUB_HOSTED_URL & set FLUTTER_STORAGE_BASE_URL";
      chsrc_run (cmd, RunOpt_Default);
    }
  else
    {
      cmd = "echo $PUB_HOSTED_URL; echo $FLUTTER_STORAGE_BASE_URL";
      chsrc_run (cmd, RunOpt_Default);
    }
}

/**
 * Dart pub 换源，参考：https://mirrors.tuna.tsinghua.edu.cn/help/dart-pub/
 */
void
pl_dart_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_dart);

  char *towrite1, *towrite2 = NULL;

  char *pub = xy_2strjoin (source.url, "dart-pub");
  char *flutter = xy_2strjoin (source.url, "flutter");

  if (xy_on_windows)
    {
      towrite1 = xy_strjoin (3, "$env:PUB_HOSTED_URL = \"", pub, "\"");
      towrite2 = xy_strjoin (3, "$env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\"");

      if (xy_file_exist (xy_win_powershell_profile))
        {
          chsrc_append_to_file (towrite1, xy_win_powershell_profile);
          chsrc_append_to_file (towrite2, xy_win_powershell_profile);
        }

      if (xy_file_exist (xy_win_powershellv5_profile))
        {
          chsrc_append_to_file (towrite1, xy_win_powershellv5_profile);
          chsrc_append_to_file (towrite2, xy_win_powershellv5_profile);
        }
    }
  else
    {
      char *zshrc  = "~/.zshrc";
      char *bashrc = "~/.bashrc";

      chsrc_backup (zshrc);
      towrite1 = xy_strjoin (3, "export PUB_HOSTED_URL=\"", pub, "\"");
      towrite2 = xy_strjoin (3, "export FLUTTER_STORAGE_BASE_URL=\"", flutter, "\"");
      chsrc_append_to_file (towrite1, zshrc);
      chsrc_append_to_file (towrite2, zshrc);

      if (xy_file_exist (bashrc))
        {
          chsrc_backup (bashrc);
          chsrc_append_to_file (towrite1, bashrc);
          chsrc_append_to_file (towrite2, bashrc);
        }
    }
  chsrc_conclude (&source, ChsrcTypeUntested);
}


FeatInfo
pl_dart_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = false;

  fi.stcan_locally = CanNot;
  fi.locally = NULL;
  fi.can_english = true;

  /* 该换源方案中，URL存在拼凑，因此不能让用户手动使用某URL来换源 */
  fi.can_user_define = false;

  fi.note = "该换源通过写入环境变量实现，若多次换源，请手动清理profile文件";
  return fi;
}

def_target_gsf(pl_dart);
