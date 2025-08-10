/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : recipe-template.c
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 *               |
 * Created On    : <2024-08-09>
 * Last Modified : <2025-08-09>
 * -------------------------------------------------------------
 * 本文件作为一个通用模板：
 *
 *    为一个【换源目标(target)】定义具体的【换源方法(recipe)】
 *
 * 模版中:
 * <target>   为该换源目标的名称
 * <category> 为该换源目标的类别，仅有3类: pl,  os, wr
 *            分别对应3个子目录:         lang, os, ware
 * ------------------------------------------------------------*/




/* 模版文件内容从下方第30行正式开始 */





/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(<category>_<target>);

/**
 * 定义专服务于该target的镜像站，该例数据为虚拟填充
 */
static MirrorSite_t
RubyMetric = {"rbmt",                   // 该镜像站的 code, 可以这么使用: chsrc set <target> rbmt
              "RubyMetric",             // 该镜像站的缩写
              "RubyMetric.com",         // 该镜像站的全名
              "https://rubymetirc.com", // 镜像站首页
                                        // 镜像站某个较大的可下载物的下载链接，用于测速
              "https://rubymetirc.com/target/aws/aws-sdk-go/@v/v1.45.2.zip",

              ACCURATE};                // 是否为精准测速，若使用间接URL来测速，则填ROUGH


void
<category>_<target>_prelude (void)
{
  use_this(<category>_<target>);

  chef_set_created_on   (this, "2024-08-09");
  chef_set_last_updated (this, "2025-08-12");
  chef_set_sources_last_updated (this, "2025-08-10");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Nil Null", "nil@null.org");

  chef_has_get();
  chef_has_set();
  chef_has_reset();
  this.cap_local = PartiallyCan;
  this.cap_local_explain = "具体说明是否支持项目级换源...";

  this.can_english = false;
  this.can_user_define = false;
  // chef_allow_user_define(this);
  chef_forbid_user_define(this);

  this.note = "备注说明...";

  def_upstream("https://github.com/microsoft/winget-cli/");
  def_sources_begin()
  {&upstream,      "上游地址，若维护者暂时未知，可填NULL，这个主要用于reset", DelegateToUpstream}
  {&RubyMetric,    "https://rubymetirc.com/target",       DelegateToMirror},
  {&RubyInstaller, "https://rubyinstaller.cn/target",     DelegateToMirror},
  {&Gitee,         "https://gitee.com/RubyMetric/chsrc",  DelegateToMirror},
  {&GitHub,        "https://github.com/RubyMetric/chsrc", "https://一个精准测速链接"}
  def_sources_end()
}


/**
 * @required 非必需
 *
 * 用于 chsrc get <target>
 */
void
<category>_<target>_getsrc (char *option)
{
  // chsrc get <target>
}


/**
 * @required 必需
 * @consult  写明换源实现的参考地址
 *
 * 用于 chsrc set <target>
 */
void
<category>_<target>_setsrc (char *option)
{
  /* 下面这行是必须的，注入source变量 */
  chsrc_yield_source_and_confirm (<category>_<target>);

  /* 具体的换源步骤，如调用第三方命令... */

  /* 最后总结输出 */
  chsrc_determine_chgtype (ChgType_xxx);
  chsrc_conclude (&source);
}


/**
 * @required 非必需
 *
 * 用于 chsrc reset <target>
 */
void
<category>_<target>_resetsrc (char *option)
{
  /* 往往统一在 _setsrc() 中实现，直接调用即可 */
  // <category>_<target>_setsrc (option);
}
