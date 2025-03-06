/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-06-08>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @update 2024-06-08
 */
static Source_t wr_cocoapods_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/git/CocoaPods/Specs.git"},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/git/CocoaPods/Specs.git"}
};
def_sources_n(wr_cocoapods);


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/CocoaPods/
 */
void
wr_cocoapods_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_cocoapods);

  chsrc_note2 ("请手动执行以下命令:");
  p("cd ~/.cocoapods/repos");
  p("pod repo remove master");
  char *git_cmd = xy_strjoin (3, "git clone ", source.url, " master");
  p(git_cmd);
  br();

  chsrc_note2 ("最后进入项目工程目录，在Podfile中第一行加入:");
  char *source_str = xy_strjoin (3, "source '", source.url, "'");
  p(source_str);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}

def_target_s (wr_cocoapods);
