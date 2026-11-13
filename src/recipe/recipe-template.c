/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : recipe-template.c
 * File Authors  : @ccmywish
 * Contributors  : @Mikachu2333
 *               |
 * Created On    : <2024-08-09>
 * Last Modified : <2026-10-28>
 * -------------------------------------------------------------
 * 本文件作为一个通用模板：
 *
 *    为一个【菜品(dish)】定义具体的【换源方法(recipe)】
 *
 * 模版中:
 * <dish>     为该换源菜品的名称
 * <category> 为该换源菜品的类别，仅有3类: pl,  os, wr
 *            分别对应3个子目录:         lang, os, ware
 * ------------------------------------------------------------*/




/* 模版文件内容从下方第30行正式开始 */





/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(<category>_<dish>);

/**
 * 定义专服务于该 dish 的镜像站，该例数据为虚拟填充
 */
static MirrorSite_t
RubyMetric = {
  IS_DedicatedMirrorSite, /* 镜像站类型 */
  "rbmt",                 /* 该镜像站的 code, 可以这么使用: chsrc set <dish> rbmt */

  /* 该镜像站的英文缩写 |   该镜像站的全名    |     镜像站首页  */
  "RubyMetric",           "RubyMetric 镜像站",  "https://rubymetirc.com",

  /* 是否跳过测速 | 跳过原因(中文) | 跳过原因(英文) */
  {NotSkip,             NA,                NA,
  /* 镜像站某个较大的可下载物的下载链接，用于测速 */
  "https://rubymetirc.com/dishes/aws/aws-sdk-go/@v/v1.45.2.zip",
  /* 是否为精准测速，若使用间接URL来测速，则填ROUGH */
   ACCURATE
  };
}



void
<category>_<dish>_prepare (void)
{
  // op 可以为 NOOP|s|sr|gsr|gs, 代表支持 Get Set Reset 三种操作
  chef_prep_this_dish (<category>_<dish>, op);

  chef_set_recipe_created_on   (this, "2024-08-09"); // 文件创建日期
  chef_set_recipe_last_updated (this, "2025-08-12"); // 文件最后一次更新日期

  chef_set_chefs (this, 2, "@ccmywish", "@nilnull");  // recipe 主要作者
  // 做了贡献？将自己的信息加在这里！
  chef_set_sauciers (this, 2, "@nulnone", "@someone");

  /* 可选枚举: ScopeCap_Unknown | ScopeCap_Unable | ScopeCap_Able_But_Not_Implemented
   *           ScopeCap_Able_But_Just_Docs | ScopeCap_Able_And_Implemented */
  chef_set_scope_cap (this, ProjectScope, ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_But_Not_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  // chef_allow_english(this); // 项目是否支持英文
  chef_deny_english(this);

  // chef_allow_user_define(this); // 是否支持用户自定义镜像源
  chef_deny_user_define(this);

  chef_set_note ("中文备注说明...", "English note...");

  def_sources_begin()
  {&UpstreamProvider, "上游默认源链接, 若维护者暂时未知, 可填NULL, 这个主要用于reset", DelegateToUpstream}
  {&RubyMetric,       "https://rubymetirc.com/dishes",       DelegateToMirror},
  {&RubyInstaller,    "https://rubyinstaller.cn/dishes",     DelegateToMirror},
  {&Gitee,            "https://gitee.com/RubyMetric/chsrc",  DelegateToMirror},
  {&GitHub,           "https://github.com/RubyMetric/chsrc", "https://一个精准测速链接"}
  def_sources_end()
}


/**
 * @required 非必需
 *
 * 用于 chsrc get <dish>
 */
void
<category>_<dish>_getsrc (char *option)
{
  // chsrc get <dish>
  // 若为环境变量型 recipe，可调用: chsrc_get_env ("VAR_NAME", NULL);
}


/**
 * @required 必需
 * @consult  写明换源实现的参考地址
 *
 * 用于 chsrc set <dish>
 */
void
<category>_<dish>_setsrc (char *option)
{
  /* 下面这行是必须的，注入 source 变量 */
  chsrc_use_this_source (<category>_<dish>);

  /* 你可能想要指定不同的 dish 来使用它的源 */
  // Source_t source = chsrc_yield_source_and_confirm (&pl_pypi_dish, option);

  /* 具体的换源步骤，如调用第三方命令，或使用 chsrc_set_env / chsrc_set_env_block 持久化环境变量... */

  /* 最后总结输出 */
  chsrc_determine_chgtype (ChgType_xxx);
  chsrc_conclude (&source);
}


/**
 * @required 非必需
 *
 * 用于 chsrc reset <dish>
 */
void
<category>_<dish>_resetsrc (char *option)
{
  /* 往往统一在 _setsrc() 中实现，直接调用即可 */
  // <category>_<dish>_setsrc (option);
}


// 最后，请将自己的文件加入到 menu.c 和 chsrc-main.c 对应的位置
// 形式可以参考其附近的其他食谱
