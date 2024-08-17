/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-08-15>
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
  SourceInfo source;
  chsrc_yield_source (pl_dart);
  chsrc_confirm_source (&source);

  char *towrite = NULL;

  char *pub = xy_2strjoin(source.url, "dart-pub");
  char *flutter = xy_2strjoin(source.url, "flutter");

  if (xy_on_windows)
    {
      if (xy_file_exist (xy_win_powershell_profile))
        {
          towrite = xy_strjoin (4, "$env:PUB_HOSTED_URL = \"", pub, "\"");
          chsrc_append_to_file (towrite, xy_win_powershell_profile);

          towrite = xy_strjoin (4, "$env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\"");
          chsrc_append_to_file (towrite, xy_win_powershell_profile);
        }

      if (xy_file_exist (xy_win_powershellv5_profile))
        {
          towrite = xy_strjoin (4, "$env:PUB_HOSTED_URL = \"", pub, "\"");
          chsrc_append_to_file (towrite, xy_win_powershellv5_profile);

          towrite = xy_strjoin (4, "$env:FLUTTER_STORAGE_BASE_URL = \"", flutter, "\"");
          chsrc_append_to_file (towrite, xy_win_powershellv5_profile);
        }
    }
  else
    {
      towrite = xy_strjoin (3, "export PUB_HOSTED_URL=\"", pub, "\"");
      chsrc_append_to_file (towrite, "~/.bashrc >> ~/.zshrc");

      towrite = xy_strjoin (3, "export FLUTTER_STORAGE_BASE_URL=\"", flutter, "\"");
      chsrc_append_to_file (towrite, "~/.bashrc >> ~/.zshrc");
    }
  chsrc_say_lastly (&source, ChsrcTypeUntested);
}

def_target(pl_dart);
