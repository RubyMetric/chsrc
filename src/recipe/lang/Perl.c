/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_perl, "perl/cpan");

void
pl_perl_prelude ()
{
  chef_prep_this (pl_perl, gs);

  chef_set_created_on   (this, "2023-09-31");
  chef_set_last_updated (this, "2025-10-07");
  chef_set_sources_last_updated (this, "2025-10-07");

  chef_set_chef     (this, NULL);
  chef_set_cooks    (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@hezonglun", "@Mikachu2333");

  chef_allow_local_mode  (this, CanNot, NULL, NULL);
  chef_deny_english    (this);
  chef_allow_user_define (this);

  def_sources_begin()
  {&UpstreamProvider, "https://www.cpan.org/",                      FeedByPrelude},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/CPAN/",        FeedByPrelude},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/CPAN/", FeedByPrelude},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/CPAN/",          FeedByPrelude},
  {&Cqu,              "https://mirrors.cqu.edu.cn/CPAN/",           FeedByPrelude},
  {&Bjtu,             "https://mirror.bjtu.edu.cn/cpan/",           FeedByPrelude},
  {&Nju,              "https://mirrors.nju.edu.cn/CPAN/",           FeedByPrelude},
  {&Nyist,            "https://mirror.nyist.edu.cn/CPAN/",          FeedByPrelude},
  {&Ustc,             "https://mirrors.ustc.edu.cn/CPAN/",          FeedByPrelude},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/cpan/",           FeedByPrelude},
  {&Iscas,            "https://mirror.iscas.ac.cn/CPAN/",           FeedByPrelude},
  {&Zju,              "https://mirrors.zju.edu.cn/CPAN/",           FeedByPrelude},
  {&Lzuoss,           "https://mirrors.lzu.edu.cn/CPAN/",           FeedByPrelude},
  {&Hust,             "https://mirrors.hust.edu.cn/CPAN/",          FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/CPAN/",           FeedByPrelude}
  def_sources_end()

  chef_set_all_smurl_with_postfix (this, "authors/id/D/DB/DBAURAIN/Bio-MUST-Apps-FortyTwo-0.213470.tar.gz");
}


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
  chsrc_use_this_source (pl_perl);

  char *cmd = xy_strcat (3,
  "perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('urllist', 'unshift', '", source.url, "'); CPAN::HandleConfig->commit()\"");
  chsrc_run (cmd, RunOpt_Default);

  chsrc_alert2 ("请使用 perl -v 以及 cpan -v，若 Perl >= v5.36 或 CPAN >= 2.29，请额外手动调用下面的命令");
  p ("perl -MCPAN -e \"CPAN::HandleConfig->load(); CPAN::HandleConfig->edit('pushy_https', 0);; CPAN::HandleConfig->commit()\"");

  chsrc_conclude (&source);
}
