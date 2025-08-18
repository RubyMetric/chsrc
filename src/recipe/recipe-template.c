/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : recipe-template.c
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 *               |
 * Created On    : <2024-08-09>
 * Last Modified : <2025-08-11>
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
RubyMetric = {
  IS_DedicatedMirrorSite, /* 镜像站类型 */
  "rbmt",                 /* 该镜像站的 code, 可以这么使用: chsrc set <target> rbmt */

  /* 该镜像站的英文缩写 |   该镜像站的全名    |     镜像站首页  */
  "RubyMetric",           "RubyMetric 镜像站",  "https://rubymetirc.com",

  /* 是否跳过测速 | 跳过原因(中文) | 跳过原因(英文) */
  {NotSkip,             NA,                NA,
  /* 镜像站某个较大的可下载物的下载链接，用于测速 */
  "https://rubymetirc.com/target/aws/aws-sdk-go/@v/v1.45.2.zip",
  /* 是否为精准测速，若使用间接URL来测速，则填ROUGH */
   ACCURATE
  };
}



void
<category>_<target>_prelude (void)
{
  use_this(<category>_<target>);
  chef_allow_gsr(<category>_<target>); //代表支持 Get Set Reset 三种操作
  // chef_allow_s(<category>_<target>); //以此类推
  // chef_allow_gs(<category>_<target>);
  // chef_allow_sr(<category>_<target>);

  chef_set_created_on   (this, "2024-08-09"); //文件创建日期
  chef_set_last_updated (this, "2025-08-12"); //文件最后一次更新日期
  chef_set_sources_last_updated (this, "2025-08-11"); //镜像站点最后一次更新日期

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com"); //作者
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0); //想一直为它贡献和更新？将自己加在这里！
  chef_set_contributors (this, 1, //为它做了贡献？将自己的信息加在这里！
    "Nil Null", "nil@null.org");


  chef_allow_local_mode (this, PartiallyCan, "具体说明是否支持项目级换源...", "Tell users the local mode support");

  // chef_allow_english(this); //项目是否支持英文
  chef_forbid_english(this);

  // chef_allow_user_define(this); //是否支持用户自定义镜像源
  chef_forbid_user_define(this);

  chef_set_note ("中文备注说明...", "English note...");

  def_sources_begin()
  {&UpstreamProvider, "上游默认源链接, 若维护者暂时未知, 可填NULL, 这个主要用于reset", DelegateToUpstream}
  {&RubyMetric,       "https://rubymetirc.com/target",       DelegateToMirror},
  {&RubyInstaller,    "https://rubyinstaller.cn/target",     DelegateToMirror},
  {&Gitee,            "https://gitee.com/RubyMetric/chsrc",  DelegateToMirror},
  {&GitHub,           "https://github.com/RubyMetric/chsrc", "https://一个精准测速链接"}
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
  /* 下面这行是必须的，注入 source 变量 */
  use_this_source(<category>_<target>);

  /* 如果是 target group，你可能想要指定不同的 target 来使用它的源 */
  // Source_t source = chsrc_yield_source_and_confirm (&pl_js_group_target, option);

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


//最后，别忘了将自己的文件加入 menu.c 和 chsrc-main.c 之中嗷～
