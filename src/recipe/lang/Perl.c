/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-31>
 * Last Modified : <2024-08-09>
 * ------------------------------------------------------------*/

/**
 * @update 2024-05-24
 */
static SourceInfo
pl_perl_sources[] = {
  {&Upstream,       NULL},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CPAN/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CPAN/"},
  {&Bjtu,          "https://mirror.bjtu.edu.cn/cpan/"},
  {&Hust,          "https://mirrors.hust.edu.cn/CPAN/"},
  {&Ali,           "https://mirrors.aliyun.com/CPAN/"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/CPAN/"}
};
def_sources_n(pl_perl);


void
pl_perl_check_cmd ()
{
  chsrc_ensure_program ("perl");
}

void
pl_perl_getsrc (char *option)
{
  pl_perl_check_cmd ();
  // @ccmywish: 注意，prettyprint 仅仅是一个内部实现，可能不稳定，如果需要更稳定的，
  //            可以使用 CPAN::Shell->o('conf', 'urllist');
  //            另外，上述两种方法无论哪种，都要首先load()
  char *cmd = "perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->prettyprint('urllist')\" ";
  chsrc_run (cmd, RunOpt_Default);
}


/**
 * @consult https://help.mirrors.cernet.edu.cn/CPAN/
 */
void
pl_perl_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_perl);

  char *cmd = xy_strjoin (3,
  "perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('urllist', 'unshift', '", source.url, "'); CPAN::HandleConfig->commit()\"");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_note2 ("请您使用 perl -v 以及 cpan -v，若 Perl >= v5.36 或 CPAN >= 2.29，请额外手动调用下面的命令");
  puts ("perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('pushy_https', 0);; CPAN::HandleConfig->commit()\"");
  chsrc_conclude (&source, SetsrcType_SemiAuto);
}

def_target(pl_perl);
