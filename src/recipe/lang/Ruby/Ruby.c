/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

static MirrorSite_t RubyChina =
{
  IS_DedicatedMirrorSite,
  "rubychina", "RubyChina", "Ruby China 社区", "https://gems.ruby-china.com/",
  "https://gems.ruby-china.com/rubygems/gems/nokogiri-1.15.0-java.gem", // 9.9 MB
  ACCURATE
};

def_target(pl_ruby);

void
pl_ruby_prelude (void)
{
  use_this(pl_ruby);
  chef_allow_gsr(pl_ruby);

  chef_set_created_on   (this, "2023-08-29");
  chef_set_last_updated (this, "2025-07-14");
  chef_set_sources_last_updated (this, "2024-12-18");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, PartiallyCan, "Support `bundler`. No support for `gem`", "Support `bundler`. No support for `gem`");
  chef_allow_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&upstream,      "https://rubygems.org",  "https://rubygems.org/gems/nokogiri-1.15.0-java.gem"},
  {&RubyChina,     "https://gems.ruby-china.com/", DelegateToMirror},
  {&Ustc,          "https://mirrors.ustc.edu.cn/rubygems/", DelegateToMirror}

  // {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/", DelegateToMirror},
  // {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/",          DelegateToMirror},

  // {&Tencent,   "https://mirrors.tencent.com/rubygems/",          DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/rubygems/",   DelegateToMirror},
  // {&Ali,       "https://mirrors.aliyun.com/rubygems/",            DelegateToMirror},
  // {&Huawei,    "https://mirrors.huaweicloud.com/repository/rubygems/", DelegateToMirror},
  def_sources_end()
}


void
pl_ruby_getsrc (char *option)
{
  chsrc_run ("gem sources", RunOpt_Default);
  chsrc_run ("bundle config get mirror.https://rubygems.org", RunOpt_Default);
}

void
pl_ruby_remove_gem_source (const char *source)
{
  char *cmd = NULL;
  if (chef_is_url (source))
    {
      cmd = xy_2strjoin ("gem sources -r ", source);
      chsrc_run (cmd, RunOpt_Default);
    }
}

/**
 * @consult https://gitee.com/RubyMetric/rbenv-cn
 */
void
pl_ruby_setsrc (char *option)
{
  chsrc_ensure_program ("gem");

  use_this(pl_ruby);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

  char *cmd = NULL;

  xy_run_iter ("gem sources -l", 0, pl_ruby_remove_gem_source);

  cmd = xy_2strjoin ("gem source -a ", source.url);
  chsrc_run (cmd, RunOpt_Default);


  chsrc_ensure_program ("bundle");

  char *where = " --global ";
  if (chsrc_in_local_mode())
    {
      where = " --local ";
    }

  cmd = xy_strjoin (4, "bundle config", where, "'mirror.https://rubygems.org' ", source.url);
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

void
pl_ruby_resetsrc (char *option)
{
  pl_ruby_setsrc (option);
}
