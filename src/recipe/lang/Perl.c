/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(pl_perl, "perl/cpan");

void
pl_perl_prepare ()
{
  chef_prep_this_dish (pl_perl, gs);

  chef_set_recipe_created_on   (this, "2023-09-31");
  chef_set_recipe_last_updated (this, "2026-09-06");

  chef_set_chefs    (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@hezonglun", "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unknown);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unknown);
  chef_set_default_scope (this, UserScope);

  chef_deny_english    (this);
  chef_allow_user_define (this);

  def_sources_begin()
  {&UpstreamProvider, "https://www.cpan.org/",                      FeedByPrepare},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/CPAN/",        FeedByPrepare},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/CPAN/", FeedByPrepare},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/CPAN/",          FeedByPrepare},
  {&Bjtu,             "https://mirror.bjtu.edu.cn/cpan/",           FeedByPrepare},
  {&Nju,              "https://mirrors.nju.edu.cn/CPAN/",           FeedByPrepare},
  {&Nyist,            "https://mirror.nyist.edu.cn/CPAN/",          FeedByPrepare},
  {&Ustc,             "https://mirrors.ustc.edu.cn/CPAN/",          FeedByPrepare},
  {&Sjtug_Siyuan,     "https://mirror.sjtu.edu.cn/cpan/",           FeedByPrepare},
  {&Iscas,            "https://mirror.iscas.ac.cn/CPAN/",           FeedByPrepare},
  {&Zju,              "https://mirrors.zju.edu.cn/CPAN/",           FeedByPrepare},
  {&Lzuoss,           "https://mirrors.lzu.edu.cn/CPAN/",           FeedByPrepare},
  {&Hust,             "https://mirrors.hust.edu.cn/CPAN/",          FeedByPrepare},
  {&Ali,              "https://mirrors.aliyun.com/CPAN/",           FeedByPrepare}
  // @note 2026-09-06 CQU 全面封杀校外ip，弃用
  //{&Cqu,              "https://mirrors.cqu.edu.cn/CPAN/",           FeedByPrepare},
  def_sources_end()

  chef_set_rest_smURL_with_postfix (this, "authors/id/D/DB/DBAURAIN/Bio-MUST-Apps-FortyTwo-0.213470.tar.gz");
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
