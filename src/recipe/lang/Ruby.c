/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2024-12-18>
 * ------------------------------------------------------------*/

static SourceProvider_t pl_ruby_upstream =
{
  def_upstream, "https://rubygems.org",
  {NotSkip, NA, NA, "https://rubygems.org/gems/nokogiri-1.15.0-java.gem"}
};

static MirrorSite_t RubyChina =
{
  "rubychina", "RubyChina", "Ruby China 社区", "https://gems.ruby-china.com/",
  {NotSkip, NA, NA, "https://gems.ruby-china.com/rubygems/gems/nokogiri-1.15.0-java.gem"} // 9.9 MB
};

/**
 * @update 2024-12-18
 * @sync https://github.com/RubyMetric/chsrc/wiki/Ruby-MirrorSite
 * @sync https://github.com/RubyMetric/chsrc/discussions/62
 *
 * @note 曾经的问题 https://ruby-china.org/topics/43331
 */
static Source_t pl_ruby_sources[] =
{
  {&pl_ruby_upstream, "https://rubygems.org"},
  {&RubyChina,    "https://gems.ruby-china.com/"},
  {&Ustc,         "https://mirrors.ustc.edu.cn/rubygems/"}

  // {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  // {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},

  // {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/rubygems/"},
  // {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
  // {&Huawei,    "https://mirrors.huaweicloud.com/repository/rubygems/"},
};
def_sources_n(pl_ruby);


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
  if (is_url (source))
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

  chsrc_yield_source_and_confirm (pl_ruby);

  char *cmd = NULL;

  xy_run_iter ("gem sources -l", 0, pl_ruby_remove_gem_source);

  cmd = xy_2strjoin ("gem source -a ", source.url);
  chsrc_run (cmd, RunOpt_Default);


  chsrc_ensure_program ("bundle");

  char *where = " --global ";
  if (CliOpt_Locally==true)
    {
      where = " --local ";
    }

  cmd = xy_strjoin (4, "bundle config", where, "'mirror.https://rubygems.org' ", source.url);
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  ProgMode_ChgType = ProgMode_CMD_Reset ? ChgType_Reset : ChgType_Auto;
  chsrc_conclude (&source);
}

void
pl_ruby_resetsrc (char *option)
{
  pl_ruby_setsrc (option);
}


Feature_t
pl_ruby_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = PartiallyCan;
  f.cap_locally_explain = "Support `bundler`. No support for `gem`";
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}


def_target_gsrf(pl_ruby);
